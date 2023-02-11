#define _GNU_SOURCE
#include "chat.h"
#define BUFFER_SIZE 128

void u2u(void){
    char buf[BUFFER_SIZE];
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    // Créez le socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("82.65.173.135");
    server.sin_family = AF_INET;
    server.sin_port = htons( 13080 );
    
    // Connectez le socket au serveur
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    puts("Connected\n");
    
    // Boucle pour envoyer et recevoir des messages
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        
        // Envoyez un message au serveur
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        puts("Data Send\n");
        
        // Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
        }
        puts("Reply received\n");
        puts(server_reply);
    }
    
    return 0;
}