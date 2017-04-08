#include<math.h>

#include "camera.h"
#include "interpolate.h"
#include "log/log.h"
#include "mainloop.h"
#include "matrix.h"
#include "mesh.h"
#include "quat.h"
#include "shader_init.h"
#include "shader.h"
#include "transform.h"
#include "vertex.hd"
#include "window.h"

GLFWwindow* win;
mesh m;
camera c;
shader s;
transform t;
vec4 start_color = (vec4){.data={1, 1, 1, 1}};
vec4 end_color = (vec4){.data={0, 0, 0, 1}};
float f = 0;

vt_pc tri[] = {
    {.position={.data={0, -0.85, 0}},.color={1,0,0}},
    {.position={.data={-1, 0.85, 0}},.color={0,1,0}},
    {.position={.data={1,  0.85, 0}},.color={0,0,1}},
};

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(s.id);

    transform t2 = camera_get_transform(c);
    transform_translate(t2, (vec3){.x=0.005}, true);
    transform_rotate(t, 0.05f, true);

    f += 0.025;

    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(c), transform_get_matrix(t)));
    shader_bind_uniform_name(s, "u_color", lerp(start_color, end_color, sin(f)));
    shader_bind_attribute_mesh(s, m, "i_pos", VT_POSITION, "i_color", VT_COLOR);

    mesh_render(m, GL_TRIANGLES);
    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

int main(int argc, char** argv) {
    win = window_new_default(1280, 720, "Test Window");
    shaders_init();
    s = shader_basic_untex_get();
    m = mesh_new(3, tri);
    c = camera_new();
    t = transform_new();
    /* transform_scale(t, (vec3){.x=0.2, .y=0.2, .z=0.2}, false); */
    /* glEnable(GL_CULL_FACE); */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    mainloop l = mainloop_new(loop);
    mainloop_run(l);
    mainloop_free(l);
    shaders_cleanup();
    mesh_free(m);

    window_free_final(win);

    return 0;
}
