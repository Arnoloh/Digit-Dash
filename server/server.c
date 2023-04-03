#include "server.h"
#include <pthread.h>
#define PORT_NO 13080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

typedef struct
{
    int player_one;
    int player_two;
} Game;

struct thread_data
{

    int client_socket;
    FILE *fd;
    Chat_info *Chat;
    Game *ALL_GAME;
};
size_t INITIAL = 5;

Game *init_game()
{
    Game *all_game = calloc(INITIAL, sizeof(Game));
    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &all_game[i];
        game->player_one = 0;
        game->player_two = 0;
    }
    return all_game;
}

int find_fd(Game *ALL_GAME, int pid)
{
    for (size_t i = 0; i < INITIAL; i++)
    {

        Game *game = &ALL_GAME[i];

        if (game->player_one == pid)
        {
            return game->player_two;
        }
        if (game->player_two == pid)
        {
            return game->player_one;
        }
    }
    // s// printf("not found\n");
    return 0;
}
void add_player(Game *ALL_GAME, int pid)
{
    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &ALL_GAME[i];
        if (game->player_one == 0)
        {
            game->player_one = pid;
            return;
        }
        if (game->player_two == 0)
        {
            game->player_two = pid;
            return;
        }
    }
    errx(1, "find_game: No game alone");
    return;
}
void error(const char *message)

{
    perror(message);
    exit(1);
}

void client_disconnected(Game *ALL_GAME, int pid)
{
    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &ALL_GAME[i];

        if (game->player_one == pid)
        {
            game->player_two = 0;
            return;
        }
        if (game->player_two == pid)
        {
            game->player_one = 0;
            return;
        }
    }
}


void *worker_message(void *args)
{
    struct thread_data *thread_args = args;
    int cfd = thread_args->client_socket;
    FILE *fd = thread_args->fd;
    Game *ALL_GAME = thread_args->ALL_GAME;
    char client_message[BUFFER_SIZE];
    int read_size;
    int second_cfd = find_fd(ALL_GAME, cfd);

    char buffer[BUFFER_SIZE] = {0};
    while ((read_size = recv(cfd, client_message, BUFFER_SIZE, 0)) > 0)
    {

        if (read_size == 0)
            break;
        strcpy(buffer, thread_args->Chat->name);
        strcat(buffer, ": ");
        strcat(buffer, client_message);
        fprintf(fd, "%s: %s", thread_args->Chat->name, client_message);
        fflush(fd);
        second_cfd = find_fd(ALL_GAME, cfd);
        if (second_cfd == 0)
        {
            write(cfd, "No one is connected.\n", strlen("No one is connected.\n"));
            continue;
        }
        int e = write(second_cfd, buffer, strlen(buffer));
        if (e < 0)
        {
            write(cfd, "Your friend is disconnected.\n", strlen("Your friend is disconnected.\n"));
            client_disconnected(ALL_GAME,second_cfd);
            second_cfd = 0;

            continue;
        }
        bzero(client_message, BUFFER_SIZE);
    }

    close(cfd);
    pthread_exit(NULL);
    return NULL;
}

void *handle_connection(void *args)
{

    struct thread_data *targs = args;
    int cfd = targs->client_socket;
    FILE *fd = targs->fd;

    pthread_t th;

    char *server_ask = "Server: Give me a name: ";
    write(cfd, server_ask, strlen(server_ask));
    char name[20] = {0};
    read(cfd, name, 20);
    for (int i = 0; i < 20; i++)
        if (name[i] == '\n')
        {
            name[i] = '\0';
            break;
        }

    Chat_info player_chat = {{Chat, Chat}, name, NULL};
    targs->Chat = &player_chat;

    fprintf(fd, "Client connected: %i\n", cfd);

    if (pthread_create(&th, NULL, worker_message, args) != 0)
        errx(1, "handle_connection: error creating thread.");
    // if (pthread_create(&th, NULL, worker_message_reading, args) != 0)
    //     errx(1, "handle_connection: error reading creating thread.");
    while (1)
    {
        if (fcntl(cfd, F_GETFD) == -1)
        {
            fprintf(fd, "Client disconnected: %i\n", cfd);
            fflush(fd);
            break;
        }
    }
    pthread_exit(NULL);
    free(args);
    return NULL;
}
int server()
{
    Game *ALL_GAME = init_game();
    FILE *fd = fopen("log.txt", "a");
    int server_socket, client_socket;
    socklen_t client_len;
    struct sockaddr_in server_address, client_address;
    int optval = 1;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        error("ERROR opening socket");

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT_NO);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        error("ERROR on binding");

    listen(server_socket, MAX_CLIENTS);
    client_len = sizeof(client_address);
    // sprintf("Waiting for connections...\n");
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_len);

        if (client_socket < 0)
            error("ERROR on accept");

        pthread_t thread_id;
        struct thread_data *args = malloc(sizeof(struct thread_data));
        args->client_socket = client_socket;
        args->fd = fd;
        args->Chat = NULL;
        args->ALL_GAME = ALL_GAME;
        add_player(ALL_GAME, client_socket);

        if (pthread_create(&thread_id, NULL, handle_connection, (void *)args) != 0)
            error("ERROR on pthread_create");

        pthread_detach(thread_id); // Detach the thread to automatically release resources on termination
    }
    fclose(fd);
    close(server_socket);
    return 0;
}
