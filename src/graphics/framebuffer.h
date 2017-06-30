#ifndef DF_GRAPHICS_FRAMEBUFFER
#define DF_GRAPHICS_FRAMEBUFFER
#include <GL/glew.h>
#include <GL/gl.h>

#include "texture.h"

typedef struct framebuffer {
    GLuint handle;
} framebuffer;

framebuffer framebuffer_new();

#endif
