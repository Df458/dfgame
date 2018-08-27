// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "sprite.h"

#include "check.h"
#include "mesh.h"
#include "memory/alloc.h"
#include "shader.h"
#include "spriteset.h"

typedef struct sprite {
    spriteset src;
    animation* current_animation;

    uint32 position;
    uint16 frame;
    uint8 orient;
    bool is_playing;
}* sprite;

sprite sprite_new(spriteset set) {
    sprite spr = salloc(sizeof(struct sprite));

    spr->src = set;
    spr->current_animation = spriteset_get_animation(set, NULL);
    spr->position = 0;
    spr->frame = 0;
    spr->orient = 0;
    spr->is_playing = spr->current_animation->autoplay;

    return spr;
}

void sprite_set_animation_common(sprite spr, animation* anim, bool force_reset) {
    if(!anim || (anim->texture_id == spr->current_animation->texture_id && !force_reset)) {
        return;
    }

    spr->current_animation = anim;

    spr->position = 0;
    spr->frame = 0;
    sprite_set_playing(spr, spr->current_animation->autoplay);
}

void sprite_set_animation_name(sprite spr, const char* handle, bool force_reset) {
    sprite_set_animation_common(spr, spriteset_get_animation(spr->src, handle), force_reset);
}

void sprite_set_animation_id(sprite spr, int16 handle, bool force_reset) {
    sprite_set_animation_common(spr, spriteset_get_animation(spr->src, handle), force_reset);
}

void sprite_set_orientation(sprite spr, uint8 orient) {
    spr->orient = orient;
}

uint8 sprite_get_orientation(sprite spr) {
    return spr->orient;
}

void sprite_set_playing(sprite spr, bool playing) {
    spr->is_playing = playing;
}

bool sprite_get_playing(sprite spr) {
    return spr->is_playing;
}

uint32 sprite_get_position(const sprite spr) {
    return spr->position;
}
void sprite_set_position(sprite spr, uint32 position) {
    spr->position = position;
    if(animation_get_frame(spr->current_animation, spr->position, &spr->frame, &spr->position) && !spr->current_animation->autoloop) {
        spr->position = spr->current_animation->total_time - 1;
        spr->frame = spr->current_animation->frame_count - 1;
        sprite_set_playing(spr, false);

        // Reset to the default if the settings allow it
        if(spr->current_animation->default_on_finish) {
            sprite_set_animation(spr, NULL, true);
        }
    }
}

uint16 sprite_get_frame(const sprite spr) {
    return spr->frame;
}
void sprite_set_frame(sprite spr, uint16 frame) {
    sprite_set_position(spr, animation_get_time(spr->current_animation, frame));
}

void sprite_update(sprite spr, float dt) {
    if(!spr->is_playing)
        return;

    sprite_set_position(spr, spr->position + (dt * 1000));
}

aabb_2d sprite_get_box(sprite spr) {
    aabb_2d box = spr->current_animation->texture_box;
    box.width /= spr->current_animation->frame_count;
    box.height /= spr->current_animation->orient_count;

    box.x += box.width * spr->frame;
    box.y += box.height * (spr->orient % spr->current_animation->orient_count);

    return box;
}

gltex sprite_get_texture(sprite spr) {
    return spriteset_get_texture(spr->src);
}

spriteset sprite_get_data(sprite spr) {
    return spr->src;
}

int16 sprite_get_anim_id(sprite spr) {
    return spr->current_animation->texture_id;
}

void sprite_draw(sprite spr, shader s, mat4 model, mat4 view) {
    aabb_2d box = sprite_get_box(spr);
    glUseProgram(s.id);
    shader_bind_uniform_name(s, "u_transform", mat4_mul(view, mat4_mul(model, mat4_scale(mat4_ident, box.dimensions))));
    shader_bind_uniform_texture_name(s, "u_texture", sprite_get_texture(spr), GL_TEXTURE0);
    box.vec = vec4_mul(box.vec, 1.0f / sprite_get_texture(spr).width);
    shader_bind_uniform_name(s, "uv_offset", box.position);
    shader_bind_uniform_name(s, "uv_scale", box.dimensions);
    mesh_render(s, mesh_quad(), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);

    aabb_2d clear = (aabb_2d) {
        .position = (vec2){0},
        .dimensions = (vec2){.x = 1, .y = 1}
    };
    shader_bind_uniform_name(s, "uv_offset", clear.position);
    shader_bind_uniform_name(s, "uv_scale", clear.dimensions);
}

void _sprite_free(sprite spr, bool free_src) {
    check_return(spr, "Can't free sprite: Sprite is NULL", );

    if(free_src)
        spriteset_free(spr->src);
    sfree(spr);
}
