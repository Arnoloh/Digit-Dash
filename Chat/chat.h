#pragma once
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ncurses.h>

#include "../Client/customer.h"
#include "../game/display/player.h"
#include "../game/find_word/markov.h"

#define PORT 13080
#define IP "82.65.173.135"
#define BUFFER_SIZE 256

int connect_to_server();
char *u2u(int sockfd, bool name_already);
struct message
{
    int id;
    char *message;
    char *name;
};