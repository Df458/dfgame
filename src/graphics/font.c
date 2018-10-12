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

// Search function for finding a glyph that matches a specific id
bool find_glyph_with_id(void* o1, void* o2, void* user) {
    int16 id = *(int16*)o1;
    glyph* gp = (glyph*)o2;

    return gp->index == id;
}

typedef struct font {
    FT_Face font_face;
    texture_atlas atlas;
    array glyphs;
    float height;

    char* asset_path;
}* font;


// Creates a new font
font font_new(uint16 height, const char* path) {
    font f = mscalloc(1, struct font);

    f->atlas = texture_atlas_new();
    f->glyphs = array_mnew(glyph, 256);
    f->height = height;
    f->font_face = NULL;
    f->asset_path = nstrdup(path);

    return f;
}

// Adds a bew glyph to the font
void font_add_glyph(font f, glyph gp) {
    check_return(f, "Font is NULL", );

    array_add(f->glyphs, gp);
}

// Returns a glyph from the font
glyph* font_get_glyph(const font f, int16 id) {
    check_return(f, "Font is NULL", NULL);

    int32 glyph_id = array_findp(f->glyphs, (void*)&id, find_glyph_with_id, NULL);
    check_return(glyph_id >= 0, "Couldn't find glyph 0x%d", NULL, id);
    glyph* gp = (glyph*)array_get(f->glyphs, glyph_id);

    return gp;
}

// Returns the bounding box for a font's glyph, in texture coordinates
aabb_2d font_get_glyph_bounds(const font f, int16 id) {
    check_return(f, "Font is NULL", aabb_2d_zero);

    return texture_atlas_get(f->atlas, id);
}

// Returns the font's texture atlas
gltex font_get_texture(const font f) {
    check_return(f, "Font is NULL", (gltex){0});

    return texture_atlas_get_texture(f->atlas);
}

// Returns the font's line height
float font_get_height(const font f) {
    check_return(f, "Font is NULL", 0);

    return f->height;
}

// Frees the given font, and all resources contained within it
// NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _font_free(font f) {
    check_return(f, "Font is NULL", );

    if(f->font_face)
        FT_CALL_NORETURN(FT_Done_Face(f->font_face));

    array_free(f->glyphs);
    texture_atlas_free(f->atlas);
    if(f->asset_path)
        sfree(f->asset_path);
    sfree(f);
}
