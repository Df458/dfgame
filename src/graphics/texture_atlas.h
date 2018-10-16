#ifndef DF_GRAPHICS_TEXTURE_ATLAS
#define DF_GRAPHICS_TEXTURE_ATLAS

#include "texture_atlas.hd"

#include "aabb.h"
#include "texture.h"

texture_atlas texture_atlas_new();

#define texture_atlas_add(a, t) _Generic(t,\
    rawtex: texture_atlas_add_raw,\
    gltex: texture_atlas_add_gl\
)(a, t, GL_RGBA)
int16 texture_atlas_add_raw(texture_atlas atlas, rawtex tex, GLenum mode);
int16 texture_atlas_add_gl(texture_atlas atlas, gltex tex, GLenum mode);
void texture_atlas_remove(texture_atlas atlas, int16 index);

aabb_2d texture_atlas_get(texture_atlas atlas, int16 index);
uint16 texture_atlas_get_texture_count(texture_atlas atlas);

// Frees the given atlas, and all resources contained within it
#define texture_atlas_free(atlas) { _texture_atlas_free(atlas); atlas = NULL; }
void _texture_atlas_free(texture_atlas atlas);
gltex texture_atlas_get_texture(texture_atlas atlas);

#endif
