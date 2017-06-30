// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "font.h"

#include "check.h"
#include "container/array.h"
#include "memory/alloc.h"
#include "texture_atlas.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

bool find_glyph_with_id(void* o1, void* o2, void* user) {
    int16 id = *(int16*)o1;
    glyph* gp = (glyph*)o2;

    return gp->index == id;
}

typedef struct font {
    FT_Face font_face;
    texture_atlas atlas;
    uarray glyphs;
    float height;
}* font;

font font_new(uint16 height) {
    font f = salloc(sizeof(struct font));
    f->atlas = texture_atlas_new();
    f->glyphs = uarray_new(256);
    f->height = height;
    return f;
}

void font_add_glyph(font f, glyph gp) {
    array_copyadd(f->glyphs, &gp, sizeof(gp));
}

glyph font_get_glyph(font f, int16 id) {
    int32 glyph_id = array_findp(f->glyphs, (void*)&id, find_glyph_with_id, NULL);
    check_return(glyph_id >= 0, "Couldn't find glyph 0x%d", (glyph){0}, id);
    glyph* gp = (glyph*)array_get(f->glyphs, glyph_id);

    return *gp;
}

aabb_2d font_get_glyph_bounds(font f, int16 id) {
    return texture_atlas_get(f->atlas, id);
}

gltex font_get_texture(font f) {
    return texture_atlas_get_texture(f->atlas);
}

float font_get_height(font f) {
    return f->height;
}
