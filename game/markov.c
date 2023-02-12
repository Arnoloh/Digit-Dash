#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

struct list
{
  struct list *next;
  char *value;
};

struct inlist
{
    struct inlist *next;
    int value;
};


struct list *init_list(char *value) 
{
  struct list *nlist = malloc(sizeof(struct list));
  nlist->value = value;
  nlist->next = NULL;
  return nlist;
}

void *inlist_init(struct inlist *nlist)
{
	nlist->next = NULL;
	nlist->value=0;
}
void add_node(struct inlist *head, int value)
{
  struct inlist *new_node = malloc(sizeof(struct inlist));
  new_node->value = value;
  new_node->next = head;
  head = new_node;
}
size_t list_len(struct inlist *list)
{
	size_t i=0;
	struct inlist *a=list;
	while (a->next != NULL)
	{
		a=a->next;
		i++;
	}
	return i;
}

struct inlist *findindex(struct list *list, char *find)
{
    struct list *temp=list;
    int i=0;
    struct inlist *ans=malloc(sizeof(struct inlist));
    inlist_init(ans);
    while (temp->next != NULL)
    {
        if (temp->value == find)
        {
            add_node(ans,i);
        }
        temp=temp->next;
        i++;
    }
    return ans;
}

int indexofword(struct inlist *ans)
{
    srand(time(NULL));
    int r = rand() % (list_len(ans));
    for (int i=0; i<r+1; i++)
    {
        ans=ans->next;
    }
    return ans->value;
}

char *findword(struct list *list, struct inlist *ans)
{
    int c = indexofword(ans);
    for (int i=0; i<c+1; i++)
    {
        list=list->next;
    }
    return list->value;
}
void remove_newline(char *str) 
{
    int j = 0;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != '\n') {
            str[j++] = str[i];
        }
    }

    str[j] = '\0';
}
void print_list(struct list *list)
{
    int line = 1;

    printf("[");
    if (list->next)
    {
        goto pastfst;
        while (list->next)
        {
            line += printf(";");

            if (line > 72)
            {
                printf("\n ");
                line = 1;
            }

            pastfst:
            char *temp=list->next->value;
            if (strchr(temp,'\n') != NULL) remove_newline(temp);
            line += printf(" %2s", temp);
            list = list->next;
        }
    }

    printf(" ]\n");
}

void print_inlist(struct inlist *list)
{
    int line = 1;

    printf("[");
    if (list->next)
    {
        goto pastfst;
        while (list->next)
        {
            line += printf(";");

            if (line > 72)
            {
                printf("\n ");
                line = 1;
            }

            pastfst:
            line += printf(" %2d", list->next->value);
            list = list->next;
        }
    }

    printf(" ]\n");
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

        char* pch = strtok(database, " ");
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

/*void removeFirstOccurrence(char *string, char *word) 
{
    char *ptr = strstr(string, word);
    if (ptr != NULL) 
    {
        int len = strlen(word);
        memmove(ptr, ptr + len, strlen(ptr + len) + 1);
    }
}

void removeFirstWord(char *string) 
{
    int i = 0;
    while (isspace(string[i]))
    {
        i++;
    }
    int j = i;
    while (!isspace(string[j]) && string[j] != '\0')
    {
        j++;
    }
    int len = j - i;
    memmove(string, string + len, strlen(string + len) + 1);
}

int findIndex(const char *string, char *word) 
{
    char *ptr = strstr(string, word);
    if (ptr != NULL)
    {
        return ptr - string;
    }
    return -1;
}*/


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

int main(int argc, char* argv[])
{
    char *file=filetochar("database.txt");
    remove_newline(file);
    struct list *list=chartolist(file);
    struct inlist *inlist=create_index_list(argv[1],list);
    char *ans=findword(list,inlist);
    printf("%s\n",ans);
    //print_list(list);
    //print_inlist(index);   
}