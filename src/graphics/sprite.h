#ifndef DF_GRAPHICS_SPRITE
#define DF_GRAPHICS_SPRITE

#include "aabb.h"
#include "matrix.h"
#include "shader.hd"
#include "sprite.hd"
#include "spriteset.hd"
#include "texture.h"

sprite sprite_new(spriteset set);
#define sprite_set_animation(s, h, f) _Generic(h,\
    void*: sprite_set_animation_name,\
    char*: sprite_set_animation_name,\
    const char*: sprite_set_animation_name,\
    int16: sprite_set_animation_id\
)(s, h, f)
void sprite_set_animation_name(sprite spr, const char* handle, bool force_reset);
void sprite_set_animation_id(sprite spr, int16 handle, bool force_reset);
void sprite_set_orientation(sprite spr, uint8 orient);
uint8 sprite_get_orientation(sprite spr);
void sprite_set_playing(sprite spr, bool playing);
bool sprite_get_playing(sprite spr);
void sprite_set_position(sprite spr, float position);
void sprite_update(sprite spr, float dt);
int16 sprite_get_anim_id(sprite spr);
aabb_2d sprite_get_box(sprite spr);
gltex sprite_get_texture(sprite spr);
spriteset sprite_get_data(sprite spr);

void sprite_draw(sprite spr, shader s, mat4 model, mat4 view);

#define sprite_free(s, f) { _sprite_free(s, f); s = NULL; }
void _sprite_free(sprite spr, bool free_src);

#endif
