#ifndef DF_GRAPHICS_SPRITESET_PRIV
#define DF_GRAPHICS_SPRITESET_PRIV
#include "core/container/array.h"
#include "graphics/spriteset.h"
#include "graphics/texture_atlas.h"

typedef struct spriteset {
    texture_atlas atlas;
    array animations;
    animation* default_animation;

    char* asset_path;
}* spriteset;

#endif
