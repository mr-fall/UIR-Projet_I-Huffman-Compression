#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

#define ALPHABET 256

typedef struct HuffmanNode {
    unsigned char ch;
    unsigned long freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

HuffmanNode *create_node(unsigned char ch, unsigned long freq);
void free_tree(HuffmanNode *root);
int build_frequency_table(const char *input_file, unsigned long freq_table[ALPHABET]);
HuffmanNode *build_huffman_tree(unsigned long freq_table[ALPHABET]);
void generate_codes(HuffmanNode *root, char **codes);
void free_codes(char **codes);
int write_codes_file(const char *codes_file, char **codes);
int read_codes_file_and_rebuild(const char *codes_file, HuffmanNode **out_root);
int encode_file(const char *input_file, const char *output_file, char **codes);
int decode_file(const char *compressed_file, const char *meta_file, const char *codes_file, const char *output_file);

#endif