#include "list.h"
#include "huffman_tree.h"

int main()
{
    FILE * fin = fopen("liste_francais.txt", "r");
    FILE * fout = fopen("res2.bin", "wb");
    FILE * fdata = fopen("data.dat", "w");
    encode(fin, fout, fdata);
    return 0;
}