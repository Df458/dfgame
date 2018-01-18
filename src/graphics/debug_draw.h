#ifndef DF_GRAPHICS_DEBUG_DRAW
#define DF_GRAPHICS_DEBUG_DRAW
#include "matrix.h"
#include "vertex.hd"

void debug_draw_point(mat4 m, vt_pc v, int size);
void debug_draw_line(mat4 m, vt_pc start, vt_pc end, int size);

#endif
