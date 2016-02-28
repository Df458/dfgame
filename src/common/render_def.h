#ifndef DF_RENDER_DEF_H
#define DF_RENDER_DEF_H
#include <stdbool.h>

bool _checkGLError(const char* file, unsigned line);

// Buffers
static const float QUAD_BUFFER_DATA[] =
{
    // Vertex positon/uv
     0.5,  -0.5,  0.0,  1.0,  1.0,
    -0.5,  -0.5,  0.0,  0.0,  1.0,
    -0.5,   0.5,  0.0,  0.0,  0.0,

     0.5,  -0.5,  0.0,  1.0,  1.0,
    -0.5,   0.5,  0.0,  0.0,  0.0,
     0.5,   0.5,  0.0,  1.0,  0.0,
};

// Shaders
static const char* quad_vs[] =
{
    "#version 330\n"
    "uniform mat4 transform;\n"
    "layout(location = 0) in  vec3 i_pos;\n"
    "in  vec2 i_uv;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "gl_Position = transform * vec4(i_pos, 1.0);\n"
    "v_uv  = i_uv;\n"
    "}\n"
};
static const char* quad_subtex_vs[] =
{
    "#version 330\n"
    "uniform mat4 transform;\n"
    "uniform vec4 uv_offset;\n"
    "layout(location = 0) in  vec3 i_pos;\n"
    "in  vec2 i_uv;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "gl_Position = transform * vec4(i_pos, 1.0);\n"
    "v_uv  = i_uv * uv_offset.zw + uv_offset.xy;\n"
    "}\n"
};
static const char* quad_untex_vs[] =
{
    "#version 330\n"
    "uniform mat4 transform;\n"
    "layout(location = 0) in vec3 i_pos;\n"
    "void main() {\n"
    "gl_Position = transform * vec4(i_pos, 1.0);\n"
    "}\n"
};

static const char* quad_fs[] =
{
    "#version 330\n"
    "uniform sampler2D texture;\n"
    "uniform vec4 color;\n"
    "in  vec2 v_uv;\n"
    "layout(location = 0) out vec4 f_color;\n"
    "void main() {\n"
    "f_color = texture2D(texture, v_uv) * color;"
    "}\n"
};
static const char* quad_untex_fs[] =
{
    "#version 330\n"
    "uniform vec4 color;\n"
    "layout(location = 0) out vec4 f_color;\n"
    "void main() {\n"
    "f_color = color;\n"
    "}\n"
};

#endif
