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

    if(!handle || !hashmap_has_key(set->animations, make_hash_key(handle)))
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

// Gets the frame of an animation based on a given time
bool animation_get_frame(const animation* anim, uint32 time, uint16* out_frame, uint32* adjusted_time) {
    uint32 counter = 0;

    *adjusted_time = time % anim->total_time;

    for(int frame = 0; frame < anim->frame_count; ++frame) {
        if(anim->frame_times[frame] != 0) {
            counter += anim->frame_times[frame];
        } else {
            counter += anim->default_frame_time;
        }
        if(counter >= *adjusted_time) {
            *out_frame = frame;
            break;
        }
    }

    return time > anim->total_time;
}

// Gets the time (ms) that a given frame starts at
uint32 animation_get_time(const animation* anim, uint16 frame) {
    if(check_warn(frame < anim->frame_count, "Frame %d is out of bounds, animation only has %d frames", frame, anim->frame_count + 1)) {
        return 0;
    }
    uint32 counter = 1;

    for(int i = 0; i < frame; ++i) {
        if(anim->frame_times[i] != 0) {
            counter += anim->frame_times[i];
        } else {
            counter += anim->default_frame_time;
        }
    }

    return counter;
}

// Recalculates an animation's length
void animation_calculate_total_time(animation* anim) {
    anim->total_time = 0;
    for(int i = 0; i < anim->frame_count; ++i) {
        if(anim->frame_times[i] != 0) {
            anim->total_time += anim->frame_times[i];
        } else {
            anim->total_time += anim->default_frame_time;
        }
    }

    // Default the animation to 1ms if the total is 0
    if(anim->total_time == 0) {
        warn("Animations must be at least 1ms in length");
        anim->total_time = 1;
    }
}
