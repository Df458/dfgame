#ifndef DF_RESOURCE_SPRITE_LOADER
#define DF_RESOURCE_SPRITE_LOADER
#include "graphics/spriteset.hd"

spriteset load_spriteset(const char* path);
spriteset load_spriteset_from_xml(const char* path);
spriteset load_spriteset_from_image(const char* path);

void save_spriteset(const char* path, spriteset set);

#endif
