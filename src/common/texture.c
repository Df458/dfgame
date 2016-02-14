#include "io_util.h"
#include "texture.h"
#include "util.h"

#include <stdio.h>
#include <jpeglib.h>
#include <png.h>
#include <tiffio.h>
#include <tga.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

png_byte* load_png_to_buffer(const char* path, uint16_t* w, uint16_t* h)
{
    FILE* infile = fopen(path, "rb");
    if(!infile) {
        warn("Could not open file: %s", path);
        return 0;
    }
    
    uint8_t header[8];
    png_structp pstruct;
    png_infop info_struct;
    uint16_t width, height;
    png_byte* image_data;
    png_bytep* row_ptrs;
    
    fread(header, sizeof(uint8_t), 8, infile);
    if(png_sig_cmp(header, 0, 8)) {
        warn("File has an invalid header.");
        return 0;
    }
    pstruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pstruct) {
        warn("Could not read structure of file.");
        return 0;
    }
    info_struct = png_create_info_struct(pstruct);
    if(!info_struct) {
        png_destroy_read_struct(&pstruct, NULL, NULL);
        warn("Could not create info_struct for file.");
        return 0;
    }
    if(setjmp(png_jmpbuf(pstruct))) {
        return 0;
    }
    
    png_init_io(pstruct, infile);
    png_set_sig_bytes(pstruct, 8);
    png_read_info(pstruct, info_struct);
    
    width = png_get_image_width(pstruct, info_struct);
    height = png_get_image_height(pstruct, info_struct);
    png_byte color_type = png_get_color_type(pstruct, info_struct);
    png_byte bit_depth = png_get_bit_depth(pstruct, info_struct);
    int number_of_passes = png_set_interlace_handling(pstruct);
    
    if(color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(pstruct, 0xff, PNG_FILLER_AFTER);
    }
    
    png_read_update_info(pstruct, info_struct);
    
    if(setjmp(png_jmpbuf(pstruct))){
        return 0;
    }
    
    int rowbytes = png_get_rowbytes(pstruct, info_struct);
    //rowbytes += 3 - ((rowbytes-1) % 4);
    
    // TODO: This next line came out weird, please confirm that this is correct
    image_data = (png_byte*)malloc(rowbytes * height * sizeof(png_byte)+15);
    row_ptrs = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int i = 0; i < height; i++){
        row_ptrs[height - 1 - i] = image_data + i * rowbytes;
    }
    
    png_read_image(pstruct, row_ptrs);
    
    if(png_get_color_type(pstruct, info_struct) != PNG_COLOR_TYPE_RGBA) {
        png_set_add_alpha(pstruct, 0xff, PNG_FILLER_AFTER);
    }
    
    png_destroy_read_struct(&pstruct, &info_struct, NULL);
    free(row_ptrs);
    fclose(infile);

    *w = width;
    *h = height;
    return image_data;
}

typedef struct jpeg_error
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
} jpeg_error;

void handle_jpeg_error(j_common_ptr ptr)
{
    jpeg_error* err = (jpeg_error*)ptr->err;

    (*ptr->err->output_message) (ptr);

    longjmp(err->setjmp_buffer, 1);
}

