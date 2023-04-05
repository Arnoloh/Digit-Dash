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
#include <glib.h>

#define BUFFER_SIZE 512


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

void echo(int fd_in, int fd_out)
{
    char buf[BUFFER_SIZE] = {0};
    while(1)
    {
        ssize_t r = read(fd_in, buf, BUFFER_SIZE);
        
        if(!r)
            break;
        if(r == -1)
            errx(3, "Error reading command");

        rewrite(fd_out, buf, r);
    }

    return;
}

void query(char message[])
{
	scanf("%s", message);
}

void welcome()
{
	char msg[] = "Bienvenue sur DigitDash.\n\n";
	rewrite(STDOUT_FILENO, msg, strlen(msg));
}

void help()
{
	rewrite(STDOUT_FILENO, "Menu Principal\n", strlen("Menu Principal\n"));
	rewrite(STDOUT_FILENO, "[entrainement] --- Affûte ton clavier avant d'abattre l'adversaire\n", strlen("[entrainement] --- Affûte ton clavier avant d'abattre l'adversaire\n"));
	rewrite(STDOUT_FILENO, "[multijoueur] --- Affronte des joueurs en ligne\n", strlen("[multijoueur] --- Affronte des joueurs en ligne\n"));
	rewrite(STDOUT_FILENO, "[chat] --- Echange avec tes adversaires\n", strlen("[chat] --- Echange avec tes adversaires\n"));
	rewrite(STDOUT_FILENO, "[quitter]   --- Quitter DigitDash\n", strlen("[quitter]   --- Quitter DigitDash\n"));
	rewrite(STDOUT_FILENO, "[aide]   --- Permet de connaître les options disponibles\n\n", strlen("[aide]   --- Permet de connaître les options disponibles\n\n"));
}

void client()
{
	char message[BUFFER_SIZE] = {0};
	welcome();
	help();
	
	while(1)
	{	
		
		char *welcome = "Choisissez une option: ";
		rewrite(STDOUT_FILENO, welcome, strlen(welcome));
		query(message);
		
		if(strcmp(message,"chat") == 0)
			system ("../Chat/main");
		
		else if(strcmp(message,"quitter") == 0)
			break;
		
		else if(strcmp(message,"entrainement") == 0)
			system ("../training/display/test");
		else if(strcmp(message,"multijoueur") == 0)
			break;
			
		else
		{
			rewrite(STDOUT_FILENO, "Option invalide. Entrez [aide] pour connaître les options disponibles.\n", strlen("Option invalide. Entrez [aide] pour connaître les options disponibles.\n"));
		}
		
		if(!strcmp(message,"aide"))
		{
			help();
		}
    }
    
    if(strcmp(message,"quitter"))
	{
		int sfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addrClient;
		addrClient.sin_addr.s_addr = inet_addr("82.65.173.135");
		addrClient.sin_family = AF_INET;
		addrClient.sin_port = htons(13080);
		connect(sfd, (const struct sockaddr *)&addrClient, sizeof(addrClient));
		
		//Première connexion au serveur
		char buffer[BUFFER_SIZE]  = {0};
		recv(sfd, &buffer, BUFFER_SIZE,0);
		
		//Affichage de la première requête serveur
		/*char message_tmp[BUFFER_SIZE] = {0};
		rewrite(1, buffer, strlen(buffer));
		ssize_t r = read(1, message_tmp, BUFFER_SIZE);
		if(r == -1)
        	errx(3, "Error reading option");
        
		rewrite(sfd, &message_tmp, strlen(message_tmp));
		printf("Requête envoyée : %s\n", message_tmp);*/
		
		char *waiting = "En attente de joueurs...\n\n";
		rewrite(1, waiting, strlen(waiting));
		char *welcome = "Envoyez une requête: ";
		
		//Boucle pour les différents envois de requêtes
		while(1)
		{	
			char message2[BUFFER_SIZE] = {0};
			
			rewrite(1, welcome, strlen(welcome));
			ssize_t r = read(1, message2, BUFFER_SIZE);
			if(r == -1)
            	errx(3, "Error reading option");
            
            if(strcmp(message2, "quitter\n") == 0)
				break;
            
            rewrite(sfd, message2, r);
		   	printf("\nRéponse envoyée : %s\n\n", message2);
		   	
		   	char answer[256] = {0};
		   	recv(sfd, &answer, 256,0);
		   	printf("%s\n",answer);
		}
		
		
		//Déconnexion du joueur
		close(sfd);
		rewrite(STDOUT_FILENO, "\nDigitDash fermé.\n", strlen("\nDigitDash fermé.\n"));
		printf("Joueur déconnecté : %d\n", sfd);
	}
	rewrite(STDOUT_FILENO, "DigitDash fermé.\n", strlen("DigitDash fermé.\n"));
}
