#define LOG_CATEGORY "Audio"

#include "audio_player.h"

#include "audio_log.h"
#include "audio_source.h"
#include "check.h"
#include "memory/alloc.h"
#include "types.h"

#include <AL/al.h>

typedef struct audio_player {
    audio_source src;
    uint32 pos;
    bool play;
    bool loop;

    ALuint al_source;
    ALuint buffers[2];
}* audio_player;

bool prepare_buffer(audio_player player, ALuint* buf) {
    uint32 len;
    if(check_error((audio_source_get_next_buffer(player->src, *buf, player->pos, &len)), "Failed to stream new audio data")) {
        player->pos = 0;
        player->play = false;
        return false;
    }
    player->pos += len;
    AL_CALL(alSourceQueueBuffers(player->al_source, 1, buf), false);

    return len > 0 && player->pos < audio_source_get_length(player->src);
}

audio_player audio_player_new(audio_source src) {
    audio_player player = salloc(sizeof(struct audio_player));
    player->play = false;
    player->loop = false;
    player->pos = 0;
    player->src = src;

    AL_CALL(alGenBuffers(2, player->buffers), NULL);
    AL_CALL(alGenSources(1, &player->al_source), NULL);
    AL_CALL(alSourcei(player->al_source, AL_LOOPING, AL_FALSE), NULL);

    return player;
}

void audio_player_set_playing(audio_player player, bool playing) {
    player->play = playing;

    int num;
    alGetSourcei(player->al_source, AL_BUFFERS_QUEUED, &num);
    if(playing && !num) {
        if(prepare_buffer(player, &player->buffers[0]))
            prepare_buffer(player, &player->buffers[1]);

        AL_CALL(alSourcePlay(player->al_source), );
    }
}

bool audio_player_get_playing(audio_player player) {
    return player->play;
}

void audio_player_set_loop(audio_player player, bool loop) {
    player->loop = loop;
}

void audio_player_set_position(audio_player player, float pos) {
    player->pos = audio_source_get_length(player->src) * pos;
    AL_CALL(alSourceStop(player->al_source), );
    int num;
    alGetSourcei(player->al_source, AL_BUFFERS_PROCESSED, &num);

    ALuint buffer;
    while(num--) {
        AL_CALL(alSourceUnqueueBuffers(player->al_source, 1, &buffer), );
        prepare_buffer(player, &buffer);
    }
}

void audio_player_set_translation(audio_player player, vec3 pos) {
    AL_CALL(alSourcefv(player->al_source, AL_POSITION, pos.data), );
}

void audio_player_set_distances(audio_player player, float reference_distance, float max_distance) {
    AL_CALL(alSourcef(player->al_source, AL_REFERENCE_DISTANCE, reference_distance), );
    AL_CALL(alSourcef(player->al_source, AL_MAX_DISTANCE, max_distance), );
}

void audio_player_update(audio_player player, float dt) {
    if(!player->play)
        return;

    int processed;
    int remaining;
    AL_CALL(alGetSourcei(player->al_source, AL_BUFFERS_PROCESSED, &processed), );
    AL_CALL(alGetSourcei(player->al_source, AL_BUFFERS_QUEUED, &remaining), );

    if(player->pos >= audio_source_get_length(player->src) && !remaining) {
        player->pos = 0;
        if(!player->loop) {
            player->play = false;
            AL_CALL(alSourceStop(player->al_source), );

            ALuint buffer;
            while(processed--) {
                AL_CALL(alSourceUnqueueBuffers(player->al_source, 1, &buffer), );
            }
            return;
        }
    }

    while(processed--) {
        ALuint buffer;
        AL_CALL(alSourceUnqueueBuffers(player->al_source, 1, &buffer), );

        if(player->pos < audio_source_get_length(player->src))
            prepare_buffer(player, &buffer);

        ALenum state;
        AL_CALL(alGetSourcei(player->al_source, AL_SOURCE_STATE, &state), );
        if(state != AL_PLAYING)
            AL_CALL(alSourcePlay(player->al_source), );
    }
}

void _audio_player_free(audio_player player, bool deep) {
    AL_CALL(alDeleteSources(1, &player->al_source), );

    if(deep)
        audio_source_free(player->src);

    sfree(player);
}
