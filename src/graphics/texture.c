// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "texture.h"

#include "check.h"
#include "memory/alloc.h"
#include "stringutil.h"

gltex gltex_new(GLenum type, uint16 w, uint16 h) {
    gltex tex;
    tex.width = w;
    tex.height = h;
    tex.type = type;
    tex.asset_path = NULL;
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
    tex.asset_path = NULL;
    glGenTextures(1, &tex.handle);
    glBindTexture(type, tex.handle);
    glTexImage2D(type, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    return tex;
}
gltex gltex_new_from_raw(GLenum type, rawtex raw, bool clone_path) {
    check_return(raw.elements == 4, "Auto-converting a raw texture to an OpenGL texture requires 4 channels", (gltex){0});
    check_return(raw.width > 0 && raw.height > 0 && raw.data, "Trying to convert an invalid raw texture into an OpenGL texture", (gltex){0});
    gltex tex = gltex_new(type, raw.width, raw.height);

    for(int i = 0; i < raw.height; ++i)
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, raw.width, 1, GL_RGBA, GL_UNSIGNED_BYTE, raw.data + ((raw.height - i - 1) * raw.width * 4));

    // If we have an asset path and we want to clone it, make a copy
    if(clone_path)
        tex.asset_path = nstrdup(raw.asset_path);

    return tex;
}

// Creates a new rawtex, and initializes the data block based on the arguments
rawtex rawtex_new(uint16 w, uint16 h, uint8 elements) {
    return rawtex_new_data(w, h, elements, scalloc(w * h * elements, sizeof(uint8)));
}

// Creates a new rawtex, and initializes the data block based on an existing opengl texture
rawtex rawtex_new_from_gl(const gltex tex, bool clone_path) {
    check_return(tex.width > 0 && tex.height > 0, "Trying to read raw data from an invalid OpenGL texture", (rawtex){0});

    rawtex raw = rawtex_new(tex.width, tex.height, 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw.data);

    // If we have an asset path and we want to clone it, make a copy
    if(clone_path)
        raw.asset_path = nstrdup(tex.asset_path);

    return raw;
}

// Creates a new rawtex, and initializes the data block with the provided data
rawtex rawtex_new_data(uint16 w, uint16 h, uint8 elements, ubyte* data) {
    return (rawtex) {
        .width = w,
        .height = h,
        .elements = elements,
        .data = data,
        .asset_path = NULL
    };
}

// Converts a single-channel rawtex to a white RGBA rawtex, where the original
// data becomes the alpha.
// If clone_path is true, the asset_path on the new rawtex is a copy of the old
// one. Otherwise, both point to the same data.
rawtex rawtex_mask_to_rgba(rawtex tex, bool clone_path) {
    check_return(tex.elements == 1, "Trying to convert multi-channel texture as if it had 1", tex);
    check_return(tex.width > 0 && tex.height > 0 && tex.data, "Trying to convert an invalid texture", tex);

    rawtex tex2 = tex;

    // If we have an asset path and we want to clone it, make a copy
    if(clone_path && tex.asset_path)
        tex2.asset_path = nstrdup(tex.asset_path);

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

// Deletes the OpenGL texture handle and path of the given gltex
void gltex_cleanup(gltex* tex) {
    glDeleteTextures(1, &tex->handle);
    if(tex->asset_path)
        sfree(tex->asset_path);
}
// Deletes the buffer and path of the given rawtex
void rawtex_cleanup(rawtex* tex) {
    sfree(tex->data);
    if(tex->asset_path)
        sfree(tex->asset_path);
}
