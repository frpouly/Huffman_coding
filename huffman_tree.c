#include "huffman_tree.h"

int transformToTree(list *l)
{
    int size = 0;
    element *new_elt, *cur = *l;
    while(cur != NULL)
    {
        if(cur->character != '\0')
            size++;
        if(cur->next != NULL) 
        {
            new_elt = (element *) malloc(sizeof(element));
            new_elt->left = cur;
            new_elt->right = cur->next;
            new_elt->character = '\0';
            if(cur->next->character != '\0')
                size++;
            cur = cur->next->next;
            new_elt->occurences = new_elt->left->occurences + new_elt->right->occurences;
            *l = cur;
            insert(l, new_elt);
            cur = *l;
        } else {
            cur = cur->next;
        }
    }
    return size;
}

void prefix(tree t, char current, int size) 
{
    if(t->character != '\0')
        printf("%c : %d %d\n", t->character, current, size);
    else
    {
        size++;
        prefix(t->left, current << 1, size);
        current <<= 1;
        current |= 1;
        prefix(t->right, current, size);
    }
}

character * writeTreeToArray(tree *t, int size)
{
    character * characters = (character *) malloc(sizeof(character) * size);
    return characters;
}