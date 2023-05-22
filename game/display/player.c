#include "player.h"

Player* new_player(char* name)
{
    Player* p = malloc(sizeof(Player));

    p->name = name;
    p->nb_correct = 0;
    p->nb_error = 0;
    p->malus = 0;

    return p;
}

void display_stat(WINDOW* frame, Player* p)
{
    // Titre
    mvwprintw(frame, 0, 2, "[ %s's statistics ]", p->name);

    // Données statistiques
    mvwprintw(frame, 3, 2, "Score               : [%li points]\n", p->nb_correct * 5 - p->nb_error * 2);
    mvwprintw(frame, 4, 2, "Correct character   : [%lu chars]\n", p->nb_correct);
    mvwprintw(frame, 5, 2, "Invalid character   : [%lu chars]\n", p->nb_error);

    if ((p->nb_correct + p->nb_error) == 0)
        mvwprintw(frame, 6, 2, "Mean error      : [--%%]\n");
    else
        mvwprintw(frame, 6, 2, "Mean error      : [%ld%%]\n", (p->nb_error * 100) / (p->nb_correct + p->nb_error));

    wrefresh(frame);
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

void free_window(WINDOW** w)
{
    for (size_t i = 0 ; i < 4 ; i++)
    {
        delwin(w[i]);
    }

    free(w);
}


void display(WINDOW* frame, char** input, char** level, size_t len)
{
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);

    for (size_t i = 0; i < len; i++)
    {
        size_t j = 0;

        wmove(frame, i + 2 , 2);

        while (input[i][j] != '\0')
        {
            if (input[i][j] != level[i][j])
            {
                wattron(frame, COLOR_PAIR(RED));

                if (input[i][j] == ' ' || input[i][j] == '\n' || input[i][j] == '\t')
                    // Print character.
                    wprintw(frame, "_");
                else
                    // Print character.
                    wprintw(frame, "%c", input[i][j]);

                wattroff(frame, COLOR_PAIR(RED));
            }
            else
            {
                wattron(frame, COLOR_PAIR(GREEN));
                // Print character.
                wprintw(frame, "%c", input[i][j]);
                wattroff(frame, COLOR_PAIR(GREEN));
            }

            
            // Go to the next character.
            j++;
        }

        // Displays the rest of the level in grey.
        wattron(frame, COLOR_PAIR(WHITE));
        while (level[i][j] != '\0')
        {
            wprintw(frame, "%c", level[i][j]);
            j++;
        }
        wattroff(frame, COLOR_PAIR(WHITE));
    }

    // Refresh the frame to display changes
    wrefresh(frame);
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


void progress_bar(WINDOW* frame, size_t level_len, size_t input_len)
{
    int height, width;   
    getmaxyx(frame, height, width);
    init_pair(21, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(22, COLOR_WHITE, COLOR_WHITE);

    wmove(frame, 0, 2);
    
    size_t percentage = (input_len * 100) / level_len;

    int to_write = (percentage * (width-4)) / 100 ;
    int space = width - 4 - to_write;

    wprintw(frame, "[ Game progression | (200 char) | %lu%% ]", percentage);

    wmove(frame, 2, 2);

    wattron(frame, COLOR_PAIR(21));
    while (to_write > 0)
    {
        wprintw(frame, ".");
        to_write--;
    }
    wattroff(frame, COLOR_PAIR(21));
    wattron(frame, COLOR_PAIR(22));
    while (space > 0)
    {
        wprintw(frame, ".");
        space--;
    }
    wattroff(frame, COLOR_PAIR(22));

    wrefresh(frame);
}


int run(Player* p, char** level, size_t n)
{
    char** input = malloc(sizeof(char*) * n);

    for (size_t i = 0; i < n; i++)
    {
        size_t l = strlen(level[i]);
        input[i] = malloc(sizeof(char) * (l + 1));
        input[i][0] = '\0';
    }

    size_t level_len = get_size(level, n);

    refresh();

    WINDOW** all_win = malloc(sizeof(WINDOW*) * 4);

    display_win(all_win);
    progress_bar(all_win[2], level_len, 0);
    display_stat(all_win[3], p);
    display(all_win[1], input, level, n);

    size_t i = 0;

    while (i < n)
    {
        size_t j = 0;
        size_t l = strlen(level[i]);
        while (j < l)
        {
            int c = getch();

            if ((c == KEY_BACKSPACE || c == KEY_DC) && j > 0)
            {

                
                j--;
                if (input[i][j] == level[i][j])
                    p->nb_correct--;
                else
                    p->nb_error--;
                
                input[i][j] = '\0';
            }
            else if (c != KEY_BACKSPACE && c != KEY_DC)
            {
                if (c == level[i][j])
                    p->nb_correct++;
                else
                    p->nb_error++;

                input[i][j] = c;
                input[i][j+1] = '\0';
                j++;
            }

            size_t input_len = get_size(input, n);
            
            progress_bar(all_win[2], level_len, input_len);
            display_stat(all_win[3], p);
            display(all_win[1], input, level, n); 
        }

        p->malus = p->nb_correct / MALUS_THREESHOLD;
        i++;
    }

    free_input(input, n);
    free_window(all_win);

    return 1;
}