#ifndef DF_GRAPHICS_FONT_PRIV
#define DF_GRAPHICS_FONT_PRIV
#include "core/container/array.h"
#include "graphics/texture_atlas.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

typedef struct font {
    FT_Face font_face;
    texture_atlas atlas;
    array glyphs;
    float height;

    char* asset_path;
}* font;

#endif
