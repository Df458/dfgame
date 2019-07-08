/**
 * @file framebuffer.h
 * @author Hugues Ross
 * @date 8 July 2019
 * @brief Provides functions for handling OpenGL framebuffers
 */

#ifndef DF_GRAPHICS_FRAMEBUFFER
#define DF_GRAPHICS_FRAMEBUFFER
#include "core/types.h"
#include <GL/glew.h>
#include <GL/gl.h>

/** @brief Represents an FBO which can be rendered to
 *
 */
typedef struct framebuffer {
    /** The handle for binding in OpenGL calls */
    GLuint handle;
    /** The width, in pixels */
    uint16 width;
    /** The height, in pixels */
    uint16 height;
} framebuffer;

/** @brief Create a new framebuffer
 *
 * Note: This will not bind textures, set up the viewport, etc.
 * For that, use a renderpass
 *
 * @param width The width of the framebuffer
 * @param height The height of the framebuffer
 */
framebuffer framebuffer_new(uint16 width, uint16 height);

/** @brief Destroy the content of a framebuffer
 *
 * @param fbo The framebuffer to cleanup
 */
void framebuffer_cleanup(framebuffer* fbo);

#endif
