#include "game.h"

Game* init_game(char* name_1, char* name_2)
{
    // Initialisation of the game.
    Game* game = malloc(sizeof(Game));

    // Set game in running.
    game->running = 0;

    // Create players.
    Player* p1 = new_player(name_1);
    Player* p2 = new_player(name_2);

    game->player_1 = p1;
    game->player_2 = p2;

    return game;
}

int main(void)
{
    size_t n = 3;
    char** level = malloc(sizeof(char*) * n);

    for (size_t i = 0 ; i < n ; i++)    
    {
        level[i] = malloc(sizeof(char) * 11);

        for (size_t j = 0 ; j < 10 ; j++)
            level[i][j] = 'a';
        
        level[i][10] = '\0';
    }

    Game* game = init_game("Ethan", "Jessy");

    run(game->player_1, level, n);

    free(game->player_1);
    free(game->player_2);
    free(game);

    free_input(level, n);

    return EXIT_SUCCESS;
}
