// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "graphics_log.h"

#include "log/log.h"

// Log function used for KHR_debug OpenGL logging
void graphics_log(GLenum src, GLenum type, GLuint id, GLenum level, GLsizei len, const GLchar* message, const void* user) {
    const char* source = NULL;
    switch(src) {
        case GL_DEBUG_SOURCE_API:
            source = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source = "Window";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source = "Shader";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source = "3rd Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source = "Application";
            break;
        default:
            source = "Other";
            break;
    }
    switch(level) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            info("OpenGL error (%s) %s", source, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            warn("OpenGL error (%s) %s", source, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            error("OpenGL error (%s) %s", source, message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            fatal("OpenGL error (%s) %s", source, message);
            break;
        default:
            warn("OpenGL error (%s) %s", source, message);
    }
}
