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

void server()
{
	int socketServer = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(30000);
	
	bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
	printf("Server connected : %d\n", socketServer);
	
	listen(socketServer, 5);
	printf("Wait a client\n");
	
	struct sockaddr_in addrClient;
	socklen_t csize = sizeof(addrClient);
	int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
	printf("Connection accepted\n");
	
	printf("Client %d\n", socketClient);
	
	char welcome[24] = "Bienvenue, Gabriel_Dnl.";
	
	send(socketClient, &welcome, sizeof(welcome),0);
	
	close(socketClient);
	close(socketServer);
	printf("Closed\n");
}

int main()
{
	server();
}
