#ifndef DF_GRAPHICS_TEXT
#define DF_GRAPHICS_TEXT

#include "text.hd"

#include "font.hd"
#include "mesh.hd"

text text_new(font f, const char* s);
void text_set_str(text t, const char* s);
mesh text_get_mesh(text t);

#endif
