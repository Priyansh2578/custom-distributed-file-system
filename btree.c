#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create new node
BTreeNode* create_node(int leaf) {
    BTreeNode *node = malloc(sizeof(BTreeNode));
    node->keys = malloc(sizeof(int) * (ORDER - 1));
    node->children = malloc(sizeof(BTreeNode*) * ORDER);
    node->values = malloc(sizeof(char*) * (ORDER - 1));
    node->n = 0;
    node->leaf = leaf;
    return node;
}

// Search key in B-Tree
int btree_search(BTreeNode *root, int key, char **value) {
    if (!root) return 0;
    
    int i = 0;
    while (i < root->n && key > root->keys[i]) {
        i++;
    }
    
    if (i < root->n && key == root->keys[i]) {
        if (root->leaf) {
            *value = root->values[i];
            return 1;
        }
        return btree_search(root->children[i + 1], key, value);
    }
    
    if (root->leaf) return 0;
    
    return btree_search(root->children[i], key, value);
}

// Split child (helper for insert)
void split_child(BTreeNode *parent, int index, BTreeNode *child) {
    BTreeNode *new_child = create_node(child->leaf);
    new_child->n = ORDER - 1;
    
    for (int j = 0; j < ORDER - 1; j++) {
        new_child->keys[j] = child->keys[j + ORDER];
        if (child->leaf) {
            new_child->values[j] = child->values[j + ORDER];
        }
    }
    
    if (!child->leaf) {
        for (int j = 0; j < ORDER; j++) {
            new_child->children[j] = child->children[j + ORDER];
        }
    }
    
    child->n = ORDER - 1;
    
    for (int j = parent->n; j > index; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = new_child;
    
    for (int j = parent->n - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
        if (parent->leaf) {
            parent->values[j + 1] = parent->values[j];
        }
    }
    parent->keys[index] = child->keys[ORDER - 1];
    parent->n++;
}

// Insert into non-full node
void insert_non_full(BTreeNode *node, int key, char *value) {
    int i = node->n - 1;
    
    if (node->leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = strdup(value);
        node->n++;
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        
        if (node->children[i]->n == 2 * ORDER - 1) {
            split_child(node, i, node->children[i]);
            if (node->keys[i] < key) {
                i++;
            }
        }
        insert_non_full(node->children[i], key, value);
    }
}

// Insert into B-Tree
void btree_insert(BTreeNode **root, int key, char *value) {
    BTreeNode *r = *root;
    
    if (r->n == 2 * ORDER - 1) {
        BTreeNode *new_root = create_node(0);
        new_root->children[0] = r;
        split_child(new_root, 0, r);
        insert_non_full(new_root, key, value);
        *root = new_root;
    } else {
        insert_non_full(r, key, value);
    }
}

// Traverse (print all keys in order)
void btree_traverse(BTreeNode *root) {
    if (!root) return;
    
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->leaf) {
            btree_traverse(root->children[i]);
        }
        printf("%d ", root->keys[i]);
    }
    if (!root->leaf) {
        btree_traverse(root->children[i]);
    }
}