#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "list.h"


struct list *init_list(char *value) 
{
  struct list *nlist = malloc(sizeof(struct list));
  nlist->value = value;
  nlist->next = NULL;
  return nlist;
}

int list_is_empty(struct list *list)
{
	return list->next == NULL;
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
	struct list *a=list;
	while (a->next != NULL)
	{
		a=a->next;
		i++;
	}
	return i;
}

int list_is_sorted(struct list *list)
{
	while (list->next != NULL)
	{
		if (list->value > list->next->value) return 0;
		list=list->next;
    }
	return 1;
}


void print_list(struct list *list)
{
    printf("list_is_empty() = %s\n", list_is_empty(list) ? "yes" :  "no");
    printf("list_is_sorted() = %s\n", list_is_sorted(list) ? "yes" :  "no");

    int line = 1;

    printf("[");
    if (list)
    {
        line += printf(" %2s", list->value);
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
            line += printf(" %2s", list->next->value);
            list = list->next;
        }
    }

    printf(" ]\n");
}

void print_inlist(struct inlist *list)
{
    printf("list_is_empty() = %s\n", list_is_empty(list) ? "yes" :  "no");
    printf("list_is_sorted() = %s\n", list_is_sorted(list) ? "yes" :  "no");

    int line = 1;

    printf("[");
    if (list)
    {
        line += printf(" %2d;", list->value);
        while (list->next)
        {
            goto pastfst;
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