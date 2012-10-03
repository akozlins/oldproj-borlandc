#ifndef __Hufftree_h
#define __Hufftree_h

#include "Filework.h"
#include <string.h>
#include <conio.h>

#define n_bits 8                    // 4 or 8
#define n_nodes 511                 // = 2*n_laters-1
#define n_laters 256                // = 2^n_bits

typedef unsigned long ulong;

class Tree {
public:
    ulong u_size;                   //uncompressed size in bloc by n_bits
    ulong c_size;                   //compressed size in bytes
    struct Cod {
        ulong cod;                  //later cod
        int size;                   //cod size (in bits)
    } codes[n_laters];
    int index[n_nodes];
    struct Node {
        int left;
        int right;
        int up;
        ulong weight;
    } nodes[n_nodes];
    int last;
    int first;
    int last_node;
private:
    void FillCodes(int, ulong, int);
public:
    Tree();
    void FillTree(File *);
    void Sort(void);
    void PrintInfo();
    ~Tree();
};

#endif