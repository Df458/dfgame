#include "font.h"
#include "io_util.h"
#include "vector.h"

FT_Library font_library;

bool init_fonts()
{
    int error = FT_Init_FreeType(&font_library);

    if(error) {
        warn("Failed to initialize FreeType. Error code: %d", error);
        return false;
    }
    return true;
}

void cleanup_fonts()
{
    FT_Done_FreeType(font_library);
}

font* create_font()
{
    font* f = salloc(sizeof(font));
    f->atlas = create_texture(GLYPH_ATLAS_SIZE, GLYPH_ATLAS_SIZE);
    f->special_glyphs = 0;
    f->atlas_xbounds = 0;
    f->atlas_ybounds = 0;
    f->atlas_ybounds_next = 0;
    return f;
}

void destroy_font_full(font* ft)
{
    if(ft->special_glyphs)
        destroy_sorted_tree_deep(ft->special_glyphs);
    destroy_texture(ft->atlas);
    sfree(ft);
}

font* load_resource_to_font(resource_pair, bool prepare_basic_glyphs, float font_height)
{
    font* f = create_font();
    int error = FT_New_Face(font_library, construct_extended_resource_path(resource_location, resource_name), 0, &f->font_face);
    if(error) {
        error("Can't load font: Error code %d", error);
        sfree(f);
        return 0;
    }

    // TODO: Add HIDPI support
    /* FT_Set_Char_Size(f->font_face, 0, font_height * 64, 0, 0); */
    FT_Set_Pixel_Sizes(f->font_face, 0, font_height);
    f->height = font_height;

    if(prepare_basic_glyphs) {
        for(int i = BASIC_GLYPHS_START; i < BASIC_GLYPHS_END; ++i)
            font_prepare_glyph(f, i);
    }

    return f;
}

bool save_font_to_resource(resource_pair, font* fnt)
{
    stub(false);
}

bool font_prepare_glyph(font* ft, int glyph_id)
{
    int index = FT_Get_Char_Index(ft->font_face, glyph_id);
    FT_Load_Glyph(ft->font_face, index, FT_LOAD_DEFAULT);
    if(ft->font_face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
        FT_Render_Glyph(ft->font_face->glyph, FT_RENDER_MODE_NORMAL);
    glyph* gp;
    if(glyph_id >= BASIC_GLYPHS_START && glyph_id < BASIC_GLYPHS_END)
        gp = &ft->basic_glyphs[glyph_id - BASIC_GLYPHS_START];
    else {
        gp = salloc(sizeof(glyph));
        if(ft->special_glyphs)
            sorted_tree_set(ft->special_glyphs, glyph_id, gp, true);
        else
            ft->special_glyphs = create_sorted_tree(glyph_id, gp);
    }

    // TODO: Make this fit the textures more precisely
    gp->texture_position.data[0] = 0;
    gp->texture_position.data[1] = 0;
    gp->texture_size.data[0] = ft->font_face->glyph->bitmap.width;
    gp->texture_size.data[1] = ft->font_face->glyph->bitmap.rows;

    if(ft->atlas_xbounds + gp->texture_size.data[0] + 1 > GLYPH_ATLAS_SIZE) {
        ft->atlas_xbounds = 0;
        ft->atlas_ybounds = ft->atlas_ybounds_next + 1;
    }
    if(ft->atlas_ybounds + gp->texture_size.data[1] + 1 > GLYPH_ATLAS_SIZE) {
        error("Can't fit glyph in texture atlas: 0x%x", glyph_id);
        return false;
    }
    if(ft->atlas_ybounds + gp->texture_size.data[1] + 1 > ft->atlas_ybounds_next)
        ft->atlas_ybounds_next = ft->atlas_ybounds + gp->texture_size.data[1];

    gp->texture_position.data[0] = ft->atlas_xbounds;
    gp->texture_position.data[1] = ft->atlas_ybounds;
    ft->atlas_xbounds += gp->texture_size.data[0] + 1;

    gp->bearing.data[0] = ft->font_face->glyph->metrics.horiBearingX;
    gp->bearing.data[1] = ft->font_face->glyph->metrics.horiBearingY;
    gp->advance = ft->font_face->glyph->metrics.horiAdvance;
    for(int i = 0; i < ft->font_face->glyph->bitmap.rows; ++i)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, gp->texture_position.data[0], gp->texture_position.data[1] + i, gp->texture_size.data[0], 1, GL_ALPHA, GL_UNSIGNED_BYTE, ft->font_face->glyph->bitmap.buffer + ((ft->font_face->glyph->bitmap.rows - i - 1) * ft->font_face->glyph->bitmap.pitch));
    }
    return true;
}

glyph* font_get_glyph(font* ft, int glyph_id, bool prepare_if_missing)
{
    glyph* gp = 0;
    if(glyph_id >= BASIC_GLYPHS_START && glyph_id < BASIC_GLYPHS_END)
        gp = &ft->basic_glyphs[glyph_id - BASIC_GLYPHS_START];
    else
        gp = sorted_tree_get(ft->special_glyphs, glyph_id);
    if(!gp && prepare_if_missing && font_prepare_glyph(ft, glyph_id)) {
        if(glyph_id >= BASIC_GLYPHS_START && glyph_id < BASIC_GLYPHS_END)
            gp = &ft->basic_glyphs[glyph_id - BASIC_GLYPHS_START];
        else
            gp = sorted_tree_get(ft->special_glyphs, glyph_id);
    }
        

    return gp;
}
