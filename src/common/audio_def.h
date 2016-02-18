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

struct wav_data
{
    FILE*    infile;
    uint32_t data_size;
    uint32_t sample_rate;
    uint32_t position;
};

typedef union audio_content
{
    struct vorbis_data ogg;
    struct wav_data    wav;
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
    bool simple;
};
