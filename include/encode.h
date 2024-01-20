#pragma once

#define NSYMBOLS 256

typedef struct node Node;
   
// ファイルをエンコードし木のrootへのポインタを返す
Node *encode(const char *filename);
// Treeを走査して表示する
void traverse_tree(const int depth, const Node *root, char **code_list);

