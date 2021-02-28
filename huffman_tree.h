#ifndef _HUFFMAN_TREE_
#define _HUFFMAN_TREE_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef element *tree;

typedef struct character
{
    char character;
    unsigned long encoding;
    int size;
} character;

int transformToTree(list *l);
character * writeTreeToArray(tree t, int size, unsigned long *nb_bits);
void prefix(tree t, unsigned long current, int depth, character * characters, int * pos, unsigned long * nb_bits);
void encode(FILE * fin, FILE * fout);
void encodeToFile(FILE * file_in, FILE * file_out, character * characters, int nb_characters);
character * getEncodeingCodes(FILE * file_in, int *nb_characters, unsigned long * nb_bits);
character getCharacter(char c, character * characters, int nb_characters);
void writeDataToFile(character * characters, int nb_characters, unsigned long nb_bits, FILE * file_data);
tree readData(int * nb_characters, unsigned long * nb_bits, FILE * file_data);
void insertElement(character c, tree t);
void decode(FILE * file_in, FILE * file_out);

#endif