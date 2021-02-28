#include "list.h"
#include "huffman_tree.h"

int main()
{
    FILE * fin = fopen("enwik9", "r");
    FILE * fout = fopen("res.bin", "wb");
    encode(fin, fout);
    fclose(fin);
    fclose(fout);
    fin = fopen("res.bin", "rb");
    fout = fopen("huffman_tree.c.txt", "w");
    decode(fin, fout);
    fclose(fin);
    fclose(fout);
    return 0;
}