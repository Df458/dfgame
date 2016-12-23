#include "framebuffer.h"
#include "util.h"

framebuffer* create_framebuffer(uint16_t width, uint16_t height)
{
    framebuffer* fbo = salloc(sizeof(framebuffer));
    glGenFramebuffers(1, &fbo->handle);

    return fbo;
}

void destroy_framebuffer_full(framebuffer* fbo)
{
    glDeleteFramebuffers(1, &fbo->handle);
    sfree(fbo);
}
