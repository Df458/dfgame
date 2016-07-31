#ifndef DF_PARTICLE_H
#define DF_PARTICLE_H
#include "texture.h"
#include "framebuffer.h"

#define PARTICLE_BUFFER_DIMENSION 1024

typedef struct particleSystem
{
    texture* positions[2];
    texture* velocities[2];
    texture* s_buffer;
    GLuint   vertex_buffer;
    framebuffer* f_buffer[2];

    vec4  acceleration;
    float lifetime;
    texture* color;
    texture* scale;
    texture* tex;
}
particleSystem;

particleSystem* create_particle_system();

void destroy_particle_system_full(particleSystem* sys);
#define destroy_particle_system(sys) { destroy_particle_system_full(sys); sys = 0; }

void particle_system_update(particleSystem* sys, float dt);

#endif
