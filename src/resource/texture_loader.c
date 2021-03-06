#define LOG_CATEGORY "Resource"

#include "texture_loader.h"

#include "log/log.h"
#include "check.h"
#include "memory/alloc.h"
#include "paths.h"
#include "stringutil.h"
#ifdef enable_gif
#include <gif_lib.h>
#endif
#ifdef enable_png
#include <png.h>
#endif
#ifdef enable_jpeg
#include <jpeglib.h>
#endif
#ifdef enable_tiff
#include <tiffio.h>
#endif
#ifdef enable_tga
#include <tga.h>
#endif
#include <stdio.h>
#include <string.h>

#ifdef enable_jpeg
typedef struct jpeg_error {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
} jpeg_error;

void handle_jpeg_error(j_common_ptr ptr) {
    jpeg_error* err = (jpeg_error*)ptr->err;

    (*ptr->err->output_message) (ptr);

    longjmp(err->setjmp_buffer, 1);
}
#endif

///////////////////////////////////////////////////////////////////////////////

gltex load_texture_gl(const char* path) {
    rawtex raw = load_texture_raw(path);
    check_return(raw.data, "Failed to load texture %s", (gltex){0}, path);

    gltex tex = gltex_new_from_raw(GL_TEXTURE_2D, raw, true);

    rawtex_cleanup(&raw);

    return tex;
}

rawtex load_texture_raw(const char* path) {
    rawtex tex = rawtex_empty;
    char* load_path = as_resource_path(path);
    const char* ext = get_extension(load_path);

    if (false) {}
#ifdef enable_gif
    else if(!strcmp(ext, "gif")) {
        tex = load_gif_raw(load_path);
    }
#endif
#ifdef enable_png
    else if(!strcmp(ext, "png")) {
        tex = load_png_raw(load_path);
    }
#endif
#ifdef enable_jpeg
    else if(!strcmp(ext, "jpg") || !strcmp(ext, "jpeg")) {
        tex = load_jpeg_raw(load_path);
    }
#endif
#ifdef enable_tga
    else if(!strcmp(ext, "tga")) {
        tex = load_tga_raw(load_path);
    }
#endif
#ifdef enable_tiff
    else if(!strcmp(ext, "tif") || !strcmp(ext, "tiff")) {
        tex = load_tiff_raw(load_path);
    }
#endif
    else {
        error("Failed to load texture: File extension %s not recognized", ext);
    }

    sfree(load_path);

    return tex;
}

#ifdef enable_gif
GifFileType* load_and_slurp_gif(const char* path) {
    // Open the file
    int error;
    GifFileType* file_type = DGifOpenFileName(path, &error);

    check_return(file_type, "Can't load gif file %s: \"%s\"", NULL, path, GifErrorString(error));

    // Load the data into memory
    error = DGifSlurp(file_type);
    if(check_error(error == GIF_OK, "Failed to load data for gif file %s", path)) {
        DGifCloseFile(file_type, &error);
        return NULL;
    }

    return file_type;
}

bool gif_read_gcb(GifFileType* file_type, int index, GraphicsControlBlock* block) {
    int error = DGifSavedExtensionToGCB(file_type, index, block);
    if(error == GIF_OK) {
        return true;
    } 

    warn("Failed to read gif GCB: %s. Animation may be a little screwed up...", GifErrorString(error));
    return false;
}

rawtex load_gif_raw(const char* path) {
    rawtex tex = rawtex_empty;

    // Open the file
    int error;
    GifFileType* file_type = load_and_slurp_gif(path);
    if(!file_type) {
        return tex;
    }

    tex = rawtex_new(file_type->SWidth, file_type->SHeight, 4);

    // Fill the texture with gif data
    for(int i = 0; i < file_type->ImageCount; ++i) {
        GifImageDesc desc = file_type->SavedImages[i].ImageDesc;
        GifByteType* raster = file_type->SavedImages[i].RasterBits;

        // One of these maps may be NULL, so we check the local one first
        GifColorType* colors = desc.ColorMap ? desc.ColorMap->Colors : file_type->SColorMap->Colors;

        int transparency = -1;
        GraphicsControlBlock block;
        if(gif_read_gcb(file_type, i, &block)) {
            transparency = block.TransparentColor;
        }

        for(int y = 0; y < desc.Height; ++y) {
            for(int x = 0; x < desc.Width; ++x) {
                int index = ((tex.height - y - desc.Top - 1) * tex.width) + (x + desc.Left);
                int color = raster[y * desc.Width + x];
                // Don't replace previous layers with the transparent color
                if(color != transparency) {
                    tex.data[index * 4 + 0] = colors[color].Red;
                    tex.data[index * 4 + 1] = colors[color].Green;
                    tex.data[index * 4 + 2] = colors[color].Blue;
                    tex.data[index * 4 + 3] = 0xff; // Alpha of 1
                }
            }
        }
    }

    tex.asset_path = nstrdup(path);

    DGifCloseFile(file_type, &error);
    return tex;
}
#endif

#ifdef enable_png
void rawtex_fill_png_rgba(rawtex* tex, png_structp pstruct, int rowbytes) {
    png_bytep* row_ptrs = (png_bytep*)salloc(sizeof(png_bytep) * tex->height);
    for(int i = 0; i < tex->height; i++) {
        row_ptrs[tex->height - 1 - i] = tex->data + i * rowbytes;
    }

    png_read_image(pstruct, row_ptrs);

    sfree(row_ptrs);
}

