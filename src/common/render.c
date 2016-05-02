#include "render.h"
#include "render_def.h"
#include "util.h"

///////////////////////////////////////////////////////////////////////////////
// Hidden data
///////////////////////////////////////////////////////////////////////////////

GLuint quad_buffer;

GLuint p_quad;
GLuint u_quad_transform;
GLuint u_quad_texture;
GLuint u_quad_color;
GLuint va_quad_pos;
GLuint va_quad_uv;

GLuint p_quad_untex;
GLuint va_quad_untex;
GLuint u_quad_untex_color;
GLuint u_quad_untex_transform;

GLuint p_quad_subtex;
GLuint u_quad_subtex_transform;
GLuint u_quad_subtex_texture;
GLuint u_quad_subtex_color;
GLuint u_quad_subtex_offsets;
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

// TODO: Improve this with better logging
bool create_program(GLuint* program, const char** vs, const char** fs, ...)
{
    GLuint vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    *program = glCreateProgram();
    glShaderSource(vertex_shader, 1, vs, NULL);
    glShaderSource(fragment_shader, 1, fs, NULL);
    GLsizei len;
    char log[1024];

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);
    glAttachShader(*program, vertex_shader);
    glAttachShader(*program, fragment_shader);
    glLinkProgram(*program);
    if(checkGLError())
        return false;
    glGetShaderInfoLog(vertex_shader, 1024, &len, log);
    if(len)
        info("Vertex Shader Log\n%s\n", log);
    glGetShaderInfoLog(fragment_shader, 1024, &len, log);
    if(len)
        info("Fragment Shader Log\n%s\n", log);
    glGetProgramInfoLog(*program, 1024, &len, log);
    if(len)
        info("Program Log\n%s\n", log);
    if(checkGLError())
        return false;

    glDetachShader(*program, vertex_shader);
    glDetachShader(*program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return true;
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
    create_program(&p_quad_untex, quad_untex_vs, quad_untex_fs
        /*"transform",  &u_quad_untex_transform,*/
        /*"color",      &u_quad_untex_color,*/
        /*"i_pos",      &va_quad_untex*/
    );
    u_quad_untex_transform = glGetUniformLocation(p_quad_untex, "transform");
    u_quad_untex_color = glGetUniformLocation(p_quad_untex, "color");
    va_quad_untex = glGetAttribLocation(p_quad_untex, "i_pos");
    if(checkGLError())
        return false;

    create_program(&p_quad, quad_vs, quad_fs);
    u_quad_transform = glGetUniformLocation(p_quad, "transform");
    u_quad_color = glGetUniformLocation(p_quad, "color");
    u_quad_texture = glGetUniformLocation(p_quad, "texture");
    va_quad_pos = glGetAttribLocation(p_quad, "i_pos");
    va_quad_uv = glGetAttribLocation(p_quad, "i_uv");
    if(checkGLError())
        return false;

    create_program(&p_quad_subtex, quad_subtex_vs, quad_fs);
    u_quad_subtex_transform = glGetUniformLocation(p_quad_subtex, "transform");
    u_quad_subtex_offsets = glGetUniformLocation(p_quad_subtex, "uv_offset");
    u_quad_subtex_color = glGetUniformLocation(p_quad_subtex, "color");
    u_quad_subtex_texture = glGetUniformLocation(p_quad_subtex, "texture");
    va_quad_subtex_pos = glGetAttribLocation(p_quad_subtex, "i_pos");
    va_quad_subtex_uv = glGetAttribLocation(p_quad_subtex, "i_uv");
    if(checkGLError())
        return false;

    return true;
}

bool cleanup_renderer()
{
    glDeleteBuffers(1, &quad_buffer);

    return true;
}

bool render_quad(mat4 camera, mat4 transform, texture* tex, bool use_dims)
{
    glUseProgram(p_quad);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glEnableVertexAttribArray(va_quad_pos);
    glVertexAttribPointer(va_quad_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_quad_uv);
    glVertexAttribPointer(va_quad_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), /*QUAD_BUFFER_DATA + */(void*)(3 * sizeof(float)));
    if(checkGLError())
        return false;

    // TODO: Color
    /*glUniform4f(u_quad_untex_color, col.r, col.g, col.b, col.a);*/
    glUniform4f(u_quad_color, 1.0f, 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    glUniform1i(u_quad_texture, 0);
    if(checkGLError())
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mul(camera, transform);
    if(use_dims) {
        mat4 iscale = ident;
        scale(&iscale, tex->width, tex->height, 0);
        final = mul(final, iscale);
    }
    glUniformMatrix4fv(u_quad_transform, 1, GL_FALSE, final.data);
    if(checkGLError())
        return false;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_quad_pos);
    glDisableVertexAttribArray(va_quad_uv);

    return true;
}

bool render_quad_untex(mat4 camera, mat4 transform)
{
    glUseProgram(p_quad_untex);

    glEnableVertexAttribArray(va_quad_untex);
    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glVertexAttribPointer(va_quad_untex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    if(checkGLError())
        return false;

    // TODO: Color
    /*glUniform4f(u_quad_untex_color, col.r, col.g, col.b, col.a);*/
    glUniform4f(u_quad_untex_color, 1.0f, 1.0f, 1.0f, 1.0f);

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mul(camera, transform);
    glUniformMatrix4fv(u_quad_untex_transform, 1, GL_FALSE, final.data);
    if(checkGLError())
        return false;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return false;

    glDisableVertexAttribArray(va_quad_untex);

    return true;
}

bool render_quad_subtex(mat4 camera, mat4 transform, texture* tex, bool use_dims, float tx, float ty, float tw, float th)
{
    glUseProgram(p_quad_subtex);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
    glEnableVertexAttribArray(va_quad_subtex_pos);
    glVertexAttribPointer(va_quad_subtex_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(va_quad_subtex_uv);
    glVertexAttribPointer(va_quad_subtex_uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), /*QUAD_BUFFER_DATA + */(void*)(3 * sizeof(float)));
    if(checkGLError())
        return false;

    // TODO: Color
    /*glUniform4f(u_quad_untex_color, col.r, col.g, col.b, col.a);*/
    glUniform4f(u_quad_subtex_color, 1.0f, 1.0f, 1.0f, 1.0f);
    if(checkGLError())
        return false;
    glUniform4f(u_quad_subtex_offsets, tx, ty, tw, th);
    if(checkGLError())
        return false;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->handle);
    glUniform1i(u_quad_subtex_texture, 0);
    if(checkGLError())
        return false;

    mat4 tt = ident;
    mat4 rt = ident;
    mat4 st = ident;
    mat4 final = mul(camera, transform);
    if(use_dims) {
        mat4 iscale = ident;
        scale(&iscale, tex->width * tw, tex->height * th, 0);
        final = mul(final, iscale);
    }
    glUniformMatrix4fv(u_quad_subtex_transform, 1, GL_FALSE, final.data);
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
