#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int key;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode *insert(TreeNode *root, int key) {
    if (!root) {
        TreeNode *new_node = malloc(sizeof(TreeNode));
        new_node->key = key;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}

TreeNode *search(TreeNode *root, int key) {
    if (!root || root->key == key) return root;
    return key < root->key ? search(root->left, key) : search(root->right, key);
}

TreeNode *delete(TreeNode *root, int key) {
    if (!root) return NULL;
    if (key < root->key) root->left = delete(root->left, key);
    else if (key > root->key) root->right = delete(root->right, key);
    else {
        if (!root->left) {
            TreeNode *temp = root->right; 
            free(root); 
            return temp; 
        }
        if (!root->right) { 
            TreeNode *temp = root->left; 
            free(root); 
            return temp; 
        }
        TreeNode *min = root->right;
        while (min->left) min = min->left;
        root->key = min->key;
        root->right = delete(root->right, min->key);
    }
    return root;
}

void free_tree(TreeNode *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    TreeNode *root = NULL;
    char op;
    int key;
    while (scanf(" %c%d", &op, &key) != EOF) {
        if (op == '+') root = insert(root, key);
        else if (op == '-') root = delete(root, key);
        else if (op == '?') printf("%d %s\n", key, search(root, key) ? "yes" : "no");
    }
    free_tree(root);
    return 0;
}