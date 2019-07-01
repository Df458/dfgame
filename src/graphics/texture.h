/**
 * @file texture.h
 * @author Hugues Ross
 * @date 23 June 2019
 * @brief Provides functions for creating and manipulating textures
 */

#ifndef DF_GRAPHICS_TEXTURE
#define DF_GRAPHICS_TEXTURE
#include "core/types.h"
#include <GL/glew.h>
#include <GL/gl.h>

/** @brief Represents an OpenGL texture
 *
 * This provides access to the texture, as well as additional data such as the
 * dimensions and source image path
 */
typedef struct gltex {
    /** The handle for binding in OpenGL calls */
    GLuint handle;
    /** The OpenGL target type */
    GLuint type;

    /** The width, in pixels */
    uint16 width;
    /** The height, in pixels */
    uint16 height;

    /** The path to the source image file, if any exists */
    char* asset_path;
} gltex;

/** @brief Simple container for texture data
 *
 * This provides access to the texture, as well as additional data such as the
 * dimensions and source image path
 */
typedef struct rawtex {
    /** The raw image data */
    ubyte* data;

    /** The width, in pixels */
    uint16 width;
    /** The height, in pixels */
    uint16 height;
    /** The number of elements/channels per pixel */
    uint8  elements;

    /** The path to the source image file, if any exists */
    char* asset_path;
} rawtex;

/** A constant for returning an empty, invalid texture
 */
const extern rawtex rawtex_empty;

gltex gltex_new(GLenum type, uint16 w, uint16 h);
gltex gltex_new_depth(GLenum type, uint16 w, uint16 h);
gltex gltex_new_from_raw(GLenum type, rawtex raw, bool clone_path);

// Creates a new rawtex, and initializes the data block based on the arguments
rawtex rawtex_new(uint16 w, uint16 h, uint8 elements);

// Creates a new rawtex, and initializes the data block based on an existing opengl texture
rawtex rawtex_new_from_gl(gltex tex, bool clone_path);

// Creates a new rawtex, and initializes the data block with the provided data
rawtex rawtex_new_data(uint16 w, uint16 h, uint8 elements, ubyte* data);

/** @brief Create a rawtex from a region of another
 *
 * As the name implies, this will create data in the new texture that must be
 * cleaned up
 *
 * @param src The texture to clone
 * @param x The x origin point to clone from
 * @param y The y origin point to clone from
 * @param w The width to take. A negative width will flip the result, and
 *          a width of zero will take the full width of src
 * @param h The height to take. A negative height will flip the result, and
 *          a height of zero will take the full height of src
 */
rawtex rawtex_clone_section(rawtex src, uint16 x, uint16 y, int16 w, int16 h);

/** @brief Copy the content of one rawtex to another
 *
 * This function assumes that src is smaller or equal in size to dest
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
