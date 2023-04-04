#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define BACKSPACE_KEY 8
#define DELETE_KEY 127
#define MALUS_THREESHOLD 35

enum colors {
    GREY,
    GREEN,
    RED,
    WHITE
};

typedef struct Player {
    char*   name;
    size_t  nb_correct;
    size_t  nb_error;
    int     malus;
} Player ;

#include "game.h"

Player* new_player(char* name);
void    display_stat(Player* p);
void    set_color(int color);
int     getch(void);
void    free_input(char** input, size_t n);
void    display(char** input, char** level, size_t len);
size_t  get_size(char** level, size_t len);
void    progress_bar(size_t level_len, size_t input_len);
int     run(Player* p, char** level, size_t n);