uint8_t* load_jpeg_to_buffer(const char* path, uint16_t* w, uint16_t* h)
{
    struct jpeg_decompress_struct decompresser;

    jpeg_error err;
    
    FILE* infile = fopen(path, "rb");
    JSAMPARRAY row;
    uint16_t row_width;
    if(!infile) {
        warn("Could not open file: %s", path);
        return 0;
    }

    decompresser.err = jpeg_std_error(&err.pub);
    err.pub.error_exit = handle_jpeg_error;

    if(setjmp(err.setjmp_buffer)) {
        jpeg_destroy_decompress(&decompresser);
        fclose(infile);

        return 0;
    }

    jpeg_create_decompress(&decompresser);
    jpeg_stdio_src(&decompresser, infile);
    jpeg_read_header(&decompresser, TRUE);
    jpeg_start_decompress(&decompresser);
    row_width = decompresser.output_width * decompresser.output_components;

    row = (*decompresser.mem->alloc_sarray) ((j_common_ptr) &decompresser, JPOOL_IMAGE, row_width, 1);
    *w = decompresser.output_width;
    *h = decompresser.output_height;
    uint8_t* buffer = calloc(decompresser.output_width * decompresser.output_height * 4, sizeof(uint8_t));
    for(int j = 0; j < *h; ++j) {
        jpeg_read_scanlines(&decompresser, row, 1);
        for(int i = 0; i < *w; ++i) {
            buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 0] = (uint8_t)row[0][(i * 3) + 0];
            buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 1] = (uint8_t)row[0][(i * 3) + 1];
            buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 2] = (uint8_t)row[0][(i * 3) + 2];
            buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 3] = 255;
            /*info("Getting (%d, %d, %d, %d)", buffer[((j * (*w)) + i) * 4 + 1], buffer[((j * (*w)) + i) * 4 + 1], buffer[((j * (*w)) + i) * 4 + 2], buffer[((j * (*w)) + i) * 4 + 3]);*/
        }
    }

    jpeg_finish_decompress(&decompresser);
    jpeg_destroy_decompress(&decompresser);

    fclose(infile);

    return buffer;
}

// TODO: Add support for all TGA features
uint8_t* load_tga_to_buffer(const char* path, uint16_t* w, uint16_t* h)
{
    // In order to satisfy libtga's irrational desire for a non-const char
    // array, we copy the path to a temporary buffer.
    char* temp_path = strdup(path);
    TGA* image = TGAOpen(temp_path, "r");
    TGAData data;
    free(temp_path);
    if(!image) {
        error("Could not open file: %s", path);
        return 0;
    }

    int res = TGAReadHeader(image);
    if(res != TGA_OK) {
        error("Failed to parse tga file: %s", TGAStrError(res));
        TGAClose(image);
        return 0;
    }

    // If there's no image data, stop
    if(image->hdr.img_t == 0) {
        error("Failed to load tga file: No image data found");
        TGAClose(image);
        return 0;
    }

    // If we have color maps, stop
    // TODO: Add support for colormapped TGAs
    if(image->hdr.map_t) {
        error("Failed to load tga file: Color-mapped tga files are unsupported");
        TGAClose(image);
        return 0;
    }

    bool alpha = (image->hdr.alpha);
    int px_size = image->hdr.depth;
    *w = image->hdr.width;
    *h = image->hdr.height;
    if(px_size != 24 && px_size != 32) {
        error("Error trying to load tga file: Expected 24 or 32-bit color depth, got %d", image->hdr.depth);
        TGAClose(image);
        return 0;
    }

    // Get the byte count, not the bit count.
    // TODO: Use bitshifting for this instead
    px_size /= 8;

    data.img_data = malloc(image->hdr.depth * image->hdr.width * image->hdr.height);
    size_t size = TGAReadScanlines(image, data.img_data, 0, image->hdr.height, TGA_RGB);
    if(size != image->hdr.height) {
        error("Failed to load tga file: Expected %d lines, but got %d.", image->hdr.height, size);
        TGAClose(image);
        free(data.img_data);
        return 0;
    }
    uint8_t* final_buffer = calloc(4 * image->hdr.width * image->hdr.height, sizeof(uint8_t));
    info("w: %d, h: %d, size: %d\n", *w, *h, px_size);
    for(int j = 0; j < *h; ++j) {
        for(int i = 0; i < *w; ++i) {
            final_buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 0] = (uint8_t)data.img_data[((j * (*w)) + i) * px_size + 0];
            final_buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 1] = (uint8_t)data.img_data[((j * (*w)) + i) * px_size + 1];
            final_buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 2] = (uint8_t)data.img_data[((j * (*w)) + i) * px_size + 2];
            if(!alpha)
                final_buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 3] = 255;
            else
                final_buffer[((((*h - 1) - j) * (*w)) + i) * 4 + 3] = (uint8_t)data.img_data[((j * (*w)) + i) * px_size + 3];
            /*info("Getting [%d, %d] (%d, %d, %d, %d)", i, j, final_buffer[((j * (*w)) + i) * 4 + 1], final_buffer[((j * (*w)) + i) * 4 + 1], final_buffer[((j * (*w)) + i) * 4 + 2], final_buffer[((j * (*w)) + i) * 4 + 3]);*/
        }
    }

    return final_buffer;
}

