// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "graphics/spriteset.h"
#include "graphics/spriteset.priv.h"

#include "core/check.h"
#include "core/container/array.h"
#include "core/memory/alloc.h"
#include "core/stringutil.h"
#include "graphics/texture_atlas.h"
#include "math/aabb.h"

bool animation_equals(void* a, void* b, void* user) {
    return nstreq(((animation*)a)->name, (char*)b);
}

// Creates a new empty spriteset
spriteset spriteset_new(const char* path) {
    spriteset set = mscalloc(1, struct spriteset);
    set->atlas = texture_atlas_new();
    set->animations = array_mnew(animation, 8);
    set->default_animation = NULL;
    set->asset_path = nstrdup(path);

    return set;
}

// Adds a new *empty* animation to the spriteset, for editing purposes
animation* spriteset_add_animation_empty(spriteset set) {
    char* name = nstrdup(ANIMATION_DEFAULT);

    // If we have a default, we need to find a new unique name
    if(set->default_animation) {
        animation* found = set->default_animation;
        sfree(name);

        // Pick a new name
        name = nstrdup("Untitled Animation");
        uint16 i = 1;

        while(found != NULL) {
            found = NULL;

            // Search for an existing animation with the same name
            array_foreach(set->animations, iter) {
                if(nstreq(((animation*)iter.data)->name, name)) {
                    found = iter.data;

                    // Increment name
                    sfree(name);
                    name = saprintf("Untitled Animation %d", i);
                    ++i;

                    break;
                }
            }

            // Sanity check
            if(check_error(i != CONTAINER_INDEX_INVALID, "Out of default names, please rename or delete some existing animations")) {
                sfree(name);
                return NULL;
            }
        }
    }

    animation anim = (animation) {
        .orient_count = 1,
        .origin = {
            .x = 0,
            .y = 0
        },
        .texture_box = aabb_2d_zero,
        .frame_count = 1,
        .default_frame_time = 16,
        .frame_times = mscalloc(1, uint16),
        .total_time = 16,
        .texture_id = CONTAINER_INDEX_INVALID,
        .autoplay = false,
        .autoloop = false,
        .default_on_finish = false,
        .filepath = NULL,
        .name = name
    };

    array_add(set->animations, anim);

    animation* a = array_get(set->animations, array_get_length(set->animations) - 1);

    if(!set->default_animation) {
        set->default_animation = a;
    }

    return a;
}
// Adds a new animation to the spriteset, using raw pixel data
animation* spriteset_add_animation_raw(spriteset set, animation anim, rawtex texture) {
    check_return(set, "Spriteset is NULL", NULL);

    if(!array_containsp(set->animations, anim.name, animation_equals, NULL)) {
        uint16 index = texture_atlas_add_raw(set->atlas, texture, GL_RGBA);
        check_return(index != CONTAINER_INDEX_INVALID, "Can't add animation %s: texture atlas is full", NULL, anim.name);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        array_add(set->animations, anim);

        animation* a = array_get(set->animations, array_get_length(set->animations) - 1);

        // Update the default
        if(nstreq(a->name, ANIMATION_DEFAULT)) {
            set->default_animation = a;
        }
        return a;
    }

    return array_getp(set->animations, anim.name, animation_equals, NULL);
}
// Adds a new animation to the spriteset, copying from an OpenGL texture
animation* spriteset_add_animation_gl(spriteset set, animation anim, gltex texture) {
    check_return(set, "Spriteset is NULL", NULL);

    if(!array_containsp(set->animations, anim.name, animation_equals, NULL)) {
        uint16 index = texture_atlas_add_gl(set->atlas, texture, GL_RGBA);
        check_return(index != CONTAINER_INDEX_INVALID, "Can't add animation %s: texture atlas is full", NULL, anim.name);

        anim.texture_id = index;
        anim.texture_box = texture_atlas_get(set->atlas, index);
        array_add(set->animations, anim);

        animation* a = array_get(set->animations, array_get_length(set->animations) - 1);

        // Update the default
        if(nstreq(a->name, ANIMATION_DEFAULT)) {
            set->default_animation = a;
        }
        return a;
    }

    return array_getp(set->animations, anim.name, animation_equals, NULL);
}
// Removes an animation from the spriteset and deletes it
void spriteset_remove_animation(spriteset set, animation* a) {
    uint16 id = a->texture_id;
    if(id != CONTAINER_INDEX_INVALID) {
        texture_atlas_remove(set->atlas, id);
    }

    if(set->default_animation == a) {
        set->default_animation = NULL;
    }

    // Delete everything
    if(a->filepath != NULL) {
        sfree(a->filepath);
    }
    sfree(a->name);
    sfree(a->frame_times);

    array_remove(set->animations, a);

    uint16 size = texture_atlas_get_texture_count(set->atlas);
    array_foreach(set->animations, iter) {
        animation* anim = (animation*)iter.data;

        // If the animation was pointing to the last slot in the atlas, it has been swapped with the old animation
        if(id != CONTAINER_INDEX_INVALID && anim->texture_id == size) {
            anim->texture_id = id;
        }

        if(nstreq(anim->name, ANIMATION_DEFAULT)) {
            set->default_animation = anim;
        }
    }
}

