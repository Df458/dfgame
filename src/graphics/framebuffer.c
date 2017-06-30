// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "framebuffer.h"

framebuffer framebuffer_new() {
    framebuffer fbo;
    glGenFramebuffers(1, &fbo.handle);

    return fbo;
}
