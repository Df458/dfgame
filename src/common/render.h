#ifndef DF_RENDER_H
#define DF_RENDER_H
#include "matrix.h"
#include "texture.h"
#include "vector.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdbool.h>

bool _checkGLError(const char* file, unsigned line);

#define checkGLError() _checkGLError(__FILE__, __LINE__)

bool init_renderer();
bool cleanup_renderer();

bool render_quad_color(mat4 camera, mat4 transform, texture* tex, bool use_dims, vec4 color);
#define render_quad(camera, transform, tex, use_dims) render_quad_color(camera, transform, tex, use_dims, create_vec4_data(1, 1, 1, 1))
bool render_quad_untex_color(mat4 camera, mat4 transform, vec4 color);
#define render_quad_untex(camera, transform) render_quad_untex_color(camera, transform, create_vec4_data(1, 1, 1, 1))
bool render_quad_subtex_color(mat4 camera, mat4 transform, texture* tex, bool use_dims, vec4 uv_rect, vec4 color);
#define render_quad_subtex(camera, transform, tex, use_dims, uv_rect) render_quad_subtex_color(camera, transform, tex, use_dims, uv_rect, create_vec4_data(1, 1, 1, 1))

GLuint get_quad_buffer();

#endif