// Updates an animation's texture, using raw pixel data
void spriteset_set_animation_texture_raw(spriteset set, animation* anim, rawtex texture) {
    check_return(set, "Spriteset is NULL", );

    if(anim == NULL) {
        anim = set->default_animation;
    }

    check_return(anim != NULL, "Animation is NULL, and no default exists", );

    if(anim->texture_id != CONTAINER_INDEX_INVALID) {
        texture_atlas_remove(set->atlas, anim->texture_id);
    }

    anim->filepath = nstrdup(texture.asset_path);
    anim->texture_id = texture_atlas_add_raw(set->atlas, texture, GL_RGBA);
    anim->texture_box = texture_atlas_get(set->atlas, anim->texture_id);
}
// Updates an animation's texture, copying from an OpenGL texture
void spriteset_set_animation_texture_gl(spriteset set, animation* anim, gltex texture) {
    check_return(set, "Spriteset is NULL", );

    if(anim == NULL) {
        anim = set->default_animation;
    }

    check_return(anim != NULL, "Animation is NULL, and no default exists", );

    if(anim->texture_id != CONTAINER_INDEX_INVALID) {
        texture_atlas_remove(set->atlas, anim->texture_id);
    }

    anim->texture_id = texture_atlas_add_gl(set->atlas, texture, GL_RGBA);
    anim->texture_box = texture_atlas_get(set->atlas, anim->texture_id);
}

// Updates an animation's name, but only if it's unique
bool spriteset_set_animation_name(spriteset set, animation* anim, char* name) {
    check_return(set, "Spriteset is NULL", false);

    if(anim == NULL) {
        anim = set->default_animation;
    }

    check_return(anim != NULL, "Animation is NULL, and no default exists", false);

    if(!array_containsp(set->animations, name, animation_equals, NULL)) {
        sfree(anim->name);
        anim->name = nstrdup(name);

        return true;
    }

    warn("Can't change animation \"%s\"'s name to \"%s\", as an animation with that name already exists");

    return false;
}

// Gets the numer of animations stored in the spriteset
uint16 spriteset_get_animation_count(spriteset set) {
    check_return(set, "Spriteset is NULL", 0);

    return array_get_length(set->animations);
}

// Gets an animation from the spriteset, based on the hash key
animation* spriteset_get_animation_by_name(spriteset set, const char* handle) {
    check_return(set, "Spriteset is NULL", NULL);

    array_foreach(set->animations, iter) {
        animation* anim = iter.data;

        if(nstreq(anim->name, handle)) {
            return anim;
        }
    }

    check_error(set->default_animation != NULL, "Failed to find animation \"%s\", and could not fall back to the default", handle);
    return set->default_animation;
}

// Gets an animation from the spriteset, based on the numeric id
animation* spriteset_get_animation_by_id(spriteset set, container_index handle) {
    check_return(set, "Spriteset is NULL", NULL);

    array_foreach(set->animations, iter) {
        animation* anim = iter.data;

        if(anim->texture_id == handle) {
            return anim;
        }
    }

    check_error(set->default_animation != NULL, "Failed to find animation ID %d, and could not fall back to the default", handle);
    return set->default_animation;
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

    array_foreach(set->animations, iter) {
        animation* anim = (animation*)iter.data;
        sfree(anim->filepath);
        sfree(anim->frame_times);
        sfree(anim->name);
    }
    array_free(set->animations)
    if(set->asset_path) {
        sfree(set->asset_path);
    }
    sfree(set);
}

// Gets the frame of an animation based on a given time
bool animation_get_frame(const animation* anim, uint32 time, uint16* out_frame, uint32* adjusted_time) {
    check_return(anim != NULL, "Animation is NULL", false);

    uint32 counter = 0;
    bool loop = time >= anim->total_time;

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

    return loop;
}

// Gets the time (ms) that a given frame starts at
uint32 animation_get_time(const animation* anim, uint16 frame) {
    check_return(anim != NULL, "Animation is NULL", 0);

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
    check_return(anim != NULL, "Animation is NULL", );

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
