#include "audio.h"
#include "audio_def.h"
#include "io_util.h"
#include "util.h"

#include <AL/alc.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Hidden data
///////////////////////////////////////////////////////////////////////////////

ALCdevice* device;
ALCcontext* context;

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

bool prepare_ogg(audio* a, const char* path)
{
    int result;
    
    if((result = ov_fopen(path, &a->content.ogg.file)) < 0)
    {
        const char* message = "Unknown Error";
        switch(result) {
            case OV_EREAD: message = "Media read error"; break;
            case OV_ENOTVORBIS: message = "Media is not vorbis"; break;
            case OV_EVERSION: message = "Version mismatch"; break;
            case OV_EBADHEADER: message = "Invalid header"; break;
            case OV_EFAULT: message = "Logic Fault"; break;
            default: message = "Unknown Error"; break;
        }
        error("Error loading Ogg stream %s: %s", path, message);
        return false;
    }
    a->content.ogg.info = ov_info(&a->content.ogg.file, -1);
 
    if(a->content.ogg.info->channels == 1)
        a->channels = AL_FORMAT_MONO16;
    else
        a->channels = AL_FORMAT_STEREO16;

    return true;
}

// FIXME: WAV loader is currently broken
bool prepare_wav(audio* a, const char* path)
{
    a->content.wav.infile = fopen(path, "rb");
    FILE* infile = a->content.wav.infile;
    if(!infile) {
        error("Failed to prepare WAV stream, file %s not found", path);
        return false;
    }
    
    char id[5];
    id[4] = 0;
    uint8_t* input_buffer;
    uint32_t file_size, format_size, uint8_ts_per_sec;
    short tag, channel_count, block_alignment, bits_per_sample;
    a->channels = AL_FORMAT_MONO8;
    
    fread(&id, sizeof(uint8_t), 4, infile);
    if(strcmp(id, "RIFF")) {
        error("Cannot prepare WAV stream: Not a RIFF file. (Got %s)", id);
        fclose(infile);
        return false;
    }
    fread(&file_size, sizeof(uint32_t), 1, infile);
    fread(&id, sizeof(uint8_t), 4, infile);
    if(strcmp(id, "WAVE")) {
        error("Cannot prepare WAV stream: Not a WAVE file. (Got %s)", id);
        fclose(infile);
        return false;
    }
    
    fread(&id, sizeof(uint8_t), 4, infile);
    if(strcmp(id, "fmt ")) {
        error("Cannot prepare WAV stream: Unrecognized format. (fmt not found, got %s)", id);
        fclose(infile);
        return false;
    }
    fread(&format_size, sizeof(uint32_t), 1, infile);
    if(format_size != 16) {
        error("Cannot prepare WAV stream: Unrecognized format. (nonstandard formatting length)");
        fclose(infile);
        return false;
    }
    fread(&tag, sizeof(short), 1, infile);
    fread(&channel_count, sizeof(short), 1, infile);
    fread(&a->content.wav.sample_rate, sizeof(uint32_t), 1, infile);
    fread(&uint8_ts_per_sec, sizeof(uint32_t), 1, infile);
    fread(&block_alignment, sizeof(short), 1, infile);
    fread(&bits_per_sample, sizeof(short), 1, infile);
    
    fread(&id, sizeof(uint8_t), 4, infile);
    if(strcmp(id, "data")) {
        error("Cannot prepare WAV stream: Unrecognized format. (data not found)");
        fclose(infile);
        return false;
    }
    // TODO: Implement this
    fread(&a->content.wav.data_size, sizeof(uint32_t), 1, infile);
    /*input_buffer = (uint8_t*) malloc(sizeof(uint8_t) * data_size);*/
    /*fread(input_buffer, sizeof(uint8_t), data_size, infile);*/
    
    if(channel_count == 1) {
        if(bits_per_sample == 8) {
            a->channels = AL_FORMAT_MONO8;
        } else if(bits_per_sample == 16) {
            a->channels = AL_FORMAT_MONO16;
        }
    } else if(channel_count == 2) {
        if(bits_per_sample == 8) {
            a->channels = AL_FORMAT_STEREO8;
        } else if(bits_per_sample == 16) {
            a->channels = AL_FORMAT_STEREO16;
        }
    }
    
    /*free(input_buffer);*/
    a->content.wav.position = 0;

    return true;
}

