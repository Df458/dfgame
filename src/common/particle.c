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
"uniform vec4 acceleration;\n"
"uniform float lifetime;"
"in vec2 v_uv;\n"
"void main() {\n"
"vec4 vi = texture2D(velocity_in, v_uv);\n"
"vec4 pi = texture2D(position_in, v_uv);\n"
"velocity = vi + vec4(acceleration.xyz * 0.1, 0);\n"
"float death_checker = ceil(1 - min(pi.a / lifetime, 1));\n"
"position = (pi + vec4(velocity.xyz * 0.1, dt)) / death_checker;\n"
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
    sys->tex           = 0;

    sys->acceleration = create_vec4_data(0, 0, 0, 0);
    sys->color = create_texture(1024, 1024);
    sys->scale = create_texture_storage(1024, 1024, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    fill_texture(sys->color, create_vec4_data(1, 1, 1, 1));
    sys->lifetime = 1;

    float* v_data = scalloc(PARTICLE_BUFFER_DIMENSION * PARTICLE_BUFFER_DIMENSION * 2, sizeof(float));
    float* s_data = scalloc(PARTICLE_BUFFER_DIMENSION * PARTICLE_BUFFER_DIMENSION * 4, sizeof(float));
    float* sc_data = scalloc(PARTICLE_BUFFER_DIMENSION * PARTICLE_BUFFER_DIMENSION * 4, sizeof(float));
    for(int i = 0; i < PARTICLE_BUFFER_DIMENSION; ++i) {
        for(int j = 0; j < PARTICLE_BUFFER_DIMENSION; ++j) {
            v_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 2 + 0] = (float)j / (float)PARTICLE_BUFFER_DIMENSION;
            v_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 2 + 1] = (float)i / (float)PARTICLE_BUFFER_DIMENSION;

            s_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 4 + 0] = (float)(rand() % 1000) * 0.001;
            s_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 4 + 1] = (float)(rand() % 1000) * 0.001;
            sc_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 4 + 0] = 1;
            sc_data[(i * PARTICLE_BUFFER_DIMENSION + j) * 4 + 1] = 10;
        }
    }

    glGenBuffers(1, &sys->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sys->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, PARTICLE_BUFFER_DIMENSION * PARTICLE_BUFFER_DIMENSION * 2 * sizeof(float), v_data, GL_DYNAMIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, sys->s_buffer->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, 0, GL_RGBA, GL_FLOAT, s_data);
    glBindTexture(GL_TEXTURE_2D, sys->scale->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, sc_data);

    sfree(v_data);
    sfree(s_data);
    sfree(sc_data);

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
    glBindFramebuffer(GL_FRAMEBUFFER, sys->f_buffer[0]->handle);
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

    if(!bind_texture_to_program(p_particle_compute, "position_in", sys->positions[1], GL_TEXTURE0))
        return;
    if(!bind_texture_to_program(p_particle_compute, "velocity_in", sys->velocities[1], GL_TEXTURE1))
        return;

    if(!bind_mat4_to_program(p_particle_compute, "transform", t))
        return;

    if(!bind_vec4_to_program(p_particle_compute, "acceleration", sys->acceleration))
        return;

    if(!bind_float_to_program(p_particle_compute, "dt", dt))
        return;

    if(!bind_float_to_program(p_particle_compute, "lifetime", sys->lifetime))
        return;

    glDrawArrays(GL_TRIANGLES, 0, 6);
    if(checkGLError())
        return;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, sys->f_buffer[0]->handle);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sys->f_buffer[1]->handle);
    glBlitFramebuffer(0, 0, PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, 0, 0, PARTICLE_BUFFER_DIMENSION, PARTICLE_BUFFER_DIMENSION, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_BLEND);

    glViewport(vp[0], vp[1], vp[2], vp[3]);
}
