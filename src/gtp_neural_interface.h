#ifndef gtp_neural_interface_h
#define gtp_neural_interface_h

#include "game_player.h"

typedef enum
{
	BLACK,
	WHITE
} player_t;

void getresponse(char str[]);
template <class PLAYER, class GAME>
int play_gtp_game(PLAYER* neural_player, GAME* game, int color);
void close_gtp_file();

#endif
