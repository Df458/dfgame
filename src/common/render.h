#ifndef DF_RENDER_H
#define DF_RENDER_H
#include "render_def.h"
#include "matrix.h"
#include "texture.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdbool.h>

#define checkGLError() _checkGLError(__FILE__, __LINE__)

bool init_renderer();
bool cleanup_renderer();

bool render_quad(mat4 camera, mat4 transform, texture* tex, bool use_dims);
bool render_quad_untex(mat4 camera, mat4 transform);
bool render_quad_subtex(mat4 camera, mat4 transform, texture* tex, bool use_dims, float tx, float ty, float tw, float th);

#endif
