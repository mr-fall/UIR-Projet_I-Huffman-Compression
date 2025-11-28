#ifndef MINHEAP_H
#define MINHEAP_H

#include "huffman.h"

typedef struct MinHeap {
    int capacity;
    int size;
    HuffmanNode **array;
} MinHeap;

MinHeap *create_minheap(int capacity);
void minheap_free(MinHeap *h);
void minheap_insert(MinHeap *h, HuffmanNode *node);
HuffmanNode *minheap_extract_min(MinHeap *h);
void build_minheap_from_table(MinHeap *h, unsigned long freq_table[]);

#endif