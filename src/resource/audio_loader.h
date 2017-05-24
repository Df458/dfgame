#ifndef DF_RESOURCE_AUDIO_LOADER
#define DF_RESOURCE_AUDIO_LOADER
#include "audio_source.hd"

audio_source load_audio_source(const char* path, bool preload);
audio_source load_wav_audio(const char* path, bool preload);
audio_source load_ogg_audio(const char* path, bool preload);

#endif
