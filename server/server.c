#define _GNU_SOURCE
#include "server.h"
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
        printf("error: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);

    }
    char buf[BUFFER_SIZE] ;
    int fd = open("log.txt",O_WRONLY | O_APPEND); 
    if (fd == -1)
        errx(1,"error opening file");
    char *messagesend="received\n";
    while(1) {
        // Accept an incoming connection
        int cfd = accept(sfd, NULL, NULL);
        pid_t pid = fork();
        if(pid > 0) {
            close(cfd);
            continue;
        } 
        if (pid == 0)
        {
            close(sfd);
            int a = 0;
            a = read(cfd,buf,BUFFER_SIZE);
            write(fd,buf,a);
            write(cfd,messagesend,strlen(messagesend));
            // Process data (read/write)
            close(cfd);
            exit(EXIT_SUCCESS);
        }
        else
        {
            close(fd);
            close(cfd);
            close(sfd);
            errx(1,"error");
        }
        // If error...
        if (cfd == -1) {
            errx(EXIT_FAILURE, "accept failed.");
        }
    }
    close(fd);
    return 0;

} 
