#ifndef BTREE_H
#define BTREE_H

#define ORDER 3

typedef struct BTreeNode {
    int *keys;
    struct BTreeNode **children;
    int n;
    int leaf;
    char **values;
} BTreeNode;

BTreeNode* create_node(int leaf);
int btree_search(BTreeNode *root, int key, char **value);
void btree_insert(BTreeNode **root, int key, char *value);
void btree_traverse(BTreeNode *root);

#endif