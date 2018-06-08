#ifndef DF_GRAPHICS_FONT
#define DF_GRAPHICS_FONT

#include "graphics/font.hd"
#include "graphics/texture.h"
#include "math/aabb.h"

typedef struct glyph {
    int16 index;
    int16 texture_index;
    vec2 bearing;
    float advance;
} glyph;

font font_new(uint16 height, const char* path);
void font_add_glyph(font f, glyph gp);
glyph* font_get_glyph(font f, int16 id);
aabb_2d font_get_glyph_bounds(font f, int16 id);
gltex font_get_texture(font f);
float font_get_height(font f);

// Frees the given font, and all resources contained within it
#define font_free(f) { _font_free(f); f = NULL; }
void _font_free(font f);

#endif
