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
#include "font.h"
#include "input.h"
#include "interpolate.h"
#include "log/log.h"
#include "mainloop.h"
#include "mesh.h"
#include "paths.h"
#include "shader_init.h"
#include "sprite.h"
#include "sprite_io.h"
#include "spriteset.h"
#include "text.h"
#include "transform.h"
#include "window.h"

void* win;
camera      c_main;
shader      s_default;

vec2  player_velocity;
int player_life = 3;
transform t_player;
sprite s_player;
float damage_cooldown = 0;
float shot_cooldown = 0;

axis_id   a_rotate;
axis_id   a_accel;
action_id a_shoot;

array rocks;
spriteset rock_set;
audio_source au_explosion;

array bullets;
spriteset bullet_set;
audio_source au_shot;

array audio;

text info_text;

const char* const info_str =
"Controls\n"
"Arrow Keys: Turn/Accelerate/Decelerate\n"
"Z: Shoot";

typedef struct rock {
    transform trans;
    sprite spr;
    float life;
    uint8 size;
    vec2 velocity;
    float avel;
} rock;
typedef struct bullet {
    transform trans;
    sprite spr;
    float life;
    vec2 velocity;
} bullet;

// ----------------------------------------------------------------------------

void play_audio(audio_source se) {
    audio_player pl = audio_player_new(se);
    audio_player_set_playing(pl, true);
    array_add(audio, pl);
}
vec2 wrap_position(vec2 pos) {
    if(pos.x < -640)
        pos.x = 639;
    else if(pos.x > 640)
        pos.x = -639;
    if(pos.y < -360)
        pos.y = 359;
    else if(pos.y > 360)
        pos.y = -359;

    return pos;
}
bool check_collision(transform t1, float r1, transform t2, float r2) {
    return vec_len_squared(vec_sub(transform_get_position(t1).xy, transform_get_position(t2).xy)) < square(r1 + r2);
}

// ----------------------------------------------------------------------------

void create_bullet(vec2 position, float orient, float angle_multiplier) {
    vec2 facing = vec2_rotate(vec2_forward, -orient);
    bullet b = (bullet) {
        .trans = transform_new(),
        .spr   = sprite_new(bullet_set),
        .life  = 0.75,
        .velocity = vec_mul(facing, 850)
    };
    transform_rotate(b.trans, orient, false);
    transform_translate(b.trans, vec_add(position, vec_mul(vec_rotate(facing, PI * angle_multiplier), 5)), false);
    array_add(bullets, b);
}
void bullet_cleanup(bullet* b) {
    transform_free(b->trans);
    sprite_free(b->spr, false);
}
void update_bullets(float dt) {
    array_foreach(bullets, bul) {
        bullet* b = bul.data;
        vec4 color = color_white;
        b->life -= dt;
        if(b->life <= 0) {
            bullet_cleanup(b);
            array_remove_iter(bullets, &bul);
            continue;
        } else if(b->life <= 0.25) {
            color.a = lerp(0.0f, 1.0f, b->life * 4);
        }

        vec2 pos = wrap_position(vec_add(transform_get_position(b->trans).xy, vec_mul(b->velocity, dt)));
        transform_translate(b->trans, pos, false);

        glUseProgram(s_default.id);
        shader_bind_uniform_name(s_default, "u_color", color);
        sprite_update(b->spr, dt);
        sprite_draw(b->spr, s_default, transform_get_matrix(b->trans), camera_get_vp(c_main));
    }
}

// ----------------------------------------------------------------------------

void update_player(float dt) {
    if(player_life <= 0)
        return;

    float fdt = dt * 60;

    transform_rotate(t_player, degtorad(get_axis_value(a_rotate)) * fdt, true);

    float orient = transform_get_orientation_2d(t_player);
    vec2 friction = vec2_len_squared(player_velocity) == 0 ? (vec2){0} : vec_mul(vec_normalize(player_velocity), 0.04);
    player_velocity = vec_add(vec_sub(player_velocity, friction), vec_mul(vec2_rotate(vec2_forward, -orient), get_axis_value(a_accel) * fdt));

    vec2 pos = wrap_position(vec_add(transform_get_position(t_player).xy, vec_mul(player_velocity, dt)));
    transform_translate(t_player, pos, false);

    if(damage_cooldown > 0)
        damage_cooldown -= dt;

    if(shot_cooldown > 0) {
        shot_cooldown -= dt;
    } else if(action_is_active(a_shoot)) {
        create_bullet(transform_get_position(t_player).xy, orient, 0.5);
        create_bullet(transform_get_position(t_player).xy, orient, -0.5);

        play_audio(au_shot);
        shot_cooldown = 0.1;
    }

    sprite_update(s_player, dt);

    glUseProgram(s_default.id);
    shader_bind_uniform_name(s_default, "u_color", lerp(color_white, color_red, damage_cooldown * 2));
    sprite_draw(s_player, s_default, transform_get_matrix(t_player), camera_get_vp(c_main));
}
bool player_hit_rock(rock* r) {
    if(damage_cooldown > 0 || !check_collision(r->trans, 20 * r->size, t_player, 14))
        return false;

    r->life = 0;
    damage_cooldown = 0.5;
    player_life--;

    return true;
}

