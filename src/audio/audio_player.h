#ifndef DF_AUDIO_PLAYER
#define DF_AUDIO_PLAYER
#include "audio/audio_player.hd"
#include "audio/audio_source.hd"

#include "math/vector.h"

// Creates a new audio player for the given source
audio_player audio_player_new(audio_source src);

// Gets/sets the player's playing status
bool audio_player_get_playing(audio_player player);
void audio_player_set_playing(audio_player player, bool playing);

// Gets/sets whether or not the player should loop audio when it finishes
bool audio_player_get_loop(audio_player player);
void audio_player_set_loop(audio_player player, bool loop);

// Gets/sets the position in the audio
float audio_player_get_position(audio_player player);
void audio_player_set_position(audio_player player, float pos);

// Gets/sets the position of the listener relative to the player
vec3 audio_player_get_translation(audio_player player);
void audio_player_set_translation(audio_player player, vec3 pos);

// Sets the minimum/maximum distances for volume purposes
void audio_player_set_distances(audio_player player, float reference_distance, float max_distance);

// Gets/sets the relative volume of the audio playback
float audio_player_get_gain(audio_player player);
void audio_player_set_gain(audio_player player, float gain);

// Gets the source that is playing
audio_source audio_player_get_source(audio_player player);

// Updates the player, buffering new audio data as needed
void audio_player_update(audio_player player, float dt);

// Frees the player and sets it to NULL to make it harder to double-free.
#define audio_player_free(p, d) { _audio_player_free(p, d); p = NULL; }

// Frees the player. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _audio_player_free(audio_player player, bool deep);

#endif
