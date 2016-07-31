#ifndef DF_RENDER_DEF_H
#define DF_RENDER_DEF_H
#include <stdbool.h>

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
static const char* text_gs[] =
{
    "#version 330\n"
    "layout(points) in;\n"
    "layout(triangle_strip, max_vertices=4) out;\n"
    "void main() {\n"
    "gl_Position = gl_in[0].gl_Position + vec4(-0.5, -0.5, 0, 0);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + vec4(0.5, -0.5, 0, 0);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + vec4(-0.5, 0.5, 0, 0);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + vec4(0.5, 0.5, 0, 0);\n"
    "EmitVertex();\n"
    "EndPrimitive();\n"
    "}\n"
};
static const char* particle_gs[] =
{
    "#version 330\n"
    "layout(points) in;\n"
    "layout(triangle_strip, max_vertices=4) out;\n"
    "in vec2 i_uv[1];\n"
    "in vec2 i_scale[1];\n"
    "out vec2 v_cuv;\n"
    "out vec2 v_tuv;\n"
    "uniform mat4 transform;\n"
    "void main() {\n"
    "gl_Position = gl_in[0].gl_Position + transform * (vec4(-0.5 * i_scale[0].x, -0.5 * i_scale[0].y, 0, 0));\n"
    "v_cuv = i_uv[0];\n"
    "v_tuv = vec2(0, 0);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + transform * (vec4(0.5 * i_scale[0].x, -0.5 * i_scale[0].y, 0, 0));\n"
    "v_cuv = i_uv[0];\n"
    "v_tuv = vec2(1, 0);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + transform * (vec4(-0.5 * i_scale[0].x, 0.5 * i_scale[0].y, 0, 0));\n"
    "v_cuv = i_uv[0];\n"
    "v_tuv = vec2(0, 1);\n"
    "EmitVertex();\n"
    "gl_Position = gl_in[0].gl_Position + transform * (vec4(0.5 * i_scale[0].x, 0.5 * i_scale[0].y, 0, 0));\n"
    "v_cuv = i_uv[0];\n"
    "v_tuv = vec2(1, 1);\n"
    "EmitVertex();\n"
    "EndPrimitive();\n"
    "}\n"
};

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
    "layout(location = 0) in vec3 i_pos;\n"
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
static const char* particle_vs[] =
{
    "#version 330\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D data;\n"
    "uniform sampler2D s_data;\n"
    "uniform sampler2D scale;\n"
    "uniform mat4 transform;\n"
    "uniform float lifetime;\n"
    "out vec2 i_uv;\n"
    "out vec2 i_scale;\n"
    "void main() {\n"
    "vec4 pos = texture2D(data, texCoord);\n"
    "vec4 dat = texture2D(s_data, texCoord);\n"
    "gl_Position = transform * vec4(pos.rgb, 1);\n"
    "i_uv    = vec2(dat.r, 1 - (pos.a / lifetime));\n"
    "i_scale = texture2D(scale, vec2(dat.g, 1 - (pos.a / lifetime))).rg;\n"
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
    "f_color = texture2D(texture, v_uv) * color;\n"
    "}\n"
};
static const char* text_fs[] =
{
    "#version 330\n"
    "uniform sampler2D texture;\n"
    "uniform vec4 color;\n"
    "in  vec2 v_uv;\n"
    "layout(location = 0) out vec4 f_color;\n"
    "void main() {\n"
    "f_color = color;\n"
    "f_color.a = texture2D(texture, v_uv).a;\n"
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
static const char* particle_fs[] =
{
    "#version 330\n"
    "uniform sampler2D color;\n"
    "uniform sampler2D texture;\n"
    "in vec2 v_cuv;\n"
    "in vec2 v_tuv;\n"
    "layout(location = 0) out vec4 f_color;\n"
    "void main() {\n"
    "f_color = texture2D(texture, v_tuv) * texture2D(color, v_cuv);\n"
    "}\n"
};

#endif
