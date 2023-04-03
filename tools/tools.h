#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structure chat
#define MAX_STRING_LENGTH 1024
enum TYPE
{
    Game_info,
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