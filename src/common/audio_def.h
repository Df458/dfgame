#include <AL/al.h>
#include <stdio.h>
#include <vorbis/vorbisfile.h>

enum audio_format
{
    AFMT_INVALID,
    AFMT_WAV,
    AFMT_OGG,
};

struct vorbis_data
{
    OggVorbis_File file;
    vorbis_info*   info;
};

typedef union audio_content
{
    struct vorbis_data ogg;
    FILE*              wav;
} audio_content;

struct audio
{
    uint8_t fmt;
    ALenum  channels;
    union audio_content content;
};

struct player_data
{
    ALuint source;
    ALuint buffers[2];

    bool playing;
    bool stopped;
};
