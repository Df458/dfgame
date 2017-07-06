// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "sprite.h"

#include "check.h"
#include "memory/alloc.h"
#include "spriteset.h"

typedef struct sprite {
    spriteset src;
    animation current_animation;

    float position;
    uint8 orient;
    bool is_playing;
}* sprite;

sprite sprite_new(spriteset set) {
    sprite spr = salloc(sizeof(struct sprite));

    spr->src = set;
    spr->current_animation = spriteset_get_animation(set, NULL);
    spr->position = 0;
    spr->orient = 0;
    spr->is_playing = false;

    return spr;
}

void sprite_set_animation(sprite spr, const char* handle, bool force_reset) {
    animation anim = spriteset_get_animation(spr->src, handle);
    if(anim.texture_id == spr->current_animation.texture_id && !force_reset)
        return;
    spr->current_animation = anim;
    spr->position = 0;
    sprite_set_playing(spr, spr->current_animation.autoplay);
}

void sprite_set_orientation(sprite spr, uint8 orient) {
    spr->orient = orient;
}

void sprite_set_playing(sprite spr, bool playing) {
    spr->is_playing = playing;
}

void sprite_set_position(sprite spr, float position) {
    spr->position = position;
}

void sprite_update(sprite spr, float dt) {
    if(!spr->is_playing)
        return;

    spr->position += dt;
    if((spr->position - spr->current_animation.start_delay) * spr->current_animation.fps >= spr->current_animation.frame_count) {
        if(spr->current_animation.autoloop) {
            while((spr->position - spr->current_animation.start_delay) * spr->current_animation.fps >= spr->current_animation.frame_count) {
                spr->position -= (spr->current_animation.frame_count + spr->current_animation.start_delay) / (float)spr->current_animation.fps;
            }
        } else {
            spr->position = (spr->current_animation.frame_count - 1) * spr->current_animation.start_delay;
            sprite_set_playing(spr, false);
            sprite_set_animation(spr, NULL, false);
        }
    }
}

aabb_2d sprite_get_box(sprite spr) {
    aabb_2d box = spr->current_animation.texture_box;
    box.width /= spr->current_animation.frame_count;
    box.height /= spr->current_animation.orient_count;

    int frame = (spr->position - spr->current_animation.start_delay) * spr->current_animation.fps;

    box.x += box.width * frame;
    box.y += box.height * (spr->orient % spr->current_animation.orient_count);

    return box;
}

gltex sprite_get_texture(sprite spr) {
    return spriteset_get_texture(spr->src);
}

int16 sprite_get_anim_id(sprite spr) {
    return spr->current_animation.texture_id;
}

void _sprite_free(sprite spr, bool free_src) {
    check_return(spr, "Can't free sprite: Sprite is NULL", );

    if(free_src)
        spriteset_free(spr->src);
    sfree(spr);
}
