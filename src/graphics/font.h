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

// Creates a new font
font font_new(uint16 height, const char* path);

// Adds a bew glyph to the font
void font_add_glyph(font f, glyph gp);

// Returns a glyph from the font
glyph* font_get_glyph(const font f, int16 id);

// Returns the bounding box for a font's glyph, in texture coordinates
aabb_2d font_get_glyph_bounds(const font f, int16 id);

// Returns the font's texture atlas
gltex font_get_texture(const font f);

// Returns the font's line height
float font_get_height(const font f);

// Frees the given font, and all resources contained within it
#define font_free(f) { _font_free(f); f = NULL; }
void _font_free(font f);

#endif