rawtex load_png_raw(const char* path) {
    rawtex tex = rawtex_empty;
    FILE* infile = fopen(path, "rb");
    check_return(infile, "Could not open file: %s", tex, path)
    
    uint8 header[8];
    png_structp pstruct;
    png_infop info_struct;
    
    size_t read = fread(header, sizeof(uint8), 8, infile);
    if(check_error(read == 8 && !png_sig_cmp(header, 0, 8), "File has an invalid header.")) {
        fclose(infile);
        return tex;
    }

    pstruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    check_return(pstruct, "Could not read structure of file.", tex)

    info_struct = png_create_info_struct(pstruct);
    if(check_warn(info_struct, "Could not create info_struct for %s.", path)) {
        png_destroy_read_struct(&pstruct, NULL, NULL);
        return tex;
    }
    if(setjmp(png_jmpbuf(pstruct))) {
        png_destroy_read_struct(&pstruct, NULL, NULL);
        return tex;
    }
    
    png_init_io(pstruct, infile);
    png_set_sig_bytes(pstruct, 8);
    png_read_info(pstruct, info_struct);

    png_byte color_type = png_get_color_type(pstruct, info_struct);
    
    // Convert other types to RGBA
    switch(color_type) {
        case PNG_COLOR_TYPE_GRAY:
              png_set_gray_to_rgb(pstruct);
              png_set_filler(pstruct, 0xff, PNG_FILLER_AFTER);
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
              png_set_gray_to_rgb(pstruct);
            break;
        case PNG_COLOR_TYPE_PALETTE:
              png_set_palette_to_rgb(pstruct);
              png_set_filler(pstruct, 0xff, PNG_FILLER_AFTER);
            break;
        case PNG_COLOR_TYPE_RGB:
              png_set_filler(pstruct, 0xff, PNG_FILLER_AFTER);
            break;
    }
    
    png_read_update_info(pstruct, info_struct);
    if(setjmp(png_jmpbuf(pstruct))) {
        png_destroy_read_struct(&pstruct, &info_struct, NULL);
        return tex;
    }
    
    tex = rawtex_new(png_get_image_width(pstruct, info_struct), png_get_image_height(pstruct, info_struct), 4);
    tex.asset_path = nstrdup(path);
    rawtex_fill_png_rgba(&tex, pstruct, png_get_rowbytes(pstruct, info_struct));

    png_destroy_read_struct(&pstruct, &info_struct, NULL);
    fclose(infile);

    return tex;
}

/** @brief Save a rawtex to a PNG file
 *
 * @param path The filepath to save, or NULL to use the texture's asset_path
 * @param tex The texture
 */
void save_png_raw(const char* path, rawtex tex) {
    if (path == NULL) {
        path = tex.asset_path;
        check_return(path, "No filepath provided", );
    }
    FILE* outfile = fopen(path, "wb");
    check_return(outfile, "Could not open file: %s", , path);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    check_return(png_ptr, "Failed to create write struct", );

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(check_error(info_ptr, "Failed to create info struct")) {
        png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
        fclose(outfile);
        return;
    }

    if(setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(outfile);

        return;
    }

    png_init_io (png_ptr, outfile);

    int color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    switch (tex.elements) {
        case 1: // 1 element, grayscale
            color_type = PNG_COLOR_TYPE_GRAY;
            break;

        case 3: // 3 elements, RGB
            color_type = PNG_COLOR_TYPE_RGB;
            break;

        case 4: // 4 elements, RGBA
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
            break;

        default:
            error("Element count %d is unsupported", tex.elements);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(outfile);
            return;
    }

    png_set_IHDR(png_ptr, info_ptr, tex.width, tex.height, 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    ubyte** row_ptrs = mscalloc(tex.height, ubyte*);
    for(uint16 i = 0; i < tex.height; ++i) {
        row_ptrs[i] = tex.data + (i * tex.width * tex.elements);
    }

    png_set_rows (png_ptr, info_ptr, row_ptrs);
    png_write_png (png_ptr, info_ptr, 0, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(outfile);
}

/** @brief Save a gltex to a PNG file
 *
 * @param path The filepath to save, or NULL to use the texture's asset_path
 * @param tex The texture
 */
void save_png_gl(const char* path, gltex tex) {
    rawtex rt = rawtex_new_from_gl(tex, true);
    save_png_raw(path, rt);
    rawtex_cleanup(&rt);
}
#endif

#ifdef enable_jpeg
rawtex load_jpeg_raw(const char* path) {
    struct jpeg_decompress_struct decompresser;
    rawtex tex = rawtex_empty;

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
    tex = rawtex_new(decompresser.output_width, decompresser.output_height, 4);
    tex.asset_path = nstrdup(path);
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
#endif

#ifdef enable_tga
rawtex load_tga_raw(const char* path) {
    // In order to satisfy libtga's irrational desire for a non-const char
    // array, we copy the path to a temporary buffer.
    char* temp_path = nstrdup(path);
    TGA* image = TGAOpen(temp_path, "r");
    TGAData data;
    sfree(temp_path);

    rawtex tex = rawtex_empty;

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
    tex.elements = image->hdr.depth / sizeof(byte);
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

    tex.asset_path = nstrdup(path);

    return tex;
}
#endif

#ifdef enable_tiff
rawtex load_tiff_raw(const char* path) {
    rawtex tex = rawtex_empty;
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
    tex.elements = 4;

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

    tex.asset_path = nstrdup(path);

    return tex;
}
#endif
