#ifndef DF_GRAPHICS_TEXTURE
#define DF_GRAPHICS_TEXTURE
#include "core/types.h"
#include <GL/glew.h>
#include <GL/gl.h>

typedef struct gltex {
    GLuint handle;
    GLuint type;
    uint16 width;
    uint16 height;

    char* asset_path;
} gltex;
typedef struct rawtex {
    ubyte* data;
    uint16 width;
    uint16 height;
    uint8  elements;

    char* asset_path;
} rawtex;
const extern rawtex rawtex_empty;

gltex gltex_new(GLenum type, uint16 w, uint16 h);
gltex gltex_new_depth(GLenum type, uint16 w, uint16 h);
gltex gltex_new_from_raw(GLenum type, rawtex raw, bool clone_path);

// Creates a new rawtex, and initializes the data block based on the arguments
rawtex rawtex_new(uint16 w, uint16 h, uint8 elements);

// Creates a new rawtex, and initializes the data block based on an existing opengl texture
rawtex rawtex_new_from_gl(const gltex tex, bool clone_path);

// Creates a new rawtex, and initializes the data block with the provided data
rawtex rawtex_new_data(uint16 w, uint16 h, uint8 elements, ubyte* data);

/** @brief Copy the content from one rawtex to another
 *
 * This function assumes that src is amaller or equal in size to dest
 *
 * @param dest The destination texture
 * @param src The source texture
 * @param x The x position to place the source texture
 * @param y The y position to place the source texture
 */
bool rawtex_copy_data(rawtex dest, rawtex src, uint16 x, uint16 y);

// Converts a single-channel rawtex to a white RGBA rawtex, where the original
// data becomes the alpha.
// If clone_path is true, the asset_path on the new rawtex is a copy of the old
// one. Otherwise, both point to the same data.
rawtex rawtex_mask_to_rgba(rawtex tex, bool clone_path);

// Deletes the OpenGL texture handle and path of the given gltex
void gltex_cleanup(gltex* tex);
// Deletes the buffer and path of the given rawtex
void rawtex_cleanup(rawtex* tex);

#endif
