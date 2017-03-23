#ifndef DF_GRAPHICS_LOG
#define DF_GRAPHICS_LOG
#include <GL/glew.h>
#include <GL/gl.h>

// Log function used for KHR_debug OpenGL logging
void graphics_log(GLenum src, GLenum type, GLuint id, GLenum level, GLsizei len, const GLchar* message, const void* user);

#endif
