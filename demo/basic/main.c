#include <math.h>
#include <stdlib.h>

#include "audio_loader.h"
#include "audio_player.h"
#include "audio_source.h"
#include "audio_system.h"
#include "camera.h"
#include "font_loader.h"
#include "font.h"
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
#include "renderpass.h"
#include "shader_init.h"
#include "shader.h"
#include "sprite_loader.h"
#include "spriteset.h"
#include "sprite.h"
#include "text.h"
#include "texture_loader.h"
#include "transform.h"
#include "types.h"
#include "vertex.hd"
#include "window.h"

GLFWwindow* win;

camera player_camera;
transform player_transform;

font test_font;
text test_text;
text shader_text;
spriteset test_spriteset;
sprite test_sprite;
renderpass test_rpass1;
renderpass test_rpass2;
renderpass test_rpass3;

shader s, s_gray, s_blur, s_wave;

action_id hello_action;
action_id gray_action;
action_id blur_action;
action_id wave_action;
action_id full_action;
axis_id movement_axis_x;
axis_id movement_axis_y;

uint8 effects = 0;

vec2 camera_orient = {0};
vec2 camera_target = {0};
vec2 ball_position = {0};
float y_vel = 0;
bool is_ortho = true;
float fov = 60;
float fov_target = 60;
float dist = 0;
float val = 0;
int v = 0;

vec2 offset = (vec2){.x = 0, .y = 0};
float offset_timer;

const char* shader_strs[] = {
    "Normal",
    "Inverted Colors",
    "Blur",
    "Wave",
    "Everything"
};

projection_settings build_projection() {
    projection_settings settings = (projection_settings) {
        .dims=(vec4) {
            .x=1280,
            .y=720,
            .z=-1,
            .w=100.0
        },
        .fov = fov,
        .is_ortho = is_ortho
    };
    return settings;
}

void a_effect(action_id action, void* user) {
    effects = (uint8)user;
    text_set_str(shader_text, shader_strs[effects]);
}
void a_text(action_id action, void* user) {
    v++;
}

bool loop(mainloop l, float dt) {
    float vx = get_axis_value(movement_axis_x);
    float vy = get_axis_value(movement_axis_y);
    if(sprite_get_anim_id(test_sprite) != 2) {
        if(vx != 0 || vy != 0) {
            sprite_set_animation(test_sprite, "move", false);
            if(vx != 0)
                sprite_set_orientation(test_sprite, vx < 0 ? 3 : 1);
            else
                sprite_set_orientation(test_sprite, vy < 0 ? 0 : 2);
        } else {
            sprite_set_animation(test_sprite, "move", true);
        }
    }

    ball_position = vec2_add(ball_position, (vec2){.x=vx, .y=vy});

    val += dt;

    char data[26];
    snprintf(data, 25, "Hello, %d time%s", v, v > 1 ? "s!" : ".");
    text_set_str(test_text, data);

    quat q = transform_get_orientation(player_transform);
    vec3 forward = (vec3){.x=0, .y=0, .z=-1};
    vec3 up = (vec3){.x=0, .y=1, .z=0};
    forward = vec_rotate(forward, q);
    forward.x *= -1;
    up = vec_rotate(up, q);

    sprite_update(test_sprite, dt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(effects)
        renderpass_start(test_rpass1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s.id);
    vec2 v0 = (vec2){.x=0,.y=0};
    vec2 v1 = (vec2){.x=1,.y=1};
    if(v > 0) {
        if(offset_timer < 1) {
            offset_timer += dt * 16;
            offset = (vec2){.x=lerp(4, 16, offset_timer), .y=lerp(-4, -16, offset_timer)};
        }
        shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), mat4_translate(mat4_ident, vec_add(ball_position, offset))));
        shader_bind_uniform_texture_name(s, "u_texture", font_get_texture(test_font), GL_TEXTURE0);
        shader_bind_uniform_name(s, "uv_offset", v0);
        shader_bind_uniform_name(s, "uv_scale", v1);
        mesh_render(s, text_get_mesh(test_text), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    }

    aabb_2d box = sprite_get_box(test_sprite);
    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), mat4_translate(mat4_scale(mat4_ident, box.dimensions), ball_position)));
    shader_bind_uniform_texture_name(s, "u_texture", sprite_get_texture(test_sprite), GL_TEXTURE0);
    box.vec = vec4_mul(box.vec, 1.0f / sprite_get_texture(test_sprite).width);
    shader_bind_uniform_name(s, "uv_offset", box.position);
    shader_bind_uniform_name(s, "uv_scale", box.dimensions);
    mesh_render(s, mesh_quad(), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);

    switch(effects) {
        case 1:
            renderpass_next(NULL, s_gray);
        break;
        case 2:
            renderpass_next(NULL, s_blur);
        break;
        case 3:
            glUseProgram(s_wave.id);
            shader_bind_uniform_name(s_wave, "u_time", val);
            renderpass_next(NULL, s_wave);
        break;
        case 4:
            renderpass_next(test_rpass2, s_gray);
            renderpass_next(test_rpass3, s_wave);
            glUseProgram(s_wave.id);
            shader_bind_uniform_name(s_wave, "u_time", val);
            renderpass_next(NULL, s_blur);
        break;
    }

    if(effects > 0) {
        glUseProgram(s.id);
        vec2 corner = (vec2){.x=-620, .y=340};
        shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), mat4_translate(mat4_ident, corner)));
        shader_bind_uniform_texture_name(s, "u_texture", font_get_texture(test_font), GL_TEXTURE0);
        shader_bind_uniform_name(s, "uv_offset", v0);
        shader_bind_uniform_name(s, "uv_scale", v1);
        mesh_render(s, text_get_mesh(shader_text), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    }

    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

