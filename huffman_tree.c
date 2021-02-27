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
            *l = cur;
            cur = cur->next;
        }
    }
    return size;
}

void prefix(tree t, unsigned long current, int depth, character * characters, int * pos) 
{
    if(t->character != '\0')
    {
        characters[*pos].character = t->character;
        characters[*pos].encoding = current;
        characters[*pos].size = depth;
        *pos = *pos + 1;
    }
    else
    {
        depth++;
        prefix(t->left, current << 1, depth, characters, pos);
        current <<= 1;
        current |= 1;
        prefix(t->right, current, depth, characters, pos);
    }
}

character * writeTreeToArray(tree t, int size)
{
    character * characters = (character *) malloc(sizeof(character) * size);
    int pos = 0;
    prefix(t, '\0', 0, characters, &pos);
    return characters;
}

void writeDataToFile(character * characters, int nb_characters, unsigned long nb_bits, FILE * file_data)
{
    int i;
    fprintf(file_data, "%d\t%lu\n", nb_characters, nb_bits);
    for(i = 0; i < nb_characters; i++)
    {
        fprintf(file_data, "%c\t%lu\t%d\n", characters[i].character, characters[i].encoding, characters[i].size);
    }
}

void encode(FILE * file_in, FILE * file_out, FILE * file_data)
{
    int nb_characters;
    character * codes = getEncodeingCodes(file_in, &nb_characters);
    unsigned long nb_bits = encodeToFile(file_in, file_out, codes, nb_characters);
    writeDataToFile(codes, nb_characters, nb_bits, file_data);
}

unsigned long encodeToFile(FILE * file_in, FILE * file_out, character * characters, int nb_characters)
{
    unsigned long buffer = 0;
    unsigned long nb_bits = 0;
    int size_unsigned_long = sizeof(unsigned long) * 8;
    character character;
    int to_add, actual_size_buffer = 0;
    int nb_writes = 0;
    char c;
    unsigned long tmp;
    do {
        c = fgetc(file_in);
        character = getCharacter(c, characters, nb_characters);
        if(actual_size_buffer + character.size >= size_unsigned_long)           // il faut ecrire dans le fichier
        {
            to_add = size_unsigned_long - actual_size_buffer;
            buffer <<= to_add;
            tmp = character.encoding >> (character.size - to_add);
            character.size -= to_add;
            character.encoding <<= size_unsigned_long - character.size;
            character.encoding >>= size_unsigned_long - character.size;
            buffer |= tmp;
            fwrite((const void*) & buffer,sizeof(unsigned long),1,file_out);
            nb_writes++;
            buffer = 0;
            actual_size_buffer = 0;
            nb_bits += size_unsigned_long;
        }
        actual_size_buffer += character.size;
        buffer <<= character.size;
        buffer |= character.encoding;
    } while (c != EOF);
    if(actual_size_buffer > 0) 
    {
        fwrite((const void *) & buffer, sizeof(unsigned long), 1, file_out);
        nb_bits += actual_size_buffer;
    }
    return nb_bits;
}

character getCharacter(char c, character * characters, int nb_characters)
{
    int i = 0;
    for(i; i < nb_characters; i++) 
    {
        if(characters[i].character == c) {
            return characters[i];
        }
    }
}

character * getEncodeingCodes(FILE * file_in, int *nb_characters)
{
    char c;
    list l = NULL;
    int size;
    do {
        c = fgetc (file_in);
        push(&l, c);
    } while (c != EOF);
    rewind(file_in);
    sortList(&l);
    *nb_characters = transformToTree(&l);
    return writeTreeToArray(l, *nb_characters);
}