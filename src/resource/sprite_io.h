#ifndef DF_RESOURCE_SPRITE_LOADER
#define DF_RESOURCE_SPRITE_LOADER
#include "graphics/spriteset.h"

spriteset load_spriteset(const char* path);
spriteset load_spriteset_from_xml(const char* path);
spriteset load_spriteset_from_image(const char* path);

// Load animation and texture data from an image file, for use in a spriteset
bool load_animation_from_image(const char* path, animation* anim, rawtex* tex);

void save_spriteset(const char* path, spriteset set);

#endif