int8_t stream_ogg_to_buffer(audio* a, ALuint buf, uint64_t *position)
{
    char data[STREAM_BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    if(*position != a->content.ogg.file.pcm_offset) {
        int ret = ov_pcm_seek(&a->content.ogg.file, *position);
        if(ret) {
            error("Failed to move ogg stream to the correct location");
            return AUDIO_STREAM_FAILURE;
        }
    }
 
    while(size < STREAM_BUFFER_SIZE) {
        result = ov_read(&a->content.ogg.file, data + size, STREAM_BUFFER_SIZE - size, 0, 2, 1, &section);
    
        if(result > 0)
            size += result;
        else
            if(result < 0) {
                error("Failed to stream ogg file");
                return AUDIO_STREAM_FAILURE;
            } else
                break;
    }

    
    if(size == 0)
        return AUDIO_STREAM_FINISHED;
 
    alBufferData(buf, a->channels, data, size, a->content.ogg.info->rate);
    checkALError();

    *position = a->content.ogg.file.pcm_offset;
 
    return AUDIO_STREAM_SUCCESS;
}

int8_t stream_wav_to_buffer(audio* a, ALuint buf, uint64_t *position)
{
    char data[STREAM_BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;
 
    while(size < STREAM_BUFFER_SIZE && size < (a->content.wav.data_size - a->content.wav.position))
    {
        result = fread(data + size, STREAM_BUFFER_SIZE - size, sizeof(uint8_t), a->content.wav.infile);
    
        if(result > 0)
            size += result;
        else
            if(result < 0) {
                error("Failed to stream wav file");
                return AUDIO_STREAM_FAILURE;
            } else
                break;
    }

    a->content.wav.position += size;
    
    if(size == 0)
        return AUDIO_STREAM_FINISHED;
 
    alBufferData(buf, a->channels, data, size, a->content.wav.sample_rate);
 
    return AUDIO_STREAM_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////

bool _checkALError(const char* file, unsigned line)
{
    ALuint err;
    bool error_found = false;
    err = alGetError();
    if (err != AL_NO_ERROR) {
        error_found = true;
        const char* message;
        switch(err) {
            case AL_INVALID_NAME:
                message = "Invalid Name";
            break;
            case AL_INVALID_ENUM:
                message = "Invalid Enum";
            break;
            case AL_INVALID_VALUE:
                message = "Invalid Value";
            break;
            case AL_INVALID_OPERATION:
                message = "Invalid Operation";
            break;
            case AL_OUT_OF_MEMORY:
                message = "Out of Memory";
            break;
            default:
                _log(file, line, LOG_WARNING, "OpenAL: Received error code 0x%0X. Consider adding a custom message to handle it", err);
                message = "Unknown Error";
        }

        _log(file, line, LOG_ERROR, "OpenAL: %s", message);
    }
    return error_found;
}

// TODO: Add OpenAL error checking to init/cleanup functions
bool init_audio()
{
    device = NULL;
    context = NULL;

    device = alcOpenDevice(NULL);
    if(!device) {
        error("Failed to open audio device.");
        return false;
    }
    context = alcCreateContext(device, NULL);
    if(!context) {
        error("Failed to create OpenAL context.");
        alcCloseDevice(device);
        return false;
    }
    alcMakeContextCurrent(context);
    checkALError();
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    /*alListener3f(AL_ORIENTATION, 0, 0, -1);*/
    checkALError();

    return true;
}

bool cleanup_audio()
{
    alcDestroyContext(context);
    alcCloseDevice(device);

    return true;
}

audio* create_audio()
{
    audio* a = malloc(sizeof(audio));

    return a;
}

// FIXME: Need to destroy open file data
void destroy_audio(audio* a)
{
    free(a);
}

bool save_audio_to_resource(audio* a, resource_pair)
{
    stub(false);
}

audio* load_resource_to_audio(resource_pair)
{
    char* path = construct_extended_resource_path(resource_location, resource_name);
    nulltest(path);

    audio* a = create_audio();
    a->fmt = AFMT_INVALID;
    const char* ext = get_extension(resource_name);

    if(!strcmp(ext, "wav")) {
        if(prepare_wav(a, path));
            a->fmt = AFMT_WAV;
    } else if(!strcmp(ext, "ogg")) {
        if(prepare_ogg(a, path));
            a->fmt = AFMT_OGG;
    } else {
        error("Failed to load texture: File extension %s not recognized", ext);
    }

    free(path);

    return a;
}

// FIXME: Right now, only one player can exist per audio struct, or you get
// issues. This needs to be fixed
int8_t stream_audio_to_buffer(audio* a, ALuint buf, uint64_t* position)
{
    nulltest(a);

    switch(a->fmt) {
        case AFMT_OGG:
            return stream_ogg_to_buffer(a, buf, position);
        case AFMT_WAV:
            return stream_wav_to_buffer(a, buf, position);
        case AFMT_INVALID:
            error("Cannot stream audio: Invalid format");
            return AUDIO_STREAM_FAILURE;
    
    }
    return AUDIO_STREAM_FAILURE;
}

player* create_player(audio* source)
{
    player* p = malloc(sizeof(player));

    p->source  = source;
    p->loop    = false;
    p->volume  = 1.0f;

    p->data = malloc(sizeof(struct player_data));
    p->data->position = 0;
    p->data->playing = false;
    p->data->stopped = true;
    p->data->simple = false;
	alGenBuffers(2, p->data->buffers);
    checkALError();
	alGenSources(1, &p->data->source);
    checkALError();

    return p;
}

player* load_resource_to_simple_player(resource_pair)
{
    audio* a = load_resource_to_audio(resource_location, resource_name);

    player* p = create_player(a);
    p->data->simple = true;

    return p;
}

void destroy_player(player* p)
{
    if(p->data->simple)
        destroy_audio(p->source);
    alSourceStop(p->data->source);
    alDeleteBuffers(2, p->data->buffers);
    alDeleteSources(1, &p->data->source);
    free(p->data);
    free(p);
}

void player_set_playing(player* p, bool play)
{
    if(!p->source) {
        error("Failed to play audio: Player is disconnected");
        return;
    }

    if(p->data->stopped == true) {
        if(stream_audio_to_buffer(p->source, p->data->buffers[0], &p->data->position) == AUDIO_STREAM_FAILURE || stream_audio_to_buffer(p->source, p->data->buffers[1], &p->data->position) == AUDIO_STREAM_FAILURE) {
            error("Can't play: failed to load buffer");
            return;
        }
        p->data->stopped = false;
    }

    p->data->playing = play;
    if(play) {
        alSourceQueueBuffers(p->data->source, 2, p->data->buffers);
        checkALError();
        alSourcePlay(p->data->source);
        checkALError();
    } else {
        alSourcePause(p->data->source);
    }
}

bool player_get_playing(player* p)
{
    return p->data->playing;
}

void player_stop(player* p)
{
    p->data->playing = false;
    p->data->stopped = true;
    p->data->position = 0;
    alSourceStop(p->data->source);
    ALuint left;
    alGetSourcei(p->data->source, AL_BUFFERS_QUEUED, &left);
    alSourceUnqueueBuffers(p->data->source, left, p->data->buffers);
}

bool player_update(player* p)
{
    if(!p->data->playing)
        return false;
    ALenum state;
    alGetSourcei(p->data->source, AL_SOURCE_STATE, &state);
    checkALError();
    if(state != AL_PLAYING) {
        alSourcePlay(p->data->source);
        /*if(state == AL_PAUSED)*/
            /*info("Paused.");*/
        /*else if(state == AL_STOPPED)*/
            /*info("Stopped.");*/
        /*return false;*/
    }
    int processed;
 
    alGetSourcei(p->data->source, AL_BUFFERS_PROCESSED, &processed);
 
    while(processed--)
    {
        ALuint buffer;
        ALuint queued;
        
        alSourceUnqueueBuffers(p->data->source, 1, &buffer);
        checkALError();
 
        int8_t res = stream_audio_to_buffer(p->source, buffer, &p->data->position);
 
        switch(res) {
            case AUDIO_STREAM_SUCCESS:
                alSourceQueueBuffers(p->data->source, 1, &buffer);
                break;
            case AUDIO_STREAM_FAILURE:
                player_stop(p);
                return false;
            case AUDIO_STREAM_FINISHED:
                alGetSourcei(p->data->source, AL_BUFFERS_QUEUED, &queued);
                checkALError();
                if(p->loop) {
                    p->data->position = 0;
                    if(stream_audio_to_buffer(p->source, buffer, &p->data->position) == AUDIO_STREAM_SUCCESS)
                        alSourceQueueBuffers(p->data->source, 1, &buffer);
                    /*if(queued == 0) {*/
                        /*if(stream_audio_to_buffer(p->source, p->data->buffers[0], &p->data->position) == AUDIO_STREAM_FAILURE || stream_audio_to_buffer(p->source, p->data->buffers[1], &p->data->position) == AUDIO_STREAM_FAILURE) {*/
                            /*error("Can't loop: failed to reload buffer");*/
                            /*player_stop(p);*/
                            /*break;*/
                        /*}*/
                        /*alSourceQueueBuffers(p->data->source, 2, p->data->buffers);*/
                        /*checkALError();*/
                    /*}*/
                } else {/* if(queued == 0) { */
                    player_stop(p);
                }
                break;
        }
    }

    return true;
}
