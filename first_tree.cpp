#include <stdio.h>
#include <stdlib.h>
#include <TXLib.h>

typedef struct TreeNode{
    int data = 0;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode * operator_new(int data);
TreeNode * add_node(int data, TreeNode *tree);
void tree_print(TreeNode * node_t);
int delete_tree_node(TreeNode * tree);

int main()
{
    TreeNode * tree = operator_new(10);

    add_node(5, tree);
    add_node(15, tree);
    add_node(12, tree);
    add_node(20, tree);
    add_node(11, tree);

    tree_print(tree);
    return 0;
}

TreeNode * operator_new(int data)
{
    TreeNode* new_node = (TreeNode*)calloc(sizeof(TreeNode), 1);

    if (new_node == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    new_node->data = data;
    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
}

TreeNode * add_node(int data, TreeNode * tree)
{
    if (tree == NULL)
    {
        tree = operator_new(data);
    }

    else  if (data < tree->data)
    {
        tree->left = add_node(data, tree->left);
    }

    else
    {
        tree->right = add_node(data, tree->right);
    }

  return(tree);
}

int delete_tree_node(TreeNode * tree)
{
    if(tree == NULL)
    {
        free(tree);
        return 0;
    }

    tree->left = NULL;
    tree->right = NULL;
    tree->data = 0;

    free(tree);

    return 1;
}

void tree_print(TreeNode * node_t)
{
    if(node_t == NULL)
    {
        txSetConsoleAttr(FOREGROUND_YELLOW | BACKGROUND_BLACK);
        printf(" nil ");
        txSetConsoleAttr(FOREGROUND_LIGHTGRAY | BACKGROUND_BLACK);
        return;
    }

    if(node_t != NULL)
    {
        txSetConsoleAttr(FOREGROUND_RED | BACKGROUND_BLACK);
        printf("(");
        txSetConsoleAttr(FOREGROUND_LIGHTGRAY | BACKGROUND_BLACK);

        tree_print(node_t->left);
        printf("%d", node_t->data);
        tree_print(node_t->right);

        txSetConsoleAttr(FOREGROUND_GREEN | BACKGROUND_BLACK);
        printf(")");
        txSetConsoleAttr(FOREGROUND_LIGHTGRAY | BACKGROUND_BLACK);
    }
}
