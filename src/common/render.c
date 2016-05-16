#include "render.h"
#include "render_def.h"
#include "util.h"
#include "program.h"

///////////////////////////////////////////////////////////////////////////////
// Hidden data
///////////////////////////////////////////////////////////////////////////////

GLuint quad_buffer;

program p_quad;
GLuint va_quad_pos;
GLuint va_quad_uv;

program p_quad_untex;
GLuint va_quad_untex;

program p_quad_subtex;
GLuint va_quad_subtex_pos;
GLuint va_quad_subtex_uv;

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////

bool _checkGLError(const char* file, unsigned line)
{
    GLenum error;
    bool error_found = false;
    do {
        error = glGetError();
        if(error != GL_NO_ERROR) {
            error_found = 1;
            const char* message;

            switch(error) {
                case GL_INVALID_ENUM:
                    message = "Invalid Enum";
                    break;
                case GL_INVALID_VALUE:
                    message = "Invalid Parameter";
                    break;
                case GL_INVALID_OPERATION:
                    message = "Illegal Operation";
                    break;
                default:
                    _log(file, line, LOG_WARNING, "OpenGL: Received error code 0x%0X. Consider adding a custom message to handle it", error);
                    message = "Unknown Error";
            }

            _log(file, line, LOG_ERROR, "OpenGL: %s", message);
        }
    } while(error != GL_NO_ERROR);
    return error_found;
}

bool init_renderer()
{
    // Buffers
    glGenBuffers(1, &quad_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), QUAD_BUFFER_DATA, GL_STATIC_DRAW);
    if(checkGLError())
        return false;

    // TODO: Add variadic uniform/attribute handle retrieval
    // Shaders
    p_quad_untex = create_program(quad_untex_vs, quad_untex_fs
        /*"transform",  &u_quad_untex_transform,*/
        /*"color",      &u_quad_untex_color,*/
        /*"i_pos",      &va_quad_untex*/
    );
    va_quad_untex = glGetAttribLocation(p_quad_untex.handle, "i_pos");
    if(checkGLError())
        return false;

    p_quad = create_program(quad_vs, quad_fs);
    va_quad_pos = glGetAttribLocation(p_quad.handle, "i_pos");
    va_quad_uv = glGetAttribLocation(p_quad.handle, "i_uv");
    if(checkGLError())
        return false;

    p_quad_subtex = create_program(quad_subtex_vs, quad_fs);
    va_quad_subtex_pos = glGetAttribLocation(p_quad_subtex.handle, "i_pos");
    va_quad_subtex_uv = glGetAttribLocation(p_quad_subtex.handle, "i_uv");
    if(checkGLError())
        return false;

    return true;
}

bool cleanup_renderer()
{
    glDeleteBuffers(1, &quad_buffer);
    delete_program(&p_quad);
    delete_program(&p_quad_untex);
    delete_program(&p_quad_subtex);

    return true;
}

bool render_quad_color(mat4 camera, mat4 transform, texture* tex, bool use_dims, vec4 color)
{
    glUseProgram(p_quad.handle);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glEnableVertexAttribArray(va_quad_pos);
    glVertexAttribPointer(va_quad_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_quad_uv);
    glVertexAttribPointer(va_quad_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), /*QUAD_BUFFER_DATA + */(void*)(3 * sizeof(float)));
    if(checkGLError())
        return false;

    if(!bind_vec4_to_program(p_quad, "color", color))
        return false;
    if(!bind_texture_to_program(p_quad, "texture", tex, GL_TEXTURE0))
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mat4_mul(camera, transform);
    if(use_dims) {
        mat4 iscale = ident;
        mat4_scale(&iscale, tex->width, tex->height, 0);
        final = mat4_mul(final, iscale);
    }
    if(!bind_mat4_to_program(p_quad, "transform", final))
        return false;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_quad_pos);
    glDisableVertexAttribArray(va_quad_uv);

    return true;
}

bool render_quad_untex_color(mat4 camera, mat4 transform, vec4 color)
{
    glUseProgram(p_quad_untex.handle);

    glEnableVertexAttribArray(va_quad_untex);
    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glVertexAttribPointer(va_quad_untex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    if(checkGLError())
        return false;

    if(!bind_vec4_to_program(p_quad_untex, "color", color))
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mat4_mul(camera, transform);
    if(!bind_mat4_to_program(p_quad_untex, "transform", final))
        return false;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_quad_untex);

    return true;
}

bool render_quad_subtex_color(mat4 camera, mat4 transform, texture* tex, bool use_dims, vec4 uv_rect, vec4 color)
{
    glUseProgram(p_quad_subtex.handle);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glEnableVertexAttribArray(va_quad_subtex_pos);
    glVertexAttribPointer(va_quad_subtex_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_quad_subtex_uv);
    glVertexAttribPointer(va_quad_subtex_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), /*QUAD_BUFFER_DATA + */(void*)(3 * sizeof(float)));
    if(checkGLError())
        return false;

    if(!bind_vec4_to_program(p_quad_subtex, "color", color))
        return false;

    if(!bind_vec4_to_program(p_quad_subtex, "uv_offset", uv_rect))
        return false;
    if(!bind_texture_to_program(p_quad_subtex, "texture", tex, GL_TEXTURE0))
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mat4_mul(camera, transform);
    if(use_dims) {
        mat4 iscale = create_mat4();
        mat4_scale(&iscale, tex->width * uv_rect.data[2], tex->height * uv_rect.data[3], 0);
        final = mat4_mul(final, iscale);
    }
    bind_mat4_to_program(p_quad_subtex, "transform", final);
    if(checkGLError())
        return false;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_quad_subtex_pos);
    glDisableVertexAttribArray(va_quad_subtex_uv);

    return true;
}

GLuint get_quad_buffer() { return quad_buffer; }
