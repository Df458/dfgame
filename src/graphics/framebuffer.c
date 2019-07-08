// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "framebuffer.h"

/** @brief Create a new framebuffer
 *
 * Note: This will not bind textures, set up the viewport, etc.
 * For that, use a renderpass
 *
 * @param width The width of the framebuffer
 * @param height The height of the framebuffer
 */
framebuffer framebuffer_new(uint16 width, uint16 height) {
    framebuffer fbo = {
        .width = width,
        .height = height,
    };
    glGenFramebuffers(1, &fbo.handle);

    return fbo;
}

/** @brief Destroy the content of a framebuffer
 *
 * @param fbo The framebuffer to cleanup
 */
void framebuffer_cleanup(framebuffer* fbo) {
    glDeleteFramebuffers(1, &fbo->handle);
    fbo->width = 0;
    fbo->height = 0;
}
