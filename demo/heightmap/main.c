#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "color.h"
#include "data_loader.h"
#include "font_loader.h"
#include "input.h"
#include "log/log.h"
#include "mainloop.h"
#include "matrix.h"
#include "mesh.h"
#include "paths.h"
#include "shader_init.h"
#include "text.h"
#include "texture_loader.h"
#include "transform.h"
#include "window.h"

#define MESH_DIM 100

GLFWwindow* win;

camera      c_main;
camera      c_ui;
transform t_water;

shader      s_default;
shader      s_water;
shader      s_text;

gltex t_heightmap;
mesh  m_terrain;
text info_text;

action_id a_drag;
axis_id   a_horizontal;
axis_id   a_vertical;
axis_id   a_zoom;

vec2 control_point = { 0 };
float cam_distance = 35;

const char* const info_str =
"Controls\n"
"Left Click & Drag: Rotate camera\n"
"Scroll: Zoom";

vt_p mesh_data[square(MESH_DIM) * 6];

projection_settings camera_settings_main = {
    .dims = { .x = -6, .y = 3.5, .z = 0.1, .w = 1000 },
    .fov = 90,
    .is_ortho = false
};

void update_camera() {
    if(action_is_active(a_drag)) {
        control_point.x -= get_axis_value(a_horizontal) * 0.5f;
        control_point.y = clamp(control_point.y - (get_axis_value(a_vertical) * 5), 0, 25);
    }

    cam_distance += get_axis_value(a_zoom);

    vec2 vplane = (vec2) { .x = cos(control_point.y / 25.0f) * cam_distance, .y = sin(control_point.y / 25.0f) * cam_distance };
    vec3 v = (vec3){.x = 25 + (sin(control_point.x) * (vplane.x)), .y=25 + vplane.y, .z = 25 + (cos(control_point.x) * (vplane.x))};
    transform_translate(camera_get_transform(c_main), v, false);
    transform_rotate(camera_get_transform(c_main), (vec3){.x=-control_point.x}, false);
    transform_rotate(camera_get_transform(c_main), (vec3){.y=atan2(90 * (control_point.y / 25), cam_distance)}, true);
}
bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_camera();

    glUseProgram(s_default.id);
    shader_bind_uniform_name(s_default, "transform", camera_get_vp(c_main));
    shader_bind_uniform_texture_name(s_default, "u_texture", t_heightmap, GL_TEXTURE0);
    mesh_render(s_default, m_terrain, GL_TRIANGLES, "i_pos", VT_POSITION);

    glUseProgram(s_water.id);
    vec4 water_color = color_blue;
    water_color.a = 0.5f;
    shader_bind_uniform_name(s_water, "transform", mat4_mul(camera_get_vp(c_main), transform_get_matrix(t_water)));
    shader_bind_uniform_name(s_water, "u_color", water_color);
    mesh_render(s_water, mesh_quad(), GL_TRIANGLES, "i_pos", VT_POSITION);

    glUseProgram(s_text.id);
    vec2 offset = (vec2){ .x = -634, .y=354 };
    shader_bind_uniform_name(s_text, "u_color", color_white);
    text_draw(info_text, s_text, mat4_mul(camera_get_vp(c_ui), mat4_translate(mat4_ident, offset)));

    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

void init_resources() {
    init_base_resource_path(NULL);
    char* path = assets_path("Heightmap.png", NULL);
    t_heightmap = load_texture_gl(path);
    sfree(path);

    a_drag = input_add_mouse_button_action(0, NULL);
    a_horizontal = input_add_mouse_position_axis(false, 20.0, 0.05f, false);
    a_vertical = input_add_mouse_position_axis(true, 20.0, 0.05f, false);
    a_zoom = input_add_mouse_scroll_axis(true, 20.0, -0.5f, false);

    path = assets_path("heightmap.vert", NULL);
    char* vert = (char*)load_data_buffer(path, NULL);
    sfree(path);

    path = assets_path("heightmap.frag", NULL);
    char* frag = (char*)load_data_buffer(path, NULL);
    sfree(path);
    s_default = shader_new_vf((const char**)&vert, (const char**)&frag);
    shaders_init();
    s_water = shader_basic_untex_get();
    s_text = shader_basic_tex_get();

    path = assets_path("OpenSans-Regular.ttf", NULL);
    info_text = text_new(load_font(path, 16), info_str);
    text_set_align(info_text, TEXT_ALIGN_BOTTOM_LEFT);
    sfree(path);

}
void prepare_mesh() {
    for(uint16 i = 0; i < MESH_DIM; ++i) {
        for(uint16 j = 0; j < MESH_DIM; ++j) {
            mesh_data[(i * MESH_DIM + j) * 6 + 0] = (vt_p){.data={j + 0, 0, i + 0}};
            mesh_data[(i * MESH_DIM + j) * 6 + 1] = (vt_p){.data={j + 1, 0, i + 0}};
            mesh_data[(i * MESH_DIM + j) * 6 + 2] = (vt_p){.data={j + 1, 0, i + 1}};
            mesh_data[(i * MESH_DIM + j) * 6 + 3] = (vt_p){.data={j + 0, 0, i + 0}};
            mesh_data[(i * MESH_DIM + j) * 6 + 4] = (vt_p){.data={j + 1, 0, i + 1}};
            mesh_data[(i * MESH_DIM + j) * 6 + 5] = (vt_p){.data={j + 0, 0, i + 1}};
        }
    }
    m_terrain = mesh_new(square(MESH_DIM) * 6, mesh_data);
}
void cleanup_resources() {
    mesh_free(m_terrain);
    glDeleteTextures(1, &t_heightmap.handle);
    text_free(info_text, true);

    resource_path_free();
}

int main() {
    win = window_new_default(1280, 720, "Heightmap Demo");

    init_resources();

    c_main = camera_new(camera_settings_main);
    c_ui = window_create_2d_camera(win);

    t_water = transform_new();
    vec3 v = (vec3){.x=25, .y=17, .z=25};
    vec3 vr = (vec3){ .y=PI * 0.5f };
    transform_translate(t_water, v, false);
    transform_rotate(t_water, vr, false);
    transform_scale(t_water, 50, false);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    prepare_mesh();

    mainloop_create_run(loop);

    cleanup_resources();

    camera_free(c_main);
    camera_free(c_ui);
    window_free_final(win);
    shaders_cleanup();

    return 0;
}
