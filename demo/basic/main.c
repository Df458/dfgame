#include <math.h>
#include <stdlib.h>

#include "camera.h"
#include "input.h"
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
camera player_camera;
transform player_transform;
shader s;
vec4 start_color = (vec4){.data={2, 2, 2, 1}};
vec4 end_color = (vec4){.data={0.5, 0.5, 0.5, 1}};
float timer = 0;
action_id jump_action;
axis_id test_axis;
axis_id mouselook_axis_x;
axis_id mouselook_axis_y;
axis_id movement_axis_x;
axis_id movement_axis_y;

vec2 camera_orient = {0};
float y_vel = 0;

vt_pc tri[] = {
    {.position={.data={1,   -0.85, -1}}, .color={1,0,1}},
    {.position={.data={0,    0.85, -1}}, .color={0,1,1}},
    {.position={.data={2,    0.85, -1}}, .color={0,0,1}},
    {.position={.data={-1,  -0.85, -3}}, .color={1,0,0}},
    {.position={.data={-2,   0.85,  -3}},.color={1,1,0}},
    {.position={.data={0,    0.85, -3}}, .color={1,0,1}},
    {.position={.data={1,    0.15, -5}}, .color={0,0,1}},
    {.position={.data={0,    1.85, -5}}, .color={0,1,1}},
    {.position={.data={2,    1.85, -5}}, .color={1,0,1}},
    {.position={.data={-1,  -1.85, -7}}, .color={1,0,1}},
    {.position={.data={-2,  -0.15,  -7}},.color={1,1,0}},
    {.position={.data={0,   -0.15, -7}}, .color={1,0,0}},
    {.position={.data={0,   -0.85, -9}}, .color={1,1,0}},
    {.position={.data={-1,   0.85, -9}}, .color={0,1,0}},
    {.position={.data={1,    0.85, -9}}, .color={0,1,1}},

    {.position={.data={-10,  -1.85, 10}}, .color={1,0,1}},
    {.position={.data={ 10,  -1.85, 10}}, .color={1,1,0}},
    {.position={.data={ 10,  -1.85,-10}}, .color={1,0,0}},
    {.position={.data={-10,  -1.85, 10}}, .color={1,1,0}},
    {.position={.data={-10,  -1.85,-10}}, .color={0,1,0}},
    {.position={.data={ 10,  -1.85,-10}}, .color={0,1,1}},
};

void jump(action_id action, void* user) {
    float y = transform_get_position(player_transform).y;
    if(y <= 0)
        y_vel = 0.25;
}

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(s.id);

    timer += dt;

    camera_orient.x += degtorad(get_axis_value(mouselook_axis_x));
    camera_orient.y = clamp(camera_orient.y + degtorad(get_axis_value(mouselook_axis_y)), -PI*.5, PI*.5);
    transform_rotate(player_transform, (vec3){.x=camera_orient.x}, false);
    transform_rotate(player_transform, (vec3){.y=camera_orient.y}, true);

    vec3 position = (vec3){ .x=(sin(camera_orient.x) * get_axis_value(movement_axis_y)) + (cos(camera_orient.x) * get_axis_value(movement_axis_x)), .z=-cos(camera_orient.x) * get_axis_value(movement_axis_y) + (sin(camera_orient.x) * get_axis_value(movement_axis_x)) };
    vec3 p = transform_get_position(player_transform);
    if(p.y + y_vel <= 0 && abs(p.x) < 10 && abs(p.z) < 10) {
        position.y = -p.y;
        y_vel = 0;
    } else {
        position.y = y_vel;
        y_vel -= 0.015;
    }
    transform_translate(player_transform, position, true);

    shader_bind_uniform_name(s, "transform", camera_get_vp(player_camera));
    shader_bind_uniform_name(s, "u_color", lerp(start_color, end_color, sin(timer)));
    shader_bind_attribute_mesh(s, m, "i_pos", VT_POSITION, "i_color", VT_COLOR);

    mesh_render(m, GL_TRIANGLES);
    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

int main(int argc, char** argv) {
    // Set up graphics
    win = window_new_default(1280, 720, "Test Window");
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    shaders_init();
    s = shader_basic_untex_get();
    m = mesh_new(21, tri);
    projection_settings settings = (projection_settings) {
        .dims=(vec4) {
            .x=12,
            .y=7,
            .z=0.1,
            .w=100.0
        },
        .fov = 60,
        .is_ortho = false
    };
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set up player
    player_camera = camera_new(settings);
    player_transform = camera_get_transform(player_camera);

    // Set up input
    jump_action = input_add_key_action(GLFW_KEY_SPACE, &(action_event){.func=jump});
    mouselook_axis_x = input_add_mouse_position_axis(false, 0, -1.0f);
    mouselook_axis_y = input_add_mouse_position_axis(true, 0, -1.0f);
    movement_axis_x = input_add_key_axis(GLFW_KEY_D, 0, 0.1f);
    input_bind_key_axis(GLFW_KEY_A, movement_axis_x, -0.1f);
    movement_axis_y = input_add_key_axis(GLFW_KEY_W, 0, 0.1f);
    input_bind_key_axis(GLFW_KEY_S, movement_axis_y, -0.1f);

    // Run the demo
    mainloop l = mainloop_new(loop);
    mainloop_run(l);

    // Cleanup
    mainloop_free(l);
    shaders_cleanup();
    mesh_free(m);
    window_free_final(win);

    return 0;
}
