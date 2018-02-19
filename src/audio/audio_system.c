#define LOG_CATEGORY "Audio"

#include "audio_system.h"

#include "audio_log.h"
#include "check.h"
#include <AL/al.h>
#include <AL/alc.h>

static ALCdevice* al_device = NULL;
static ALCcontext* al_context = NULL;

void audio_init() {
    al_device = alcOpenDevice(NULL);
    check_return(al_device, "Failed to open device", );

    al_context = alcCreateContext(al_device, NULL);

    check_return(al_context, "Failed to create OpenAL context", );

    AL_CALL(alcMakeContextCurrent(al_context), );
    AL_CALL(alListener3f(AL_POSITION, 0, 0, 0), );
    AL_CALL(alListener3f(AL_VELOCITY, 0, 0, 0), );
}

void audio_update_listener(vec3 position, vec3 forward, vec3 up, vec3 velocity) {
    alListenerfv(AL_POSITION, position.data);
    vec3 orient[2] = { forward, up };
    alListenerfv(AL_ORIENTATION, (float*)orient);
    alListenerfv(AL_VELOCITY, velocity.data);
}

void audio_set_listener_gain(float gain) {
    alListenerf(AL_GAIN, gain);
}

void audio_cleanup() {
    if(al_context)
        alcDestroyContext(al_context);
    if(al_device)
        alcCloseDevice(al_device);
}
