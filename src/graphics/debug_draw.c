// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "debug_draw.h"

#include "mesh.h"
#include "shader_init.h"

void debug_draw_point(mat4 mt, vt_pc v, int size) {
    glUseProgram(shader_debug_get().id);
    mesh me = mesh_new(1, &v, NULL);
    shader_bind_uniform_name(shader_debug_get(), "transform", mt);
    glPointSize(size);
    mesh_render(shader_debug_get(), me, GL_POINTS, "i_pos", VT_POSITION, "i_color", VT_COLOR);
    mesh_free(me);
}

void debug_draw_line(mat4 mt, vt_pc start, vt_pc end, int size) {
    glUseProgram(shader_debug_get().id);
    vt_pc data[2] = { start, end };

    mesh me = mesh_new(2, data, NULL);
    shader_bind_uniform_name(shader_debug_get(), "transform", mt);
    glLineWidth(size);
    mesh_render(shader_debug_get(), me, GL_LINES, "i_pos", VT_POSITION, "i_color", VT_COLOR, NULL);
    mesh_free(me);
}
