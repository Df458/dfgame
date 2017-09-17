#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "audio_loader.h"
#include "audio_player.h"
#include "audio_source.h"
#include "audio_system.h"
#include "camera.h"
#include "color.h"
#include "control.h"
#include "container/array.h"
#include "font_loader.h"
#include "input.h"
#include "interpolate.h"
#include "mainloop.h"
#include "mesh.h"
#include "paths.h"
#include "shader_init.h"
#include "sprite.h"
#include "sprite_loader.h"
#include "spriteset.h"
#include "text.h"
#include "transform.h"
#include "window.h"

GLFWwindow* win;
camera      c_main;
camera      c_ui;
shader      s_default;
shader      s_planet;
shader      s_contrast;
shader      s_bloom;

axis_id   a_zoom;
action_id a_select;

text info_text;

mesh sun_mesh;

const char* const info_str =
"Controls\n"
"Left Click: Select/Deselect Planet\n"
"Scroll: Zoom";

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s_planet.id);

    shader_bind_uniform_name(s_planet, "u_color", color_white);
    shader_bind_uniform_name(s_planet, "u_transform", camera_get_vp(c_main));
    mesh_render(s_planet, sun_mesh, GL_TRIANGLES, "i_pos", VT_POSITION);

    glUseProgram(s_default.id);

    vec2 offset = (vec2){ .x = -634, .y=354 };
    shader_bind_uniform_name(s_default, "u_color", color_white);
    text_draw(info_text, s_default, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, offset)));

    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

mesh generate_circle(uint points, float r) {
    mesh circle_mesh = mesh_new_type(points + 1, VT_POSITION | VT_NORMAL, NULL);
    vt_pn* data = mesh_get_data(circle_mesh);

    data[0] = (vt_pn) {
            .position = (vec3) { 0 },
            .normal = (vec3) { .z = 1 }
        };
    for(uint i = 1; i <= points; ++i) {
        float angle = 2 * PI / points * i;
        data[i] = (vt_pn) {
            .position = (vec3) {
                .x = cos(angle) * r,
                .y = sin(angle) * r,
            },
            .normal = (vec3) {
                .x = cos(angle),
                .y = sin(angle),
            }
        };
    }

    mesh_update(circle_mesh);
    return circle_mesh;
}

int main() {
    win = window_new_default(1280, 720, "Planets Demo");
    shaders_init();

    s_default = shader_basic_tex_get();
    s_planet = shader_basic_untex_get();
    c_main = window_create_2d_camera(win);
    c_ui = window_create_2d_camera(win);

    init_base_resource_path(NULL);
    char* path = assets_path("OpenSans-Regular.ttf", NULL);
    info_text = text_new(load_font(path, 16), info_str);
    text_set_align(info_text, TEXT_ALIGN_BOTTOM_LEFT);
    sfree(path);
    resource_path_free();

    sun_mesh = generate_circle(20, 50);

    srand(time(NULL));

    mainloop_create_run(loop);

    text_free(info_text, true);
    camera_free(c_main);
    camera_free(c_ui);
    shaders_cleanup();
    window_free_final(win);

    return 0;
}
