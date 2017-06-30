#ifndef DF_GRAPHICS_SPRITE
#define DF_GRAPHICS_SPRITE

#include "aabb.h"
#include "sprite.hd"
#include "spriteset.hd"
#include "texture.h"

sprite sprite_new(spriteset set);
void sprite_set_animation(sprite spr, const char* handle, bool force_reset);
void sprite_set_orientation(sprite spr, uint8 orient);
void sprite_set_playing(sprite spr, bool playing);
void sprite_set_position(sprite spr, float position);
void sprite_update(sprite spr, float dt);
int16 sprite_get_anim_id(sprite spr);
aabb_2d sprite_get_box(sprite spr);
gltex sprite_get_texture(sprite spr);

#endif
