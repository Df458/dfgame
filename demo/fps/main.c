#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "camera.h"
#include "audio_loader.h"
#include "audio_player.h"
#include "audio_system.h"
#include "color.h"
#include "control.h"
#include "container/array.h"
#include "data_loader.h"
#include "font_loader.h"
#include "input.h"
#include "log/log.h"
/* #include "interpolate.h" */
#include "mainloop.h"
#include "mesh.h"
#include "paths.h"
#include "shader_init.h"
#include "sprite.h"
#include "sprite_loader.h"
#include "spriteset.h"
#include "text.h"
#include "texture_loader.h"
#include "transform.h"
#include "window.h"

GLFWwindow* win;
camera      c_main;
camera      c_ui;
shader      s_default;

axis_id   a_side;
axis_id   a_forward;
axis_id   a_turn;

text info_text;

gltex t_floor;

mesh m_floor;
mesh m_wall;
sprite pistol_sprite;
audio_player pistol_sfx;
spriteset barrel_set;

int32 width;
int32 height;
uint8* m_level;

uarray bullets;
uarray barrels;

const char* const info_str =
"Controls\n"
"WASD: Move/Strafe\n"
"Mouse: Look\n"
"Left Click: Shoot";

typedef struct bullet {
    vec3 position;
    vec3 velocity;
    bool destroyed;
} bullet;
typedef struct barrel {
    transform trans;
    sprite spr;
    bool destroyed;
} barrel;

projection_settings camera_settings_main = {
    .dims = { .x = -6, .y = 3.5, .z = 0.1, .w = 200 },
    .fov = 90,
    .is_ortho = false
};

bool tile_collision(vec3 pos) {
    pos = vec3_add(pos, (vec3){.x=10, .z=10});
    int tile_x = pos.x / 20;
    int tile_y = pos.z / 20;
    return tile_x <  0 || pos.x < 0 || tile_x >= width || tile_y < 0 || pos.z < 0 || tile_y >= height || m_level[(tile_y * width) + tile_x] == 0;
}

void create_bullet(vec3 position, quat orient) {
    bullet b = (bullet) {
        .position = position,
        .velocity = vec3_mul(vec3_rotate(vec3_forward, orient), 10),
        .destroyed = false
    };
    array_copyadd_simple(bullets, b, bullet);
}
void create_barrel(vec3 position) {
    barrel b = (barrel) {
        .trans = transform_new(),
        .spr = sprite_new(barrel_set),
        .destroyed = false
    };
    transform_scale(b.trans, -10, false);
    transform_translate(b.trans, position, false);

    array_copyadd_simple(barrels, b, barrel);
}

iter_result barrel_collision(void* bar, void* user) {
    barrel* br = (barrel*)bar;

    if(br->destroyed)
        return iter_continue;

    bullet* bl = (bullet*)user;
    vec3 pos1 = transform_get_position(br->trans);
    vec3 pos2 = bl->position;

    if(vec_len_squared(vec_sub(pos1, pos2)) < square(15)) {
        sprite_set_playing(br->spr, true);
        br->destroyed = true;
        bl->destroyed = true;
    }

    return iter_continue;
}
iter_result update_bullet(void* bul, void* user) {
    bullet* b = (bullet*)bul;
    b->position = vec3_sub(b->position, b->velocity);

    if(tile_collision(b->position)) {
        return iter_delete;
    }

    array_foreach(barrels, barrel_collision, b);
    return b->destroyed ? iter_delete : iter_continue;
}
iter_result update_barrel(void* bar, void* user) {
    barrel* b = (barrel*)bar;

    quat q = camera_get_orientation(c_main);
    transform_rotate(b->trans, q, false);
    shader_bind_uniform_name(s_default, "u_transform", mat4_mul(camera_get_vp(c_main), transform_get_matrix(b->trans)));
    shader_bind_uniform_texture_name(s_default, "u_texture", sprite_get_texture(b->spr), GL_TEXTURE0);
    aabb_2d barrel_box = sprite_get_box(b->spr);
    barrel_box.vec = vec4_mul(barrel_box.vec, 1.0f / sprite_get_texture(b->spr).width);
    shader_bind_uniform_name(s_default, "uv_offset", barrel_box.position);
    shader_bind_uniform_name(s_default, "uv_scale", barrel_box.dimensions);
    mesh_render(s_default, mesh_quad(), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);

    sprite_update(b->spr, *(float*)user);
    if(b->destroyed && !sprite_get_playing(b->spr))
        sprite_set_position(b->spr, 0.25);

    return iter_continue;
}

