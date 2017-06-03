#include <math.h>
#include <stdlib.h>

#include "audio_loader.h"
#include "audio_player.h"
#include "audio_source.h"
#include "audio_system.h"
#include "camera.h"
#include "input.h"
#include "interpolate.h"
#include "log/log.h"
#include "mainloop.h"
#include "matrix.h"
#include "memory/alloc.h"
#include "mesh.h"
#include "mesh_loader.h"
#include "paths.h"
#include "quat.h"
#include "shader_init.h"
#include "shader.h"
#include "texture_loader.h"
#include "transform.h"
#include "types.h"
#include "vertex.hd"
#include "window.h"

GLFWwindow* win;

camera player_camera;
transform player_transform;
audio_player step_players[7];
audio_player jump_player;

mesh radio_mesh;
gltex radio_tex;
transform radio_transform;
audio_player music_player;

mesh suzanne_mesh;
gltex suzanne_tex;
transform suzanne_transform;

mesh df_mesh;
gltex df_tex;
transform df_transform;

mesh level_mesh;
gltex level_tex;

shader s;

action_id jump_action;
axis_id mouselook_axis_x;
axis_id mouselook_axis_y;
axis_id movement_axis_x;
axis_id movement_axis_y;

vec2 camera_orient = {0};
vec2 camera_target = {0};
float y_vel = 0;
bool is_ortho = false;
float fov = 60;
float fov_target = 60;
float dist = 0;

projection_settings build_projection() {
    projection_settings settings = (projection_settings) {
        .dims=(vec4) {
            .x=is_ortho ? -6 : 6,
            .y=3.5,
            .z=0.1,
            .w=100.0
        },
        .fov = fov,
        .is_ortho = is_ortho
    };
    return settings;
}

void jump(action_id action, void* user) {
    float y = transform_get_position(player_transform).y;
    if(y <= 1.86) {
        y_vel = 0.25;
        audio_player_set_playing(jump_player, true);
    }
}
void toggle_ortho(action_id action, void* user) {
    is_ortho = !is_ortho;
    camera_set_projection(player_camera, build_projection());
}
void toggle_fov(action_id action, void* user) {
    if(fov_target == 60)
        fov_target = 120;
    else
        fov_target = 60;
}

