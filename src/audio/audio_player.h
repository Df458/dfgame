#ifndef DF_AUDIO_PLAYER
#define DF_AUDIO_PLAYER
#include "audio_player.hd"
#include "audio_source.hd"

#include "vector.h"

audio_player audio_player_new(audio_source src);
void audio_player_set_playing(audio_player player, bool playing);
bool audio_player_get_playing(audio_player player);
void audio_player_set_loop(audio_player player, bool loop);
void audio_player_set_position(audio_player player, float pos);
void audio_player_set_translation(audio_player player, vec3 pos);
void audio_player_set_distances(audio_player player, float reference_distance, float max_distance);
void audio_player_update(audio_player player, float dt);

void _audio_player_free(audio_player player, bool deep);
#define audio_player_free(p, d) { _audio_player_free(p, d); p = NULL; }

#endif
