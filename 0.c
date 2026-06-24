#include <stdlib.h>
#include <stdio.h>

typedef struct s_node {
    int             value;
    struct s_node   *left;
    struct s_node   *right;
}   t_node;

typedef struct s_tree {
    t_node **nodes;
    int     size;
} t_tree;

t_node *ft_create_node(int value)
{
    t_node  *node;
    node = malloc(sizeof(t_node));
    if (!node)
        return (NULL);
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void ft_link_tree(t_tree *tree)
{
    int i;

    i = 0;

    while (i < tree->size - 2)
    {
        tree->nodes[i]->left = tree->nodes[i + 1];
        tree->nodes[i]->left = tree->nodes[i + 2];
        i++;
    }
}

int ft_create_tree(int l_nodes, int *values, t_tree *tree)
{
    int i;

    i = 0;
    tree->nodes = malloc(l_nodes * sizeof(t_node));
    tree->size = 0;
    if (!(tree->nodes))
    {
        tree->nodes = NULL;
        return -1;
    }
    while (i < l_nodes)
    {
        tree->nodes[i] = ft_create_node(values[i]);
        tree->size += 1;
        i++;
    }
    ft_link_tree(tree);
    return 1;
}
void ft_print_node(t_node *node)
{
    if (!node)
        return;
    ft_print_node(node->left);
    printf("%d\n", node->value);
    ft_print_node(node->right); 
}

t_node *ft_insert(t_node *root, int value)
{
    if (!root)
        return ft_create_node(value);
    if (value < root->value)
        root->left = ft_insert(root->left, value);
    else
        root->right = ft_insert(root->right, value);
    return root;
}
t_node *ft_search(t_node *root, int value)
{
    if (!root)
        return root;
    if (root->value == value)
        return root;
    else if (value < root->value)
        return ft_search(root->left, value);
    else
        return ft_search(root->right, value);
}

t_node *ft_delete(t_node *node, int value)
{
    t_node *right;
    t_node *tmp;

    if (!node)
        return (NULL);
    if (value < node->value)
        node->left = ft_delete(node->left, value);
    else if (value > node->value)
        node->right = ft_delete(node->right, value);
    else
    {
        if (node->left && node->right)
        {
            right = node->right;
            while(right->left)
                right = right->left;
            node->value = right->value;
            node->right = ft_delete(node->right, right->value);
        }
        else if (node->left)
        {
            tmp = node->left;
            free(node);
            return tmp;
        }
        else if (node->right)
        {
            tmp = node->right;
            free(node);
            return tmp;
        }
        else
        {
            free(node);
            return NULL;
        }
    }
    return (node);
}

int main()
{
    t_node *root;

    root = NULL;
    root = ft_insert(root, 10);
    root = ft_insert(root, 5);
    root = ft_insert(root, 15);
    root = ft_insert(root, 7);

    ft_print_node(root);
}
// int main()
// {
//     t_node *root;
//     root = ft_create_node(10);
//     root->left = ft_create_node(5);
//     root->right = ft_create_node(15);

//     ft_print_node(root);
// }
// int main()
// {
//     // t_tree tr;
//     // int v[] = {10, 5, 15};
//     // ft_create_tree(3, v, &tr);
//     // printf("%d\n",root->value);    
//     // printf("%d\n",root->left->value);    
//     // printf("%d\n",root->right->value);    
//     // t_node *l_child;
//     // t_node *r_child;

//     // root = ft_create_node(10);
//     // l_child = ft_create_node(5);
//     // r_child = ft_create_node(15);
//     // ft_link_tree

// }