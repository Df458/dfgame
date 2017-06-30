#ifndef DF_GRAPHICS_SHADER_INIT
#define DF_GRAPHICS_SHADER_INIT
#include "shader.hd"

// Initializes all built-in shaders.
void shaders_init();

// Deletes all built-in shaders.
void shaders_cleanup();

// Returns built-in shaders.
shader shader_basic_tex_get();
shader shader_basic_untex_get();
shader shader_text_get();
shader shader_test_post_get();
shader shader_wave_get();
shader shader_invert_get();
shader shader_blur_get();

#endif
