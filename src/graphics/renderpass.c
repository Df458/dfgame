// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "renderpass.h"

#include "framebuffer.h"
#include "matrix.h"
#include "memory/alloc.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

static renderpass prev_pass = NULL;

typedef struct renderpass {
    framebuffer fbo;
    gltex texture;
}* renderpass;

renderpass renderpass_new(uint16 w, uint16 h) {
    renderpass pass = salloc(sizeof(struct renderpass));
    pass->fbo = framebuffer_new();
    pass->texture = gltex_new(GL_TEXTURE_2D, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, pass->fbo.handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pass->texture.handle, 0);
    GLenum targets[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, targets);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return pass;
}

void renderpass_start(renderpass pass) {
    glBindFramebuffer(GL_FRAMEBUFFER, pass->fbo.handle);
    glViewport(0, 0, pass->texture.width, pass->texture.height);

    prev_pass = pass;
}

void renderpass_next(renderpass pass, shader s) {
    renderpass prev = prev_pass;
    if(pass)
        renderpass_start(pass);
    else {
        prev_pass = NULL;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    if(prev)
        renderpass_present(prev, s);
}

void renderpass_present(renderpass pass, shader s) {
    glUseProgram(s.id);
    shader_bind_uniform_name(s, "transform", mat4_scale(mat4_ident, 2));
    shader_bind_attribute_mesh(s, mesh_quad(), "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    shader_bind_uniform_texture_name(s, "u_texture", pass->texture, GL_TEXTURE0);
    mesh_render(mesh_quad(), GL_TRIANGLES);
}
