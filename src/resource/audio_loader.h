#ifndef DF_RESOURCE_AUDIO_LOADER
#define DF_RESOURCE_AUDIO_LOADER
#include "audio/audio_source.hd"

// Prepare an audio_source from the provided file
// Setting preload to true will load the data into memory, false will stream
// from the disk
audio_source load_audio_source(const char* path, bool preload);

// Prepare an audio_source from the provided wav file
// Setting preload to true will load the data into memory, false will stream
// from the disk
audio_source load_wav_audio(const char* path, bool preload);

// Prepare an audio_source from the provided ogg file
// Setting preload to true will load the data into memory, false will stream
// from the disk
audio_source load_ogg_audio(const char* path, bool preload);

#endif
