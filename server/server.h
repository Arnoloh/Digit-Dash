#pragma once
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <stdbool.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <errno.h>
#include <netinet/in.h>
#include "../tools/tools.h"
#include <strings.h>
#include <time.h>
int server(void);
int generate_seed();


