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
spriteset test_spriteset;
sprite test_sprite;
renderpass test_rpass1;
renderpass test_rpass2;

shader s, s2, s3;

action_id jump_action;
axis_id mouselook_axis_x;
axis_id mouselook_axis_y;
axis_id movement_axis_x;
axis_id movement_axis_y;

vec2 camera_orient = {0};
vec2 camera_target = {0};
vec2 ball_position = {0};
float y_vel = 0;
bool is_ortho = true;
float fov = 60;
float fov_target = 60;
float dist = 0;
float val = 0;

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
    snprintf(data, 25, "Hello, %2d World", rand() % 100);
    test_text = text_new(test_font, data);

    quat q = transform_get_orientation(player_transform);
    vec3 forward = (vec3){.x=0, .y=0, .z=-1};
    vec3 up = (vec3){.x=0, .y=1, .z=0};
    forward = vec_rotate(forward, q);
    forward.x *= -1;
    up = vec_rotate(up, q);

    sprite_update(test_sprite, dt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* renderpass_start(test_rpass1); */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s.id);
    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), mat4_ident));
    shader_bind_attribute_mesh(s, text_get_mesh(test_text), "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", font_get_texture(test_font), GL_TEXTURE0);
    vec2 v0 = (vec2){.x=0,.y=0};
    vec2 v1 = (vec2){.x=1,.y=1};
    shader_bind_uniform_name(s, "uv_offset", v0);
    shader_bind_uniform_name(s, "uv_scale", v1);
    mesh_render(text_get_mesh(test_text), GL_TRIANGLES);

    aabb_2d box = sprite_get_box(test_sprite);
    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), mat4_translate(mat4_scale(mat4_ident, box.dimensions), ball_position)));
    shader_bind_attribute_mesh(s, mesh_quad(), "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", sprite_get_texture(test_sprite), GL_TEXTURE0);
    box.vec = vec4_mul(box.vec, 1.0f / sprite_get_texture(test_sprite).width);
    shader_bind_uniform_name(s, "uv_offset", box.position);
    shader_bind_uniform_name(s, "uv_scale", box.dimensions);
    mesh_render(mesh_quad(), GL_TRIANGLES);

    glUseProgram(s2.id);
    shader_bind_uniform_name(s2, "u_time", val);
    /* renderpass_next(test_rpass2, s2); */
    /* renderpass_next(NULL, s3); */

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
    s2 = shader_wave_get();
    s3 = shader_blur_get();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    test_font = load_font(assets_path("test.ttf", NULL), 64);
    test_text = text_new(test_font, "Hello, World");

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

    test_rpass1 = renderpass_new(1280, 720);
    test_rpass2 = renderpass_new(1280, 720);

    // Run the demo
    mainloop l = mainloop_new(loop);
    mainloop_run(l);

    // Cleanup
    mainloop_free(l);
    shaders_cleanup();

    window_free_final(win);

    return 0;
}
