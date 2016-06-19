#ifndef DF_TEXTURE_H
#define DF_TEXTURE_H
#include "resource_def.h"
#include "vector.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct texture
{
    GLuint    handle;
    GLuint    type;
    uint16_t  width;
    uint16_t  height;
}
texture;

/*!
 * Creates an empty texture.
 * w and h define the width and height of the allocated texture
 */
texture* create_texture_storage(uint16_t w, uint16_t h, GLuint in_storage_type, GLuint out_storage_type, GLuint storage_format);
#define create_texture(w, h) create_texture_storage(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE)

/*!
 * Frees a texture and its resources
 */
void destroy_texture(texture* tex);

/*!
 * This loads a png file to a texture.
 * See get_extended_resource_path(io_util.h) for usage details
 */
// TODO: Determine filetype and load based on that
texture* load_resource_to_texture(resource_pair);

/*!
 * This saves a texture to a png file.
 * See get_extended_resource_path(io_util.h) for usage details
 */
bool save_texture_to_resource(texture* tex, resource_pair);

/*!
 * Loads a texture, but instead of initializing it via OpenGL, it returns the
 * data in a buffer. The format of the data should be RGBA(8 bits per channel),
 * and w/h are filled with the appropriate dimensions.
 */
uint8_t* load_resource_to_texture_buffer(resource_pair, uint16_t* w, uint16_t* h);

bool fill_texture(texture* tex, vec4 color);

#endif
