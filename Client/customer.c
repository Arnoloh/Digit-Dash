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


void rewrite(int fd, const void *buf, size_t count)
{
    ssize_t bytes_written = 0;
    
    while ((size_t)bytes_written < count){
    
        ssize_t current_write = write(fd, buf + bytes_written, count - bytes_written);
        
        if (current_write < 0) {
            errx(3,"Error writing to file");
        }
        
        bytes_written += current_write;
    }
    
    return;
}

void client()
{
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(30000);
    connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
	printf("Client connected : %d\n", socketClient);
	
	char message2[24];
	recv(socketClient, &message2, sizeof(message2), 0);
	printf("%s\n", message2);
	
	close(socketClient);
}
