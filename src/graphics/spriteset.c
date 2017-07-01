// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "spriteset.h"

#include "aabb.h"
#include "check.h"
#include "container/hashmap.h"
#include "memory/alloc.h"
#include "texture_atlas.h"

typedef struct spriteset {
    texture_atlas atlas;
    hashmap animations;
}* spriteset;
typedef struct sprite {
    spriteset src;
    animation anim;
    float position;
    float orientation;
    bool playing;
} sprite;

spriteset spriteset_new() {
    spriteset set = salloc(sizeof(struct spriteset));
    set->atlas = texture_atlas_new();
    set->animations = hashmap_new();

    return set;
}

void spriteset_add_animation_raw(spriteset set, animation anim, rawtex texture, const char* handle) {
    if(!hashmap_has_key(set->animations, make_hash_key(handle))) {
        int16 index = texture_atlas_add_raw(set->atlas, texture, GL_RGBA);
        check_return(index >= 0, "Can't add animation %s: texture atlas is full", , handle);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        hashmap_copyset(set->animations, make_hash_key(handle), &anim, sizeof(animation));

        if(hashmap_size(set->animations) == 1)
            hashmap_copyset(set->animations, make_hash_key("default"), &anim, sizeof(animation));
    }
}
void spriteset_add_animation_gl(spriteset set, animation anim, gltex texture, const char* handle) {
    if(!hashmap_has_key(set->animations, make_hash_key(handle))) {
        int16 index = texture_atlas_add_gl(set->atlas, texture, GL_RGBA);
        check_return(index >= 0, "Can't add animation %s: texture atlas is full", , handle);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        hashmap_copyset(set->animations, make_hash_key(handle), &anim, sizeof(animation));

        if(hashmap_size(set->animations) == 1)
            hashmap_copyset(set->animations, make_hash_key("default"), &anim, sizeof(animation));
    }
}
uint16 spriteset_get_animation_count(spriteset set) {
    return hashmap_size(set->animations);
}

animation spriteset_get_animation(spriteset set, const char* handle) {
    if(!handle || !hashmap_has_key(set->animations, make_hash_key("default")))
        return *(animation*)hashmap_get(set->animations, make_hash_key("default"));
    return *(animation*)hashmap_get(set->animations, make_hash_key(handle));
}

gltex spriteset_get_texture(spriteset set) {
    return texture_atlas_get_texture(set->atlas);
}
