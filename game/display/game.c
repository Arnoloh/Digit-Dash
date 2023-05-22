#include "game.h"

unsigned int train_seed;

Game* init_game(char* name_1)
{
    // Initialisation of the game.
    Game* game = malloc(sizeof(Game));

    // Set game in running.
    game->running = 0;

    // Create players.
    Player* p1 = new_player(name_1);

    game->player_1 = p1;

    return game;
}

void start(void)
{
    Game* game = init_game("Training player");

    int dict_size = 0;
    DictEntry* dict = generate_dict("game/find_word/database/c.txt", &dict_size);
    if (!dict)
    {
        return; // Failed to generate the dictionary
    }

    int progress = 0;
    srand(time(NULL));

    while (progress < 200)
    {
        char **lines = generate_lines(dict, dict_size, 5);
        progress = run(game->player_1, lines, 5, progress);
    }

    free(game->player_1);
    free(game);
}