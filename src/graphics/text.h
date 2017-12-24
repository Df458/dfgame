#ifndef DF_GRAPHICS_TEXT
#define DF_GRAPHICS_TEXT

#include "text.hd"

#include "font.hd"
#include "mesh.hd"
#include "matrix.hd"
#include "shader.hd"

typedef enum text_alignment {
    TEXT_ALIGN_TOP,
    TEXT_ALIGN_BOTTOM,
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_RIGHT,

    TEXT_ALIGN_TOP_LEFT,
    TEXT_ALIGN_TOP_RIGHT,
    TEXT_ALIGN_BOTTOM_LEFT,
    TEXT_ALIGN_BOTTOM_RIGHT,

    TEXT_ALIGN_CENTER,

    TEXT_ALIGN_DEFAULT = TEXT_ALIGN_TOP_LEFT
} text_alignment;

text text_new(font f, const char* s);
void text_set_str(text t, const char* s, ...);
mesh text_get_mesh(text t);

void text_set_align(text t, text_alignment align);
text_alignment text_get_align(text t);

font text_get_font(text t);
void text_set_font(text t, font f);

void text_draw(text t, shader s, mat4 m);

#define text_free(t, f) { _text_free(t, f); t = NULL; }
void _text_free(text t, bool free_src);

#endif
