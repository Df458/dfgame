// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#undef FTERRORS_H_
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, NULL } };

const struct
{
    int          err_code;
    const char*  err_msg;
} ft_errors[] =
#include FT_ERRORS_H

#include "graphics_log.h"

#include "log/log.h"

// Log function used for KHR_debug OpenGL logging
void graphics_log(GLenum src, GLenum type, GLuint id, GLenum level, GLsizei len, const GLchar* message, const void* user) {
    const char* source = NULL;
    switch(src) {
        case GL_DEBUG_SOURCE_API:
            source = "GL API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source = "GL Window";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source = "GL Shader";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source = "GL (3rd Party)";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source = "GL Application";
            break;
        default:
            source = "OpenGL";
            break;
    }
    switch(level) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            info("%s: %s", source, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            warn("%s: %s", source, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            error("%s: %s", source, message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            fatal("%s: %s", source, message);
            break;
        default:
            warn("%s: %s", source, message);
    }
}

void _LogFTError(const char* file, unsigned line, const char* category, int err) {
    _log(file, line, category, LOG_ERROR, "FreeType error: %s", ft_errors[err].err_msg);
}
