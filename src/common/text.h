#ifndef DF_TEXT_H
#define DF_TEXT_H
#include "font.h"
#include "matrix.h"
#include "resource_def.h"

enum text_alignment
{
    TEXT_ALIGN_START = 0,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_END,
};

typedef struct text
{
    font*   font;
    char*   str;
    ssize_t len;
    int     buflen;
    int     alignment;
    vec2    limits;
    GLuint  handle;
}
text;

text* create_text();
void  destroy_text_full(text* txt);
#define destroy_text(txt) { destroy_text_full(txt); txt = 0; }
text* load_resource_to_text(resource_pair);
void save_text_to_resource(resource_pair, text* txt);

void text_set_font(text* txt, font* fnt);
void text_set_string(text* txt, const char* str, ssize_t len);
void text_draw_color(text* txt, mat4 camera, mat4 transform, vec4 color);
#define text_draw(txt, camera, transform) text_draw_color(txt, camera, transform, create_vec4_data(1, 1, 1, 1))

#endif
