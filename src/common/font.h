#ifndef DF_FONT_H
#define DF_FONT_H
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <stdbool.h>
#include "resource_def.h"
#include "sorted_tree.h"
#include "texture.h"
#include "util.h"
#include "vector.h"

#define BASIC_GLYPHS_START 32
#define BASIC_GLYPHS_END 127
#define GLYPH_ATLAS_SIZE 1024

typedef struct glyph
{
    vec2  texture_position;
    vec2  texture_size;
    vec2  bearing;
    float advance;
}
glyph;

typedef struct font
{
    FT_Face font_face;
    texture* atlas;
    glyph basic_glyphs[95];
    sorted_tree* special_glyphs;
    float atlas_xbounds;
    float atlas_ybounds;
    float atlas_ybounds_next;
    float height;
}
font;

bool init_fonts();
void cleanup_fonts();

font* create_font();
void destroy_font_full(font* font);
#define destroy_font(f) { destroy_font_full(f); f = 0; }

font* load_resource_to_font(resource_pair, bool prepare_basic_glyphs, float font_height);
bool save_font_to_resource(resource_pair, font* fnt);

bool font_prepare_glyph(font* ft, int glyph_id);
glyph* font_get_glyph(font* ft, int glyph_id, bool prepare_if_missing);

#endif
