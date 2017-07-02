#include "camera.h"
#include "color.h"
#include "interpolate.h"
#include "mainloop.h"
#include "matrix.h"
#include "mesh.h"
#include "paths.h"
#include "shader_init.h"
#include "shader.h"
#include "texture_loader.h"
#include "vector.h"
#include "window.h"

GLFWwindow* win;
camera      c_main;
shader      s_default;
float       timer;
gltex       t_logo;
vec4        color;
projection_settings camera_settings = {
    .dims = { .x = 1280, .y = 720, .z = -1, .w = 1 },
    .is_ortho = true
};

bool loop(mainloop l, float dt) {
    timer += dt;
    color.a = lerp(0.0f, 1.0f, timer) + lerp(0.0f, -1.0f, timer - 2.5);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(s_default.id);
    shader_bind_uniform_name(s_default, "transform", mat4_mul(camera_get_vp(c_main), mat4_scale(mat4_ident, 400)));
    shader_bind_uniform_texture_name(s_default, "u_texture", t_logo, GL_TEXTURE0);
    shader_bind_uniform_name(s_default, "u_color", color);
    mesh_render(s_default, mesh_quad(), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);

    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win) && timer < 4.0f;
}
int main() {
    win = window_new_default(1280, 720, "Logo Demo");
    init_base_resource_path(NULL);
    shaders_init();

    s_default = shader_basic_tex_get();
    color = color_white;
    c_main = camera_new(camera_settings);
    char* path = assets_path("logo.png", NULL);
    t_logo = load_texture_gl(path);
    sfree(path);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mainloop_create_run(loop);

    glDeleteTextures(1, &t_logo.handle);
    camera_free(c_main);
    shaders_cleanup();
    resource_path_free();
    window_free_final(win);
    return 0;
}
