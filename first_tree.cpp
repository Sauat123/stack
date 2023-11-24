#include <stdio.h>
#include <stdlib.h>
#include <TXLib.h>

const int MULTIPLIER = 2;
const int START_SIZE = 8;

typedef struct TreeNode{
    const char * data = 0;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode * operator_new(const char * data);
TreeNode * add_node(TreeNode * tree);

void go_on_tree(TreeNode * tree);
void tree_print(TreeNode * node_t);
int delete_tree_node(TreeNode * tree);

int analyse_answers(TreeNode * node_t);
int scan_answers();

const char ** scan_tree_file(char * text, const char ** pointers_to_nodes, const int symbols, int file_size);
TreeNode * build_tree(const char ** pointers_to_nodes, TreeNode * tree_node, int * index, int symbols);

char * my_scanf();
int count_text_symbols(const char * text, int file_size);
int get_file_size(FILE * fp);

int main()
{
    FILE * fp_read_tree = fopen("tree_data.txt", "rb");
    if(fp_read_tree == NULL)
    {
        fprintf(stderr, "Can't open tree_data.txt file");
        exit(EXIT_FAILURE);
    }

    int file_size = get_file_size(fp_read_tree);

    char * text = (char*)calloc(file_size, sizeof(char));
    fread(text, sizeof(char), file_size, fp_read_tree);

    int symbols = count_text_symbols(text, file_size);
    printf("symbols = %d\n", symbols);
    const char ** pointers_to_nodes = (const char**)calloc(symbols, sizeof(char));
    pointers_to_nodes = scan_tree_file(text, pointers_to_nodes, symbols, file_size);

    TreeNode * tree = (TreeNode*)calloc(1, sizeof(TreeNode));
    int index = 0;
    tree = build_tree(pointers_to_nodes, tree, &index, symbols);

    tree_print(tree);
    return 0;
}

TreeNode * operator_new(const char * data)
{
    TreeNode* new_node = (TreeNode*)calloc(sizeof(TreeNode), 1);

    if (new_node == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    new_node->data = strdup(data);
    new_node->right = NULL;
    new_node->left = NULL;

    printf("assignment data - %s\n", new_node->data);
    return new_node;
}

TreeNode * add_node(TreeNode * tree)
{
    printf("Who is this???\n");

    const char* differ = (const char*)calloc(START_SIZE * 2, sizeof(char));
    const char* new_person = (const char*)calloc(START_SIZE * 2, sizeof(char));
    new_person = my_scanf();

    printf("How does %s differ from %s", tree->data, new_person);

    differ = my_scanf();

    tree = operator_new(differ);
    tree->right = operator_new(tree->data);
    tree->left = operator_new(new_person);
    return tree;
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
        printf(" nil ");
        return;
    }

    if(node_t != NULL)
    {
        txSetConsoleAttr(FOREGROUND_RED | BACKGROUND_BLACK);
        printf("(");
        txSetConsoleAttr(FOREGROUND_LIGHTGRAY | BACKGROUND_BLACK);

        txSetConsoleAttr(FOREGROUND_YELLOW | BACKGROUND_BLACK);
        printf("%s", node_t->data);
        txSetConsoleAttr(FOREGROUND_LIGHTGRAY | BACKGROUND_BLACK);

        tree_print(node_t->left);
        tree_print(node_t->right);

        txSetConsoleAttr(FOREGROUND_GREEN | BACKGROUND_BLACK);
        printf(")");
        txSetConsoleAttr(FOREGROUND_LIGHTGRAY | BACKGROUND_BLACK);
    }
}

int analyse_answers(TreeNode * node_t)
{
    if(node_t == NULL)
    {
        printf("Maybe it's unknown who?\n");
        if(scan_answers())
        {
            node_t = add_node(node_t);
            return 0;
        }

        else
        {
            return 0;
        }
    }

    if(node_t->right == NULL && node_t->left == NULL)
    {
        printf("Is this %s?\n", node_t->data);
        if(scan_answers() == 1)
        {
            return 1;
        }

        else
        {
            add_node(node_t);
        }
    }

    if(scan_answers())
    {
        analyse_answers(node_t->right);
    }

    else
    {
        analyse_answers(node_t->left);
    }

    return 1;
}

int scan_answers()
{
    const char * str = (const char*)calloc(START_SIZE, sizeof(char));
    str = my_scanf();

    if(str != "yes" && str != "no")
    {
        printf("You can only answer yes or no \n");
        return -1;
    }

    if(str == "no")
    {
        return 0;
    }

    return 1;
}

const char ** scan_tree_file(char * text, const char ** pointers_to_nodes, const int symbols, int file_size)
{
    pointers_to_nodes[0] = (const char*)calloc(symbols, sizeof(char));
    pointers_to_nodes[0] = strtok(text, " ");

    for(int i = 1; i < file_size; i++)
    {
        pointers_to_nodes[i] = (const char*)calloc(symbols, sizeof(char));

        if(pointers_to_nodes == NULL)
        {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        pointers_to_nodes[i] = strtok(NULL, " ");
    }

    return pointers_to_nodes;
}

TreeNode * build_tree(const char ** pointers_to_nodes, TreeNode * tree_node, int * index, int symbols)
{
    if(strcmp(pointers_to_nodes[*index], "nil") == 0)
    {
        return NULL;
    }

    tree_node = operator_new(pointers_to_nodes[(*index)]);

    (*index)++;

    tree_node->left = build_tree(pointers_to_nodes, tree_node->left, index, symbols);
    (*index)++;
    tree_node->right = build_tree(pointers_to_nodes, tree_node->right, index, symbols);

    return tree_node;
}

void go_on_tree(TreeNode * tree)
{
    if(scan_answers())
    {
        analyse_answers(tree->right);
    }

    else
    {
        analyse_answers(tree->left);
    }
}

char * my_scanf()
{
    int i = 0;
    int start_size = START_SIZE;
    char ch = 0;

    char * str = (char*)calloc(START_SIZE, sizeof(char));

    while((ch = (char)getchar()) != '\n')
    {
        if(i > start_size - 1)
        {
            start_size *= MULTIPLIER;
            str = (char*)realloc(str, start_size);
        }

        str[i] = ch;
        i++;
    }

    str[i] = '\0';

    //printf("%s \n", str);

    return str;
}

int count_text_symbols(const char * text, int file_size)
{
    int counter = 0;
    printf("file_size = %d\n", file_size);

    for(int i = 0; i < file_size; i++)
    {
        if(isspace(text[i]))
        {
            counter++;
        }
    }

    return counter+1;
}

int get_file_size(FILE * fp)
{
    fseek(fp, 0L, SEEK_END);
    int file_size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    return file_size;
}
