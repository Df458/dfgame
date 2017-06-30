#ifndef DF_RESOURCE_FONT_LOADER
#define DF_RESOURCE_FONT_LOADER
#include "font.hd"

font load_font(const char* path, uint16 height);
font load_freetype_font(const char* path, uint16 height);

#endif
