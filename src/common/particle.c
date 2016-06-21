#include "particle.h"
#include "util.h"
#include "matrix.h"
#include "program.h"
#include "render.h"
#include "render_def.h"

static const char* particle_compute_fs[] =
{
"#version 330 core\n"
"layout (location = 0) out vec4 position;\n"
"layout (location = 1) out vec4 velocity;\n"
"uniform sampler2D position_in;\n"
"uniform sampler2D velocity_in;\n"
"uniform float dt;"
"in vec2 v_uv;\n"
"void main() {\n"
"vec4 pi = texture2D(position_in, v_uv);\n"
"vec4 vi = texture2D(velocity_in, v_uv);\n"
"position = pi + (vi * dt);\n"
"velocity = vi + vec4(0, 10, 0, 0) * dt;\n"
"}\n"
};

GLuint att[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

program p_particle_compute;

particleSystem* create_particle_system()
{
    p_particle_compute = create_program(quad_vs, particle_compute_fs);

    particleSystem* sys = salloc(sizeof(particleSystem));
    sys->positions[0]  = create_texture_storage(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    sys->positions[1]  = create_texture_storage(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    sys->velocities[0] = create_texture_storage(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    sys->velocities[1] = create_texture_storage(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    sys->s_buffer      = create_texture_storage(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    sys->next = 0;

    float* v_data = scalloc(PARTICLE_BUFFER_DIMENSION * PARTICLE_BUFFER_DIMENSION * 2, sizeof(float));
    for(int i = 0; i < PARTICLE_BUFFER_DIMENSION; ++i) {
        for(int j = 0; j < PARTICLE_BUFFER_DIMENSION; ++j) {
            v_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 2 + 0] = (float)j / (float)PARTICLE_BUFFER_DIMENSION;
            v_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 2 + 1] = (float)i / (float)PARTICLE_BUFFER_DIMENSION;
        }
    }

    glGenBuffers(1, &sys->v_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sys->v_buffer);
    glBufferData(GL_ARRAY_BUFFER, PARTICLE_BUFFER_DIMENSION * PARTICLE_BUFFER_DIMENSION * 2 * sizeof(float), v_data, GL_DYNAMIC_DRAW);
    
    sfree(v_data);

    sys->f_buffer[0] = create_framebuffer(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION);
    sys->f_buffer[1] = create_framebuffer(PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION);
    glBindFramebuffer(GL_FRAMEBUFFER, sys->f_buffer[0]->handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sys->positions[0]->handle, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, sys->velocities[0]->handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, sys->f_buffer[1]->handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sys->positions[1]->handle, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, sys->velocities[1]->handle, 0);

    return sys;
}

void destroy_particle_system_full(particleSystem* sys)
{
    if(!sys)
        return;

    destroy_texture(sys->positions[0]);
    destroy_texture(sys->positions[1]);
    destroy_texture(sys->velocities[0]);
    destroy_texture(sys->velocities[1]);
    destroy_texture(sys->s_buffer);
    sfree(sys);
}

void particle_system_update(particleSystem* sys, float dt)
{
    glBindFramebuffer(GL_FRAMEBUFFER, sys->f_buffer[sys->next]->handle);
    if(checkGLError())
        return;
    glDrawBuffers(2, att);
    if(checkGLError())
        return;
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glViewport(0, 0, PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION);
    glDisable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // TODO: Render the quad for processing
    mat4 t = create_mat4();
    mat4_scale(&t, 2, -2, false);

    glUseProgram(p_particle_compute.handle);
    if(checkGLError())
        return;

    glBindBuffer(GL_ARRAY_BUFFER, get_quad_buffer());
    glEnableVertexAttribArray(glGetAttribLocation(p_particle_compute.handle, "i_pos"));
    glVertexAttribPointer(glGetAttribLocation(p_particle_compute.handle, "i_pos"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(glGetAttribLocation(p_particle_compute.handle, "i_uv"));
    glVertexAttribPointer(glGetAttribLocation(p_particle_compute.handle, "i_uv"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    if(checkGLError())
        return;

    if(!bind_texture_to_program(p_particle_compute, "position_in", sys->positions[abs(sys->next - 1)], GL_TEXTURE0))
        return;
    if(!bind_texture_to_program(p_particle_compute, "velocity_in", sys->velocities[abs(sys->next - 1)], GL_TEXTURE1))
        return;

    if(!bind_mat4_to_program(p_particle_compute, "transform", t))
        return;

    if(!bind_float_to_program(p_particle_compute, "dt", dt))
        return;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return;

    /* glDisableVertexAttribArray(va_quad_pos); */
    /* glDisableVertexAttribArray(va_quad_uv); */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_BLEND);

    sys->next = abs(sys->next - 1);
    glViewport(vp[0], vp[1], vp[2], vp[3]);
}
