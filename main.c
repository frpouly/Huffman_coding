#include "list.h"
#include "huffman_tree.h"

int main()
{
    list l = NULL;
    push(&l, 'a');
    push(&l, 'a');
    push(&l, 'b');
    push(&l, 'a');
    push(&l, 'b');
    push(&l, 'c');
    push(&l, 'c');
    push(&l, 'c');
    push(&l, 'c');
    push(&l, 'f');
    sortList(&l);
    int size = transformToTree(&l);
    printf("size : %d\n", size);
    prefix(l, '\0', 0);
    return 0;
}