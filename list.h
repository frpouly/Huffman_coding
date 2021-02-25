#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct element
{
    int occurences;
    char character;
    struct element *left;
    struct element *right;
    struct element *next;
} element;

typedef element *list;

void push(list *l, char character);
element * createElement(char character);
void displayList(list l, FILE *f);
void destroyList(list l);
void sortList(list *l);
void insert(list *l, element *elt);

#endif