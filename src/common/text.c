#include "text.h"
#include "render.h"
#include "vertex_def.h"
#include "array_list.h"

struct text_element
{
    const char* position;
    ssize_t len;
    float width;
    char followed;
};

void recalculate_buffer(text* txt)
{
    txt->buflen = 0;
    glBindBuffer(GL_ARRAY_BUFFER, txt->handle);
    checkGLError();

    array_list* elem_list = create_array_list();
    array_list* width_list = create_array_list();
    float counter  = 0;
    int   lcounter = 0;
    ssize_t i      = 0;
    float* llen = salloc(sizeof(float));
    *llen = 0;
    char last_sep = '\0';
    array_list_push(width_list, llen);
    for(i = 0; i < txt->len; ++i) {
        if(txt->str[i] == ' ' || txt->str[i] == '\n' || txt->str[i] == '\t') {
            /* if(lcounter > 0) { */
                float c_w = 0;
                if(last_sep != '\0' && last_sep != '\n') {
                    glyph* tg = font_get_glyph(txt->font, last_sep, true);
                    c_w = tg->advance / 64;
                }

                if((*llen + c_w + counter <= txt->limits.x && txt->limits.x != 0) && last_sep != '\n') {
                    *llen += c_w + counter;
                } else {
                    llen = salloc(sizeof(float));
                    *llen = counter;
                    array_list_push(width_list, llen);
                }


                struct text_element* e = salloc(sizeof(struct text_element));
                e->position = txt->str + (i - lcounter);
                e->len      = lcounter;
                e->width    = counter;
                e->followed = txt->str[i];
                array_list_push(elem_list, e);
                txt->buflen += lcounter;
            /* } */
            last_sep = txt->str[i];
            lcounter = 0;
            counter  = 0;
            continue;
        } else if(txt->str[i] == '\0')
            break;
        ++lcounter;
        glyph* gp = font_get_glyph(txt->font, txt->str[i], true);
        if(!gp) {
            error("Can't load glyph 0x%x", txt->str[i]);
            continue;
        }
        counter += gp->advance / 64;
    }
    if(lcounter > 0) {
        float c_w = 0;
        if(last_sep != '\0' && last_sep != '\n') {
            glyph* tg = font_get_glyph(txt->font, last_sep, true);
            c_w = tg->advance / 64;
        }

        if(*llen + c_w + counter <= txt->limits.x || last_sep == '\n') {
            *llen += c_w + counter;
        } else {
            llen = salloc(sizeof(float));
            *llen = counter;
            array_list_push(width_list, llen);
        }
        struct text_element* e = salloc(sizeof(struct text_element));
        e->position = txt->str + (i - lcounter);
        e->len      = lcounter;
        e->width    = counter;
        e->followed = txt->str[i];
        array_list_push(elem_list, e);
        txt->buflen += lcounter;
    }

    vt_pnt* buffer = scalloc(txt->buflen * 6, sizeof(vt_pnt));

    i = 0;
    vec2 pen;
    float xoffset = 0;
    int line_counter = 0;
    if(txt->alignment == TEXT_ALIGN_CENTER)
        xoffset = txt->limits.x - (*(float*)array_list_get(width_list, line_counter)) * 0.5f;
    else if(txt->alignment == TEXT_ALIGN_END)
        xoffset = txt->limits.x - (*(float*)array_list_get(width_list, line_counter));
    for(int j = 0; j < elem_list->length; ++j) {
        struct text_element* e = array_list_get(elem_list, j);
        if(txt->limits.x != 0 && pen.x != 0 && pen.x + e->width > txt->limits.x) {
            pen.x = 0;
            pen.y += txt->font->height;
            ++line_counter;
            if(txt->alignment == TEXT_ALIGN_CENTER)
                xoffset = txt->limits.x - (*(float*)array_list_get(width_list, line_counter)) * 0.5f;
            else if(txt->alignment == TEXT_ALIGN_END)
                xoffset = txt->limits.x - (*(float*)array_list_get(width_list, line_counter));
        }
        for(int k = 0; k < e->len; ++k) {
            glyph* gp = font_get_glyph(txt->font, e->position[k], true);
            if(!gp) {
                continue;
            }
            float l, r, t, b, lt, rt, tt, bt;
            float bear = gp->bearing.x / 64;
            if(pen.x == 0)
                bear = 0;
            l = pen.x + bear - gp->texture_size.x * 0.5f + xoffset;
            r = pen.x + bear + gp->texture_size.x * 0.5f + xoffset;
            t = pen.y - (gp->bearing.y / 128) - gp->texture_size.y * 0.5f;
            b = pen.y - (gp->bearing.y / 128) + gp->texture_size.y * 0.5f;
            lt = gp->texture_position.x / (float) GLYPH_ATLAS_SIZE;
            rt = gp->texture_position.x / (float) GLYPH_ATLAS_SIZE + gp->texture_size.x / (float)GLYPH_ATLAS_SIZE;
            tt = gp->texture_position.y / (float) GLYPH_ATLAS_SIZE + gp->texture_size.y / (float)GLYPH_ATLAS_SIZE;
            tt = gp->texture_position.y / (float) GLYPH_ATLAS_SIZE + gp->texture_size.y / (float)GLYPH_ATLAS_SIZE;
            bt = gp->texture_position.y / (float) GLYPH_ATLAS_SIZE;

            buffer[i * 6 + 0].position[0] = r;
            buffer[i * 6 + 0].position[1] = t;
            buffer[i * 6 + 0].position[2] = 0.0f;
            buffer[i * 6 + 0].texture[0] = rt;
            buffer[i * 6 + 0].texture[1] = tt;
            buffer[i * 6 + 0].normal[0] = 0.0f;
            buffer[i * 6 + 0].normal[1] = 0.0f;
            buffer[i * 6 + 0].normal[2] = 1.0f;

            buffer[i * 6 + 1].position[0] = l;
            buffer[i * 6 + 1].position[1] = t;
            buffer[i * 6 + 1].position[2] = 0.0f;
            buffer[i * 6 + 1].texture[0] = lt;
            buffer[i * 6 + 1].texture[1] = tt;
            buffer[i * 6 + 1].normal[0] = 0.0f;
            buffer[i * 6 + 1].normal[1] = 0.0f;
            buffer[i * 6 + 1].normal[2] = 1.0f;

            buffer[i * 6 + 2].position[0] = l;
            buffer[i * 6 + 2].position[1] = b;
            buffer[i * 6 + 2].position[2] = 0.0f;
            buffer[i * 6 + 2].texture[0] = lt;
            buffer[i * 6 + 2].texture[1] = bt;
            buffer[i * 6 + 2].normal[0] = 0.0f;
            buffer[i * 6 + 2].normal[1] = 0.0f;
            buffer[i * 6 + 2].normal[2] = 1.0f;

            buffer[i * 6 + 3].position[0] = r;
            buffer[i * 6 + 3].position[1] = t;
            buffer[i * 6 + 3].position[2] = 0.0f;
            buffer[i * 6 + 3].texture[0] = rt;
            buffer[i * 6 + 3].texture[1] = tt;
            buffer[i * 6 + 3].normal[0] = 0.0f;
            buffer[i * 6 + 3].normal[1] = 0.0f;
            buffer[i * 6 + 3].normal[2] = 1.0f;

            buffer[i * 6 + 4].position[0] = l;
            buffer[i * 6 + 4].position[1] = b;
            buffer[i * 6 + 4].position[2] = 0.0f;
            buffer[i * 6 + 4].texture[0] = lt;
            buffer[i * 6 + 4].texture[1] = bt;
            buffer[i * 6 + 4].normal[0] = 0.0f;
            buffer[i * 6 + 4].normal[1] = 0.0f;
            buffer[i * 6 + 4].normal[2] = 1.0f;

            buffer[i * 6 + 5].position[0] = r;
            buffer[i * 6 + 5].position[1] = b;
            buffer[i * 6 + 5].position[2] = 0.0f;
            buffer[i * 6 + 5].texture[0] = rt;
            buffer[i * 6 + 5].texture[1] = bt;
            buffer[i * 6 + 5].normal[0] = 0.0f;
            buffer[i * 6 + 5].normal[1] = 0.0f;
            buffer[i * 6 + 5].normal[2] = 1.0f;

            ++i;
            pen.data[0] += ((gp->advance - gp->bearing.data[0]) / 64) + bear;
        }
        switch(e->followed) {
            case ' ': {
                glyph* tg = font_get_glyph(txt->font, ' ', true);
                pen.data[0] += tg->advance / 64;
            } break;
            case '\t': {
                glyph* tg = font_get_glyph(txt->font, '\t', true);
                pen.data[0] += tg->advance / 64;
            } break;
            case '\n':
                pen.data[0] = 0;
                pen.data[1] += txt->font->height;
                line_counter++;
                if(txt->alignment == TEXT_ALIGN_CENTER)
                    xoffset = txt->limits.data[0] - (*(float*)array_list_get(width_list, line_counter)) * 0.5f;
                else if(txt->alignment == TEXT_ALIGN_END)
                    xoffset = txt->limits.data[0] - (*(float*)array_list_get(width_list, line_counter));
                break;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, txt->buflen * 6 * sizeof(vt_pnt), buffer, GL_STATIC_DRAW);
    checkGLError();
    array_list_destroy_deep(width_list);
    array_list_destroy_deep(elem_list);
}

//-----------------------------------------------------------------------------

text* create_text()
{
    text* txt = salloc(sizeof(text));
    txt->font = 0;
    txt->str  = 0;
    txt->len  = 0;
    txt->alignment = TEXT_ALIGN_START;
    /* txt->limits = create_vec2(); */
    glGenBuffers(1, &txt->handle);
    checkGLError();

    return txt;
}

void  destroy_text_full(text* txt)
{
    sfree(txt->str);
    sfree(txt);
}

text* load_resource_to_text(resource_pair)
{
    stub(0);
}

void save_text_to_resource(resource_pair, text* txt)
{
    stub();
}

void text_set_font(text* txt, font* fnt)
{
    txt->font = fnt;
}

void text_set_string(text* txt, const char* str, ssize_t len)
{
    if(len == 0)
        len = strlen(str);
    sfree(txt->str);
    txt->str = strndup(str, len);
    txt->len = len;
}

void text_draw_color(text* txt, mat4 camera, mat4 transform, vec4 color)
{
    if(!txt->font) {
        error("Trying to render text without a font");
        return;
    }
    if(!txt->str) {
        error("Trying to render text without any text to render");
        return;
    }
    recalculate_buffer(txt);

    render_text_string_color(camera, transform, txt, color);
}