void shoot(action_id id, void* user) {
    if(!sprite_get_playing(pistol_sprite)) {
        sprite_set_playing(pistol_sprite, true);
        audio_player_set_playing(pistol_sfx, true);

        quat q = camera_get_orientation(c_main);
        q.w *= -1;
        create_bullet(camera_get_position(c_main), q);
    }
}

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s_default.id);

    transform_rotate(camera_get_transform(c_main), (vec3){ .x=get_axis_value(a_turn) }, true);

    quat q = camera_get_orientation(c_main);
    q.w *= -1;
    vec3 forward = vec3_mul(vec3_rotate(vec3_forward, q), get_axis_value(a_forward));
    vec3 right = vec3_mul(vec3_rotate(vec3_right, q), get_axis_value(a_side));

    vec3 cpos = camera_get_position(c_main);
    vec3 pos = vec3_add(forward, right);
    vec3 test = (vec3) { .x = cpos.x + (pos.x * 1.1), .z = cpos.z };
    if(tile_collision(test)) {
        pos.x = 0;
        test.x = cpos.x + (pos.x * 1.1);
    }
    test.z = cpos.z + (pos.z * 1.1);
    if(tile_collision(test)) {
        pos.z = 0;
    }

    transform_translate(camera_get_transform(c_main), vec3_add(pos, cpos), false);

    shader_bind_uniform_name(s_default, "u_transform", camera_get_vp(c_main));
    shader_bind_uniform_texture_name(s_default, "u_texture", t_floor, GL_TEXTURE0);
    mesh_render(s_default, m_floor, GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);

    array_foreach(bullets, update_bullet, &dt);
    array_foreach(barrels, update_barrel, &dt);

    vec2 offset = (vec2){ .x = -634, .y=354 };
    shader_bind_uniform_name(s_default, "u_color", color_white);
    text_draw(info_text, s_default, mat4_mul(camera_get_vp(c_ui), mat4_translate(mat4_ident, offset)));

    sprite_update(pistol_sprite, dt);
    audio_player_update(pistol_sfx, dt);
    sprite_draw(pistol_sprite, s_default, mat4_translate(mat4_scale(mat4_ident, 15), (vec3){.y = 120}), camera_get_vp(c_ui));

    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

void prepare_mesh(const char* path) {
    uint32 size = 0;
    uint32 len = 0;
    byte* loaded = load_data_buffer(path, &len);

    width = strchr((char*)loaded, '\n') - (char*)loaded;

    if(width <= 0)
        width = len;
    height = len / (width + 1);

    m_level = mscalloc(width * height, uint8);

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            if(loaded[(i * (width + 1)) + j] != ' ') {
                if(loaded[(i * (width + 1)) + j] == '@') {
                    vec3 pos = (vec3){ .x = j * 20, .y = 10, .z = i * 20 };
                    transform_translate(camera_get_transform(c_main), pos, false);
                } else if(loaded[(i * (width + 1)) + j] == 'O') {
                    vec3 pos = (vec3){ .x = j * 20, .y = 5, .z = i * 20 };
                    create_barrel(pos);
                }

                m_level[(i * width) + j] = 1;
                size += 12;
                if(i == 0 || m_level[((i - 1) * width) + j] == 0)
                    size += 6;
                if(i == height - 1)
                    size += 6;
                if(j == 0 || m_level[(i * width) + j - 1] == 0)
                    size += 6;
                if(j == width - 1)
                    size += 6;
            } else {
                m_level[(i * width) + j] = 0;
                if(i != 0 && m_level[((i - 1) * width) + j] != 0)
                    size += 6;
                if(j != 0 && m_level[(i * width) + j - 1] != 0)
                    size += 6;
            }
        }
    }

    vt_pt* data = mscalloc(size, vt_pt);

    uint32 counter = 0;
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            if(m_level[(i * width) + j] != 0) {
                // FLOOR
                data[counter + 0]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) - 10, 0,    0}};
                data[counter + 1]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) - 10, 0.25, 0}};
                data[counter + 2]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) + 10, 0.25, 0.25}};
                data[counter + 3]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) - 10, 0,    0}};
                data[counter + 4]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) + 10, 0.25, 0.25}};
                data[counter + 5]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) + 10, 0,    0.25}};
                data[counter + 6]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) - 10, 0,    0}};
                data[counter + 7]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) - 10, 0.25, 0}};
                data[counter + 8]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) + 10, 0.25, 0.25}};
                data[counter + 9]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) - 10, 0,    0}};
                data[counter + 10] = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) + 10, 0.25, 0.25}};
                data[counter + 11] = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) + 10, 0,    0.25}};
                counter += 12;

                if(rand() % 5 == 0) {
                    for(int k = counter - 12; k < counter - 6; ++k) {
                        data[k].uv.y += 0.25;
                    }
                }

                if(i == 0 || m_level[((i - 1) * width) + j] == 0) {
                    data[counter + 0]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) - 10, 0.50, 0.25}};
                    data[counter + 1]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) - 10, 0.75, 0.25}};
                    data[counter + 2]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) - 10, 0.75, 0}};
                    data[counter + 3]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) - 10, 0.50, 0.25}};
                    data[counter + 4]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) - 10, 0.75, 0}};
                    data[counter + 5]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) - 10, 0.50, 0}};
                    counter += 6;

                    if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.y += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.x += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv = vec2_add(data[k].uv, (vec2){.x=0.25, .y=0.25});
                    }
                }

                if(i == height - 1 || m_level[((i + 1) * width) + j] == 0) {
                    data[counter + 0]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) + 10, 0.50, 0.25}};
                    data[counter + 1]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) + 10, 0.75, 0.25}};
                    data[counter + 2]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) + 10, 0.75, 0}};
                    data[counter + 3]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) + 10, 0.50, 0.25}};
                    data[counter + 4]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) + 10, 0.75, 0}};
                    data[counter + 5]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) + 10, 0.50, 0}};
                    counter += 6;

                    if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.y += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.x += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv = vec2_add(data[k].uv, (vec2){.x=0.25, .y=0.25});
                    }
                }

                if(j == 0 || m_level[(i * width) + j - 1] == 0) {
                    data[counter + 0]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) - 10, 0.50, 0.25}};
                    data[counter + 1]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) - 10, 0.50, 0}};
                    data[counter + 2]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) + 10, 0.75, 0}};
                    data[counter + 3]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) - 10, 0.50, 0.25}};
                    data[counter + 4]  = (vt_pt){.data={(j * 20) - 10, 20, (i * 20) + 10, 0.75, 0}};
                    data[counter + 5]  = (vt_pt){.data={(j * 20) - 10, 0,  (i * 20) + 10, 0.75, 0.25}};
                    counter += 6;

                    if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.y += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.x += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv = vec2_add(data[k].uv, (vec2){.x=0.25, .y=0.25});
                    }
                }

                if(j == width - 1 || m_level[(i * width) + j + 1] == 0) {
                    data[counter + 0]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) - 10, 0.50, 0.25}};
                    data[counter + 1]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) - 10, 0.50, 0}};
                    data[counter + 2]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) + 10, 0.75, 0}};
                    data[counter + 3]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) - 10, 0.50, 0.25}};
                    data[counter + 4]  = (vt_pt){.data={(j * 20) + 10, 20, (i * 20) + 10, 0.75, 0}};
                    data[counter + 5]  = (vt_pt){.data={(j * 20) + 10, 0,  (i * 20) + 10, 0.75, 0.25}};
                    counter += 6;

                    if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.y += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv.x += 0.25;
                    } else if(rand() % 30 == 0) {
                        for(int k = counter - 6; k < counter; ++k)
                            data[k].uv = vec2_add(data[k].uv, (vec2){.x=0.25, .y=0.25});
                    }
                }
            }
        }
    }

    m_floor = mesh_new(size, data);

    sfree(loaded);
}

