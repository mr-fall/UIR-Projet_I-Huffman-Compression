#include <stdlib.h>
#include "minheap.h"

static void swap_nodes(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode *t = *a; *a = *b; *b = t;
}

MinHeap *create_minheap(int capacity) {
    MinHeap *h = (MinHeap*) malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->capacity = capacity > 0 ? capacity : 1;
    h->size = 0;
    h->array = (HuffmanNode**) malloc(sizeof(HuffmanNode*) * h->capacity);
    if (!h->array) { free(h); return NULL; }
    return h;
}

void minheap_free(MinHeap *h) {
    if (!h) return;
    free(h->array);
    free(h);
}

static void heapify_down(MinHeap *h, int idx) {
    int smallest = idx;
    int l = 2*idx + 1;
    int r = 2*idx + 2;
    if (l < h->size && h->array[l]->freq < h->array[smallest]->freq) smallest = l;
    if (r < h->size && h->array[r]->freq < h->array[smallest]->freq) smallest = r;
    if (smallest != idx) {
        swap_nodes(&h->array[smallest], &h->array[idx]);
        heapify_down(h, smallest);
    }
}

static void heapify_up(MinHeap *h, int idx) {
    while (idx && h->array[(idx-1)/2]->freq > h->array[idx]->freq) {
        swap_nodes(&h->array[idx], &h->array[(idx-1)/2]);
        idx = (idx-1)/2;
    }
}

void minheap_insert(MinHeap *h, HuffmanNode *node) {
    if (h->size == h->capacity) {
        int newcap = h->capacity * 2;
        HuffmanNode **tmp = (HuffmanNode**) realloc(h->array, sizeof(HuffmanNode*) * newcap);
        if (!tmp) return;
        h->array = tmp;
        h->capacity = newcap;
    }
    h->array[h->size] = node;
    heapify_up(h, h->size);
    h->size++;
}

HuffmanNode *minheap_extract_min(MinHeap *h) {
    if (h->size == 0) return NULL;
    HuffmanNode *root = h->array[0];
    h->array[0] = h->array[h->size - 1];
    h->size--;
    heapify_down(h, 0);
    return root;
}

void build_minheap_from_table(MinHeap *h, unsigned long freq_table[]) {
    for (int i = 0; i < ALPHABET; ++i) {
        if (freq_table[i] > 0) {
            HuffmanNode *n = (HuffmanNode*) malloc(sizeof(HuffmanNode));
            n->ch = (unsigned char)i;
            n->freq = freq_table[i];
            n->left = n->right = NULL;
            minheap_insert(h, n);
        }
    }
}