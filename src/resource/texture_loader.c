#define LOG_CATEGORY "Resource"

#include "texture_loader.h"

#include "log/log.h"
#include "check.h"
#include "memory/alloc.h"
#include "paths.h"
#include <png.h>
#include <jpeglib.h>
#include <tiffio.h>
#include <tga.h>
#include <stdio.h>
#include <string.h>

typedef struct jpeg_error {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
} jpeg_error;

void handle_jpeg_error(j_common_ptr ptr) {
    jpeg_error* err = (jpeg_error*)ptr->err;

    (*ptr->err->output_message) (ptr);

    longjmp(err->setjmp_buffer, 1);
}

///////////////////////////////////////////////////////////////////////////////

gltex load_texture_gl(const char* path) {
    gltex tex;
	glGenTextures(1, &tex.handle);
    tex.type = GL_TEXTURE_2D;
	glBindTexture(GL_TEXTURE_2D, tex.handle);
	
    rawtex raw = load_texture_raw(path);
    check_return(raw.data, "Failed to load texture %s", tex, path);
    tex.width = raw.width;
    tex.height = raw.height;
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raw.width, raw.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw.data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	sfree(raw.data);

    return tex;
}

rawtex load_texture_raw(const char* path) {
    rawtex tex = (rawtex){0};

    const char* ext = get_extension(path);
    if(!strcmp(ext, "png"))
        return load_png_raw(path);
    else if(!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
        return load_jpeg_raw(path);
    else if(!strcmp(ext, "tga"))
        return load_tga_raw(path);
    else if(!strcmp(ext, "tif") || !strcmp(ext, "tiff"))
        return load_tiff_raw(path);

    error("Failed to load texture: File extension %s not recognized", ext);

    return tex;
}

rawtex load_png_raw(const char* path) {
    rawtex tex = (rawtex){0};
    FILE* infile = fopen(path, "rb");
    check_return(infile, "Could not open file: %s", tex, path)
    
    uint8 header[8];
    png_structp pstruct;
    png_infop info_struct;
    png_bytep* row_ptrs;
    
    fread(header, sizeof(uint8), 8, infile);
    check_return(!png_sig_cmp(header, 0, 8), "File has an invalid header.", tex);

    pstruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    check_return(pstruct, "Could not read structure of file.", tex)

    info_struct = png_create_info_struct(pstruct);
    if(check_warn(info_struct, "Could not create info_struct for %s.", path)) {
        png_destroy_read_struct(&pstruct, NULL, NULL);
        return tex;
    }
    if(setjmp(png_jmpbuf(pstruct)))
        return tex;
    
    png_init_io(pstruct, infile);
    png_set_sig_bytes(pstruct, 8);
    png_read_info(pstruct, info_struct);
    
    tex.width = png_get_image_width(pstruct, info_struct);
    tex.height = png_get_image_height(pstruct, info_struct);
    png_byte color_type = png_get_color_type(pstruct, info_struct);
    /* png_byte bit_depth = png_get_bit_depth(pstruct, info_struct); */
    /* int number_of_passes = png_set_interlace_handling(pstruct); */
    
    if(color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(pstruct, 0xff, PNG_FILLER_AFTER);
    
    png_read_update_info(pstruct, info_struct);
    
    if(setjmp(png_jmpbuf(pstruct)))
        return tex;
    
    int rowbytes = png_get_rowbytes(pstruct, info_struct);
    
    tex.data = salloc(rowbytes * tex.height * sizeof(png_byte) + 15);
    row_ptrs = (png_bytep*)salloc(sizeof(png_bytep) * tex.height);
    for(int i = 0; i < tex.height; i++)
        row_ptrs[tex.height - 1 - i] = tex.data + i * rowbytes;
    
    png_read_image(pstruct, row_ptrs);
    
    if(png_get_color_type(pstruct, info_struct) != PNG_COLOR_TYPE_RGBA)
        png_set_add_alpha(pstruct, 0xff, PNG_FILLER_AFTER);
    
    png_destroy_read_struct(&pstruct, &info_struct, NULL);
    sfree(row_ptrs);
    fclose(infile);

    return tex;
}

rawtex load_jpeg_raw(const char* path) {
    struct jpeg_decompress_struct decompresser;
    rawtex tex = (rawtex){0};

    jpeg_error err;
    
    FILE* infile = fopen(path, "rb");
    JSAMPARRAY row;
    uint16_t row_width;
    check_return(infile, "Could not open file: %s", tex, path)

    decompresser.err = jpeg_std_error(&err.pub);
    err.pub.error_exit = handle_jpeg_error;

    if(setjmp(err.setjmp_buffer)) {
        jpeg_destroy_decompress(&decompresser);
        fclose(infile);

        return tex;
    }

    jpeg_create_decompress(&decompresser);
    jpeg_stdio_src(&decompresser, infile);
    jpeg_read_header(&decompresser, TRUE);
    jpeg_start_decompress(&decompresser);
    row_width = decompresser.output_width * decompresser.output_components;

    row = (*decompresser.mem->alloc_sarray) ((j_common_ptr) &decompresser, JPOOL_IMAGE, row_width, 1);
    tex.width = decompresser.output_width;
    tex.height = decompresser.output_height;
    tex.data = scalloc(decompresser.output_width * decompresser.output_height * 4, sizeof(uint8_t));
    for(int j = 0; j < tex.height; ++j) {
        jpeg_read_scanlines(&decompresser, row, 1);
        for(int i = 0; i < tex.width; ++i) {
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 0] = (uint8_t)row[0][(i * 3) + 0];
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 1] = (uint8_t)row[0][(i * 3) + 1];
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 2] = (uint8_t)row[0][(i * 3) + 2];
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 3] = 255;
        }
    }

    jpeg_finish_decompress(&decompresser);
    jpeg_destroy_decompress(&decompresser);

    fclose(infile);

    return tex;
}

