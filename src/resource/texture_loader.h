#ifndef DF_RESOURCE_TEXTURE_LOADER
#define DF_RESOURCE_TEXTURE_LOADER
#include "texture.h"

gltex  load_texture_gl(const char* path);
rawtex load_texture_raw(const char* path);
rawtex load_png_raw(const char* path);
rawtex load_jpeg_raw(const char* path);
rawtex load_tga_raw(const char* path);
rawtex load_tiff_raw(const char* path);

#endif