uint8_t* load_tiff_to_buffer(const char* path, uint16_t* w, uint16_t* h)
{
    TIFF* infile = TIFFOpen(path, "r");
    if(!infile) {
        error("Could not open file: %s", path);
        return 0;
    }

    TIFFRGBAImage img;
    char err[1024];
    if(!TIFFRGBAImageBegin(&img, infile, 0, err)) {
        error("Cannot load tiff file: %s", err);
        TIFFClose(infile);
        return 0;
    }
    *w = img.width;
    *h = img.height;
    info("Dimensions: %dx%d", *w, *h);
    uint32_t* temp_buffer = 0;
    temp_buffer = _TIFFmalloc((*w) * (*h) * sizeof(uint32_t));
    uint8_t* buffer = calloc((*w) * (*h) * 4, sizeof(uint8_t));
    if(temp_buffer == NULL) {
        error("Can't load tiff: temp_buffer could not be allocated");
        TIFFClose(infile);
        return 0;
    }

    TIFFRGBAImageGet(&img, temp_buffer, *w, *h);
    memcpy(buffer, temp_buffer, (*w) * (*h) * 4);
    _TIFFfree(temp_buffer);

    TIFFRGBAImageEnd(&img);

    TIFFClose(infile);
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////

texture* create_texture(uint16_t w, uint16_t h)
{
	texture* texture_data = malloc(sizeof(texture));
	glGenTextures(1, &texture_data->handle);
	glBindTexture(GL_TEXTURE_2D, texture_data->handle);
    texture_data->width = w;
    texture_data->height = h;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_data->width, texture_data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    return texture_data;
}

void destroy_texture(texture* tex)
{
    glDeleteTextures(1, &tex->handle);
    free(tex);
}

texture* load_resource_to_texture(resource_pair)
{
	texture* texture_data = malloc(sizeof(texture));
	glGenTextures(1, &texture_data->handle);
	glBindTexture(GL_TEXTURE_2D, texture_data->handle);
	
    uint8_t* image_data = load_resource_to_texture_buffer(resource_location, resource_name, &texture_data->width, &texture_data->height);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_data->width, texture_data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	free(image_data);

    return texture_data;
}

bool save_texture_to_resource(texture* tex, resource_pair)
{
    stub(false);
}

uint8_t* load_resource_to_texture_buffer(resource_pair, uint16_t* w, uint16_t* h)
{
    char* path = construct_extended_resource_path(resource_location, resource_name);
    nulltest(path);

    uint8_t* buffer = 0;
    const char* ext = get_extension(resource_name);
    if(!strcmp(ext, "png")) {
        buffer = load_png_to_buffer(path, w, h);
    } else if(!strcmp(ext, "jpg") || !strcmp(ext, "jpeg")) {
        buffer = load_jpeg_to_buffer(path, w, h);
    } else if(!strcmp(ext, "tga")) {
        buffer = load_tga_to_buffer(path, w, h);
    } else if(!strcmp(ext, "tif") || !strcmp(ext, "tiff")) {
        buffer = load_tiff_to_buffer(path, w, h);
    } else {
        error("Failed to load texture: File extension %s not recognized", ext);
    }

    free(path);
    return buffer;
}
