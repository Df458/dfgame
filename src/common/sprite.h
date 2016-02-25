#ifndef SPRITE_H
#define SPRITE_H
#include <stdbool.h>
#include <inttypes.h>

#include "resource_def.h"
#include "texture.h"

typedef struct animation
{
    uint8_t orients; // 0 for no(1 orient), 1 for 4(grid orient), 2 for(full orient)
    uint16_t dimensions_x;
    uint16_t dimensions_y;
    int16_t origin_x;
    int16_t origin_y;
    int8_t length;
    uint8_t delay;
    bool loop;
    bool play;
    float offset_x; // offset is for the position in the texture atlas
    float offset_y;
    float size_x; // size is for the size in the texture atlas
    float size_y;
    char* handle;
} animation;

typedef struct sprite
{
    texture* atlas;
    animation* animations;
    uint8_t animation_count;
}
sprite;


/*!
 * Creates an empty sprite
 */
sprite* create_sprite();

/*!
 * Frees a sprite and its resources
 */
void destroy_sprite(sprite* fnt);

/*!
 * This loads a sprite file, and constructs a texture atlas from the
 * necessary spritesheets.
 * See get_extended_resource_path(io_util.h) for usage details
 */
sprite* load_resource_to_sprite(resource_pair);

/*!
 * This saves a sprite.
 * See get_extended_resource_path(io_util.h) for usage details
 */
bool save_sprite_to_resource(sprite* fnt, resource_pair);

int8_t index_by_handle(sprite* spr, const char* handle);

#endif
