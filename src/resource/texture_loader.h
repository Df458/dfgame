#ifndef DF_RESOURCE_TEXTURE_LOADER
#define DF_RESOURCE_TEXTURE_LOADER
#include "texture.h"
#ifdef enable_gif
#include <gif_lib.h>
#endif

gltex  load_texture_gl(const char* path);
rawtex load_texture_raw(const char* path);

#ifdef enable_gif
GifFileType* load_and_slurp_gif(const char* path);
bool gif_read_gcb(GifFileType* file_type, int index, GraphicsControlBlock* block);
rawtex load_gif_raw(const char* path);
#endif

#ifdef enable_png
rawtex load_png_raw(const char* path);
#endif

#ifdef enable_jpeg
rawtex load_jpeg_raw(const char* path);
#endif

#ifdef enable_tga
rawtex load_tga_raw(const char* path);
#endif

#ifdef enable_tiff
rawtex load_tiff_raw(const char* path);
#endif

#endif
