#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structure chat
#define MAX_STRING_LENGTH 1024
enum TYPE
{
    Player_Info,
    Chat,
};
typedef struct
{
    enum TYPE type;
    int id;
} Generic;
typedef struct
{
    Generic info;
    char *name; 
    char *Message;
} Chat_info;
char *serialize(Generic *buf);
Generic *deserialize(const char *str);

