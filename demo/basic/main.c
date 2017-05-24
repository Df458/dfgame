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
mesh tri_mesh;
mesh circle_mesh;
mesh level_mesh;
camera player_camera;
transform player_transform;
transform ball1_transform;
transform ball2_transform;
transform tri_transform;
shader s, s2;
vec4 start_color = (vec4){.data={2, 2, 2, 1}};
vec4 end_color = (vec4){.data={0.5, 0.5, 0.5, 1}};
float timer = 0;
action_id jump_action;
axis_id test_axis;
axis_id mouselook_axis_x;
axis_id mouselook_axis_y;
axis_id movement_axis_x;
axis_id movement_axis_y;
gltex png_tex;
gltex jpeg_tex;
gltex tiff_tex;
gltex tga_tex;
audio_player aplayer;

vec2 camera_orient = {0};
vec2 camera_target = {0};
float y_vel = 0;
bool is_ortho = false;
float fov = 60;
float fov_target = 60;

vt_pt tri[] = {
    {.position={.data={ 0,   1, 0}}, .uv={.data={0.5,1}}},
    {.position={.data={-1,  -1, 0}}, .uv={.data={0,0}}},
    {.position={.data={ 1,  -1, 0}}, .uv={.data={1,0}}}
};
vt_pc level[] = {
// Floor    
    {.position={.data={  7, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={ 14, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={ 14, 0,  0}}, .color={.data={1,1,1}}},
    {.position={.data={  7, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={  7, 0,  0}}, .color={.data={1,1,1}}},
    {.position={.data={ 14, 0,  0}}, .color={.data={1,1,1}}},

    {.position={.data={ 21, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={ 14, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={ 14, 0,  7}}, .color={.data={1,1,1}}},
    {.position={.data={ 21, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={ 21, 0,  7}}, .color={.data={1,1,1}}},
    {.position={.data={ 14, 0,  7}}, .color={.data={1,1,1}}},

    {.position={.data={  7, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={  0, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={  0, 0,  7}}, .color={.data={1,1,1}}},
    {.position={.data={  7, 0, 14}}, .color={.data={1,1,1}}},
    {.position={.data={  7, 0,  7}}, .color={.data={1,1,1}}},
    {.position={.data={  0, 0,  7}}, .color={.data={1,1,1}}},

// Ceiling
    {.position={.data={  7, 5, 21}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 14, 5, 21}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 14, 5,  0}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  7, 5, 21}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  7, 5,  0}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 14, 5,  0}}, .color={.data={0.8,0.8,0.8}}},

    {.position={.data={ 21, 5, 14}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 14, 5, 14}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 14, 5,  7}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 21, 5, 14}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 21, 5,  7}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={ 14, 5,  7}}, .color={.data={0.8,0.8,0.8}}},

    {.position={.data={  7, 5, 14}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  0, 5, 14}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  0, 5,  7}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  7, 5, 14}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  7, 5,  7}}, .color={.data={0.8,0.8,0.8}}},
    {.position={.data={  0, 5,  7}}, .color={.data={0.8,0.8,0.8}}},

// North Alcove
    {.position={.data={  7, 0, 0}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={  7, 5, 0}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 14, 5, 0}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={  7, 0, 0}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 14, 0, 0}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 14, 5, 0}}, .color={.data={0.7,0.7,0.7}}},

    {.position={.data={ 7, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 5,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 5,  0}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 0,  0}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 5,  0}}, .color={.data={0.5,0.5,0.5}}},

    {.position={.data={ 14, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 5,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 5,  0}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 0,  0}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 5,  0}}, .color={.data={0.5,0.5,0.5}}},

// West Alcove
    {.position={.data={ 0, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 0, 5,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 0, 5, 14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 0, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 0, 0, 14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 0, 5, 14}}, .color={.data={0.5,0.5,0.5}}},

    {.position={.data={ 7, 0,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 7, 5,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 0, 5,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 7, 0,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 0, 0,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 0, 5,  7}}, .color={.data={0.7,0.7,0.7}}},

    {.position={.data={  7, 0, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  7, 5, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  0, 5, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  7, 0, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  0, 0, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  0, 5, 14}}, .color={.data={0.3,0.3,0.3}}},

// East Alcove
    {.position={.data={ 21, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 21, 5,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 21, 5, 14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 21, 0,  7}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 21, 0, 14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 21, 5, 14}}, .color={.data={0.5,0.5,0.5}}},

    {.position={.data={ 14, 0,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 14, 5,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 21, 5,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 14, 0,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 21, 0,  7}}, .color={.data={0.7,0.7,0.7}}},
    {.position={.data={ 21, 5,  7}}, .color={.data={0.7,0.7,0.7}}},

    {.position={.data={ 14, 0, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 14, 5, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 21, 5, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 14, 0, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 21, 0, 14}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 21, 5, 14}}, .color={.data={0.3,0.3,0.3}}},

// South "Alcove"
    {.position={.data={  7, 0, 21}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  7, 5, 21}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 14, 5, 21}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={  7, 0, 21}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 14, 0, 21}}, .color={.data={0.3,0.3,0.3}}},
    {.position={.data={ 14, 5, 21}}, .color={.data={0.3,0.3,0.3}}},

    {.position={.data={ 7, 0,  14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 5,  14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 5,  21}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 0,  14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 0,  21}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 7, 5,  21}}, .color={.data={0.5,0.5,0.5}}},

    {.position={.data={ 14, 0,  14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 5,  14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 5,  21}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 0,  14}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 0,  21}}, .color={.data={0.5,0.5,0.5}}},
    {.position={.data={ 14, 5,  21}}, .color={.data={0.5,0.5,0.5}}},
};

