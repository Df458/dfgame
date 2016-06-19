#include "render.h"
#include "render_def.h"
#include "transform.h"
#include "util.h"
#include "program.h"
#include "vertex_def.h"

///////////////////////////////////////////////////////////////////////////////
// Hidden data
///////////////////////////////////////////////////////////////////////////////

GLuint quad_buffer;

program p_quad;
GLuint va_quad_pos;
GLuint va_quad_uv;

program p_text;
GLuint va_text_pos;
GLuint va_text_uv;

program p_quad_untex;
GLuint va_quad_untex;

program p_quad_subtex;
GLuint va_quad_subtex_pos;
GLuint va_quad_subtex_uv;

program p_particle;
GLuint va_particle_buffer;

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

    /* p_text = create_program_gvf(text_gs, quad_subtex_vs, text_fs); */
    p_text = create_program(quad_vs, text_fs);
    va_text_pos = glGetAttribLocation(p_text.handle, "i_pos");
    va_text_uv = glGetAttribLocation(p_text.handle, "i_uv");
    if(checkGLError())
        return false;

    p_quad_subtex = create_program(quad_subtex_vs, quad_fs);
    va_quad_subtex_pos = glGetAttribLocation(p_quad_subtex.handle, "i_pos");
    va_quad_subtex_uv = glGetAttribLocation(p_quad_subtex.handle, "i_uv");
    if(checkGLError())
        return false;

    p_particle = create_program_gvf(particle_gs, particle_vs, quad_untex_fs);
    va_particle_buffer = glGetAttribLocation(p_particle.handle, "texCoord");
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

bool render_mesh(mat4 camera, mat4 transform, mesh* m, program* p)
{
    glUseProgram(p->handle);

    if(!bind_mat4_to_program(*p, "camera", camera))
        return false;
    if(!bind_mat4_to_program(*p, "transform", transform))
        return false;

    glBindBuffer(GL_ARRAY_BUFFER, m->handle);
    if(m->type_flags & VT_NORMAL) {
        if(m->type_flags & VT_TEXTURE) {
            GLuint va_position = glGetAttribLocation(p->handle, "va_position");
            GLuint va_normal   = glGetAttribLocation(p->handle, "va_normal");
            GLuint va_texture  = glGetAttribLocation(p->handle, "va_texture");
            glEnableVertexAttribArray(va_position);
            glVertexAttribPointer(va_position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(va_normal);
            glVertexAttribPointer(va_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(va_texture);
            glVertexAttribPointer(va_texture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            if(checkGLError())
                return false;
        } else {
            GLuint va_position = glGetAttribLocation(p->handle, "va_position");
            GLuint va_normal   = glGetAttribLocation(p->handle, "va_normal");
            glEnableVertexAttribArray(va_position);
            glEnableVertexAttribArray(va_normal);
            glVertexAttribPointer(va_position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glVertexAttribPointer(va_normal,   3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            if(checkGLError())
                return false;
        }
    } else if(m->type_flags & VT_TEXTURE) {
        GLuint va_position = glGetAttribLocation(p->handle, "va_position");
        GLuint va_texture  = glGetAttribLocation(p->handle, "va_texture");
        glEnableVertexAttribArray(va_position);
        glVertexAttribPointer(va_position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(va_texture);
        glVertexAttribPointer(va_texture, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        if(checkGLError())
            return false;
    } else {
        GLuint va_position = glGetAttribLocation(p->handle, "va_position");
        glEnableVertexAttribArray(va_position);
        glVertexAttribPointer(va_position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        if(checkGLError())
            return false;
    }

    glDrawArrays(p->method, 0, m->vertex_count);
    return !checkGLError();
}

bool render_quad_color(mat4 camera, mat4 transform, texture* tex, bool use_dims, vec4 color)
{
    glUseProgram(p_quad.handle);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glEnableVertexAttribArray(va_quad_pos);
    glVertexAttribPointer(va_quad_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_quad_uv);
    glVertexAttribPointer(va_quad_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
    glVertexAttribPointer(va_quad_subtex_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

bool render_text_color(mat4 camera, mat4 transform, font* ft, int glyph_id, bool use_dims, vec4 color)
{
    glUseProgram(p_quad_subtex.handle);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glEnableVertexAttribArray(va_text_pos);
    glVertexAttribPointer(va_text_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_text_uv);
    glVertexAttribPointer(va_text_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    if(checkGLError())
        return false;

    if(!bind_vec4_to_program(p_quad_subtex, "color", color))
        return false;

    glyph* gp = font_get_glyph(ft, glyph_id, false);
    if(!gp)
        return false;
    if(!bind_vec4_to_program(p_quad_subtex, "uv_offset", create_vec4_data(gp->texture_position.data[0] / (float)GLYPH_ATLAS_SIZE, gp->texture_position.data[1] / (float)GLYPH_ATLAS_SIZE, gp->texture_size.data[0] / (float)GLYPH_ATLAS_SIZE, gp->texture_size.data[1] / (float)GLYPH_ATLAS_SIZE)))
        return false;
    if(!bind_texture_to_program(p_quad_subtex, "texture", ft->atlas, GL_TEXTURE0))
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mat4_mul(camera, transform);
    if(use_dims) {
        mat4 iscale = create_mat4();
        mat4_scale(&iscale, gp->texture_size.data[0], gp->texture_size.data[1], 0);
        final = mat4_mul(final, iscale);
    }
    bind_mat4_to_program(p_quad_subtex, "transform", final);
    if(checkGLError())
        return false;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_text_pos);
    glDisableVertexAttribArray(va_text_uv);

    return true;
}

bool render_text_string_color(mat4 camera, mat4 transform, text* txt, vec4 color)
{
    glUseProgram(p_text.handle);
    glBindBuffer(GL_ARRAY_BUFFER, txt->handle);
    glEnableVertexAttribArray(va_text_pos);
    glVertexAttribPointer(va_text_pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_text_uv);
    glVertexAttribPointer(va_text_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    if(checkGLError())
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mat4_mul(camera, transform);
    bind_mat4_to_program(p_text, "transform", final);
    if(checkGLError())
        return false;

    if(!bind_vec4_to_program(p_text, "color", color))
        return false;

    if(!bind_texture_to_program(p_text, "texture", txt->font->atlas, GL_TEXTURE0))
        return false;

    glDrawArrays(GL_TRIANGLES, 0, txt->buflen * 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_text_pos);
    glDisableVertexAttribArray(va_text_uv);

    return true;
}

bool render_particles(mat4 camera, mat4 transform, particleSystem* system)
{
    glUseProgram(p_particle.handle);
    if(checkGLError())
        return false;
    glBindBuffer(GL_ARRAY_BUFFER, system->v_buffer);
    glEnableVertexAttribArray(va_particle_buffer);
    glVertexAttribPointer(va_particle_buffer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    if(checkGLError())
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mat4_mul(camera, transform);
    bind_mat4_to_program(p_particle, "transform", final);
    if(checkGLError())
        return false;

    if(!bind_vec4_to_program(p_particle, "color", create_vec4_data(1, 0, 1, 1)))
        return false;

    if(!bind_texture_to_program(p_particle, "data", system->positions[0], GL_TEXTURE0))
        return false;

    glDrawArrays(GL_POINTS, 0, 1);

    return true;
}

GLuint get_quad_buffer() { return quad_buffer; }