int main() {
    win = window_new_default(1280, 720, "FPS Demo");
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    shaders_init();
    audio_init();

    s_default = shader_basic_tex_get();
    c_main = camera_new(camera_settings_main);
    transform_translate(camera_get_transform(c_main), (vec3){.y=10}, true);
    c_ui = window_create_2d_camera(c_ui);

    srand(time(NULL));

    bullets = uarray_new(5);
    barrels = uarray_new(5);

    init_base_resource_path(NULL);
    char* path = assets_path("OpenSans-Regular.ttf", NULL);
    info_text = text_new(load_font(path, 16), info_str);
    text_set_align(info_text, TEXT_ALIGN_BOTTOM_LEFT);
    sfree(path);
    path = assets_path("FPSDungeonTiles.png", NULL);
    t_floor = load_texture_gl(path);
    sfree(path);
    path = assets_path("Pistol.xml", NULL);
    pistol_sprite = sprite_new(load_spriteset(path));
    sfree(path);
    path = assets_path("barrel.xml", NULL);
    barrel_set = load_spriteset(path);
    sfree(path);
    path = assets_path("test.txt", NULL);
    prepare_mesh(path);
    sfree(path);
    path = assets_path("PistolShot.wav", NULL);
    pistol_sfx = audio_player_new(load_audio_source(path, true));
    sfree(path);
    resource_path_free();

    // Input binding
    a_side = input_add_key_axis(GLFW_KEY_A, 1, 1, false);
    input_bind_key_axis(GLFW_KEY_D, a_side, -1);
    a_forward = input_add_key_axis(GLFW_KEY_W, 1, -1, true);
    input_bind_key_axis(GLFW_KEY_S, a_forward, 1);
    a_turn = input_add_mouse_position_axis(false, 15.0, 0.025f, false);

    input_add_mouse_button_action(GLFW_MOUSE_BUTTON_LEFT, as_event(action_event, shoot, NULL));

    mainloop_create_run(loop);

    text_free(info_text, true);

    sprite_free(pistol_sprite, true);
    /* sprite_free(a_barrel.spr, true); */
    audio_player_free(pistol_sfx, true);

    mesh_free(m_floor);

    camera_free(c_main);
    audio_cleanup();
    shaders_cleanup();
    window_free_final(win);

    return 0;
}
