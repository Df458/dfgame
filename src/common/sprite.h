#ifndef SPRITE_H
#define SPRITE_H
#include <stdbool.h>
#include <inttypes.h>

#include "resource_def.h"
#include "matrix.h"

typedef struct animation animation;
typedef struct spriteset spriteset;
typedef struct sprite sprite;


animation* create_animation();
void destroy_animation(animation* anim);
bool save_animation_to_resource(animation* anim, resource_pair);
animation* load_resource_to_animation(resource_pair);

spriteset* create_spriteset();
void destroy_spriteset(spriteset* spr);
bool save_spriteset_to_resource(spriteset* spr, resource_pair);
spriteset* load_resource_to_spriteset(resource_pair);

sprite* create_sprite(spriteset* set);
void destroy_sprite(sprite* spr);
bool save_sprite_to_resource(sprite* spr, resource_pair);
sprite* load_resource_to_sprite(resource_pair);


bool sprite_set_animation(sprite* spr, int16_t index);
void sprite_set_playing(sprite* spr, bool play);
bool sprite_update(sprite* spr, float delta);
bool sprite_draw(mat4 camera, mat4 transform, sprite* spr, bool use_dims);


int16_t index_by_handle(spriteset* spr, const char* handle);

#endif
