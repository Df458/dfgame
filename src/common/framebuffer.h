#ifndef DF_FRAMEBUFFER_H
#define DF_FRAMEBUFFER_H
#include "texture.h"

typedef struct framebuffer
{
    GLuint handle;
    uint16_t width;
    uint16_t height;
}
framebuffer;

framebuffer* create_framebuffer(uint16_t width, uint16_t height);

void destroy_framebuffer_full(framebuffer* fbo);
#define destroy_framebuffer(fbo) { destroy_framebuffer_full(fbo); fbo = 0; }


#endif
