#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "list.h"


int indexofword(struct inlist *ans)
{
    srand(time(NULL));
    struct inlist *temp=ans;
    int r = rand() % (list_len(ans)+1);
    for (int i=0; i<r; i++)
    {
        temp=temp->next;
    }
    return temp->value;
}

char *findword(struct list *list, struct inlist *ans)
{
    int c = indexofword(ans);
    struct list *temp=list;
    for (int i=0; i<c+1; i++)
    {
        temp=temp->next;
    }
    return temp->value;
}

void remove_newline(char *str) 
{
    size_t j = 0;

    for (size_t i = 0; i < strlen(str); i++) 
    {
        if (str[i] != '\n') 
        {
            str[j++] = str[i];
        }
    }

    str[j] = '\0';
}

char *filetochar(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory to store the file contents
    char *buffer = malloc(file_size * sizeof(char));
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    // Read the file into the buffer
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return buffer;
}

struct list *chartolist(char *database)
{
        struct list* head = NULL;
        struct list* tail = NULL;

        char* pch = strtok(database, " \n");
        while (pch != NULL) 
        {
            struct list* node = init_list(pch);
            if (tail == NULL) 
            {
                head = node;
                tail = node;
            } 
            else 
            {
                tail->next = node;
                tail = node;
            }
            pch = strtok(NULL, " ");
        }
        
        return head; 
}

struct inlist *create_index_list(char *word, struct list *list) 
{
    struct inlist *result = NULL;
    struct inlist *temp = NULL;
    int index = 0;
    struct list *a=list;
    while (a != NULL)
    {
        if (strcmp(word, a->value) == 0)
        {
                struct inlist *node = malloc(sizeof(struct inlist));
                node->value = index;
                node->next = NULL;

                if (result == NULL)
                {
                    result = node;
                    temp = result;
                }
                else
                {
                    temp->next = node;
                    temp = temp->next;
                }
        }

        a = a->next;
        index++;
    }

    return result;
}

char * new_word(char* word,char *database)
{
    char *file=filetochar(database);
    struct list *list=chartolist(file);
    struct inlist *inlist=create_index_list(word,list);
    char *ans=findword(list,inlist);
    //printf("%s ",ans);
    return ans;
}

void toLowerCase(char *str) 
{
    for(size_t i = 0; i < strlen(str); i++) 
    {
        str[i] = tolower(str[i]);
    }
}

char *choosestartword(struct list *list)
{
    srand(time(NULL));
    int r = rand() % (list__len(list)+1);
    struct list *temp=list;
    for (int i=0; i<r; i++)
    {
        temp=temp->next;
    }
    return temp->value;
}

char *function()
{
    char *file=filetochar("database/c.txt");
    struct list *a=chartolist(file);
    char *start=choosestartword(a);
    printf("%s ",start);
    char *temp=new_word(start,"database/c.txt");

    char* resultat = malloc(sizeof(char));

    for (int i=0; i<30; i++)
    {
        temp = new_word(temp,"database/c.txt");

        char* nouveau = realloc(resultat, strlen(resultat) + strlen(temp) + 1);
        resultat = nouveau;
        strcat(resultat, temp); // Ajouter la nouvelle chaîne générée à la chaîne de résultat
    }
    return resultat;
}

int nb_underscore(char *string)
{
    int count = 0;
    int len = strlen(string);

    for (int i = 0; i < len; i++) 
    {
        if (string[i] == '\n') 
        {
            count++;
        }
    }

    return count;
}

char **str_to_list(char *string)
{
    int taille_tableau=nb_underscore(string);
    char** result = malloc(sizeof(char*) * (taille_tableau + 1));

    int index = 0;
    char* token = strtok(string, "\n");

    while (token != NULL) 
    {
        result[index] = (char*)malloc(strlen(token) + 1);

        if (result[index] == NULL) 
        {
            printf("Erreur d'allocation de mémoire\n");
            exit(EXIT_FAILURE);
        }

        strcpy(result[index], token);
        token = strtok(NULL, "\n");
        index++;
    }
    return result;
}

int main()
{
    char *a=function();
    char **strings=str_to_list(a);
    int i = 0;
    while (strings[i] != NULL) {  // Parcourir chaque chaîne jusqu'à la fin (NULL)
        int j = 0;
        while (strings[i][j] != '\0') {  // Parcourir chaque caractère jusqu'à la fin de la chaîne ('\0')
            printf("%c", strings[i][j]);
            j++;
        }
        printf("    \n");
        i++;
    }
    //char *re3=new_word(re2,"c.txt");
    //char *re4=new_word(re3,"c.txt");
    /*for (int i=0; i<15; i++)
    {
        result=new_word(result,"c.txt");
    }*/
    /*printf("Please choose the language you want: C - Python\n");
    char input[50];

    //fgets(input, 50, stdin);

    scanf( "%s", input );
    toLowerCase(input);
    if(strcmp(input, "c") == 0) 
    {
        int i=0;
        while (i<15)
        {
            if (i==0)
            {
                char *a=new_word(argv[1],"c.txt");
                char *b=new_word(a,"c.txt");
            }
            else
            {
                char *b=new_word(b,"c.txt");
            }
            i++;
        }
    }*/
}