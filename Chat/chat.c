#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 13080
#define IP "127.0.0.1"
#define BUFFER_SIZE 256

void *read_from_server(void *arg)
{
    int sockfd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        ssize_t received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (received <= 0)
        {
            perror("Error reading from server");
            break;
        }
        buffer[received] = '\0';
        printf("%s", buffer);
    }

    return NULL;
}

void *write_to_server(void *arg)
{
    int sockfd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1)
    {

        fflush(stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        ssize_t sent = send(sockfd, buffer, strlen(buffer), 0);
        if (sent < 0)
        {
            perror("Error writing to server");
            break;
        }
    }

    return NULL;
}

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error connecting to server");
        return 1;
    }

    printf("Connected to server\n");

    // Recevoir le message initial du serveur et y répondre
    char buffer[BUFFER_SIZE];
    ssize_t received = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (received > 0)
    {
        buffer[received] = '\0';
        printf("%s", buffer);

        fgets(buffer, BUFFER_SIZE, stdin);
        ssize_t sent = send(sockfd, buffer, strlen(buffer), 0);
        if (sent < 0)
        {
            perror("Error writing to server");
        }
    }

    pthread_t read_thread, write_thread;

    if (pthread_create(&read_thread, NULL, read_from_server, (void *)&sockfd) != 0)
    {
        perror("Error creating read thread");
        return 1;
    }

    if (pthread_create(&write_thread, NULL, write_to_server, (void *)&sockfd) != 0)
    {
        perror("Error creating write thread");
        return 1;
    }

    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    close(sockfd);
    return 0;
}