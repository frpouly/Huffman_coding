#ifndef _HUFFMAN_TREE_
#define _HUFFMAN_TREE_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef element *tree;

typedef struct character
{
    char character;
    char encoding;
    int size;
} character;

int transformToTree(list *l);
character * writeTreeToArray(tree *t, int size);
void prefix(tree t, char current, int size);

#endif