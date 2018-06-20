#define LOG_CATEGORY "Resource"

#include "resource/audio_loader.h"

#include "audio/audio_source.h"
#include "core/check.h"
#include "core/log/log.h"
#include "core/memory/alloc.h"
#include "math/mathutil.h"
#include "core/power.h"
#include "resource/paths.h"

#include <AL/al.h>
#include <stdio.h>
#include <string.h>
#include <vorbis/vorbisfile.h>

// Prepare an audio_source from the provided file
// Setting preload to true will load the data into memory, false will stream
// from the disk
audio_source load_audio_source(const char* path, bool preload) {
    const char* ext = get_extension(path);
    if(!strcmp(ext, "wav"))
        return load_wav_audio(path, preload);
    else if(!strcmp(ext, "ogg"))
        return load_ogg_audio(path, preload);

    error("Failed to load audio: File extension %s not recognized", ext);

    return NULL;
}

#define WAV_HEADER_SIZE 44

void stream_wav_audio(byte** data, uint32 position, uint32 requested_length, uint32* final_length, void* user) {
    if(!data) { // Null data indicates a close request
        fclose(user);
        return;
    }

    FILE* infile = (FILE*)user;
    check_return(!fseek(infile, WAV_HEADER_SIZE + position, SEEK_SET), "Failed to seek wav stream: %s", , error_str());
    *data = scalloc(requested_length, sizeof(byte));
    *final_length = fread(*data, sizeof(byte), requested_length, infile);
}

// Prepare an audio_source from the provided wav file
// Setting preload to true will load the data into memory, false will stream
// from the disk
audio_source load_wav_audio(const char* path, bool preload) {
    FILE* infile = fopen(path, "r");
    check_return(infile, "Failed to prepare WAV stream, file %s not found", false, path);
    
    char id[5];
    id[4] = 0;
    byte* input_buffer;
    uint32 file_size, data_size, format_size, bytes_per_sec, final_length, sample_rate;
    uint16 tag, channel_count, block_alignment, bits_per_sample;
    ALenum channels = AL_FORMAT_MONO8;
    
    size_t read = fread(&id, sizeof(char), 4, infile);
    if(check_error(read == 4 && !strcmp(id, "RIFF"), "Cannot prepare WAV stream: Not a RIFF file. (Got %s)", id)) {
        fclose(infile);
        return false;
    }
    read = fread(&file_size, sizeof(uint32), 1, infile);
    read += fread(&id, sizeof(char), 4, infile);
    if(check_error(read == 5 && !strcmp(id, "WAVE"), "Cannot prepare WAV stream: Not a WAVE file. (Got %s)", id)) {
        fclose(infile);
        return false;
    }
    
    read = fread(&id, sizeof(char), 4, infile);
    if(check_error(read == 4 && !strcmp(id, "fmt "), "Cannot prepare WAV stream: Unrecognized format. (fmt not found, got %s)", id)) {
        fclose(infile);
        return false;
    }
    read = fread(&format_size, sizeof(uint32), 1, infile);
    if(check_error(read == 1 && format_size == 16, "Cannot prepare WAV stream: Unrecognized format. (nonstandard formatting length: %d)", format_size)) {
        fclose(infile);
        return false;
    }
    read  = fread(&tag, sizeof(uint16), 1, infile);
    read += fread(&channel_count, sizeof(uint16), 1, infile);
    read += fread(&sample_rate, sizeof(uint32), 1, infile);
    read += fread(&bytes_per_sec, sizeof(uint32), 1, infile);
    read += fread(&block_alignment, sizeof(uint16), 1, infile);
    read += fread(&bits_per_sample, sizeof(uint16), 1, infile);

    if(check_error(read == 6, "Cannot prepare WAV stream: Stream information missing")) {
        fclose(infile);
        return false;
    }
    
    read = fread(&id, sizeof(uint8), 4, infile);
    if(check_error(read == 4 && !strcmp(id, "data"), "Cannot prepare WAV stream: Unrecognized format. (data not found)")) {
        fclose(infile);
        return false;
    }

    read = fread(&data_size, sizeof(uint32), 1, infile);
    if(check_error(read == 1, "Cannot prepare WAV stream: Stream length missing")) {
        fclose(infile);
        return false;
    }
    data_size -= 8;
    
    if(channel_count == 1) {
        if(bits_per_sample == 8) {
            channels = AL_FORMAT_MONO8;
        } else if(bits_per_sample == 16) {
            channels = AL_FORMAT_MONO16;
        }
    } else if(channel_count == 2) {
        if(bits_per_sample == 8) {
            channels = AL_FORMAT_STEREO8;
        } else if(bits_per_sample == 16) {
            channels = AL_FORMAT_STEREO16;
        }
    }

    if(preload) {
        stream_wav_audio(&input_buffer, 0, data_size, &final_length, infile);
        fclose(infile);
        return audio_source_new_buffer(input_buffer, final_length, channels, sample_rate, path);
    }

    return audio_source_new_stream(&(audio_stream_event){ .func=stream_wav_audio, .user=infile }, data_size, channels, sample_rate, path);
}

