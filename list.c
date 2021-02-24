#include "list.h"

void push(list *l, char character) 
{
    element *cur = *l;
    if(cur == NULL) 
    {
        *l = createElement(character);
    } 
    else 
    {
        while(cur->next != NULL && cur->character != character)
        {
            cur = cur->next;
        }
        if(cur->next == NULL && cur->character != character) 
        {
            cur->next = createElement(character);
        } else 
        {
            cur->occurences++;
        }
    }
}

element * createElement(char character) 
{
    element *new_elt = (element *) malloc(sizeof(element));
    new_elt->character = character;
    new_elt->occurences = 1;
    new_elt->next = NULL;
    return new_elt;
}

void displayList(list l, FILE *f)
{
    element *cur = l;
    while(cur != NULL) 
    {
        fprintf(f, "character : %c, occurences : %d\n", cur->character, cur->occurences);
        cur = cur->next;
    }
}

void destroyList(list l) {
    element *cur = l;
    element *prec;
    while(cur != NULL) 
    {
        prec = cur;
        cur = cur->next;
        free(prec);
    }
}

void sortList(list *l)
{
    element *cur_new_list, *next, *cur = *l;
    list new_list = NULL;
    while(cur != NULL) 
    {
        next = cur->next;
        if(new_list == NULL)
        {
            new_list = cur;
            cur->next = NULL;
        }
        else if(new_list->occurences >= cur->occurences)
        {
            cur->next = new_list;
            new_list = cur;
        } else {
            cur_new_list = new_list;
            while(cur_new_list->next != NULL && cur->occurences > cur_new_list->occurences)
            {
                cur_new_list = cur_new_list->next;
            }
            cur->next = cur_new_list->next;
            cur_new_list->next = cur;
        }
        cur = next;
    }
    *l = new_list;
}