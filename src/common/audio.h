#ifndef DFGAME_AUDIO_H
#define DFGAME_AUDIO_H
#include "resource_def.h"

#include <AL/al.h>
#include <inttypes.h>
#include <stdbool.h>

#define STREAM_BUFFER_SIZE 4096

typedef struct audio audio;

enum audio_stream_result
{
    AUDIO_STREAM_FAILURE = -1,
    AUDIO_STREAM_SUCCESS = 0,
    AUDIO_STREAM_FINISHED
};

bool init_audio();
bool cleanup_audio();

audio* create_audio();
void   destroy_audio(audio* a);
bool   save_audio_to_resource(audio* a, resource_pair);
audio* load_resource_to_audio(resource_pair);
int8_t stream_audio_to_buffer(audio* a, ALuint buf);

/*!
 * An audio player handles actual audio output.
 * The audio struct itself provides it with the necessary stream-related
 * information to get raw audio data to play.
 */
typedef struct player
{
    audio* source;
    struct player_data* data;
    bool   loop;
    float  volume;
} player;

/*!
 * Creates a new player for a given source.
 * Note: source can be set to NULL, and further player functions won't break
 */
player* create_player(audio* source);

/*!
 * Destroys a player, and stops/deletes its OpenAL data.
 */
void destroy_player(player* p);

/*!
 * Plays/pauses an audio player
 */
void player_set_playing(player* p, bool play);

/*!
 * Stops an audio player, resetting it to the beginning
 */
void player_stop(player* p);

/*!
 * Causes the player to update, loading additional buffer data as needed
 */
bool player_update(player* p);

#endif
