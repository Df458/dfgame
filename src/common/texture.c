#include "io_util.h"
#include "texture.h"
#include "util.h"

#include <png.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

png_byte* load_png_to_buffer(const char* path, uint16_t* w, uint16_t* h)
{
    FILE* infile = fopen(path, "rb");
    if(!infile) {
        warn("Could not open file.");
        fprintf(stderr, "Error Path: %s\n", path);
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

texture* load_resource_to_texture(const char* resource_location, const char* resource_name)
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

bool save_texture_to_resource(texture* tex, const char* resource_location, const char* resource_name)
{
    stub(false);
}

uint8_t* load_resource_to_texture_buffer(const char* resource_location, const char* resource_name, uint16_t* w, uint16_t* h)
{
    char* path = construct_extended_resource_path(resource_location, resource_name);
    nulltest(path);

    // TODO: Add support for more texture formats
    uint8_t* buffer = load_png_to_buffer(path, w, h);

    free(path);
    return buffer;
}
