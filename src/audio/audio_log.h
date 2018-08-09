#ifndef DF_AUDIO_LOG
#define DF_AUDIO_LOG
#include <AL/al.h>

void _LogALError(const char* file, unsigned line, const char* function, const char* category, ALuint err);
#define AL_CALL(c, r) { c; ALuint err = alGetError(); if(err != AL_NO_ERROR) { _LogALError(__FILE__, __LINE__, __FUNCTION__, LOG_CATEGORY, err); return r; } }

#endif
