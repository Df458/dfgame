#ifndef DF_GRAPHICS_TEXTURE
#define DF_GRAPHICS_TEXTURE
#include "types.h"
#include <GL/glew.h>
#include <GL/gl.h>

typedef struct gltex {
    GLuint handle;
    GLuint type;
    uint16 width;
    uint16 height;
} gltex;
typedef struct rawtex {
    uint8* data;
    uint16 width;
    uint16 height;
} rawtex;

#endif
