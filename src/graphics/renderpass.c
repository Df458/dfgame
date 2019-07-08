// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "renderpass.h"

#include "check.h"
#include "framebuffer.h"
#include "matrix.h"
#include "memory/alloc.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

static renderpass prev_pass = NULL;
typedef struct renderpass {
    framebuffer fbo;
    framebuffer primary_fbo;
    gltex texture;
    gltex depth;
}* renderpass;

renderpass renderpass_new(uint16 w, uint16 h, framebuffer primary) {
    renderpass pass = salloc(sizeof(struct renderpass));
    pass->fbo = framebuffer_new(w, h);
    pass->texture = gltex_new(GL_TEXTURE_2D, w, h);
    pass->depth = gltex_new_depth(GL_TEXTURE_2D, w, h);
    pass->primary_fbo = primary;
    glBindFramebuffer(GL_FRAMEBUFFER, pass->fbo.handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pass->texture.handle, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, pass->depth.handle, 0);
    GLenum targets[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, targets);

    glBindFramebuffer(GL_FRAMEBUFFER, primary.handle);

    return pass;
}
void _renderpass_free(renderpass pass) {
    check_return(pass, "Can't free renderpass: pass is NULL", );
    gltex_cleanup(&pass->texture);
    gltex_cleanup(&pass->depth);
    framebuffer_cleanup(&pass->fbo);

    sfree(pass);
}

void renderpass_start(renderpass pass) {
    if(pass == NULL) {
        if(prev_pass) {
            glBindFramebuffer(GL_FRAMEBUFFER, prev_pass->primary_fbo.handle);
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        prev_pass = NULL;
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, pass->fbo.handle);
        glViewport(0, 0, pass->texture.width, pass->texture.height);
    }

    prev_pass = pass;
}

void renderpass_next(renderpass pass, shader s) {
    renderpass prev = prev_pass;
    renderpass_start(pass);

    if(prev)
        renderpass_present(prev, s);
}

void renderpass_present(renderpass pass, shader s) {
    glUseProgram(s.id);
    shader_bind_uniform_name(s, "transform", mat4_scale(mat4_ident, 2));
    shader_bind_uniform_texture_name(s, "u_texture", pass->texture, GL_TEXTURE0);
    mesh_render(s, mesh_quad(), GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
}