#define VORBIS_MAX_SAMPLE_SIZE 32

void stream_ogg_audio(byte** data, uint32 position, uint32 requested_length, uint32* final_length, void* user) {
    if(!data) { // Null data indicates a close request
        ov_clear(user);
        return;
    }

    uint32 size = 0;
    int section;
    int result;
    OggVorbis_File* infile = (OggVorbis_File*)user;
    *data = scalloc(requested_length, sizeof(byte));

    if(position != infile->offset)
        check_return(ov_pcm_seek(infile, position / 4) == 0, "Failed to move ogg stream to the correct location", );
 
    while(size < requested_length) {
        result = ov_read(infile, (char*)(*data) + size, requested_length - size, 0, 2, 1, &section);
    
        if(result > 0)
            size += result;
        else if(result != 0) {
            const char* message = "Unknown Error";
            switch(result) {
                case OV_HOLE: message = "Streaming interrupted"; break;
                case OV_EBADLINK: message = "Invalid stream/link"; break;
                case OV_EINVAL: message = "Corrupt header"; break;
                default: message = "Unknown Error"; break;
            }
            error("Failed to stream Ogg file: %s", message);
            sfree(data);
            return;
        }
        else
            break;
    }

    *final_length = size;
}

// Prepare an audio_source from the provided ogg file
// Setting preload to true will load the data into memory, false will stream
// from the disk
audio_source load_ogg_audio(const char* path, bool preload) {
    int result;

    OggVorbis_File* infile = salloc(sizeof(OggVorbis_File));
    vorbis_info* info;
    byte* input_buffer;
    uint32 data_size, sample_rate, final_length;
    ALenum channels;
    
    if((result = ov_fopen(path, infile)) < 0) {
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
        return NULL;
    }
    info = ov_info(infile, -1);

    check_return(info, "Ogg bitstream does not exist", NULL);
 
    if(info->channels == 1)
        channels = AL_FORMAT_MONO16;
    else
        channels = AL_FORMAT_STEREO16;

    sample_rate = info->rate;
    data_size = ov_pcm_total(infile, -1);

    check_return(data_size != OV_EINVAL, "Ogg bitstream is missing or non-seekable", NULL);

    data_size *= VORBIS_MAX_SAMPLE_SIZE; // Unlikely that samples will be larger than this. We scale down the buffer afterwards

    if(check_warn(infile->seekable, "Ogg file %s is non-seekable, and will be preloaded", path))
        preload = true;
    if(preload) {
        info("Preloading up to %u bytes", data_size);
        stream_ogg_audio(&input_buffer, 0, data_size, &final_length, infile);
        input_buffer = resalloc(input_buffer, final_length);
        info("Done. Loaded %u bytes", final_length);
        ov_clear(infile);
        return audio_source_new_buffer(input_buffer, final_length, channels, sample_rate, path);
    }

    return audio_source_new_stream(&(audio_stream_event){ .func=stream_ogg_audio, .user=infile }, data_size, channels, sample_rate, path);
}
