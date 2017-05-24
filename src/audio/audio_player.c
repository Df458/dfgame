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

    return player;
}

void audio_player_set_playing(audio_player player, bool playing) {
    player->play = true;
    if(prepare_buffer(player, &player->buffers[0]))
        prepare_buffer(player, &player->buffers[1]);
}

void audio_player_set_loop(audio_player player, bool loop) {
    player->loop = loop;
}

void audio_player_set_position(audio_player player, float pos) {
    player->pos = audio_source_get_length(player->src) * pos;
    AL_CALL(alSourceStop(player->al_source), );
    AL_CALL(alSourceUnqueueBuffers(player->al_source, 2, &player->buffers[0]), );
    if(prepare_buffer(player, &player->buffers[0]))
        prepare_buffer(player, &player->buffers[1]);
}

void audio_player_set_translation(audio_player player, vec3 pos) {
    alSourcefv(player->al_source, AL_POSITION, pos.data);
}

void audio_player_update(audio_player player, float dt) {
    if(!player->play)
        return;

    ALenum state;
    AL_CALL(alGetSourcei(player->al_source, AL_SOURCE_STATE, &state), );
    if(state != AL_PLAYING)
        AL_CALL(alSourcePlay(player->al_source), );

    int processed;
    AL_CALL(alGetSourcei(player->al_source, AL_BUFFERS_PROCESSED, &processed), );

    if(player->pos >= audio_source_get_length(player->src)) {
        player->pos = 0;
        if(!player->loop) {
            player->play = false;
            return;
        }
    }
 
    while(processed--) {
        ALuint buffer;
        /* uint32 len; */
        
        AL_CALL(alSourceUnqueueBuffers(player->al_source, 1, &buffer), );
        prepare_buffer(player, &buffer);
 
        /* if(check_error(audio_source_get_next_buffer(player->src, buffer, player->pos, &len), "Failed to stream new audio data")) { */
        /*     player->pos = 0; */
        /*     player->play = false; */
        /*     return; */
        /* } */
        /*  */
        /* player->pos += len; */
        /* if(player->pos >= audio_source_get_length(player->src)) { */
        /*     player->pos = 0; */
        /*     player->play = player->loop; */
        /* } */
    }
}
