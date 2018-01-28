// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "texture.h"

#include "check.h"
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
gltex gltex_new_depth(GLenum type, uint16 w, uint16 h) {
    gltex tex;
    tex.width = w;
    tex.height = h;
    tex.type = type;
    glGenTextures(1, &tex.handle);
    glBindTexture(type, tex.handle);
    glTexImage2D(type, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

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

rawtex rawtex_mask_to_rgba(rawtex tex) {
    check_return(tex.elements == 1, "Trying to convert multi-channel texture as if it had 1", tex);
    check_return(tex.width > 0 && tex.height > 0 && tex.data, "Trying to convert an invalid texture", tex);

    rawtex tex2 = tex;

    tex2.elements = 4;
    tex2.data = scalloc(tex.width * tex.height * tex.elements, sizeof(float));
    for(int i = 0; i < tex.height; ++i) {
        for(int j = 0; j < tex.width; ++j) {
            tex2.data[(i * tex.width + j) * 4]     = 255;
            tex2.data[(i * tex.width + j) * 4 + 1] = 255;
            tex2.data[(i * tex.width + j) * 4 + 2] = 255;
            tex2.data[(i * tex.width + j) * 4 + 3] = tex.data[i * tex.width + j];
        }
    }

    return tex2;
}
