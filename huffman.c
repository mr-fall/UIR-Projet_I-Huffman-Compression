#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "minheap.h"

HuffmanNode *create_node(unsigned char ch, unsigned long freq) {
    HuffmanNode *n = (HuffmanNode*) malloc(sizeof(HuffmanNode));
    if (!n) return NULL;
    n->ch = ch;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}

void free_tree(HuffmanNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int build_frequency_table(const char *input_file, unsigned long freq_table[ALPHABET]) {
    FILE *f = fopen(input_file, "rb");
    if (!f) return -1;
    for (int i = 0; i < ALPHABET; ++i) freq_table[i] = 0;
    int c;
    while ((c = fgetc(f)) != EOF) freq_table[(unsigned char)c]++;
    fclose(f);
    return 0;
}

HuffmanNode *build_huffman_tree(unsigned long freq_table[ALPHABET]) {
    int distinct = 0;
    for (int i = 0; i < ALPHABET; ++i) if (freq_table[i] > 0) distinct++;
    if (distinct == 0) return NULL;
    MinHeap *h = create_minheap(distinct);
    if (!h) return NULL;
    build_minheap_from_table(h, freq_table);
    if (h->size == 1) {
        HuffmanNode *only = minheap_extract_min(h);
        HuffmanNode *root = create_node(0, only->freq);
        root->left = only;
        root->right = NULL;
        minheap_free(h);
        return root;
    }
    while (h->size > 1) {
        HuffmanNode *x = minheap_extract_min(h);
        HuffmanNode *y = minheap_extract_min(h);
        HuffmanNode *z = create_node(0, x->freq + y->freq);
        z->left = x;
        z->right = y;
        minheap_insert(h, z);
    }
    HuffmanNode *root = minheap_extract_min(h);
    minheap_free(h);
    return root;
}

static char *my_strdup(const char *s) {
    size_t l = strlen(s) + 1;
    char *p = (char*) malloc(l);
    if (!p) return NULL;
    memcpy(p, s, l);
    return p;
}

static void generate_codes_rec(HuffmanNode *root, char *buf, int depth, char **codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        buf[depth] = '\0';
        codes[(unsigned char)root->ch] = my_strdup(buf);
        return;
    }
    if (root->left) {
        buf[depth] = '0';
        generate_codes_rec(root->left, buf, depth+1, codes);
    }
    if (root->right) {
        buf[depth] = '1';
        generate_codes_rec(root->right, buf, depth+1, codes);
    }
}

void generate_codes(HuffmanNode *root, char **codes) {
    for (int i = 0; i < ALPHABET; ++i) codes[i] = NULL;
    if (!root) return;
    char buf[512];
    if (root->left && !root->right && !root->left->left && !root->left->right) {
        codes[(unsigned char)root->left->ch] = my_strdup("0");
        return;
    }
    generate_codes_rec(root, buf, 0, codes);
}

void free_codes(char **codes) {
    if (!codes) return;
    for (int i = 0; i < ALPHABET; ++i) if (codes[i]) free(codes[i]);
}

int write_codes_file(const char *codes_file, char **codes) {
    FILE *f = fopen(codes_file, "w");
    if (!f) return -1;
    for (int i = 0; i < ALPHABET; ++i) {
        if (codes[i]) fprintf(f, "%d\t%s\n", i, codes[i]);
    }
    fclose(f);
    return 0;
}

int read_codes_file_and_rebuild(const char *codes_file, HuffmanNode **out_root) {
    FILE *f = fopen(codes_file, "r");
    if (!f) return -1;
    HuffmanNode *root = create_node(0,0);
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        char *tab = strchr(line, '\t');
        if (!tab) continue;
        *tab = '\0';
        int val = atoi(line);
        char *code = tab + 1;
        char *nl = strchr(code, '\n'); if (nl) *nl = '\0';
        HuffmanNode *cur = root;
        for (size_t i = 0; i < strlen(code); ++i) {
            if (code[i] == '0') {
                if (!cur->left) cur->left = create_node(0,0);
                cur = cur->left;
            } else {
                if (!cur->right) cur->right = create_node(0,0);
                cur = cur->right;
            }
        }
        cur->ch = (unsigned char) val;
    }
    fclose(f);
    *out_root = root;
    return 0;
}

static int write_bits_from_string(FILE *out, const char *bitstr, unsigned char *bitbuf, int *bitcount) {
    for (size_t i = 0; i < strlen(bitstr); ++i) {
        *bitbuf = (*bitbuf << 1) | (bitstr[i] == '1' ? 1 : 0);
        (*bitcount)++;
        if (*bitcount == 8) {
            if (fputc(*bitbuf, out) == EOF) return -1;
            *bitbuf = 0;
            *bitcount = 0;
        }
    }
    return 0;
}

int encode_file(const char *input_file, const char *output_file, char **codes) {
    FILE *in = fopen(input_file, "rb");
    if (!in) return -1;
    FILE *out = fopen(output_file, "wb");
    if (!out) { fclose(in); return -1; }
    unsigned char bitbuf = 0;
    int bitcount = 0;
    int c;
    while ((c = fgetc(in)) != EOF) {
        unsigned char uc = (unsigned char)c;
        if (!codes[uc]) { fclose(in); fclose(out); return -2; }
        if (write_bits_from_string(out, codes[uc], &bitbuf, &bitcount) != 0) { fclose(in); fclose(out); return -1; }
    }
    int meaningful_bits_in_last = (bitcount == 0) ? 8 : bitcount;
    if (bitcount > 0) {
        bitbuf = bitbuf << (8 - bitcount);
        if (fputc(bitbuf, out) == EOF) { fclose(in); fclose(out); return -1; }
    }
    fclose(in);
    fclose(out);
    char meta_name[512];
    snprintf(meta_name, sizeof(meta_name), "%s.meta", output_file);
    FILE *meta = fopen(meta_name, "w");
    if (!meta) return -1;
    fprintf(meta, "%d\n", meaningful_bits_in_last);
    fclose(meta);
    return 0;
}

int decode_file(const char *compressed_file, const char *meta_file, const char *codes_file, const char *output_file) {
    HuffmanNode *root = NULL;
    if (read_codes_file_and_rebuild(codes_file, &root) != 0) return -1;
    FILE *meta = fopen(meta_file, "r");
    if (!meta) { free_tree(root); return -1; }
    int last_bits = 8;
    if (fscanf(meta, "%d", &last_bits) != 1) last_bits = 8;
    fclose(meta);
    FILE *in = fopen(compressed_file, "rb");
    if (!in) { free_tree(root); return -1; }
    FILE *out = fopen(output_file, "wb");
    if (!out) { fclose(in); free_tree(root); return -1; }
    if (fseek(in, 0, SEEK_END) != 0) { fclose(in); fclose(out); free_tree(root); return -1; }
    long file_size = ftell(in);
    rewind(in);
    long index = 0;
    int byte;
    HuffmanNode *cur = root;
    while ((byte = fgetc(in)) != EOF) {
        int bits_to_read = 8;
        if (index == file_size - 1) bits_to_read = last_bits;
        for (int b = 7; b >= 8 - bits_to_read; --b) {
            int bit = (byte >> b) & 1;
            cur = (bit == 0) ? cur->left : cur->right;
            if (!cur) { fclose(in); fclose(out); free_tree(root); return -2; }
            if (!cur->left && !cur->right) {
                fputc(cur->ch, out);
                cur = root;
            }
        }
        index++;
    }
    fclose(in);
    fclose(out);
    free_tree(root);
    return 0;
}
