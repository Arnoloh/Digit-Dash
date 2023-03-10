#define _GNU_SOURCE
#include "chat.h"
#define BUFFER_SIZE 128
struct message init_message(char *name, char *message, int id)
{
    struct message *m = malloc(sizeof(struct message));
    m->id = id;
    m->name = strdup(name);
    m->message = strdup(message);
    return *m;
}
void free_message(struct message *msg)
{
    free(msg->name);
    free(msg->message);
    free(msg);
}
void u2u(void)
{
    char buffer[BUFFER_SIZE];
    int sock;
    struct sockaddr_in server;
    char message[100], server_reply[2000], name[20];
    ;
    // Créez le socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("82.65.173.135");
    server.sin_family = AF_INET;
    server.sin_port = htons(13080);

    // Connectez le socket au serveur
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return;
    }
    printf("Enter your name: ");
    bzero(name, 20);
    fgets(name, 20, stdin);
    
    puts("Connected\n");
    // Boucle pour envoyer et recevoir des messages
    while (1)
    {
        printf("Enter message : ");
        bzero(message, 100);
        fgets(message, 100, stdin);

        bzero(buffer, BUFFER_SIZE);
        strcat(buffer, name);
        buffer[strlen(name) - 1] = '\0';
        strcat(buffer, ": ");
        strcat(buffer, message);
        if (send(sock, buffer, BUFFER_SIZE, 0) < 0)
        {
            puts("Send failed");
            return;
        }
        puts("Data Send\n");
        int r=recv(sock, server_reply, 2000, 0);
        // Receive a reply from the server
        if (r < 0)
        {
            puts("recv failed");
        }
       
        write(0,server_reply, r);
    }
}