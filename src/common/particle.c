#include "particle.h"
#include "util.h"

float test_data[10] =
{
    0,0,
    1,0,
    0,1,
    1,1,
    0.5,0.5,
};

particleSystem* create_particle_system()
{
    particleSystem* sys = salloc(sizeof(particleSystem));
    sys->positions[0]  = create_texture_storage(3, 3, GL_RGB16F, GL_RGB, GL_FLOAT); // TODO: Switch this back
    sys->positions[1]  = create_texture_storage(1024, 1024, GL_RGB16F, GL_RGB, GL_FLOAT);
    sys->velocities[0] = create_texture_storage(1024, 1024, GL_RGB16F, GL_RGB, GL_FLOAT);
    sys->velocities[1] = create_texture_storage(1024, 1024, GL_RGB16F, GL_RGB, GL_FLOAT);
    sys->s_buffer      = create_texture_storage(1024, 1024, GL_RGB16F, GL_RGB, GL_FLOAT);
    glGenBuffers(1, &sys->v_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sys->v_buffer);
    glBufferData(GL_ARRAY_BUFFER, 10 * sizeof(float), test_data, GL_DYNAMIC_DRAW);
    sys->f_buffer = create_framebuffer(3, 3);

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

void particle_system_update(float dt)
{
}
