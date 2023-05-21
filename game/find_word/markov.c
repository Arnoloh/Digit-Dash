#include "markov.h"


// delete begin spaces
char *trim_leading_spaces(char *str)
{
    while (*str == ' ')
    {
        str++;
    }
    return str;
}
// delete end spaces
void trim_trailing_spaces(char *str)
{
    char *end = str + strlen(str);
    while (end > str && *(end - 1) == ' ')
    {
        end--;
    }
    *end = '\0';
}

void add_line(DictEntry *dict, int *dict_size, char *line, char *next_line)
{
    next_line = trim_leading_spaces(next_line);
    trim_trailing_spaces(next_line);
    line = trim_leading_spaces(line);
    trim_trailing_spaces(line);

    for (int i = 0; i < *dict_size; i++)
    {
        if (strcmp(dict[i].line, line) == 0)
        {
            Node *new_node = (Node *)malloc(sizeof(Node));
            new_node->line = malloc(strlen(next_line) + 1);
            strcpy(new_node->line, next_line);
            new_node->next = dict[i].next_lines;
            dict[i].next_lines = new_node;
            dict[i].count++;
            return;
        }
    }

    dict[*dict_size].next_lines = (Node *)malloc(sizeof(Node));
    dict[*dict_size].next_lines->line = malloc(strlen(next_line) + 1);
    strcpy(dict[*dict_size].next_lines->line, next_line);
    dict[*dict_size].next_lines->next = NULL;
    dict[*dict_size].line = malloc(strlen(line) + 1);
    strcpy(dict[*dict_size].line, line);
    dict[*dict_size].count = 1;
    (*dict_size)++;
}

void print_dict(DictEntry *dict, int dict_size)
{
    for (int i = 0; i < dict_size; i++)
    {
        printf("\033[0;34m%s\033[0;37m -> ", dict[i].line);
        Node *current = dict[i].next_lines;
        while (current != NULL)
        {
            printf("\033[0;31m%s", current->line);
            current = current->next;
        }
        printf("\n");
    }
}

// Generate a random number between 0 and max (exclusive)
int rand_num(int max)
{
    int new = rand() % max;
    srand(new);
    return new;
}
// always same
int rand_without_seed(int max)
{
    return rand() % max;
}

// Fetch a random line from the dictionary
char *get_random_line(DictEntry *dict, int dict_size)
{
    int random_index = rand_num(dict_size);
    return dict[random_index].line;
}

// Fetch a line from the dictionary that follows the given line
char *get_next_line(DictEntry *dict, int dict_size, char *line)
{
    // find the line in the dictionary
    for (int i = 0; i < dict_size; i++)
    {
        if (strcmp(dict[i].line, line) == 0)
        {
            // fetch a random next line
            int random_index = rand_without_seed(dict[i].count);
            Node *current = dict[i].next_lines;
            for (int j = 0; j < random_index; j++)
            {
                current = current->next;
            }
            return current->line;
        }
    }
    // if the line was not found, return a random line
    return get_random_line(dict, dict_size);
}

// Generate lines of text from the dictionary
// Generate lines of text from the dictionary and return them as an array of strings
char **generate_lines(DictEntry *dict, int dict_size, int num_lines)
{
    char **lines = malloc(num_lines * sizeof(char *));
    char *line = get_random_line(dict, dict_size);
    for (int i = 0; i < num_lines; i++)
    {
        printf("%s\n", line);

        // allocate a new string for the line and copy the line into it
        lines[i] = malloc((strlen(line) + 1) * sizeof(char)); // +1 for null terminator
        strcpy(lines[i], line);

        line = get_next_line(dict, dict_size, line);
    }
    return lines;
}

DictEntry *generate_dict(const char *file_path, int *dict_size) 
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Could not open file.\n");
        return NULL;
    }

    DictEntry *dict = malloc(10000 * sizeof(DictEntry));

    char *line = NULL, *next_line = NULL;
    size_t len = 0;

    if (getline(&line, &len, file) == -1) 
    {
        printf("File is empty.\n");
        return NULL;
    }

    // remove trailing newline character
    line[strcspn(line, "\n")] = 0;
    line = trim_leading_spaces(line);
    trim_trailing_spaces(line);

    while (getline(&next_line, &len, file) != -1) 
    {
        // remove trailing newline character
        next_line[strcspn(next_line, "\n")] = 0;

        add_line(dict, dict_size, line, next_line);
        free(line);
        line = next_line;
        next_line = NULL;
    }

    // don't forget to free the last line
    free(line);

    fclose(file);

    return dict;
}

/*int main() 
{
    int dict_size = 0;
    DictEntry *dict = generate_dict("database.txt", &dict_size);

    if (!dict) 
    {
         return 1; // Failed to generate the dictionary
    }

    // // // print_dict(dict, dict_size);

    // // // Seed the random number generator
    srand(time(NULL));

    //// Generate 5 random lines
    generate_lines(dict, dict_size, 5);
    return 0;
}*/
