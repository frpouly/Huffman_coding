#include "list.h"
#include "huffman_tree.h"

int main()
{
    FILE * fin = fopen("liste_francais.txt", "r");
    FILE * fout = fopen("res2.bin", "wb");
    FILE * fdata = fopen("data.dat", "w");
    encode(fin, fout, fdata);
    fclose(fin);
    fclose(fout);
    fclose(fdata);
    fin = fopen("res2.bin", "rb");
    fdata = fopen("data.dat", "r");
    fout = fopen("liste_francais2.text", "w");
    decode(fin, fout, fdata);
    return 0;
}