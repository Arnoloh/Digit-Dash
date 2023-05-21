#include "player.h"

/*
######### NEW #########
*/

//New include
#include <pthread.h>
#include "../../Chat/chat.h"

Player* new_player(char* name)
{
    Player* p = malloc(sizeof(Player));

    p->name = name;
    p->nb_correct = 0;
    p->nb_error = 0;
    p->malus = 0;

    return p;
}

void display_stat(Player* p)
{
    printf("\n\n\nStatistiques du joueur [%s]\n", p->name);
    printf("============================\n");
    printf("Score               : [%li points]\n", p->nb_correct * 5 - p->nb_error * 2);
    printf("Caractère correct   : [%lu]\n", p->nb_correct);
    printf("Caractère incorrect : [%lu]\n", p->nb_error);
    printf("Moyenne d'erreur    : [%ld%%]\n", (p->nb_error * 100) / (p->nb_correct + p->nb_error));
    printf("============================\n");
}

/**
  \brief Set the writing color.
  */
void set_color(int color)
{
    switch (color)
    {
        case GREEN:
            printf("\033[1;32m");
            break;
        case RED:
            printf("\033[1;31m");
            break;
        case GREY:
            printf("\033[0;30m");
            break;
        default:
            printf("\033[0;0m");
    }   
}

/**
  \brief Get char one by one in the standart input.
*/
int get_ch(void) 
{
    int ch;
    struct termios oldt;
    struct termios newt;

    // Store old settings.
    tcgetattr(STDIN_FILENO, &oldt); 

    // Copy old settings to new settings.
    newt = oldt; 
    
    // Make one change to old settings in new settings.
    newt.c_lflag &= ~(ICANON | ECHO); 

    // Apply the new settings immediatly.
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); 

    // Standard getchar call.
    ch = getchar();

    // Reapply the old settings.
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Return received char.
    return ch;
}

void free_input(char** input, size_t n)
{
    // Free each line.
    for (size_t i = 0 ; i < n ; i++)
    {
        free(input[i]);
    }

    // Free array.
    free(input);
}

/**
  \brief Display all of the game with corect color.
  */
void display(char** input, char** level, size_t len)
{
    for (size_t i = 0 ; i < len ; i++)
    {
        size_t j = 0;

        // Displays all player's inputs with appropriate color.
        while (input[i][j] != '\0')
        {
            // Reset the current color.
            set_color(GREEN);

            // Set current color to red if the player's input is incorect and print it.
            if (input[i][j] != level[i][j])
            {
                set_color(RED);

                if (input[i][j] == ' ')
                    // Print character.
                    printf("_");
                else
                    // Print character.
                    printf("%c", input[i][j]);
            }
            else
            {
                // Print character.
                printf("%c", input[i][j]);
            }
            
            // Go to the next character.
            j++;
        }

        // Displays the rest of the level in grey.  
        set_color(GREY);
        while (level[i][j] != '\0')
        {
            printf("%c", level[i][j]);
            j++;
        }

        // Jumps to the next line.
        printf("\n");
    }
}

/**
  \brief Get size of the game.
  */
size_t get_size(char** level, size_t len)
{
    size_t n = 0;

    for (size_t i = 0 ; i < len ; i++)
    {
        n += strlen(level[i]);
    }

    return n;
}

/**
  \brief Calculates and diplaye the progress bar.
  */
void progress_bar(size_t level_len, size_t input_len)
{
    // Clear the terminal.
    system("clear");

    // Calculate player's percentage of advancement.
    size_t percentage = (input_len * 100) / level_len;

    // Calculate how many '=' have to display.
    int to_write = percentage / 10;
    int space = 10 - to_write;

    set_color(WHITE);

    // Display progress bar.
    printf("\nProgression [");
    while (to_write > 0)
    {
        printf("=");
        to_write--;
    }
    while (space > 0)
    {
        printf(" ");
        space--;
    }
    printf("] %lu%%\n\n", percentage);

}

/**
  \brief Run the game.
  */
int run(Player* p, char** level, size_t n)
{
    char** input = malloc(sizeof(char*) * n);

    for (size_t i = 0 ; i < n ; i++)
    {
        size_t l = strlen(level[i]);
        input[i] = malloc(sizeof(char) * (l + 1));
        input[i][0] = '\0';
    }

    // Get the total size of level.
    size_t level_len = get_size(level, n);

    // First display of the game.
    progress_bar(level_len, 0);
    display(input, level, n);

    size_t i = 0;

    while (i < n)
    {
        size_t j = 0;
        size_t l = strlen(level[i]);
        while (j < l)
        {
            // Ask character to player.
            char c = get_ch(void);

            // In case the input is a backspace.
            if ((c == BACKSPACE_KEY || c == DELETE_KEY) && j > 0)
            {
                j--;
                if (input[i][j] == level[i][j])
                    p->nb_correct--;
                else
                    p->nb_error--;
                
                input[i][j] = '\0';
            }
            // Else add the character to the input string.
            else if (c != BACKSPACE_KEY && c!= DELETE_KEY)
            {
                // Add an error to player's stat.
                if (c == level[i][j])
                    
                    p->nb_correct++;
                // Add an error to player's stat.
                else
                    p->nb_error++;

                input[i][j] = c;
                input[i][j+1] = '\0';
                j++;
            }

            // Refreshes input len.
            size_t input_len = get_size(input, n);
            
            // Refreshes the display
            progress_bar(level_len, input_len);
            display(input, level, n);
        }

        // Add malus to the opponent.
        p->malus = p->nb_correct / MALUS_THREESHOLD;
        i++;
    }

    // Show stats of the player.
    display_stat(p);

    // Free player's input.
    free_input(input, n);
    
    /*
    ######### NEW #########
    */
    
    sleep(2);
    
    system("clear");
    
    set_color(WHITE);

    return 1;
}
