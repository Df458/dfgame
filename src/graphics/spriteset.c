// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "spriteset.h"

#include "aabb.h"
#include "check.h"
#include "container/hashmap.h"
#include "memory/alloc.h"
#include "stringutil.h"
#include "texture_atlas.h"

typedef struct spriteset {
    texture_atlas atlas;
    hashmap animations;

    char* asset_path;
}* spriteset;
typedef struct sprite {
    spriteset src;
    animation anim;
    float position;
    float orientation;
    bool playing;
} sprite;

spriteset spriteset_new(const char* path) {
    spriteset set = salloc(sizeof(struct spriteset));
    set->atlas = texture_atlas_new();
    set->animations = hashmap_new();
    set->asset_path = nstrdup(path);

    return set;
}

void spriteset_add_animation_raw(spriteset set, animation anim, rawtex texture, const char* handle) {
    if(!hashmap_has_key(set->animations, make_hash_key(handle))) {
        int16 index = texture_atlas_add_raw(set->atlas, texture, GL_RGBA);
        check_return(index >= 0, "Can't add animation %s: texture atlas is full", , handle);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        hashmap_copyset(set->animations, make_hash_key(handle), &anim, sizeof(animation));

        if(hashmap_size(set->animations) == 1 && strcmp(handle, "default"))
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

animation spriteset_get_animation_by_name(spriteset set, const char* handle) {
    if(!handle || !hashmap_has_key(set->animations, make_hash_key("default")))
        return *(animation*)hashmap_get(set->animations, make_hash_key("default"));
    return *(animation*)hashmap_get(set->animations, make_hash_key(handle));
}

bool match_animation_id(void* anim, void* id, void* user) {
    return ((animation*)anim)->texture_id == *(int*)id;
}

animation spriteset_get_animation_by_id(spriteset set, int16 handle) {
    animation* a = hashmap_get_value(set->animations, &handle, match_animation_id, NULL);
    if(handle == -1 || !a)
        return *(animation*)hashmap_get(set->animations, make_hash_key("default"));
    return *a;
}

gltex spriteset_get_texture(spriteset set) {
    return texture_atlas_get_texture(set->atlas);
}

// Frees the given spriteset, and all resources contained within it
// NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _spriteset_free(spriteset set) {
    check_return(set, "Can't free spriteset: set is NULL", );

    texture_atlas_free(set->atlas);
    hashmap_free_deep(set->animations);
    if(set->asset_path)
        sfree(set->asset_path);
    sfree(set);
}
