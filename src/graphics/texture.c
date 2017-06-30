// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "texture.h"

#include "memory/alloc.h"

gltex gltex_new(GLenum type, uint16 w, uint16 h) {
    gltex tex;
    tex.width = w;
    tex.height = h;
    tex.type = type;
    glGenTextures(1, &tex.handle);
    glBindTexture(type, tex.handle);
	glTexImage2D(type, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameterf(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, color);

    return tex;
}
rawtex rawtex_new(uint16 w, uint16 h, uint8 elements) {
    return (rawtex) {
        .width = w,
        .height = h,
        .elements = elements,
        .data = scalloc(w * h * elements, sizeof(float))
    };
}
