// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "shader_init.h"

#include "shader.h"

#include "shader_basic_tex.h"
#include "shader_basic_untex.h"
#include "shader_text.h"

static shader shader_basic_tex;
static shader shader_basic_untex;
static shader shader_text;

// Initializes all built-in shaders.
void shaders_init() {
    shader_basic_tex = compile_shader_basic_tex();
    shader_basic_untex = compile_shader_basic_untex();
    shader_text = compile_shader_text();
}

// Deletes all built-in shaders.
void shaders_cleanup() {
    shader_free(shader_basic_tex);
    shader_free(shader_basic_untex);
    shader_free(shader_text);
}

// Returns built-in shaders.
shader shader_basic_tex_get() { return shader_basic_tex; }
shader shader_basic_untex_get() { return shader_basic_untex; }
shader shader_text_get() { return shader_text; }
