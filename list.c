#include "list.h"

void push(list *l, char character) 
{
    element *cur2, *cur = *l;
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

void swapData(element *a, element *b)
{
    element tmp = *b;
    tmp.occurences = b->occurences;
    tmp.character = b->character;
    b->occurences = a->occurences;
    b->character = a->character;
    a->occurences = tmp.occurences;
    a->character = tmp.character;
}

void sortList(list *l)
{
    element *i, *j;
    i = *l;
    if(i != NULL)
    {
        for(i = *l; i->next != NULL; i = i->next)
        {
            for(j = i->next; j != NULL; j = j->next)
            {
                if(i->occurences > j->occurences) 
                {
                    swapData(i, j);
                }
            }
        }
    }
}

void insert(list *l, element *elt)
{
    element *current;
    if (*l == NULL || (*l)->occurences >= elt->occurences) { 
        elt->next = *l; 
        *l = elt; 
    } 
    else { 
        current = *l;
        while (current->next != NULL && current->next->occurences < elt->occurences) { 
            current = current->next; 
        } 
        elt->next = current->next; 
        current->next = elt; 
    }
}