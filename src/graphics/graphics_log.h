#ifndef DF_GRAPHICS_LOG
#define DF_GRAPHICS_LOG
#include <GL/glew.h>
#include <GL/gl.h>

// Log function used for KHR_debug OpenGL logging
void graphics_log(GLenum src, GLenum type, GLuint id, GLenum level, GLsizei len, const GLchar* message, const void* user);

void _LogFTError(const char* file, unsigned line, const char* category, int err);
#define FT_CALL(c, r) { int err = c; if(err != 0) { _LogFTError(__FILE__, __LINE__, LOG_CATEGORY, err); return r; } }
#define FT_CALL_NORETURN(c) { int err = c; if(err != 0) _LogFTError(__FILE__, __LINE__, LOG_CATEGORY, err); }

#endif
