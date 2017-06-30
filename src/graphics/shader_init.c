// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "shader_init.h"

#include "shader.h"

#include "shader_basic_tex.h"
#include "shader_basic_untex.h"
#include "shader_text.h"
#include "shader_test_post.h"
#include "blur.h"
#include "invert.h"
#include "wave.h"

static shader shader_basic_tex;
static shader shader_basic_untex;
static shader shader_text;
static shader shader_test_post;
static shader shader_blur;
static shader shader_invert;
static shader shader_wave;

// Initializes all built-in shaders.
void shaders_init() {
    shader_basic_tex = compile_shader_basic_tex();
    shader_basic_untex = compile_shader_basic_untex();
    shader_text = compile_shader_text();
    shader_test_post = compile_shader_test_post();
    shader_invert = compile_shader_invert();
    shader_blur = compile_shader_blur();
    shader_wave = compile_shader_wave();
}

// Deletes all built-in shaders.
void shaders_cleanup() {
    shader_free(shader_basic_tex);
    shader_free(shader_basic_untex);
    shader_free(shader_text);
    shader_free(shader_test_post);
}

// Returns built-in shaders.
shader shader_basic_tex_get() { return shader_basic_tex; }
shader shader_basic_untex_get() { return shader_basic_untex; }
shader shader_text_get() { return shader_text; }
shader shader_test_post_get() { return shader_test_post; }
shader shader_blur_get() { return shader_blur; }
shader shader_invert_get() { return shader_invert; }
shader shader_wave_get() { return shader_wave; }