vt_pc* make_shape(uint16 points) {
    vt_pc*  output = scalloc(points * 3, sizeof(vt_pc));
    for(int i = 0; i < points; ++i) {
        output[i*3+0] = (vt_pc){.position={.data={ 0, 0, 0 }}, .color={.data={ 0,0,0 }}};
        output[i*3+1] = (vt_pc){.position={.data={ cos(i/(float)points * PI * 2), sin(i/(float)points * PI * 2), 0 }}, .color={ .data={i%2,i%2,i%2} }};
        output[i*3+2] = (vt_pc){.position={.data={ cos((i+1)/(float)points * PI * 2), sin((i+1)/(float)points * PI * 2), 0 }}, .color={ .data={i%2,i%2,i%2} }};
    }

    return output;
}

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
    if(y <= 1.86)
        y_vel = 0.25;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(s.id);

    audio_player_update(aplayer, dt);

    timer += dt;
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

    transform_translate(ball1_transform, (vec3){.x=1, .y=1.85+0.5*sin(timer * 2), .z=10.5 + 0.5*cos(timer * 2)}, false);
    transform_rotate(ball1_transform, (vec3){.z=dt * 2}, true);

    transform_translate(ball2_transform, (vec3){.x=1, .y=1.85-0.5*sin(timer * 2), .z=10.5 - 0.5*cos(timer * 2)}, false);
    transform_rotate(ball2_transform, (vec3){.z=-dt * 4}, true);

    vec3 position = (vec3){ .x=(sin(camera_orient.x) * get_axis_value(movement_axis_y)) + (cos(camera_orient.x) * get_axis_value(movement_axis_x)), .z=-cos(camera_orient.x) * get_axis_value(movement_axis_y) + (sin(camera_orient.x) * get_axis_value(movement_axis_x)) };
    vec3 p = transform_get_position(player_transform);
    if(p.y + y_vel <= 1.85 && p.x > 0 && p.x < 21 && p.z > 0 && p.z < 14) {
        position.y = 1.85-p.y;
        y_vel = 0;
    } else {
        position.y = y_vel;
        y_vel -= 0.015;
    }
    transform_translate(player_transform, position, true);

    audio_update_listener(transform_get_position(player_transform), quat_to_euler(transform_get_orientation(player_transform)), position);

    shader_bind_uniform_name(s, "transform", camera_get_vp(player_camera));
    shader_bind_attribute_mesh(s, level_mesh, "i_pos", VT_POSITION, "i_color", VT_COLOR);
    mesh_render(level_mesh, GL_TRIANGLES);

    shader_bind_attribute_mesh(s, circle_mesh, "i_pos", VT_POSITION, "i_color", VT_COLOR);
    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), transform_get_matrix(ball1_transform)));
    mesh_render(circle_mesh, GL_TRIANGLES);
    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), transform_get_matrix(ball2_transform)));
    mesh_render(circle_mesh, GL_TRIANGLES);

    glUseProgram(s.id);
    shader_bind_uniform_name(s, "transform", mat4_mul(camera_get_vp(player_camera), transform_get_matrix(tri_transform)));
    /* shader_bind_uniform_texture_name(s2, "u_texture", tga_tex, GL_TEXTURE0); */
    shader_bind_attribute_mesh(s, tri_mesh, "i_pos", VT_POSITION, "i_color", VT_NORMAL);
    mesh_render(tri_mesh, GL_TRIANGLES);

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
    s = shader_basic_untex_get();
    s2 = shader_basic_tex_get();
    /* tri_mesh = mesh_new(3, tri); */
    tri_mesh = load_mesh(assets_path("test.obj", NULL));
    level_mesh = mesh_new(108, level);

    vt_pc* temp_data = make_shape(32);
    circle_mesh = mesh_new(96, temp_data);
    sfree(temp_data);

    aplayer = audio_player_new(load_audio_source(assets_path("test.wav", NULL), true));
    /* audio_player_set_position(aplayer, 0.45f); */
    audio_player_set_translation(aplayer, (vec3){.x=10.5,.y=1.85,.z=1});
    audio_player_set_playing(aplayer, true);
    audio_player_set_loop(aplayer, true);

    png_tex = load_texture_gl(assets_path("test.png", NULL));
    jpeg_tex = load_texture_gl(assets_path("test.jpeg", NULL));
    tiff_tex = load_texture_gl(assets_path("test.tiff", NULL));
    tga_tex = load_texture_gl(assets_path("test.tga", NULL));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set up player
    player_camera = camera_new(build_projection());
    player_transform = camera_get_transform(player_camera);
    transform_translate(player_transform, (vec3){.x=10.5,.y=1.85,.z=10.5}, false);

    tri_transform = transform_new();
    transform_translate(tri_transform, (vec3){.x=10.5, .y=1.85, .z=1}, false);

    ball1_transform = transform_new();
    transform_rotate(ball1_transform, (vec3){.x=degtorad(90)}, false);
    transform_scale(ball1_transform, (vec3){.data={0.5,0.5,0.5}}, false);

    ball2_transform = transform_new();
    transform_rotate(ball2_transform, (vec3){.x=degtorad(90)}, false);
    transform_scale(ball2_transform, (vec3){.data={0.5,0.5,0.5}}, false);

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
    mesh_free(tri_mesh);
    mesh_free(circle_mesh);
    window_free_final(win);

    return 0;
}
