#ifndef DF_GRAPHICS_FONT
#define DF_GRAPHICS_FONT

#include "font.hd"
#include "aabb.h"
#include "texture.h"

typedef struct glyph {
    int index;
    int16 texture_index;
    vec2 bearing;
    float advance;
} glyph;

font font_new(uint16 height);
void font_add_glyph(font f, glyph gp);
glyph* font_get_glyph(font f, int16 id);
aabb_2d font_get_glyph_bounds(font f, int16 id);
gltex font_get_texture(font f);
float font_get_height(font f);

#endif
