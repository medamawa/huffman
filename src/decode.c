#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/decode.h"

#define NSYMBOLS 256

typedef struct node{
    int symbol;
    int count;
    struct node *left;
    struct node *right;
} Node;

static Node *create_node_decode(int symbol, int count, Node *left, Node *right);

static void build_tree_decode(Node* root, char **code_list);

static void traverse_tree_decode(const int depth, const Node *np);


void read_zip(FILE *input_fp, FILE *unzip_fp) {
    // char_numを読み込む
    unsigned char char_num;
    fread(&char_num, sizeof(char), 1, input_fp);

    // symbol, code_len, codeを読み込む
    char symbol;
    char code_len;
    char code[NSYMBOLS];
    char *code_list[NSYMBOLS];
    for (int i = 0; i < NSYMBOLS; i++) code_list[i] = malloc(sizeof(char) * NSYMBOLS);

    for (int i = 0; i < char_num; i++) {
        fread(&symbol, sizeof(char), 1, input_fp);
        fread(&code_len, sizeof(char), 1, input_fp);
        fread(code, sizeof(char), code_len, input_fp);

        code[(int)code_len] = '\0';
        strcpy(code_list[(unsigned char)symbol], code);
    }

    // ハフマン木を構成する
    Node *root = create_node_decode(-1, 0, NULL, NULL);
    build_tree_decode(root, code_list);

    // ハフマン木を表示する
    traverse_tree_decode(0, root);


    // codeを読み込んでtxtに変換して書き込む
    unsigned char c;
    char tmp_code;
    Node *node = root;
    fread(&c, sizeof(char), 1, input_fp);
    for (size_t rsize = 1; rsize != 0; rsize = fread(&c, sizeof(char), 1, input_fp)) {
        for (int i = 0; i < 8; i++) {
            if ((c >> (7 - i)) & 1) {
                tmp_code = 1;
            } else {
                tmp_code = 0;
            }
            
            // 葉ノードに到達したら出力して、rootに戻る
            if (node->symbol != -1) {
                fwrite(&node->symbol, sizeof(char), 1, unzip_fp);
                node = root;
            }

            // 0なら左、1なら右に進む
            if (tmp_code == 0 && node->left != NULL) {
                node = node->left;
            } else if (tmp_code == 1 && node->right != NULL) {
                node = node->right;
            }
        }
    }
}

static Node *create_node_decode(int symbol, int count, Node *left, Node *right) {
    Node *ret = (Node *)malloc(sizeof(Node));
    *ret = (Node){ .symbol = symbol, .count = count, .left = left, .right = right};
    return ret;
}

static void build_tree_decode(Node* root, char **code_list) {
    for (int i = 0; i < NSYMBOLS; ++i) {
        // codeが割り当てられていない文字はスキップ
        if (code_list[i][0] != '0' && code_list[i][0] != '1') continue;

        Node *node = root;

        for (int j = 0; code_list[i][j] != '\0'; ++j) {
            if (code_list[i][j] == '0') {
                if (node->left == NULL) {
                    node->left = create_node_decode(-1, 0, NULL, NULL);
                }
                node = node->left;
            } else if (code_list[i][j] == '1') {
                if (node->right == NULL) {
                    node->right = create_node_decode(-1, 0, NULL, NULL);
                }
                node = node->right;
            }
        }
        node->symbol = i;
    }
}

static void traverse_tree_decode(const int depth, const Node *np) {
    static char code[NSYMBOLS];     // 現在のノードまでの符号を保持する
    
    if (np == NULL) return;

    if (np->left == NULL && np->right == NULL) {
        code[depth] = '\0';     // 終端文字
        if (np->symbol == '\n') {
            printf("[\\n]: %s\n", code);
        } else {
            printf("[%c]: %s\n", np->symbol, code);
        }
    } else {
        if (depth == 0) printf("[root]\n|\n");
        printf("+---");
        
        if (np->left != NULL) {
            code[depth] = '0';
            traverse_tree_decode(depth + 1, np->left);
        }
        if (np->right != NULL) {
            for (int i = 0; i < depth; ++i) printf("    ");
            printf("|\n");
            for (int i = 0; i < depth; ++i) printf("    ");
            printf("+---");

            code[depth] = '1';
            traverse_tree_decode(depth + 1, np->right);
        }
    }
}
