#include "log/log.h"
#include "mainloop.h"
#include "matrix.h"
#include "mesh.h"
#include "shader_init.h"
#include "shader.h"
#include "vertex.hd"
#include "window.h"

GLFWwindow* win;
mesh m;
shader s;
mat4 t;
vec4 color = (vec4){.data={1, 0, 1, 1}};

vt_pc tri[] = {
    {.position={.data={0, 1, 0}}},
    {.position={.data={-1,   -1, 0}}},
    {.position={.data={1,   -1, 0}}},
};

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(s.id);

    shader_bind_uniform_name(s, "transform", t);
    shader_bind_uniform_name(s, "u_color", color);
    shader_bind_attribute_mesh(s, m, "i_pos", VT_POSITION);

    mesh_render(m, GL_TRIANGLES);
    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

int main(int argc, char** argv) {
    win = window_new_default(1280, 720, "Test Window");
    shaders_init();
    s = shader_basic_untex_get();
    m = mesh_new(3, tri);
    t = mat4_ident;

    mainloop l = mainloop_new(loop);
    mainloop_run(l);
    mainloop_free(l);
    shaders_cleanup();
    mesh_free(m);

    window_free_final(win);

    return 0;
}
