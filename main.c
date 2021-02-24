#include "list.h"

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
    displayList(l, stdout);
    sortList(&l);
    displayList(l, stdout);
    destroyList(l);
    return 0;
}