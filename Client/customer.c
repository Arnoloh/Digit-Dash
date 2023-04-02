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

typedef struct Player
{
	char nom[20];
	int nb_game;
	int record;
	//int score_by_game[];
} User;


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

void query(char message[])
{
	char *welcome = "Choisissez une option : ";
	rewrite(STDOUT_FILENO, welcome, strlen(welcome));
	
	scanf("%s", message);
}

void welcome()
{
	char msg[] = "Bienvenue sur DigitDash.\n	Quel est ton nom jeune prodige ? ";
	rewrite(STDOUT_FILENO, msg, strlen(msg));
}

User init_Player(User player)
{
	scanf("%s", player.nom);
	player.nb_game = 0;
	player.record = 0;
	//player.score_by_game[player.nb_game] = {0};
	printf("Quel le C soit avec toi %s ! Ton record actuel est de %d et ton nombre de duel de %d.\n", player.nom, player.record, player.nb_game);
	
	return player;
}

void help()
{
	rewrite(STDOUT_FILENO, "Menu Principal\n", strlen("Menu Principal\n"));
	rewrite(STDOUT_FILENO, "[entraînement] --- Affûte ton clavier avant d'abattre l'adversaire\n", strlen("[entraînement] --- Affûte ton clavier avant d'abattre l'adversaire\n"));
	rewrite(STDOUT_FILENO, "[multijoueur] --- Affronte des joueurs en ligne\n", strlen("[multijoueur] --- Affronte des joueurs en ligne\n"));
	rewrite(STDOUT_FILENO, "[chat] --- Echange avec tes adversaires\n", strlen("[chat] --- Echange avec tes adversaires\n"));
	rewrite(STDOUT_FILENO, "[quitter]   --- Quitter DigitDash\n", strlen("[quitter]   --- Quitter DigitDash\n"));
	rewrite(STDOUT_FILENO, "[aide]   --- Permet de connaître les options disponibles\n\n", strlen("[aide]   --- Permet de connaître les options disponibles\n\n"));
}

void client()
{
	char message[256] = {0};
	User player;
	
	welcome();
	player = init_Player(player);
	
	while(strcmp(message,"multijoueur") && strcmp(message,"quitter"))
	{
		query(message);
		if(strcmp(message,"aide") && strcmp(message,"multijoueur") && strcmp(message,"quitter"))
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
		printf("Joueur connecté : %s\n", player.nom);
		
		char message2[256] = {0};
		
		while(strcmp(message2,"quitter"))
		{
			query(message2);
			
			rewrite(sfd, message2, sizeof(message2));
		   	printf("Message send : %s\n", message2);
		   	
		   	char answer[256] = {0};
		   	recv(sfd, &answer, 256,0);
		   	printf("Answer : %s",answer);
		}

		close(sfd);
		rewrite(STDOUT_FILENO, "DigitDash fermé.\n", strlen("DigitDash fermé.\n"));
		printf("Joueur déconnecté : %d\n", sfd);
	}
	rewrite(STDOUT_FILENO, "DigitDash fermé.\n", strlen("DigitDash fermé.\n"));
}
