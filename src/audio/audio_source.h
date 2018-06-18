#ifndef DF_AUDIO_SOURCE
#define DF_AUDIO_SOURCE
#include "audio/audio_source.hd"

#include <AL/al.h>

#define AUDIO_BUFFER_SIZE 32000

event(audio_stream_event, byte** data, uint32 position, uint32 desired_length, uint32* final_length)

// Creates a new audio_source that streams from a buffer in memory
audio_source audio_source_new_buffer(byte* data, uint32 length, ALenum channels, uint32 sample_rate, const char* path);
// Creates a new audio_source that streams via a callback event, most likely
// a wrapper for an existing streaming API
audio_source audio_source_new_stream(audio_stream_event* ev, uint32 length, ALenum channels, uint32 sample_rate, const char* path);

// Load/stream the next N bytes of audio starting at pos, where N is the desired length
byte* audio_source_get_next(audio_source src, uint32 pos, uint32* length);
// Load/stream the next N bytes of audio starting at pos, where N is the desired length, and load into an OpenAL buffer
bool audio_source_get_next_buffer(audio_source src, ALuint buffer, uint32 pos, uint32* length);
// Gets the total length of the source's data
uint32 audio_source_get_length(audio_source src);
// Gets the filepath to the audio
char* audio_source_get_path(audio_source src);

// Frees the source and sets it to NULL to make it harder to double-free.
#define audio_source_free(p) { _audio_source_free(p); p = NULL; }

// Frees the source. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _audio_source_free(audio_source src);

#endif
