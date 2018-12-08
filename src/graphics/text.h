#ifndef DF_GRAPHICS_TEXT
#define DF_GRAPHICS_TEXT

#include "graphics/text.hd"
#include "graphics/font.hd"
#include "graphics/mesh.hd"
#include "graphics/shader.hd"
#include "math/alignment.h"
#include "math/matrix.hd"

typedef enum text_wrap {
    TEXT_WRAP_CHARACTER = 0,
    TEXT_WRAP_WORD,

    TEXT_WRAP_LAST    = TEXT_WRAP_WORD,
    TEXT_WRAP_DEFAULT = TEXT_WRAP_CHARACTER
} text_wrap;

// Create a new text object with the provided string format
text text_new(font f, const char* s, ...);

//  Frees the text
#define text_free(t, f) { _text_free(t, f); t = NULL; }
void _text_free(text t, bool free_src);

// Update the text object's contents with the provided string format
void text_set_str(text t, const char* s, ...);

// Get the current text set to the text object
char* text_get_str(const text t);

// Get the text object's generated mesh
mesh text_get_mesh(const text t);

// Gets/sets the text's alignment
void text_set_align(text t, alignment_2d align);
alignment_2d text_get_align(const text t);

// Gets/sets the text's wrap method
void text_set_wrap(const text t, text_wrap wrap);
text_wrap text_get_wrap(text t);

// Gets/sets the text's maximum width for wrapping.
// If the value is 0, the text won't wrap.
void text_set_max_width(text t, float width);
float text_get_max_width(const text t);

// Gets/sets the text object's font
void text_set_font(text t, font f);
font text_get_font(const text t);

// Helper function to render text.
void text_draw(const text t, shader s, mat4 m);

#endif
