#define LOG_CATEGORY "Audio"

#include "audio_source.h"

#include "audio_log.h"
#include "check.h"
#include "mathutil.h"
#include "memory/alloc.h"
#include "stringutil.h"
#include "types.h"

typedef enum audio_source_type {
    SRC_TYPE_BUFFER,
    SRC_TYPE_STREAM
} audio_source_type;

typedef struct audio_source {
    uint32 length;
    ALenum channels;
    uint32 sample_rate;

    float length_mod; // Single-channel audio needs to be passed half the length

    audio_source_type type;

    union {
        byte* buffer;
        audio_stream_event* ev;
    };

    char* asset_path;
}* audio_source;

// Common initialization for audio_source structs
audio_source _audio_source_new_common(uint32 length, ALenum channels, uint32 sample_rate, const char* path) {
    audio_source src = salloc(sizeof(struct audio_source));

    src->length = length;
    src->channels = channels;
    if(src->channels == AL_FORMAT_MONO8 || src->channels == AL_FORMAT_MONO16)
        src->length_mod = 0.5f;
    else
        src->length_mod = 1.0f;
    src->sample_rate = sample_rate;
    src->asset_path = nstrdup(path);

    return src;
}

// Creates a new audio_source that streams from a buffer in memory
audio_source audio_source_new_buffer(byte* data, uint32 length, ALenum channels, uint32 sample_rate, const char* path) {
    audio_source src = _audio_source_new_common(length, channels, sample_rate, path);

    src->type = SRC_TYPE_BUFFER;
    src->buffer = data;

    return src;
}

// Creates a new audio_source that streams via a callback event, most likely
// a wrapper for an existing streaming API
audio_source audio_source_new_stream(audio_stream_event* ev, uint32 length, ALenum channels, uint32 sample_rate, const char* path) {
    audio_source src = _audio_source_new_common(length, channels, sample_rate, path);

    src->type = SRC_TYPE_STREAM;
    bind_event(src->ev, ev);

    return src;
}

byte* audio_source_get_next(audio_source src, uint32 pos, uint32* length) {
    check_return(pos < src->length, "Failed to stream audio data: Position %u must be less than %u", NULL, pos, src->length);

    byte* data = NULL;
    switch(src->type) {
        case SRC_TYPE_STREAM:
            call_event(src->ev, &data, pos, AUDIO_BUFFER_SIZE, length);
            break;
        case SRC_TYPE_BUFFER: {
            byte* data_start = src->buffer + pos;
            uint32 copy_size = min(AUDIO_BUFFER_SIZE, src->length - pos);
            data = scalloc(copy_size, sizeof(byte));
            memcpy(data, data_start, copy_size);
            if(length)
                *length = copy_size;
            } break;
    }

    return data;
}

bool audio_source_get_next_buffer(audio_source src, ALuint buffer, uint32 pos, uint32* length) {
    check_return(src, "Failed to get audio buffer: src is NULL", NULL);
    uint32 size = 0;
    byte* data = audio_source_get_next(src, pos, &size);

    if(size) {
        AL_CALL(alBufferData(buffer, src->channels, data, size, src->sample_rate), false);

        if(length)
            *length = size;
    }

    sfree(data);

    return true;
}

uint32 audio_source_get_length(audio_source src) {
    return src->length;
}

void _audio_source_free(audio_source src) {
    if(src->type == SRC_TYPE_BUFFER) {
        sfree(src->buffer);
    } else {
        call_event(src->ev, NULL, 0, 0, NULL);
    }

    if(src->asset_path)
        sfree(src->asset_path);

    sfree(src);
}
