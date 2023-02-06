#include "server.h"
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#define BUFFER_SIZE 128
int server(void)
{
    // Create the 'hints' structure.
    struct addrinfo hints;
    // Initialize all the fields to zero.
    memset(&hints, 0, sizeof(hints));
    // Specify the criteria
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Server
                                 // Get the linked list
    struct addrinfo* result;
    int err = getaddrinfo(NULL, "13080", &hints, &result);
    if (err != 0) {
        printf("error: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }
    int sfd;
    struct addrinfo* p;
    for (p = result; p != NULL; p = p->ai_next) {

        // Try to create a socket
        sfd = socket(p->ai_family,
                p->ai_socktype,
                p->ai_protocol);
        // If error, continue with the next iteration
        if (sfd == -1)
            continue;
        // If successful, break the loop (p != 0)
        err = bind(sfd, p->ai_addr, p->ai_addrlen);

        // If error, close the socket
        if (err == 0)
            break;
        else
            close(sfd);
    }
    err = listen(sfd,5);
    if (err == -1)
    {
        close(sfd);
        errx(EXIT_FAILURE, "listen()");
    }
    while(1) {
        // Accept an incoming connection
        int cfd = accept(sfd, NULL, NULL);
        // If error...
        if (cfd == -1) {
            errx(EXIT_FAILURE, "accept failed.");
        }
        printf("Connected.\n");
        close(cfd);
    }

    } 