// TODO: Add support for all TGA features
rawtex load_tga_raw(const char* path) {
    // In order to satisfy libtga's irrational desire for a non-const char
    // array, we copy the path to a temporary buffer.
    char* temp_path = strdup(path);
    TGA* image = TGAOpen(temp_path, "r");
    TGAData data;
    sfree(temp_path);

    rawtex tex = (rawtex){0};

    check_return(image, "Could not open file: %s", tex, path);

    int res = TGAReadHeader(image);
    if(check_error(res == TGA_OK, "Failed to parse tga file: %s", TGAStrError(res))) {
        TGAClose(image);
        return tex;
    }

    // If there's no image data, stop
    if(check_error(image->hdr.img_t != 0, "Failed to load tga file: No image data found")) {
        TGAClose(image);
        return tex;
    }

    // If we have color maps, stop
    // TODO: Add support for colormapped TGAs
    if(check_error(image->hdr.map_t == 0, "Failed to load tga file: Color-mapped tga files are unsupported")) {
        TGAClose(image);
        return tex;
    }

    bool alpha = (image->hdr.alpha);
    int px_size = image->hdr.depth;
    tex.width  = image->hdr.width;
    tex.height = image->hdr.height;
    if(check_error(px_size == 24 || px_size == 32, "Error trying to load tga file: Expected 24 or 32-bit color depth, got %d", image->hdr.depth)) {
        TGAClose(image);
        return tex;
    }

    // Get the byte count, not the bit count.
    px_size >>= 3;

    data.img_data = salloc(image->hdr.depth * tex.width * tex.height);
    size_t size = TGAReadScanlines(image, data.img_data, 0, tex.height, TGA_RGB);
    if(check_error(size == tex.height, "Failed to load tga file: Expected %d lines, but got %d.", tex.height, size)) {
        TGAClose(image);
        sfree(data.img_data);
        tex.width = 0;
        tex.height = 0;
        return tex;
    }
    tex.data = scalloc(4 * tex.width * tex.height, sizeof(uint8_t));
    int hmod = image->hdr.horz ? tex.width : 0;
    int vmod = image->hdr.vert ? 0 : tex.height;
    for(int j = 0; j < tex.height; ++j) {
        for(int i = 0; i != tex.width; ++i) {
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 0] = (uint8)data.img_data[((vmod - j * (1 - (2 * image->hdr.vert))) * tex.width + (hmod - i * (-1 + (2 * image->hdr.horz)))) * px_size + 0];
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 1] = (uint8)data.img_data[((vmod - j * (1 - (2 * image->hdr.vert))) * tex.width + (hmod - i * (-1 + (2 * image->hdr.horz)))) * px_size + 1];
            tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 2] = (uint8)data.img_data[((vmod - j * (1 - (2 * image->hdr.vert))) * tex.width + (hmod - i * (-1 + (2 * image->hdr.horz)))) * px_size + 2];
            if(!alpha)
                tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 3] = 255;
            else
                tex.data[((tex.height - 1 - j) * tex.width + i) * 4 + 3] = (uint8_t)data.img_data[(j * tex.width + i) * px_size + 3];
        }
    }

    return tex;
}

rawtex load_tiff_raw(const char* path) {
    rawtex tex;
    TIFF* infile = TIFFOpen(path, "r");
    check_return(infile, "Could not open file: %s", tex, path)

        TIFFRGBAImage img;
    char err[1024];
    if(check_error(TIFFRGBAImageBegin(&img, infile, 0, err),"Cannot load tiff file: %s", err)) {
        TIFFClose(infile);
        return tex;
    }
    tex.width = img.width;
    tex.height = img.height;

    uint32_t* temp_buffer = 0;
    temp_buffer = _TIFFmalloc(tex.width * tex.height * sizeof(uint32));
    if(check_error(temp_buffer, "Can't load tiff: temp_buffer could not be allocated")) {
        tex.width = 0;
        tex.height = 0;
        TIFFClose(infile);
        return tex;
    }

    tex.data = scalloc(tex.width * tex.height * 4, sizeof(uint8));
    TIFFRGBAImageGet(&img, temp_buffer, tex.width, tex.height);
    memcpy(tex.data, temp_buffer, tex.width * tex.height * 4);
    _TIFFfree(temp_buffer);

    TIFFRGBAImageEnd(&img);

    TIFFClose(infile);
    return tex;
}
