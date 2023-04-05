#pragma once

#include "player.h"

typedef struct Game {
    //Generic info;     // For serialization.
    Player* player_1;   // First player.
    Player* player_2;   // Second player.
    int     running;    // 0 : the game is running | 1 : the game isn't running.
} Game;

Game* init_game(char* name_1, char* name_2);