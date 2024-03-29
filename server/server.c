#include "server.h"
#include <pthread.h>
#define PORT_NO 13080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

typedef struct
{
    int player_one;
    char *name_player_one;
    bool player_one_ready;
    int player_two;
    char *name_player_two;
    bool player_two_ready;
} Game;

struct thread_data
{

    int client_socket;
    FILE *fd;
    Chat_info *Chat;
    Game *ALL_GAME;
};

size_t INITIAL = 5;
int generate_seed()
{
    srand(time(0));
    return rand();
}

Game *init_game()
{
    Game *all_game = calloc(INITIAL, sizeof(Game));
    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &all_game[i];
        game->player_one = 0;
        game->player_two = 0;
        game->player_one_ready = false;
        game->player_two_ready = false;
    }
    return all_game;
}
void set_ready(Game *ALL_GAME, int pid, char *name)
{

    char message[BUFFER_SIZE] = {0};
    strcpy(message, "Server: ");
    strcat(message, name);
    strcat(message, " is ready to play.\n");
    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &ALL_GAME[i];
        if (game->player_one == pid)
        {
            game->player_one_ready = true;
            write(game->player_two, message, strlen(message));
            return;
        }
        if (game->player_two == pid)
        {
            game->player_two_ready = true;
            write(game->player_one, message, strlen(message));
            return;
        }
    }
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
    };
    return 0;
}
void display_games(const Game games[], int num_games)
{
    printf("Player One | Player Two\n");
    printf("-----------|-----------\n");
    for (int i = 0; i < num_games; i++)
    {
        printf("%10d | %10d\n", games[i].player_one, games[i].player_two);
    }
}
void add_player(Game *ALL_GAME, int pid, char *name)
{
    char message[BUFFER_SIZE] = {0};
    strcpy(message, "Server: ");
    strcat(message, name);
    strcat(message, " is connected.\n");

    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &ALL_GAME[i];
        if (game->player_one == 0)
        {
            game->player_one = pid;

            game->name_player_one = name;
            if (game->player_two != 0)
            {
                write(game->player_two, message, strlen(message));
                bzero(message, BUFFER_SIZE);

                strcpy(message, "Server: ");
                strcat(message, game->name_player_two);
                strcat(message, " is connected.\n");
                write(game->player_two, message, strlen(message));
            }
            else
                write(game->player_one, "Server: No one is connected.\n", strlen("Server: No one is connected.\n"));
            return;
        }
        if (game->player_two == 0)
        {

            game->player_two = pid;
            game->name_player_two = name;

            if (game->player_one != 0)
            {
                write(game->player_one, message, strlen(message));
                bzero(message, BUFFER_SIZE);
                strcpy(message, "Server: ");
                strcat(message, game->name_player_one);
                strcat(message, " is connected.\n");
                write(game->player_two, message, strlen(message));
            }
            else
                write(game->player_two, "Server: No one is connected.\n", strlen("Server: No one is connected.\n"));
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

void client_disconnected(Game *ALL_GAME, int pid, char *name)
{
    char message[BUFFER_SIZE] = {0};
    strcpy(message, "Server: ");
    strcat(message, name);
    strcat(message, " is disconnected.\n");
    for (size_t i = 0; i < INITIAL; i++)
    {
        Game *game = &ALL_GAME[i];

        if (game->player_one == pid)
        {
            game->player_one = 0;
            game->player_one_ready = false;
            if (game->player_two != 0)
                write(game->player_two, message, strlen(message));
            return;
        }
        if (game->player_two == pid)
        {
            game->player_two = 0;
            game->player_two_ready = false;
            if (game->player_one != 0)
                write(game->player_one, message, strlen(message));

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
    char client_message[BUFFER_SIZE] = {0};
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
            write(cfd, "Server: No one is connected.\n", strlen("Server: No one is connected.\n"));
            continue;
        }

        if (strcmp(client_message, "ready") == 0)
        {
            set_ready(ALL_GAME, cfd, thread_args->Chat->name);
            continue;
        }
      
        int e = write(second_cfd, buffer, strlen(buffer));
        if (e < 0)
        {
            write(cfd, "Your friend is disconnected.\n", strlen("Your friend is disconnected.\n"));
            continue;
        }
        bzero(client_message, BUFFER_SIZE);
        bzero(buffer, BUFFER_SIZE);
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
    add_player(targs->ALL_GAME, targs->client_socket, name);
    targs->Chat = &player_chat;

    fprintf(fd, "Client connected: %i\n", cfd);

    if (pthread_create(&th, NULL, worker_message, args) != 0)
        errx(1, "handle_connection: error creating thread.");

    while (1)
    {

        if (fcntl(cfd, F_GETFD) == -1)
        {
            fprintf(fd, "Client disconnected: %i\n", cfd);
            fflush(fd);
            client_disconnected(targs->ALL_GAME, cfd, name);
            break;
        }
    }
    pthread_exit(NULL);
    free(args);
    return NULL;
}
char *int_to_string(int num)
{
    // Determine the length of the integer string representation
    int length = snprintf(NULL, 0, "%d", num);

    // Allocate memory for the string (including the null-terminator)
    char *str = (char *)malloc(length + 1);

    // Check if the memory allocation was successful
    if (!str)
    {
        fprintf(stderr, "Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    // Convert the integer to a string using sprintf
    sprintf(str, "%d", num);

    return str;
}
void *lunch_game(void *args)
{
    Game *ALL_GAME = args;
    while (1)
    {
        /* code */

        for (size_t i = 0; i < INITIAL; i++)
        {
            Game *game = &ALL_GAME[i];
            if (game->player_one_ready && game->player_two_ready)
            {
                int seed = generate_seed();
                char *seed_str = int_to_string(seed);

                char *serveur_message_stub = "Server seed: ";
                int final_message_length = strlen(serveur_message_stub) + strlen(seed_str) + 1;

                char *serveur_message = (char *)malloc(final_message_length);
                strcpy(serveur_message, serveur_message_stub);
                strcat(serveur_message, seed_str);
                strcat(serveur_message, "\n");
                game->player_one_ready = false;
                game->player_two_ready = false;
                write(game->player_one, "Server: Game start in 3\n", strlen("Server: Game start in 3\n"));
                write(game->player_two, "Server: Game start in 3\n", strlen("Server: Game start in 3\n"));
                sleep(1);

                write(game->player_one, "Server: Game start in 2\n", strlen("Server: Game start in 2\n"));
                write(game->player_two, "Server: Game start in 2\n", strlen("Server: Game start in 2\n"));
                sleep(1);

                write(game->player_one, "Server: Game start in 1\n", strlen("Server: Game start in 1\n"));
                write(game->player_two, "Server: Game start in 1\n", strlen("Server: Game start in 1\n"));
                sleep(1);

                write(game->player_one, serveur_message, strlen(serveur_message));
                write(game->player_two, serveur_message, strlen(serveur_message));
            }
        }
    }
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
    pthread_t th;
    if (pthread_create(&th, NULL, lunch_game, (void *)ALL_GAME) != 0)
        error("ERROR on pthread_create");
    pthread_detach(th);
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

        if (pthread_create(&thread_id, NULL, handle_connection, (void *)args) != 0)
            error("ERROR on pthread_create");

        pthread_detach(thread_id); // Detach the thread to automatically release resources on termination
    }
    fclose(fd);
    close(server_socket);
    return 0;
}
