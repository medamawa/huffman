#include <stdio.h>
#include "../include/encode.h"
#include "../include/write.h"

char count_string(char *s);

extern int symbol_count[NSYMBOLS];

void write_zip(FILE *input_fp, FILE *zip_fp, char **code_list) {
    int estimated_bit = 0;

    // char_numを書き込む
    unsigned char char_num = 0;
    for (int i = 0; i < NSYMBOLS; i++) {
        if (code_list[i][0] == '0' || code_list[i][0] == '1') {
            ++char_num;
        }
    }
    fwrite(&char_num, sizeof(char), 1, zip_fp);

    // symbol, code_len, codeを書き込む
    for (int i = 0; i < NSYMBOLS; i++) {
        if (code_list[i][0] == '0' || code_list[i][0] == '1') {
            char symbol = i;
            char code_len = count_string(code_list[i]);
            fwrite(&symbol, sizeof(char), 1, zip_fp);
            fwrite(&code_len, sizeof(char), 1, zip_fp);
            fwrite(code_list[i], sizeof(char), code_len, zip_fp);
            estimated_bit += symbol_count[i] * code_len;
        }
    }

    printf("\n#######\n");
    printf("estimated byte: %d\n", estimated_bit / 8);
    printf("#######\n\n");

    // txtをcodeに変換して書き込む
    char c;
    char tmp_code = 0;
    char tmp_code_len = 0;
    fread(&c, sizeof(char), 1, input_fp);
    for (size_t rsize = 1; rsize != 0; rsize = fread(&c, sizeof(char), 1, input_fp)) {
        char *code = code_list[(unsigned char)c];
        for (int i = 0; code[i] != '\0'; i++) {
            if (code[i] == '0') {
                tmp_code = tmp_code << 1;
                ++tmp_code_len;
            } else if (code[i] == '1') {
                tmp_code = (tmp_code << 1) + 1;
                ++tmp_code_len;
            }

            if (tmp_code_len % 8 == 7) {
                fwrite(&tmp_code, sizeof(char), 1, zip_fp);
                tmp_code = 0;
                tmp_code_len = 0;
            }
        }
    }
}

char count_string(char *s) {
    char count = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        ++count;
    }
    return count;
}