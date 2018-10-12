#define LOG_CATEGORY "Audio"

#include "audio/audio_player.h"

#include "audio/audio_log.h"
#include "audio/audio_source.h"
#include "core/check.h"
#include "core/memory/alloc.h"
#include "core/types.h"

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

// Creates a new audio player for the given source
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

// Gets/sets the player's playing status
bool audio_player_get_playing(audio_player player) {
    check_return(player, "Player is NULL", false);
    return player->play;
}
void audio_player_set_playing(audio_player player, bool playing) {
    check_return(player, "Player is NULL", );
    player->play = playing;

    int num;
    alGetSourcei(player->al_source, AL_BUFFERS_QUEUED, &num);
    if(playing && !num) {
        if(prepare_buffer(player, &player->buffers[0]))
            prepare_buffer(player, &player->buffers[1]);

        AL_CALL(alSourcePlay(player->al_source), );
    }
}

// Gets/sets whether or not the player should loop audio when it finishes
bool audio_player_get_loop(audio_player player) {
    check_return(player, "Player is NULL", false);
    return player->loop;
}
void audio_player_set_loop(audio_player player, bool loop) {
    check_return(player, "Player is NULL", );
    player->loop = loop;
}

// Gets/sets the position in the audio
float audio_player_get_position(audio_player player) {
    check_return(player, "Player is NULL", 0);

    uint32 len = audio_source_get_length(player->src);
    if(len == 0) {
        return 0;
    }

    return 1.0f / len;
}
void audio_player_set_position(audio_player player, float pos) {
    check_return(player, "Player is NULL", );

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

// Gets/sets the position of the listener relative to the player
vec3 audio_player_get_translation(audio_player player) {
    vec3 data = vec3_zero;
    check_return(player, "Player is NULL", data);

    AL_CALL(alGetSourcefv(player->al_source, AL_POSITION, data.data), data);

    return data;
}
void audio_player_set_translation(audio_player player, vec3 pos) {
    check_return(player, "Player is NULL", );
    AL_CALL(alSourcefv(player->al_source, AL_POSITION, pos.data), );
}

// Sets the minimum/maximum distances for volume purposes
void audio_player_set_distances(audio_player player, float reference_distance, float max_distance) {
    AL_CALL(alSourcef(player->al_source, AL_REFERENCE_DISTANCE, reference_distance), );
    AL_CALL(alSourcef(player->al_source, AL_MAX_DISTANCE, max_distance), );
}

// Gets/sets the relative volume of the audio playback
float audio_player_get_gain(audio_player player) {
    check_return(player, "Player is NULL", 0);
    float gain;
    AL_CALL(alGetSourcef(player->al_source, AL_GAIN, &gain), 0);

    return gain;
}
void audio_player_set_gain(audio_player player, float gain) {
    check_return(player, "Player is NULL", );
    alSourcef(player->al_source, AL_GAIN, gain);
}

// Gets the source that is playing
audio_source audio_player_get_source(audio_player player) {
    check_return(player, "Player is NULL", NULL);
    return player->src;
}

// Updates the player, buffering new audio data as needed
void audio_player_update(audio_player player, float dt) {
    if(!player->play)
        return;

    int processed;
    int remaining;
    AL_CALL(alGetSourcei(player->al_source, AL_BUFFERS_PROCESSED, &processed), );
    AL_CALL(alGetSourcei(player->al_source, AL_BUFFERS_QUEUED, &remaining), );

    if(player->pos >= audio_source_get_length(player->src) && !remaining) {
        player->pos = 0;

        while(processed != 0 && processed--)
            AL_CALL(alSourceUnqueueBuffers(player->al_source, 1, NULL), );

        if(!player->loop) {
            player->play = false;
            AL_CALL(alSourceStop(player->al_source), );
        } else if(prepare_buffer(player, &player->buffers[0])) {
            prepare_buffer(player, &player->buffers[1]);
        }

        return;
    }

    while(processed != 0 && processed--) {
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

// Frees the player. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _audio_player_free(audio_player player, bool deep) {
    check_return(player, "Player is NULL", );
    AL_CALL(alDeleteSources(1, &player->al_source), );

    if(deep)
        audio_source_free(player->src);

    sfree(player);
}
