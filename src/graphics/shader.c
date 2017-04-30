// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "shader.h"

#include "camera.h"
#include "log/log.h"
#include "matrix.h"
#include "mesh.h"
#include "transform.h"
#include "vector.h"
#include "vertex.hd"

#include <stdarg.h>


// Compiles a new shader program with the specified vertex and fragment shaders
shader shader_new_vf(const char* const* vs, const char* const* fs) {
    shader s;

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vs, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fs, NULL);
    glCompileShader(fragment_shader);

    s.id = glCreateProgram();
    glAttachShader(s.id, vertex_shader);
    glAttachShader(s.id, fragment_shader);
    glLinkProgram(s.id);

    GLsizei len;
    char log[1024];
    glGetShaderInfoLog(vertex_shader, 1024, &len, log);
    if(len)
        info("Vertex shader log:\n%s", log);

    glGetShaderInfoLog(fragment_shader, 1024, &len, log);
    if(len)
        info("Fragment shader log:\n%s", log);

    glDetachShader(s.id, vertex_shader);
    glDetachShader(s.id, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return s;
}

// Compiles a new shader program with the specified vertex, geometry, and fragment shaders
shader shader_new_vgf(const char* const* vs, const char* const* gs, const char* const* fs) {
    shader s;

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vs, NULL);
    glCompileShader(vertex_shader);

    GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry_shader, 1, fs, NULL);
    glCompileShader(geometry_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fs, NULL);
    glCompileShader(fragment_shader);

    s.id = glCreateProgram();
    glAttachShader(s.id, vertex_shader);
    glAttachShader(s.id, fragment_shader);
    glAttachShader(s.id, geometry_shader);
    glLinkProgram(s.id);

    GLsizei len;
    char log[1024];
    glGetShaderInfoLog(vertex_shader, 1024, &len, log);
    if(len)
        info("Vertex shader log:\n%s", log);
    glGetShaderInfoLog(fragment_shader, 1024, &len, log);
    if(len)
        info("Fragment shader log:\n%s", log);
    glGetShaderInfoLog(geometry_shader, 1024, &len, log);
    if(len)
        info("Geometry shader log:\n%s\n%s", log, *gs);

    glDetachShader(s.id, vertex_shader);
    glDetachShader(s.id, fragment_shader);
    glDetachShader(s.id, geometry_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);

    return s;
}

void _shader_free(shader s) {
    glDeleteShader(s.id);
}

// Binds a uniform to a given OpenGL handle.
void shader_bind_uniform_float(GLint handle, float f) {
    glUniform1f(handle, f);
}
void shader_bind_uniform_int(GLint handle, int i) {
    glUniform1i(handle, i);
}
void shader_bind_uniform_vec2(GLint handle, vec2 v) {
    glUniform2fv(handle, 1, v.data);
}
void shader_bind_uniform_vec3(GLint handle, vec3 v) {
    glUniform3fv(handle, 1, v.data);
}
void shader_bind_uniform_vec4(GLint handle, vec4 v) {
    glUniform4fv(handle, 1, v.data);
}
void shader_bind_uniform_mat4(GLint handle, mat4 m) {
    glUniformMatrix4fv(handle, 1, false, m.data);
}
void shader_bind_uniform_transform(GLint handle, transform m) {
    glUniformMatrix4fv(handle, 1, false, transform_get_matrix(m).data);
}
void shader_bind_uniform_camera(GLint handle, camera m) {
    glUniformMatrix4fv(handle, 1, false, camera_get_vp(m).data);
}

// Binds a uniform to shader s using a given name.
void shader_bind_uniform_float_name(shader s, const char* name, float f) {
    glUniform1f(glGetUniformLocation(s.id, name), f);
}
void shader_bind_uniform_int_name(shader s,   const char* name, int i) {
    glUniform1i(glGetUniformLocation(s.id, name), i);
}
void shader_bind_uniform_vec2_name(shader s,  const char* name, vec2 v) {
    glUniform2fv(glGetUniformLocation(s.id, name), 1, v.data);
}
void shader_bind_uniform_vec3_name(shader s,  const char* name, vec3 v) {
    glUniform3fv(glGetUniformLocation(s.id, name), 1, v.data);
}
void shader_bind_uniform_vec4_name(shader s,  const char* name, vec4 v) {
    glUniform4fv(glGetUniformLocation(s.id, name), 1, v.data);
}
void shader_bind_uniform_mat4_name(shader s,  const char* name, mat4 m) {
    glUniformMatrix4fv(glGetUniformLocation(s.id, name), 1, false, m.data);
}
void shader_bind_uniform_transform_name(shader s,  const char* name, transform m) {
    glUniformMatrix4fv(glGetUniformLocation(s.id, name), 1, false, transform_get_matrix(m).data);
}
void shader_bind_uniform_camera_name(shader s,  const char* name, camera m) {
    glUniformMatrix4fv(glGetUniformLocation(s.id, name), 1, false, camera_get_vp(m).data);
}

// Binds an attribute to shader s using a given list of names and vertex types.
// This function takes a string followed by a vertex_types, and binds that part
// of the mesh's vertices to the specified attribute name.
// This function must end in NULL.
void _shader_bind_attribute_mesh(shader s, mesh m, ...) {
    va_list args;
    va_start(args, m);

    glBindBuffer(GL_ARRAY_BUFFER, mesh_get_handle(m));
    const char* name = va_arg(args, const char*);
    while(name) {
        vertex_types type = va_arg(args, vertex_types);

        if(!mesh_has_vertex_data(m, type)) {
            warn("Mesh does not contain vertex type 0x%x. Attribute %s will not be bound.\n", type, name);
            continue;
        }

        GLvoid* offset = mesh_get_element_offset(m, type);
        int pack_distance = mesh_get_data_size(m);
        int data_size = 0;
        GLenum data_type;

        switch(type) {
            case VT_POSITION:
                data_size = 3;

                data_type = GL_FLOAT;
            break;
            case VT_NORMAL:
                data_size = 3;

                data_type = GL_FLOAT;
            break;
            case VT_TEXTURE:
                data_size = 2;

                data_type = GL_FLOAT;
            break;
            case VT_COLOR:
                data_size = 4;

                data_type = GL_FLOAT;
                break;
        }

        GLuint attrib = glGetAttribLocation(s.id, name);
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, data_size, data_type, GL_FALSE, pack_distance, (GLvoid*)offset);
        name = va_arg(args, const char*);
    }

    va_end(args);
}