// ----------------------------------------------------------------------------

void create_rock(vec2 position, uint8 size) {
    float rot = PI * (rand() % 100 * 0.01);
    rock r = (rock) {
        .trans = transform_new(),
            .spr = sprite_new(rock_set),
            .size = size,
            .avel = rand() % 100 * 0.001 - 0.05,
            .velocity = (vec2){ .x=cos(rot) * 50, .y=sin(rot) * 50 }
    };
    switch(r.size) {
        case 1: r.life = 1; break;
        case 2: r.life = 5; break;
        case 3: r.life = 8; break;
    }
    transform_translate(r.trans, position, false);
    transform_rotate(r.trans, rot, false);
    transform_scale(r.trans, (float)r.size / 3.0f, false);
    array_add(rocks, r);
}
void rock_cleanup(rock* r) {
    transform_free(r->trans);
    sprite_free(r->spr, false);
}
void update_rocks(float dt) {
    if(array_get_length(rocks) < 30 && rand() % 100 == 0)
        create_rock((vec2){ .x=rand() % 1280 - 640, .y = rand() % 720 - 360 }, rand() % 3 + 1);

    array_foreach(rocks, roc) {
        rock* r = roc.data;
        vec2 pos = wrap_position(vec_add(transform_get_position(r->trans).xy, vec_mul(r->velocity, dt)));
        transform_translate(r->trans, pos, false);
        transform_rotate(r->trans, r->avel * (dt * 60), true);

        array_foreach(bullets, i) {
            if(check_collision(r->trans, 20 * r->size, ((bullet*)i.data)->trans, 5)) {
                r->life--;
                bullet_cleanup(i.data);
                array_remove_iter(bullets, &i);
            }
        }

        if(r->life <= 0) {
            vec2 pos = transform_get_position(r->trans).xy;
            if(r->size == 2) {
                create_rock(pos, 1);
                create_rock(pos, 1);
                create_rock(pos, 1);
            } else if(r->size == 3) {
                create_rock(pos, 2);
                create_rock(pos, 2);
            }

            play_audio(au_explosion);
            rock_cleanup(r);

            array_remove_iter(rocks, &roc);
            continue;
        }

        glUseProgram(s_default.id);
        shader_bind_uniform_name(s_default, "u_color", color_white);
        sprite_draw(r->spr, s_default, transform_get_matrix(r->trans), camera_get_vp(c_main));

        player_hit_rock(r);
    }
}

// ----------------------------------------------------------------------------

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_player(dt);
    update_bullets(dt);
    update_rocks(dt);

    array_foreach(audio, i) {
        audio_player pl = array_iter_data(i, audio_player);
        if(!audio_player_get_playing(pl)) {
            audio_player_free(pl, false);
            array_remove_iter(audio, &i);
        } else {
            audio_player_update(pl, dt);
        }
    }

    vec2 offset = (vec2){ .x = -634, .y=354 };
    shader_bind_uniform_name(s_default, "u_color", color_white);
    text_draw(info_text, s_default, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, offset)));

    window_redraw(win);
    return !window_get_should_close(win);
}

int main() {
    win = window_new(1280, 720, false, "Comets Demo");
    shaders_init();

    s_default = shader_basic_tex_get();
    c_main = window_create_2d_camera(win);

    audio_init();
    audio = array_mnew(audio_player, 30);

    info_text = text_new(load_font("OpenSans-Regular.ttf", 16), info_str);
    text_set_align(info_text, ALIGN_BOTTOM_LEFT);

    bullets = array_mnew(bullet, 40);
    bullet_set = load_spriteset("bullet.xml");
    au_shot = load_audio_source("Laser01.wav", true);

    t_player = transform_new();
    s_player = sprite_new(load_spriteset("player.xml"));

    // Input binding
    a_rotate = input_add_key_axis_full(K_RIGHT, K_LEFT, 4, 4, false);
    a_accel = input_add_key_axis_full(K_DOWN, K_UP, 5, 5, true);
    a_shoot = input_add_key_action(K_Z, NULL);

    rocks = array_mnew(rock, 30);

    rock_set = load_spriteset("rock.xml");

    au_explosion = load_audio_source("Explosion01.wav", true);

    resource_path_free();

    glDisable(GL_DEPTH_TEST);

    srand(time(NULL));

    mainloop_create_run(loop);
    
    array_foreach(bullets, b) {
        bullet_cleanup(b.data);
    }
    array_free(bullets);

    spriteset_free(bullet_set);

    sprite_free(s_player, true);
    transform_free(t_player);

    array_foreach(rocks, i) {
        rock_cleanup(i.data);
    }
    array_free(rocks);
    spriteset_free(rock_set);
    audio_source_free(au_explosion);

    array_foreach(audio, i) {
        audio_player_free(i.data, false);
    }
    array_free(audio);
    audio_cleanup();
    text_free(info_text, true);

    camera_free(c_main);
    window_free_final(win);
    shaders_cleanup();

    return 0;
}
