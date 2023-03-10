#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct ID{
    int id;
    char * str;
};
char * serialize_struct(void *buf,int id);

