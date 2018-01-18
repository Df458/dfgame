#ifndef DF_GRAPHICS_SHADER_INIT
#define DF_GRAPHICS_SHADER_INIT
#include "shader.hd"

// Initializes all built-in shaders.
void shaders_init();

// Deletes all built-in shaders.
void shaders_cleanup();

// Returns built-in shaders.
shader shader_debug_get();
shader shader_basic_tex_get();
shader shader_basic_untex_get();
shader shader_text_get();

#endif