int main(int argc, char** argv) {
    // Set up graphics
    win = window_new_default(1280, 720, "Test Window");
    shaders_init();
    audio_init();
    init_base_resource_path(NULL);

    s = shader_basic_tex_get();
    s_gray = shader_invert_get();
    s_wave = shader_wave_get();
    s_blur = shader_blur_get();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    test_font = load_font(assets_path("test.ttf", NULL), 32);
    test_text = text_new(test_font, "Hello");
    shader_text = text_new(test_font, shader_strs[0]);

    test_spriteset = load_spriteset(assets_path("test.xml", NULL));
    test_sprite = sprite_new(test_spriteset);
    sprite_set_animation(test_sprite, "move", true);
    sprite_set_playing(test_sprite, true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.5, 0.5, 0.5, 1);

    // Set up player
    player_camera = camera_new(build_projection());
    player_transform = camera_get_transform(player_camera);

    // Set up input
    movement_axis_x = input_add_key_axis(GLFW_KEY_D, 0, 2.0f, false);
    input_bind_key_axis(GLFW_KEY_A, movement_axis_x, -2.0f);
    movement_axis_y = input_add_key_axis(GLFW_KEY_W, 0, -2.0f, false);
    input_bind_key_axis(GLFW_KEY_S, movement_axis_y, 2.0f);

    input_add_key_action(GLFW_KEY_0, as_event(action_event, a_effect, (void*)0));
    input_add_key_action(GLFW_KEY_1, as_event(action_event, a_effect, (void*)1));
    input_add_key_action(GLFW_KEY_2, as_event(action_event, a_effect, (void*)2));
    input_add_key_action(GLFW_KEY_3, as_event(action_event, a_effect, (void*)3));
    input_add_key_action(GLFW_KEY_4, as_event(action_event, a_effect, (void*)4));
    input_add_key_action(GLFW_KEY_SPACE, as_event(action_event, a_text, NULL));

    test_rpass1 = renderpass_new(1280, 720);
    test_rpass2 = renderpass_new(1280, 720);
    test_rpass3 = renderpass_new(1280, 720);

    // Run the demo
    mainloop l = mainloop_new(loop);
    mainloop_run(l);

    // Cleanup
    mainloop_free(l);
    shaders_cleanup();

    window_free_final(win);

    return 0;
}
