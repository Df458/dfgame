#define LOG_CATEGORY "Resource"

#include "font_loader.h"

#include "check.h"
#include "font.h"
#include "graphics_log.h"
#include "log/log.h"
#include "paths.h"
#include "texture.h"
#include "texture_atlas.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

typedef struct font {
    FT_Face font_face;
    texture_atlas atlas;
    float height;
}* font;

static FT_Library font_library;
static bool font_library_init;

glyph load_freetype_glyph(FT_Face face, int id, font f) {
    int index = FT_Get_Char_Index(face, id);
    FT_CALL(FT_Load_Glyph(face, index, FT_LOAD_DEFAULT), (glyph){0});
    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
        FT_CALL(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL), (glyph){0});

    glyph gp = (glyph) {
        .index = id,
        .bearing = (vec2) {
            .x = face->glyph->metrics.horiBearingX / 64,
            .y = face->glyph->metrics.horiBearingY / 64
        },
        .advance = face->glyph->metrics.horiAdvance / 64
    };

    rawtex tex = (rawtex) {
        .data = face->glyph->bitmap.buffer,
        .width = face->glyph->bitmap.width,
        .height = face->glyph->bitmap.rows,
        .elements = 1
    };

    gp.texture_index = texture_atlas_add_raw(f->atlas, rawtex_mask_to_rgba(tex), GL_RGBA);

    return gp;
}

font load_font(const char* path, uint16 height) {
    const char* ext = get_extension(path);
    if(!strcmp(ext, "ttf") || !strcmp(ext, "otf"))
        return load_freetype_font(path, height);

    error("Failed to load font: File extension %s not recognized", ext);

    return NULL;
}

font load_freetype_font(const char* path, uint16 height) {
    FT_Face face;

    if(!font_library_init) {
        FT_CALL(FT_Init_FreeType(&font_library), NULL);
        font_library_init = true;
    }

    FT_CALL(FT_New_Face(font_library, path, 0, &face), NULL);
    FT_CALL(FT_Set_Pixel_Sizes(face, 0, height), NULL);

    font f = font_new(height);

    for(int i = 0; i < 256; ++i) {
       glyph gp = load_freetype_glyph(face, i, f);
       if(gp.texture_index < 0)
           continue;
       font_add_glyph(f, gp);
    }

    return f;
}
