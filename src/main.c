#include <stdio.h>
#include <stdlib.h>
#include "../include/encode.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n",argv[0]);
        exit(1);
    }
    
    Node *root = encode(argv[1]);
    char *code_list[256];
    traverse_tree(0, root, code_list);

    printf("%c: %s\n", ' ', code_list[' ']);
    
    return EXIT_SUCCESS;
}
