#ifndef DF_GRAPHICS_SPRITESET
#define DF_GRAPHICS_SPRITESET

#include "spriteset.hd"
#include "aabb.h"
#include "texture.h"

typedef struct animation {
    uint8 orient_count;
    struct {
        int16 x;
        int16 y;
    } origin;
    aabb_2d texture_box;

    uint16 start_delay;
    uint16 frame_count;
    uint16 fps;

    int16 texture_id;

    bool autoplay;
    bool autoloop;
    bool default_on_finish;
} animation;

spriteset spriteset_new();
#define spriteset_add_animation(s, a, t, h) _Generic(t,\
    gltex: spriteset_add_animation_gl,\
    rawtex: spriteset_add_animation_raw\
)(s, a, t, h)
void spriteset_add_animation_raw(spriteset set, animation anim, rawtex texture, const char* handle);
void spriteset_add_animation_gl(spriteset set, animation anim, gltex texture, const char* handle);
uint16 spriteset_get_animation_count(spriteset set);
animation spriteset_get_animation(spriteset set, const char* handle);
gltex spriteset_get_texture(spriteset set);

#define spriteset_free(s) { _spriteset_free(s); s = NULL; }
void _spriteset_free(spriteset set);

#endif
