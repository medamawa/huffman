#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encode.h"
#include "../include/write.h"
#include "../include/decode.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n",argv[0]);
        exit(1);
    }
    
    // 構文解析
    printf("######## structure ########\n");
    Node *root = encode(argv[1]);
    char *code_list[NSYMBOLS];
    for (int i = 0; i < NSYMBOLS; i++) code_list[i] = malloc(sizeof(char) * NSYMBOLS);                  // free()をどこかで,,, 誰か,,,,,,

    traverse_tree(0, root, code_list);


    // .zip作成
    printf("\n\n######## zip ########\n");
    FILE *input_fp;
    if ((input_fp = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]); 
        return EXIT_FAILURE;
    }

    char *basename;
    basename = strtok(argv[1], ".");
    char *zip_filename = strcat(basename, ".zip");
    FILE *zip_fp;
    if ((zip_fp = fopen(zip_filename, "wb")) == NULL) {
        perror(zip_filename); 
        return EXIT_FAILURE;
    }

    write_zip(input_fp, zip_fp, code_list);

    fclose(input_fp);
    fclose(zip_fp);


    // 解凍
    printf("\n\n######## unzip ########\n");
    FILE *d_zip_fp;
    if ((d_zip_fp = fopen(zip_filename, "rb")) == NULL) {
        perror(zip_filename); 
        return EXIT_FAILURE;
    }


    basename = strtok(basename, ".");
    char *unzip_filename = strcat(basename, "_unzip.txt");
    FILE *unzip_fp;
    if ((unzip_fp = fopen(unzip_filename, "wb")) == NULL) {
        perror(unzip_filename); 
        return EXIT_FAILURE;
    }

    read_zip(d_zip_fp, unzip_fp);

    printf("\n\n######## DONE ########\n\n");

    fclose(d_zip_fp);
    fclose(unzip_fp);
    
    return EXIT_SUCCESS;
}
