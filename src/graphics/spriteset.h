#ifndef DF_GRAPHICS_SPRITESET
#define DF_GRAPHICS_SPRITESET

#include "graphics/spriteset.hd"
#include "math/aabb.h"
#include "graphics/texture.h"

typedef struct animation {
    uint8 orient_count;
    struct {
        int16 x;
        int16 y;
    } origin;
    aabb_2d texture_box;

    uint16 frame_count;
    uint16 default_frame_time;
    uint16* frame_times;
    uint16 total_time;

    int16 texture_id;

    bool autoplay;
    bool autoloop;
    bool default_on_finish;

    char* filepath;
    char* name;
} animation;


// Creates a new empty spriteset
spriteset spriteset_new(const char* path);

#define spriteset_add_animation(s, a, t) _Generic(t,\
    gltex: spriteset_add_animation_gl,\
    rawtex: spriteset_add_animation_raw\
)(s, a, t)
// Adds a new animation to the spriteset, using raw pixel data
void spriteset_add_animation_raw(spriteset set, animation anim, rawtex texture);
// Adds a new animation to the spriteset, copying from an OpenGL texture
void spriteset_add_animation_gl(spriteset set, animation anim, gltex texture);
// Removes an animation form the spriteset, deletes it, and rearranges the atlas
void spriteset_remove_animation(spriteset set, animation* a);

// Gets the numer of animations stored in the spriteset
uint16 spriteset_get_animation_count(spriteset set);
#define spriteset_get_animation(s, h) _Generic(h,\
    void*: spriteset_get_animation_by_name,\
    char*: spriteset_get_animation_by_name,\
    const char*: spriteset_get_animation_by_name,\
    int16: spriteset_get_animation_by_id\
)(s, h)
// Gets an animation from the spriteset, based on the hash key
animation* spriteset_get_animation_by_name(spriteset set, const char* handle);
// Gets an animation from the spriteset, based on the numeric id
animation* spriteset_get_animation_by_id(spriteset set, int16 handle);

// Gets the underlying texture of the spriteset
gltex spriteset_get_texture(spriteset set);
// Gets the filepath of the spriteset
char* spriteset_get_path(spriteset set);

// Frees the given spriteset, and all resources contained within it
#define spriteset_free(s) { _spriteset_free(s); s = NULL; }
void _spriteset_free(spriteset set);

// Gets the frame of an animation based on a given time
bool animation_get_frame(const animation* anim, uint32 time, uint16* out_frame, uint32* adjusted_time);
// Gets the time (ms) that a given frame starts at
uint32 animation_get_time(const animation* anim, uint16 frame);
// Recalculates an animation's length
void animation_calculate_total_time(animation* anim);

#endif
