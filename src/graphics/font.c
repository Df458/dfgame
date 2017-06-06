// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "font.h"

#include "texture_atlas.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

typedef struct font {
    FT_Face font_face;
    texture_atlas atlas;
    float height;
}* font;
