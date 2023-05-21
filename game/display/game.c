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

    Game* game = init_game("Ethan", "Jessy");

    size_t n = 5;

    int dict_size = 0;
    seed = time(NULL);
    DictEntry *dict = generate_dict("../find_word/database/c.txt", &dict_size);
    if (!dict) {
        return 1; // Failed to generate the dictionary
    }
    srand(seed);
    char **lines = generate_lines(dict, dict_size, 5);
    run(game->player_1, lines, n);

    free(game->player_1);
    free(game->player_2);
    free(game);



    return EXIT_SUCCESS;
}
