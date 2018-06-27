// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "spriteset.h"

#include "core/check.h"
#include "core/container/hashmap.h"
#include "core/memory/alloc.h"
#include "core/stringutil.h"
#include "graphics/texture_atlas.h"
#include "math/aabb.h"

typedef struct spriteset {
    texture_atlas atlas;
    hashmap animations;

    char* asset_path;
}* spriteset;

// Creates a new empty spriteset
spriteset spriteset_new(const char* path) {
    spriteset set = salloc(sizeof(struct spriteset));
    set->atlas = texture_atlas_new();
    set->animations = hashmap_new();
    set->asset_path = nstrdup(path);

    return set;
}

typedef struct id_pair {
    int16 old_id;
    uint16 size;
} id_pair;
iter_result fix_ids(void* a, void* id) {
    id_pair* pair = id;
    animation* anim = a;
    if(anim->texture_id == pair->size + 1) {
        anim->texture_id = pair->old_id;
    }

    return iter_continue;
}

// Adds a new animation to the spriteset, using raw pixel data
void spriteset_add_animation_raw(spriteset set, animation anim, rawtex texture) {
    check_return(set, "Spriteset is NULL", );

    if(!hashmap_has_key(set->animations, make_hash_key(anim.name))) {
        int16 index = texture_atlas_add_raw(set->atlas, texture, GL_RGBA);
        check_return(index >= 0, "Can't add animation %s: texture atlas is full", , anim.name);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        hashmap_copyset(set->animations, make_hash_key(anim.name), &anim, sizeof(animation));
    }
}
// Adds a new animation to the spriteset, copying from an OpenGL texture
void spriteset_add_animation_gl(spriteset set, animation anim, gltex texture) {
    check_return(set, "Spriteset is NULL", );

    if(!hashmap_has_key(set->animations, make_hash_key(anim.name))) {
        int16 index = texture_atlas_add_gl(set->atlas, texture, GL_RGBA);
        check_return(index >= 0, "Can't add animation %s: texture atlas is full", , anim.name);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        hashmap_copyset(set->animations, make_hash_key(anim.name), &anim, sizeof(animation));
    }
}
// Removes an animation form the spriteset and deletes it
void spriteset_remove_animation(spriteset set, animation* a) {
    hashmap_remove(set->animations, make_hash_key(a->name));

    texture_atlas_remove(set->atlas, a->texture_id);

    id_pair pair = { .old_id = a->texture_id, .size = hashmap_size(set->animations) };
    hashmap_foreach(set->animations, fix_ids, &pair);

    sfree(a->filepath);
    sfree(a->name);
}
// Gets the numer of animations stored in the spriteset
uint16 spriteset_get_animation_count(spriteset set) {
    check_return(set, "Spriteset is NULL", 0);

    return hashmap_size(set->animations);
}

// Gets an animation from the spriteset, based on the hash key
animation* spriteset_get_animation_by_name(spriteset set, const char* handle) {
    check_return(set, "Spriteset is NULL", NULL);

    if(!handle || !hashmap_has_key(set->animations, make_hash_key("default")))
        return (animation*)hashmap_get(set->animations, make_hash_key("default"));
    return (animation*)hashmap_get(set->animations, make_hash_key(handle));
}

bool match_animation_id(void* anim, void* id, void* user) {
    return ((animation*)anim)->texture_id == *(int*)id;
}

// Gets an animation from the spriteset, based on the numeric id
animation* spriteset_get_animation_by_id(spriteset set, int16 handle) {
    check_return(set, "Spriteset is NULL", NULL);

    animation* a = hashmap_get_value(set->animations, &handle, match_animation_id, NULL);
    if(handle == -1 || !a)
        return (animation*)hashmap_get(set->animations, make_hash_key("default"));
    return a;
}

// Gets the underlying texture of the spriteset
gltex spriteset_get_texture(spriteset set) {
    check_return(set, "Spriteset is NULL", (gltex){0});

    return texture_atlas_get_texture(set->atlas);
}

// Gets the filepath of the spriteset
char* spriteset_get_path(spriteset set) {
    check_return(set, "Spriteset is NULL", NULL);

    return nstrdup(set->asset_path);
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