bool loop(mainloop l, float dt) {
    if(!audio_player_get_playing(music_player))
        audio_player_set_playing(music_player, true);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(get_axis_value(mouselook_axis_x) != 0)
        camera_target.x = camera_orient.x + degtorad(get_axis_value(mouselook_axis_x));
    if(get_axis_value(mouselook_axis_y) != 0)
        camera_target.y = camera_orient.y + degtorad(get_axis_value(mouselook_axis_y));
    camera_orient.x = lerp(camera_orient.x, camera_target.x, 0.5f);
    camera_orient.y = clamp(lerp(camera_orient.y, camera_target.y, 0.5f), -PI*0.5f, PI*0.5f);
    if(fov != fov_target) {
        fov = lerp(fov, fov_target, 0.15);
        camera_set_projection(player_camera, build_projection());
    }
    transform_rotate(player_transform, (vec3){.x=camera_orient.x}, false);
    transform_rotate(player_transform, (vec3){.y=camera_orient.y}, true);

    vec3 position = (vec3){ .x=(sin(camera_orient.x) * get_axis_value(movement_axis_y)) + (cos(camera_orient.x) * get_axis_value(movement_axis_x)), .z=-cos(camera_orient.x) * get_axis_value(movement_axis_y) + (sin(camera_orient.x) * get_axis_value(movement_axis_x)) };
    vec3 p = transform_get_position(player_transform);
    if(p.y + y_vel <= 1.85 && p.x > -2 && p.x < 39 && p.z > -21 && p.z < 21) {
        position.y = 1.85-p.y;
        y_vel = 0;

        if(position.x != 0 || position.z != 0)
            dist += dt * 5;
        if(dist > 1.1) {
            dist = 0;
            audio_player_set_playing(step_players[rand() % 7], true);
        }
    } else {
        position.y = y_vel;
        y_vel -= 0.015;
    }
    transform_translate(player_transform, position, true);

    quat q = transform_get_orientation(player_transform);
    vec3 forward = (vec3){.x=0, .y=0, .z=-1};
    vec3 up = (vec3){.x=0, .y=1, .z=0};
    forward = vec_rotate(forward, q);
    forward.x *= -1;
    up = vec_rotate(up, q);
    audio_update_listener(transform_get_position(player_transform), forward, up, position);
    for(int i = 0; i < 7; ++i)
        audio_player_update(step_players[i], dt);
    audio_player_update(jump_player, dt);
    audio_player_update(music_player, dt);

    glUseProgram(s.id);
    shader_bind_uniform_name(s, "transform", camera_get_vp(player_camera));
    shader_bind_attribute_mesh(s, level_mesh, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", level_tex, GL_TEXTURE0);
    mesh_render(level_mesh, GL_TRIANGLES);

    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), transform_get_matrix(radio_transform)));
    shader_bind_attribute_mesh(s, radio_mesh, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", radio_tex, GL_TEXTURE0);
    mesh_render(radio_mesh, GL_TRIANGLES);

    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), transform_get_matrix(suzanne_transform)));
    shader_bind_attribute_mesh(s, suzanne_mesh, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", suzanne_tex, GL_TEXTURE0);
    mesh_render(suzanne_mesh, GL_TRIANGLES);

    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), transform_get_matrix(df_transform)));
    shader_bind_attribute_mesh(s, df_mesh, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", df_tex, GL_TEXTURE0);
    mesh_render(df_mesh, GL_TRIANGLES);

    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

int main(int argc, char** argv) {
    // Set up graphics
    win = window_new_default(1280, 720, "Test Window");
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    shaders_init();
    audio_init();
    init_base_resource_path(NULL);

    s = shader_basic_tex_get();

    // Load meshes
    radio_mesh = load_mesh(assets_path("radio.obj", NULL));
    suzanne_mesh = load_mesh(assets_path("centerpiece.obj", NULL));
    df_mesh = load_mesh(assets_path("plane.obj", NULL));
    level_mesh = load_mesh(assets_path("room.obj", NULL));

    // Load audio
    for(int i = 0; i < 7; ++i) {
        char name[9] = "";
        sprintf(name, "step%1d.wav", i + 1);
        step_players[i] = audio_player_new(load_audio_source(assets_path(name, NULL), true));
    }
    jump_player = audio_player_new(load_audio_source(assets_path("jump.wav", NULL), true));
    music_player = audio_player_new(load_audio_source(assets_path("music.ogg", NULL), false));
    audio_player_set_distances(music_player, 10, 30);

    // Load textures
    radio_tex = load_texture_gl(assets_path("radio.png", NULL));
    suzanne_tex = load_texture_gl(assets_path("suzanne.png", NULL));
    df_tex = load_texture_gl(assets_path("dflogo.png", NULL));
    level_tex = load_texture_gl(assets_path("RoomTex.png", NULL));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set up player
    player_camera = camera_new(build_projection());
    player_transform = camera_get_transform(player_camera);
    transform_translate(player_transform, (vec3){.x=5.0,.y=1.85,.z=0}, false);
    vec3 r = (vec3){.x=degtorad(90),.y=degtorad(-4),.z=0 };
    camera_orient = r.xy;
    camera_target = r.xy;
    transform_rotate(player_transform, r, false);

    // Set up transforms
    radio_transform   = transform_new();
    transform_translate(radio_transform, (vec3){.x=0.97,.y=0.95,.z=0}, false);
    transform_scale(radio_transform, (vec3){.x=0.4,.y=0.4,.z=0.4}, false);
    audio_player_set_translation(music_player, transform_get_position(radio_transform));

    suzanne_transform = transform_new();
    df_transform      = transform_new();
    vec3 r3 = (vec3){.x=0,.y=degtorad(180), .z=0 };
    transform_rotate_2d(df_transform, degtorad(90), false);
    transform_rotate(df_transform, r3, true);
    transform_translate(df_transform, (vec3){.x=6,.y=2,.z=0}, false);
    transform_scale(df_transform, (vec3){.x=.1,.y=.1,.z=.1}, false);

    // Set up input
    jump_action = input_add_key_action(GLFW_KEY_SPACE, &(action_event){.func=jump});
    input_add_key_action(GLFW_KEY_Q, &(action_event){.func=toggle_ortho});
    input_add_key_action(GLFW_KEY_E, &(action_event){.func=toggle_fov});
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

    mesh_free(radio_mesh);
    mesh_free(suzanne_mesh);
    mesh_free(df_mesh);
    mesh_free(level_mesh);

    window_free_final(win);

    return 0;
}
