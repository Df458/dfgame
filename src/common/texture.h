#ifndef DFGAME_TEXTURE_H
#define DFGAME_TEXTURE_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct texture
{
    GLuint    handle;
    uint16_t  width;
    uint16_t  height;
} texture;

/*!
 * Creates an empty texture.
 * w and h define the width and height of the allocated texture
 */
// TODO: Allow different channel types
texture* create_texture(uint16_t w, uint16_t h);

/*!
 * Frees a texture and its resources
 */
void destroy_texture(texture* tex);

/*!
 * This loads a png file to a texture.
 * See get_extended_resource_path(io_util.h) for usage details
 */
// TODO: Determine filetype and load based on that
texture* load_resource_to_texture(const char* resource_location, const char* resource_name);

/*!
 * This saves a texture to a png file.
 * See get_extended_resource_path(io_util.h) for usage details
 */
bool save_texture_to_resource(texture* tex, const char* resource_location, const char* resource_name);

/*!
 * Loads a texture, but instead of initializing it via OpenGL, it returns the
 * data in a buffer. The format of the data should be RGBA(8 bits per channel),
 * and w/h are filled with the appropriate dimensions.
 */
uint8_t* load_resource_to_texture_buffer(const char* resource_location, const char* resource_name, uint16_t* w, uint16_t* h);

#endif
