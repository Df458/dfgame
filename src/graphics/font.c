// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "font.h"

#include "check.h"
#include "container/array.h"
#include "graphics_log.h"
#include "memory/alloc.h"
#include "stringutil.h"
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

    char* asset_path;
}* font;

font font_new(uint16 height, const char* path) {
    font f = salloc(sizeof(struct font));
    f->atlas = texture_atlas_new();
    f->glyphs = uarray_new(256);
    f->height = height;
    f->font_face = NULL;
    f->asset_path = nstrdup(path);
    return f;
}

void font_add_glyph(font f, glyph gp) {
    array_copyadd(f->glyphs, &gp, sizeof(gp));
}

glyph* font_get_glyph(font f, int16 id) {
    int32 glyph_id = array_findp(f->glyphs, (void*)&id, find_glyph_with_id, NULL);
    check_return(glyph_id >= 0, "Couldn't find glyph 0x%d", NULL, id);
    glyph* gp = (glyph*)array_get(f->glyphs, glyph_id);

    return gp;
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

// Frees the given font, and all resources contained within it
// NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _font_free(font f) {
    if(f->font_face)
        FT_CALL_NORETURN(FT_Done_Face(f->font_face));

    uarray_free_deep(f->glyphs);
    texture_atlas_free(f->atlas);
    if(f->asset_path)
        sfree(f->asset_path);
    sfree(f);
}
