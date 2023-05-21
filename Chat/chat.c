#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

//Nouvelle include
#include "../Client/customer.h"
#include "../game/display/player.h"

#define PORT 13080
#define IP "82.65.173.135"
#define BUFFER_SIZE 256
char name[20];

//Nouvelles variables
char buffer_seed[BUFFER_SIZE];
int req = 10;

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

		/*
        #####	Nouvelle instruction #####
        */
        
        // Détection du type de requête
        req = detectRequest(buffer);
        stockRequest(buffer, req);

        printf("\r%s%s: ", buffer, name); // Effacez l'entrée en cours et imprimez le message, puis réimprimez l'entrée
        fflush(stdout);
        
        /*
        #####	Nouvelle instruction #####
        */
        if(req == 0)
        {
        	strcpy(buffer_seed, buffer);
        	break;
        }
    }

    return NULL;
}

void *write_to_server(void *arg)
{
    int sockfd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    char formatted_buffer[BUFFER_SIZE];

	end_game:
    while (1)
    {
        printf("%s: ", name);
        fflush(stdout);
        read(0, buffer, BUFFER_SIZE);

        snprintf(formatted_buffer, sizeof(formatted_buffer), "%s", buffer);

        ssize_t sent = send(sockfd, formatted_buffer, strlen(formatted_buffer), 0);
        if (sent < 0)
        {
            perror("Error writing to server");
            break;
        }
        
        if(req == 0)
        	break;
    }
    
    /*
    #####	Nouvelle instruction #####
    */
    
    //Lancement de la partie
    Player* player = new_player(name);
	int dict_size = 0;
	seed = __atoi(buffer_seed);
	DictEntry *dict = generate_dict("../game/find_word/database/c.txt", &dict_size);
	if (!dict) {
		return NULL; // Failed to generate the dictionary
	}
	srand(seed);
	char **lines = generate_lines(dict, dict_size, 5);
    run(player, lines, 5);
    goto end_game;

    return NULL;
}

int u2u()
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

    printf("Connecté au Digi-Chat\n");

    char buffer[BUFFER_SIZE] = {0};

    ssize_t received = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (received > 0)
    {
        buffer[received] = '\0';
        req = detectRequest(buffer);
        stockRequest(buffer, req);

        write(0, buffer, strlen(buffer));

        read(0, name, 20);

        size_t i = 0;
        while (name[i] != '\0')
        {
            i++;
        }
        name[i - 1] = '\0';
        ssize_t sent = send(sockfd, name, i, 0);
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
