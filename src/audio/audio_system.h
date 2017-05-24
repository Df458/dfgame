#ifndef DF_AUDIO_SYSTEM
#define DF_AUDIO_SYSTEM
#include "vector.h"

void audio_init();
void audio_update_listener(vec3 position, vec3 orientation, vec3 velocity);
void audio_cleanup();

#endif
