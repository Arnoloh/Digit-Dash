#pragma once
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <pthread.h>

#include "../Chat/chat.h"
extern unsigned int level_seed;
void help();
void client();
void welcome();
void MainMenu();
void query(char message[]);
long __atoi(char message[]);
int detectRequest(char message[]);
void stockRequest(char message[], int req);
void rewrite(int fd, const void *buf, size_t count);


