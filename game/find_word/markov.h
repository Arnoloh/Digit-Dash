#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../display/player.h"

typedef struct Node
{
    char *line;
    struct Node *next;
} Node;

typedef struct
{
    char *line;
    Node *next_lines;
    int count;
} DictEntry;


DictEntry *generate_dict(const char *file_path, int *dict_size);
char **generate_lines(DictEntry *dict, int dict_size, int num_lines, int level_seed);