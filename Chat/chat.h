#pragma once
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../tools/tools.h"

void u2u(void);

struct message {
    int id;
    char *message;
    char *name;
};