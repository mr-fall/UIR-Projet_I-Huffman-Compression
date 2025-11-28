#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

static void usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s encode <inputFile> <outputCompressed> <codesFile>\n", prog);
    printf("  %s decode <compressedFile> <metaFile> <codesFile> <outputFile>\n", prog);
}

int main(int argc, char **argv) {
    if (argc < 2) { usage(argv[0]); return 1; }
    if (strcmp(argv[1], "encode") == 0) {
        if (argc != 5) { usage(argv[0]); return 1; }
        const char *input = argv[2];
        const char *outcomp = argv[3];
        const char *codes = argv[4];
        unsigned long freq_table[ALPHABET];
        if (build_frequency_table(input, freq_table) != 0) { fprintf(stderr, "Error reading input file '%s'\n", input); return 1; }
        HuffmanNode *root = build_huffman_tree(freq_table);
        if (!root) { fprintf(stderr, "No data to encode.\n"); return 1; }
        char *codes_arr[ALPHABET];
        generate_codes(root, codes_arr);
        if (write_codes_file(codes, codes_arr) != 0) { fprintf(stderr, "Error writing codes to '%s'\n", codes); free_codes(codes_arr); free_tree(root); return 1; }
        if (encode_file(input, outcomp, codes_arr) != 0) { fprintf(stderr, "Error encoding file.\n"); free_codes(codes_arr); free_tree(root); return 1; }
        printf("Encoding done: compressed='%s', codes='%s', meta='%s.meta'\n", outcomp, codes, outcomp);
        free_codes(codes_arr);
        free_tree(root);
        return 0;
    } else if (strcmp(argv[1], "decode") == 0) {
        if (argc != 6) { usage(argv[0]); return 1; }
        const char *comp = argv[2];
        const char *meta = argv[3];
        const char *codes = argv[4];
        const char *out = argv[5];
        if (decode_file(comp, meta, codes, out) != 0) { fprintf(stderr, "Error decoding file.\n"); return 1; }
        printf("Decoded to '%s'\n", out);
        return 0;
    } else {
        usage(argv[0]);
        return 1;
    }
}