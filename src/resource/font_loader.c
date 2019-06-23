#define LOG_CATEGORY "Resource"

#include "font_loader.h"

#include "core/check.h"
#include "core/container/array.h"
#include "core/log/log.h"
#include "core/stringutil.h"
#include "graphics/font.h"
#include "graphics/font.priv.h"
#include "graphics/graphics_log.h"
#include "graphics/texture.h"
#include "graphics/texture_atlas.h"
#include "resource/paths.h"
#include "resource/texture_loader.h"
#include "resource/xmlutil.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <libxml/parser.h>
#include <libxml/tree.h>

static FT_Library font_library;
static bool font_library_init = false;

font load_font(const char* path, uint16 height) {
    const char* ext = get_extension(path);
    if(!strcmp(ext, "ttf") || !strcmp(ext, "otf"))
        return load_freetype_font(path, height);
    else if(!strcmp(ext, "xml"))
        return load_bitmap_font(path);

    error("Failed to load font: File extension %s not recognized", ext);

    return NULL;
}

// FreeType Loader
glyph load_freetype_glyph(FT_Face face, int id, font f) {
    int index = FT_Get_Char_Index(face, id);
    FT_CALL(FT_Load_Glyph(face, index, FT_LOAD_DEFAULT), (glyph){.texture_index=-1});
    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
        FT_CALL(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL), (glyph){.texture_index=-1});

    if(!face->glyph->bitmap.buffer)
        return (glyph){.texture_index=-1};

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

    rawtex rgba = rawtex_mask_to_rgba(tex, false);
    gp.texture_index = texture_atlas_add_raw(f->atlas, rgba, GL_RGBA);
    rawtex_cleanup(&rgba);

    return gp;
}

font load_freetype_font(const char* path, uint16 height) {
    FT_Face face;

    if(!font_library_init) {
        FT_CALL(FT_Init_FreeType(&font_library), NULL);
        font_library_init = true;
    }

    FT_CALL(FT_New_Face(font_library, path, 0, &face), NULL);
    FT_CALL(FT_Set_Pixel_Sizes(face, 0, height), NULL);

    font f = font_new(height, path);
    f->font_face = face;

    for(int i = 0; i < 256; ++i) {
       glyph gp = load_freetype_glyph(face, i, f);
       if(gp.texture_index < 0)
           continue;
       font_add_glyph(f, gp);
    }

    return f;
}

// XML Loader
void read_glyphs(xmlNodePtr node, font f, uint16 height, const char* path) {
    char* file = NULL;
    check_return(xml_property_read(node, "file", &file), "Font glyphs are missing a texture file", );

    char* chars = NULL;
    if (!xml_property_read(node, "chars", &chars)) {
        char* range = NULL;
        if(xml_property_read(node, "range", &range)) {
            uint16 position = 0;
            uint16 final_length = 0;
            uint16 len = nstrlen(range);
            for(uint16 i = 0; i + 1 < len; i += 2) {
                int8 iter = range[i] < range[i+1] ? 1 : -1;
                final_length += abs(range[i] - range[i + 1]) + 1;
                chars = resalloc(chars, final_length * sizeof(char) + 1);

                for(char j = range[i]; j != range[i+1] + iter; j += iter) {
                    chars[position] = j;
                    position++;
                }
            }

            sfree(range);
        }
    }

    uint16 default_width = 0;
    xml_property_read(node, "width", &default_width);
    uint16 default_advance = default_width;
    xml_property_read(node, "advance", &default_advance);
    uint16 margin = 0;
    xml_property_read(node, "margin", &margin);

    uint16 charcount = nstrlen(chars);
    check_return(charcount > 0, "Font has no glyphs", );

    rawtex font_tex = load_texture_raw(combine_paths(get_folder(path), file, true));

    uint16 cursor_x = 0;
    uint16 cursor_y = font_tex.height;// - height;
    for (uint16 i = 0; i < charcount; ++i) {
        uint16 width = default_width;
        uint16 advance = default_advance;

        xml_foreach(child, node->children, "glyph") {
            char* ch;
            if (xml_property_read(child, "char", &ch)) {
                if (ch[0] == chars[i]) {
                    xml_property_read(child, "width", &width);
                    xml_property_read(child, "advance", &advance);
                    break;
                }
                sfree(ch);
            }
        }
        check_break(width > 0, "Glyph %c has width of 0", chars[i]);

        if (cursor_x + width > font_tex.width) {
            cursor_x = 0;
            cursor_y -= height + margin;
        }
        /* check_break(cursor_x + width <= font_tex.width && cursor_y + height <= font_tex.height, "Glyph %c is out-of-bounds", chars[i]); */

        rawtex subtex = rawtex_clone_section(font_tex, cursor_x, cursor_y, width, -height);
        cursor_x += width + margin;

        glyph gp = (glyph) {
            .index = chars[i],
            .bearing = {
                .x = 0,
                .y = height
            },
            .advance = advance,
            .texture_index = texture_atlas_add_raw(f->atlas, subtex, GL_RGBA)
        };
        font_add_glyph(f, gp);
        rawtex_cleanup(&subtex);
    }

    rawtex_cleanup(&font_tex);

    sfree(chars);
}

/** Load a font from image files
 *
 * This function uses a special XML file to define source files and glyph
 * metrics, see the 'text' demo for an example
 *
 * @param path The filepath pointing to the XML file
 */
font load_bitmap_font(const char* path) {
    xmlDocPtr doc = xmlReadFile(path, NULL, 0);
    check_return(doc, "Failed to load font at path %s", NULL, path);

    xmlNodePtr root = xml_match_name(xmlDocGetRootElement(doc), "font");
    check_return(root != NULL, "Font file %s is invalid", NULL, path);

    uint16 height;
    check_return(xml_property_read(root, "height", &height), "Font %s lacks a glyph height", NULL, path);

    font f = font_new(height, path);

    xml_foreach(node, root->children, "glyphs") {
        read_glyphs(node, f, height, path);
    }

    return f;
}
