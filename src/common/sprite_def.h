#ifndef DF_SPRITE_DEF_H
#define DF_SPRITE_DEF_H
#include "texture.h"

struct texture_atlas_box
{
    float pos_x;
    float pos_y;
    float size_x;
    float size_y;
};

struct animation
{
    // TODO: update orients to orient_count, make more arbitrary
    uint8_t   orients;
    uint16_t  dimensions_x;
    uint16_t  dimensions_y;
    int16_t   origin_x;
    int16_t   origin_y;
    int16_t   length;
    uint16_t  delay;
    bool      autoplay;
    bool      autoloop;
    float     speed_mod;

    struct    texture_atlas_box box;
    char*     handle;
};

struct spriteset
{
    texture* atlas;
    struct animation* animations; // TODO: Make this use pointers, not values
    uint8_t animation_count;
};

struct sprite
{
    struct spriteset* source;
    struct animation* handle;

    float position;
    bool  playing;
};
#endif
