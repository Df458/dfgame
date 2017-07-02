#ifndef DF_GRAPHICS_SHADER
#define DF_GRAPHICS_SHADER
#include <stdarg.h>

#include "camera.hd"
#include "matrix.hd"
#include "mesh.hd"
#include "text.h"
#include "texture.h"
#include "transform.hd"
#include "vector.hd"
#include <GL/glew.h>
#include <GL/gl.h>

// Represents an OpenGL shader program.
typedef struct shader {
    GLuint id;
} shader;

// Compiles a new shader program with the specified vertex and fragment shaders
#define shader_new_simple(vs, fs) shader_new_vf(vs, fs);
shader shader_new_vf(const char* const* vs, const char* const* fs);

// Compiles a new shader program with the specified vertex, geometry, and fragment shaders
#define shader_new(vs, gs, fs) shader_new_vgf(vs, gs, fs);
shader shader_new_vgf(const char* const* vs, const char* const* gs, const char* const* fs);

#define shader_free(s) { _shader_free(s); s.id = 0; }
void _shader_free(shader s);

// Binds uniform v to shader s using name
#define shader_bind_uniform_name(s, name, v) _Generic(v,\
        float: shader_bind_uniform_float_name,\
        int:   shader_bind_uniform_int_name,\
        vec2:  shader_bind_uniform_vec2_name,\
        vec3:  shader_bind_uniform_vec3_name,\
        vec4:  shader_bind_uniform_vec4_name,\
        mat4:  shader_bind_uniform_mat4_name,\
        transform:  shader_bind_uniform_transform_name,\
        camera:  shader_bind_uniform_camera_name\
    )(s, name, v)

// Binds uniform v to the given OpenGL handle
#define shader_bind_uniform(handle, v) _Generic(v,\
        float: shader_bind_uniform_float,\
        int:   shader_bind_uniform_int,\
        vec2:  shader_bind_uniform_vec2,\
        vec3:  shader_bind_uniform_vec3,\
        vec4:  shader_bind_uniform_vec4,\
        mat4:  shader_bind_uniform_mat4,\
        transform:  shader_bind_uniform_transform,\
        camera:  shader_bind_uniform_camera\
    )(handle, v)

void shader_bind_uniform_float_name(shader s, const char* name, float f);
void shader_bind_uniform_int_name(shader s,   const char* name, int i);
void shader_bind_uniform_vec2_name(shader s,  const char* name, vec2 v);
void shader_bind_uniform_vec3_name(shader s,  const char* name, vec3 v);
void shader_bind_uniform_vec4_name(shader s,  const char* name, vec4 v);
void shader_bind_uniform_mat4_name(shader s,  const char* name, mat4 m);
void shader_bind_uniform_transform_name(shader s,  const char* name, transform m);
void shader_bind_uniform_camera_name(shader s,  const char* name, camera m);
void shader_bind_uniform_texture_name(shader s, const char* name, gltex t, GLuint bind_index);
void shader_bind_uniform_float(GLint handle, float f);
void shader_bind_uniform_int(GLint handle, int i);
void shader_bind_uniform_vec2(GLint handle, vec2 v);
void shader_bind_uniform_vec3(GLint handle, vec3 v);
void shader_bind_uniform_vec4(GLint handle, vec4 v);
void shader_bind_uniform_mat4(GLint handle, mat4 m);
void shader_bind_uniform_transform(GLint handle, transform m);
void shader_bind_uniform_camera(GLint handle, camera m);
void shader_bind_uniform_texture(GLint handle, gltex t, GLuint bind_index);

// Binds an attribute to shader s using a given list of names and vertex types.
// This function takes a string followed by a vertex_types, and binds that part
// of the mesh's vertices to the specified attribute name.
// The function that starts with an underscore must end in NULL, but the macro
// appends this for you.
#define shader_bind_attribute_mesh(s, m, ...) _shader_bind_attribute_mesh(s, m, __VA_ARGS__, 0)
void _shader_bind_attribute_mesh(shader s, mesh m, ...);
void shader_bind_attribute_mesh_va(shader s, mesh m, va_list args);

#endif
