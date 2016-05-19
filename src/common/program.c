#include "program.h"
#include "render.h"
#include "util.h"

// TODO: Improve this with better logging
program create_program(const char* const* vs, const char* const* fs)
{
    program p;
    p.valid = false;
    p.method = GL_TRIANGLES;

    GLuint vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    p.handle = glCreateProgram();
    glShaderSource(vertex_shader, 1, vs, NULL);
    glShaderSource(fragment_shader, 1, fs, NULL);
    GLsizei len;
    char log[1024];

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);
    glAttachShader(p.handle, vertex_shader);
    glAttachShader(p.handle, fragment_shader);
    glLinkProgram(p.handle);
    if(checkGLError())
        return p;
    glGetShaderInfoLog(vertex_shader, 1024, &len, log);
    if(len)
        info("Vertex Shader Log\n%s\n", log);
    glGetShaderInfoLog(fragment_shader, 1024, &len, log);
    if(len)
        info("Fragment Shader Log\n%s\n", log);
    glGetProgramInfoLog(p.handle, 1024, &len, log);
    if(len)
        info("Program Log\n%s\n", log);
    if(checkGLError())
        return p;

    glDetachShader(p.handle, vertex_shader);
    glDetachShader(p.handle, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    p.valid = true;

    return p;
}

void delete_program(program* p)
{
    glDeleteProgram(p->handle);
    (*p).handle = 0;
    (*p).valid = false;
}

bool bind_float_to_program(program p, const char* name, float f)
{
    glUseProgram(p.handle);
    GLuint uni_handle = glGetUniformLocation(p.handle, name);
    glUniform1f(uni_handle, f);
    if(checkGLError())
        return false;
    return true;
}

bool bind_vec2_to_program(program p, const char* name, vec2 v)
{
    glUseProgram(p.handle);
    GLuint uni_handle = glGetUniformLocation(p.handle, name);
    glUniform2fv(uni_handle, 1, v.data);
    if(checkGLError())
        return false;
    return true;
}

bool bind_vec3_to_program(program p, const char* name, vec3 v)
{
    glUseProgram(p.handle);
    GLuint uni_handle = glGetUniformLocation(p.handle, name);
    glUniform3fv(uni_handle, 1, v.data);
    if(checkGLError())
        return false;
    return true;
}

bool bind_vec4_to_program(program p, const char* name, vec4 v)
{
    glUseProgram(p.handle);
    GLuint uni_handle = glGetUniformLocation(p.handle, name);
    glUniform4fv(uni_handle, 1, v.data);
    if(checkGLError())
        return false;
    return true;
}

bool bind_mat4_to_program(program p, const char* name, mat4 mat)
{
    glUseProgram(p.handle);
    GLuint uni_handle = glGetUniformLocation(p.handle, name);
    glUniformMatrix4fv(uni_handle, 1, GL_FALSE, mat.data);
    return true;
}

bool bind_texture_to_program(program p, const char* name, texture* tex, GLuint bind_index)
{
    glActiveTexture(bind_index);
    GLuint uni_handle = glGetUniformLocation(p.handle, name);
    glBindTexture(tex->type, tex->handle);
    glUniform1i(uni_handle, 0);
    if(checkGLError())
        return false;
    return true;
}
