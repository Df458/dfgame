#ifndef DF_GRAPHICS_SPRITE
#define DF_GRAPHICS_SPRITE

#include "core/container/container_common.h"
#include "graphics/shader.hd"
#include "graphics/sprite.hd"
#include "graphics/spriteset.hd"
#include "graphics/texture.h"
#include "math/aabb.h"
#include "math/matrix.h"

sprite sprite_new(spriteset set);
#define sprite_set_animation(s, h, f) _Generic(h,\
    void*: sprite_set_animation_name,\
    char*: sprite_set_animation_name,\
    const char*: sprite_set_animation_name,\
    int16: sprite_set_animation_id\
)(s, h, f)
void sprite_set_animation_name(sprite spr, const char* handle, bool force_reset);
void sprite_set_animation_id(sprite spr, container_index handle, bool force_reset);
animation* sprite_get_animation(sprite spr);

void sprite_set_orientation(sprite spr, uint8 orient);
uint8 sprite_get_orientation(sprite spr);

void sprite_set_playing(sprite spr, bool playing);
bool sprite_get_playing(sprite spr);

uint32 sprite_get_position(const sprite spr);
void sprite_set_position(sprite spr, uint32 position);

uint16 sprite_get_frame(const sprite spr);
void sprite_set_frame(sprite spr, uint16 frame);

void sprite_update(sprite spr, float dt);

container_index sprite_get_anim_id(sprite spr);
aabb_2d sprite_get_box(sprite spr);
gltex sprite_get_texture(sprite spr);
spriteset sprite_get_data(sprite spr);

void sprite_draw(sprite spr, shader s, mat4 model, mat4 view);

#define sprite_free(s, f) { _sprite_free(s, f); s = NULL; }
void _sprite_free(sprite spr, bool free_src);

#endif
