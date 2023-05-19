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

struct list *init_list(char *value);
void *inlist_init(struct inlist *nlist);
int list_is_empty(struct list *list);
void add_node(struct inlist *head, int value);
size_t list_len(struct inlist *list);
size_t list__len(struct list *list);
int list_is_sorted(struct list *list);
void print_list(struct list *list);
void print_inlist(struct inlist *list);