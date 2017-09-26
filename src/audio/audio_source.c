#define LOG_CATEGORY "Audio"

#include "audio_source.h"

#include "audio_log.h"
#include "check.h"
#include "mathutil.h"
#include "memory/alloc.h"
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
}* audio_source;

audio_source audio_source_new_buffer(byte* data, uint32 length, ALenum channels, uint32 sample_rate) {
    audio_source src = salloc(sizeof(struct audio_source));

    src->type = SRC_TYPE_BUFFER;
    src->buffer = data;
    src->length = length;
    src->channels = channels;
    if(src->channels == AL_FORMAT_MONO8 || src->channels == AL_FORMAT_MONO16)
        src->length_mod = 0.5f;
    else
        src->length_mod = 1.0f;
    src->sample_rate = sample_rate;

    return src;
}
audio_source audio_source_new_stream(audio_stream_event* ev, uint32 length, ALenum channels, uint32 sample_rate) {
    audio_source src = salloc(sizeof(struct audio_source));

    src->type = SRC_TYPE_STREAM;
    bind_event(audio_stream_event, src->ev, ev);
    src->length = length;
    src->channels = channels;
    if(src->channels == AL_FORMAT_MONO8 || src->channels == AL_FORMAT_MONO16)
        src->length_mod = 0.5f;
    else
        src->length_mod = 1.0f;
    src->sample_rate = sample_rate;

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
        AL_CALL(alBufferData(buffer, src->channels, data, AUDIO_BUFFER_SIZE * src->length_mod, src->sample_rate), false);

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

    sfree(src);
}
