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

void prefix(tree t, unsigned long current, int depth, character * characters, int * pos, unsigned long *nb_bits) 
{
    if(t->character != '\0')
    {
        characters[*pos].character = t->character;
        characters[*pos].encoding = current;
        characters[*pos].size = depth;
        *pos = *pos + 1;
        *nb_bits += depth * t->occurences;
    }
    else
    {
        depth++;
        prefix(t->left, current << 1, depth, characters, pos, nb_bits);
        current <<= 1;
        current |= 1;
        prefix(t->right, current, depth, characters, pos, nb_bits);
    }
}

void freeTree(tree t)
{
    if(t->character == '\0')
    {
        freeTree(t->left);
        freeTree(t->right);
    }
    free(t);
}

character * writeTreeToArray(tree t, int size, unsigned long *nb_bits)
{
    character * characters = (character *) malloc(sizeof(character) * size);
    int pos = 0;
    prefix(t, '\0', 0, characters, &pos, nb_bits);
    freeTree(t);
    return characters;
}

void writeDataToFile(character * characters, int nb_characters, unsigned long nb_bits, FILE * file_data)
{
    int i;
    fwrite(&nb_characters, sizeof(int), 1, file_data);
    fwrite(&nb_bits, sizeof(unsigned long), 1, file_data);
    for(i = 0; i < nb_characters; i++)
    {
        fwrite(&(characters[i].character), sizeof(char), 1, file_data);
        fwrite(&(characters[i].encoding), sizeof(unsigned long), 1, file_data);
        fwrite(&(characters[i].size), sizeof(int), 1, file_data);
    }
}

void insertElement(character c, tree t)
{
    int i;
    element * cur = t;
    unsigned long tmp, mask = 1;
    mask <<= c.size - 1;
    for(i = 0; i < c.size; i++)
    {
        tmp = c.encoding & mask;
        c.encoding <<= 1;
        if(tmp == 0)
        {
            if(cur->left == NULL)
                cur->left = createElement('\0');
            cur = cur->left;
        } 
        else {
            if(cur->right == NULL)
                cur->right = createElement('\0');
            cur = cur->right;
        }
    }
    cur->character = c.character;
}

tree readData(int * nb_characters, unsigned long * nb_bits, FILE * file_data) 
{
    character * characters;
    int i;
    fread(nb_characters, sizeof(int), 1, file_data);
    fread(nb_bits, sizeof(unsigned long), 1, file_data);
    characters = (character *) malloc(sizeof(character) * (*nb_characters));
    tree t = createElement('\0');
    for(i = 0; i < *nb_characters; i++)
    {
        fread(&(characters[i].character), sizeof(char), 1, file_data);
        fread(&(characters[i].encoding), sizeof(unsigned long), 1, file_data);
        fread(&(characters[i].size), sizeof(int), 1, file_data);
        insertElement(characters[i], t);
    }
    free(characters);
    return t;
}

void decodeFromFile(FILE * file_in, FILE * file_out, tree t, unsigned long nb_bits)
{
    int j, i = 0;
    unsigned long msb, mask = 1, buffer;
    int read_in_buffer = 0;
    int size_unsigned_long = sizeof(unsigned long) * 8;
    int nb_reads = nb_bits / size_unsigned_long;
    unsigned long nb_bits_read = 0;
    if(nb_bits % size_unsigned_long != 0)
        nb_reads++;
    element * cur = t;
    mask <<= size_unsigned_long - 1; // used to get only most significant bit
    for(i = 0; i < nb_reads; i++)
    {
        fread(& buffer,sizeof(unsigned long),1,file_in);
        j = 0;
        while(nb_bits_read < nb_bits && j < size_unsigned_long)
        {
            if (cur->character != '\0')
            {
                fprintf(file_out, "%c", cur->character);
                cur = t;
            }
            msb = mask & buffer;
            if(msb == 0)
                cur = cur->left;
            else
                cur = cur->right;
            nb_bits_read++;
            j++;
            buffer <<= 1;
        }
    }
}

void decode(FILE * file_in, FILE * file_out)
{
    int nb_characters;
    unsigned long nb_bits;
    tree t = readData(&nb_characters, &nb_bits, file_in);
    decodeFromFile(file_in, file_out, t, nb_bits);
    freeTree(t);
}

void encode(FILE * file_in, FILE * file_out)
{
    int nb_characters;
    unsigned long nb_bits = 0;
    character * codes = getEncodeingCodes(file_in, &nb_characters, &nb_bits);
    writeDataToFile(codes, nb_characters, nb_bits, file_out);
    encodeToFile(file_in, file_out, codes, nb_characters);
    free(codes);
}

void encodeToFile(FILE * file_in, FILE * file_out, character * characters, int nb_characters)
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
            fwrite(&buffer, sizeof(unsigned long), 1, file_out);
            nb_writes++;
            buffer = 0;
            actual_size_buffer = 0;
        }
        actual_size_buffer += character.size;
        buffer <<= character.size;
        buffer |= character.encoding;
    } while (c != EOF);
    if(actual_size_buffer > 0) 
    {
        buffer <<= size_unsigned_long - actual_size_buffer;
        fwrite((const void *) & buffer, sizeof(unsigned long), 1, file_out);
    }
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

character * getEncodeingCodes(FILE * file_in, int *nb_characters, unsigned long *nb_bits)
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
    return writeTreeToArray(l, *nb_characters, nb_bits);
}