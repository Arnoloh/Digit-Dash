#include "server.h"
#define PORT_NO 13080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

void error(const char *message) {
	perror(message);
	exit(1);
}

void handle_connection(int cfd,FILE *fd){
	int read_size;
	char client_message[BUFFER_SIZE];
	while( (read_size = recv(cfd , client_message , BUFFER_SIZE , 0)) > 0 ) {
		fprintf(fd,"%s\n",client_message);
		fflush(fd);
		write(cfd , "received\n" , strlen("received\n"));
		bzero(client_message,BUFFER_SIZE);
	}
}
int server() {
	FILE *fd = fopen("log.txt","a");
	int server_socket, client_socket, pid;
	socklen_t client_len;
	struct sockaddr_in server_address, client_address;
	int optval = 1;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) 
		error("ERROR opening socket");

	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT_NO);

	if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) 
		error("ERROR on binding");

	listen(server_socket, MAX_CLIENTS);
	client_len = sizeof(client_address);
    printf("Waiting for connections...\n");
	while (1) {
		client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_len);
		if (client_socket < 0) 
			error("ERROR on accept");

		pid = fork();
		if (pid < 0)
			error("ERROR on fork");

		if (pid == 0)  {
			close(server_socket);
			fprintf(fd,"Client connected: %i\n",getpid());
			handle_connection(client_socket,fd);
			close(client_socket);
			fprintf(fd,"Client disconnected: %i\n",getpid());
			exit(0);
		}
		else 
            signal(SIGCHLD,SIG_IGN);
			close(client_socket);
	}

	fclose(fd);
	close(server_socket);
	return 0;
}

