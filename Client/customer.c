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

unsigned long fibonacci()
{
  unsigned long n, nbr1 = 0, nbr2 = 1, suivant, i;
 
  printf("Entrez le nombre de termes\n");
  scanf("%ld", &n);
 
  printf("Les %ld premiers termes de la série de Fibonacci sont:\n", n);
  
  if(!n)
  	errx(3,"EXIT FAILURE: Invalid parameter");
  
  if(n == 0 || n == 1)
  	return n;
  else
  {
	  for (i = 0; i < n; i++)
	  {
			if (i <= 1)
			  suivant = i;
			else
			{
			  suivant = nbr1 + nbr2;
			  nbr1 = nbr2;
			  nbr2 = suivant;
			}
			printf("%ld\n", suivant);
	  }
	 
	  return suivant;
  }
}

void query(char message[])
{
	char *welcome = "Enter a request : ";
	rewrite(STDOUT_FILENO, welcome, strlen(welcome));
	
	scanf("%s", message);
}

void help()
{
	printf("Here are the commands you are allowed to use\n");
	printf("[server] --- Will allow you to connect to the server\n");
	printf("[exit]   --- Close the client\n");
	printf("[help]   --- How use availables command\n\n");
}

void client()
{
	char message[256] = {0};
	
	while(strcmp(message,"server") && strcmp(message,"exit"))
	{
		query(message);
		if(strcmp(message,"help") && strcmp(message,"server") && strcmp(message,"exit"))
		{
			 printf("Unknown request. Enter [help] to know what command is available.\n");
		}
		
		if(!strcmp(message,"help"))
		{
			help();
		}
    }
    
    if(strcmp(message,"exit"))
	{
		int sfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addrClient;
		addrClient.sin_addr.s_addr = inet_addr("82.65.173.135");
		addrClient.sin_family = AF_INET;
		addrClient.sin_port = htons(13080);
		connect(sfd, (const struct sockaddr *)&addrClient, sizeof(addrClient));
		printf("Client connected : %d\n", sfd);
		
		char message2[256] = {0};
		
		while(strcmp(message2,"exit"))
		{
			query(message2);
			
			send(sfd, &message2, sizeof(message2), 0);
		   	printf("Message send : %s\n", message2);
		   	
		   	char answer[256] = {0};
		   	recv(sfd, &answer, 256,0);
		   	printf("Answer : %s",answer);
		}

		close(sfd);
		printf("Client disconnected : %d\n", sfd);
	}
	printf("Application closed.\n");
}
