#ifndef DF_PROGRAM_H
#define DF_PROGRAM_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdbool.h>

#include "vector.h"
#include "matrix.h"
#include "texture.h"

typedef struct program
{
    GLuint handle;
    bool   valid;
}
program;

program create_program(const char** vs, const char** fs);
void delete_program(program* p);

bool bind_float_to_program(program p,   const char* name, float f);
bool bind_vec2_to_program(program p,    const char* name, vec2 v);
bool bind_vec3_to_program(program p,    const char* name, vec3 v);
bool bind_vec4_to_program(program p,    const char* name, vec4 v);
bool bind_mat4_to_program(program p,    const char* name, mat4 mat);
bool bind_texture_to_program(program p, const char* name, texture* tex, GLuint bind_index);

#endif
