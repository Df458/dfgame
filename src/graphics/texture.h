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
    ubyte* data;
    uint16 width;
    uint16 height;
    uint8  elements;
} rawtex;

gltex gltex_new(GLenum type, uint16 w, uint16 h);
rawtex rawtex_new(uint16 w, uint16 h, uint8 elements);
rawtex rawtex_mask_to_rgba(rawtex tex);

#endif
