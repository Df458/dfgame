#ifndef DF_AUDIO_SOURCE
#define DF_AUDIO_SOURCE
#include "audio_source.hd"

#include <AL/al.h>

#define AUDIO_BUFFER_SIZE 32000

event(audio_stream_event, byte** data, uint32 position, uint32 desired_length, uint32* final_length)

audio_source audio_source_new_buffer(byte* data, uint32 length, ALenum channels, uint32 sample_rate);
audio_source audio_source_new_stream(audio_stream_event* ev, uint32 length, ALenum channels, uint32 sample_rate);
byte* audio_source_get_next(audio_source src, uint32 pos, uint32* length);
bool audio_source_get_next_buffer(audio_source src, ALuint buffer, uint32 pos, uint32* length);
uint32 audio_source_get_length(audio_source src);

void _audio_source_free(audio_source src);
#define audio_source_free(p) { _audio_source_free(p); p = NULL; }

#endif
