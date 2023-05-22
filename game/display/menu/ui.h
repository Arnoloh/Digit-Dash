#pragma once

#include "../game.h"
#include "../../../Chat/chat.h"
#include "../../../tools/tools.h"

#define NB_BOUTONS 4
extern int req;
extern unsigned int level_seed;
void display_name(int largeur, int hauteur);
void display_menu(int largeur, int hauteur);
void menu();