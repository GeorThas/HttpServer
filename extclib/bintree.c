#include "bintree.h"

#include <string.h>
#include <stdio.h>

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static void _set_key(tree_node *node, vtype_tree_t tkey, void *key);
static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value);
/*changed void->tree_node* */static tree_node *_set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static tree_node *_get_tree(tree_node *node, vtype_tree_t tkey, void *key);

static tree_node *del1_tree(Tree *tree, vtype_tree_t tkey, void *key);
static void *del2_tree(Tree *tree, tree_node *node);
static void _del3_tree(tree_node *node);
static void _free_tree(tree_node *node);

static tree_node *_get_grandparent(tree_node *node);
static tree_node *_get_uncle(tree_node *node);

static void _right_rotate(tree_node *node);
static void _left_rotate(tree_node *node);

void _balance_tree(Tree *tree, tree_node *node);
void _insert_case1(tree_node *node);
void _insert_case2(tree_node *node);
void _insert_case3(tree_node *node);
void _insert_case4(tree_node *node);
void _insert_case5(tree_node *node);

static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);

/*
int main(void){

    printf("Start\n");

    Tree *tree = new_tree(DECIMAL_ELEM, STRING_ELEM);

     printf("Tree init\n");

    set_tree(tree, decimal(50), string("F"));
    set_tree(tree, decimal(55), string("I"));
    set_tree(tree, decimal(40), string("O"));
    set_tree(tree, decimal(70), string("N"));
    set_tree(tree, decimal(69), string("A"));

    value_tree_t v = get_tree(tree, decimal(50));

    printf("%d\n", in_tree(tree, decimal(50)));

    print_tree(tree);
    free_tree(tree);
    system("pause");
    return 0;
}*/

extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value)
{
    switch (key)
    {
    case DECIMAL_ELEM:
    case STRING_ELEM:
        break;
    default:
        fprintf(stderr, "%s\n", "key type not supported");
        return NULL;
    }
    switch (value)
    {
    case DECIMAL_ELEM:
    case REAL_ELEM:
    case STRING_ELEM:
        break;
    default:
        fprintf(stderr, "%s\n", "value type not supported");
        return NULL;
    }

    Tree *tree = (Tree *)malloc(sizeof(Tree));
    tree->type.key = key;
    tree->type.value = value;
    tree->node = NULL;
    return tree;
}

extern value_tree_t get_tree(Tree *tree, void *key)
{
    tree_node *node = _get_tree(tree->node, tree->type.key, key);
    if (node == NULL)
    {
        fprintf(stderr, "%s\n", "value undefined");
        value_tree_t none;
        none.decimal = 0;
        return none;
    }
    return node->data.value;
}

extern _Bool in_tree(Tree *tree, void *key)
{
    return _get_tree(tree->node, tree->type.key, key) != NULL;
} // contains(...);

extern void set_tree(Tree *tree, void *key, void *value)
{
    if (tree->node == NULL)
    {
        tree->node = _new_node(tree->type.key, tree->type.value, key, value);
        return;
    }
    tree_node *inserted_node = _set_tree(tree->node, tree->type.key, tree->type.value, key, value);
    _balance_tree(tree, inserted_node);//not checked!!!
}

extern void *decimal(uint64_t x)
{
    return (void *)x;
}

extern void *real(double x)
{
    double *f = (double *)malloc(sizeof(double));
    *f = x;
    return (void *)f;
}

extern void *string(uint8_t *x)
{
    return (void *)x;
}

extern void print_tree(Tree *tree)
{
    _print_tree(tree->node, tree->type.key, tree->type.value);
    putchar('\n');
}

extern void print_tree_as_list(Tree *tree)
{
    putchar('[');
    _print_tree_as_list(tree->node, tree->type.key, tree->type.value);
    putchar(']');
    putchar('\n');
}

extern void free_tree(Tree *tree)
{
    _free_tree(tree->node);
    free(tree);
}

extern void del_tree(Tree *tree, void *key)
{
    tree_node *node = del1_tree(tree, tree->type.key, key);
    if (node == NULL)
    {
        return;
    }
    if (node->left != NULL && node->right != NULL)
    {
        _del_tree(node);
        return;
    }
    _del2_tree(node);
}

static tree_node *del1_tree(Tree *tree, vtype_tree_t tkey, void *key)
{
    tree_node *node = tree->node;
    node = _get_tree(node, tkey, key);
    if (node == NULL)
    {
        return NULL;
    }
    if (node->left != NULL || node->right != NULL)
    {
        return node;
    }
    tree_node *parent = node->parent;
    if (parent == NULL)
    {
        tree->node = NULL;
    }
    else if (parent->left == node)
    {
        parent->left = NULL;
    }
    else
    {
        parent->right = NULL;
    }
    free(node);
    return NULL;
}

static void *del2_tree(Tree *tree, tree_node *node)
{
    tree_node *parent = node->parent;
    tree_node *temp;
    if (node->right != NULL)
    {
        temp = node->right;
    }
    else
    {
        temp = node->left;
    }
    if (parent == NULL)
    {
        tree->node = temp;
    }
    else if (parent->left == node)
    {
        parent->left = temp;
    }
    else
    {
        parent->right = temp;
    }
    temp->parent = parent;
    free(node);
}

static void _del3_tree(tree_node *node)
{
    tree_node *ptr = node->right;
    while (ptr->left != NULL)
    {
        ptr = ptr->left;
    }
    node->data.key = ptr->data.key;
    node->data.value = ptr->data.value;
    tree_node *parent = ptr->parent;
    if (parent->left == ptr)
    {
        parent->left = NULL;
    }
    else
    {
        parent->right = NULL;
    }
    free(ptr);
}

static tree_node *_get_tree(tree_node *node, vtype_tree_t tkey, void *key)
{
    if (node == NULL)
    {
        return NULL;
    }
    switch (tkey)
    {
    case DECIMAL_ELEM:
        if ((int64_t)key > node->data.key.decimal)
        {
            return _get_tree(node->right, tkey, key);
        }
        else if ((uint64_t)key < node->data.key.decimal)
        {
            return _get_tree(node->left, tkey, key);
        }
        break;
    case STRING_ELEM:
        int condition = strcmp((uint8_t *)key, node->data.key.string);
        if (condition > 0)
        {
            return _get_tree(node->right, tkey, key);
        }
        else if (condition < 0)
        {
            return _get_tree(node->left, tkey, key);
        }
        break;
    }
    return node;
}

static void _free_tree(tree_node *node)
{
    if (node == NULL)
    {
        return;
    }
    _free_tree(node->left);
    _free_tree(node->right);
    free(node);
}

tree_node *_get_grandparent(tree_node *node)
{
    if ((node != NULL) && (node->parent != NULL))
    {
        return node->parent->parent;
    }
    else
    {
        return NULL;
    }
}

tree_node *_get_uncle(tree_node *node)
{
    tree_node *grandparent = get_grandparent(node);
    if (grandparent == NULL)
    {
        return NULL;
    }
    if (grandparent->right == node->parent)
    {
        return grandparent->left;
    }
    else
    {
        return grandparent->right;
    }
}

void _right_rotate(tree_node *node)
{
    tree_node *pivot = node->left;

    pivot->parent = node->parent;
    if (node->parent != NULL)
    {
        if (node->parent->left == node)
        {
            node->parent->left = pivot;
        }
        else
        {
            node->parent->right = pivot;
        }
    }

    node->left = pivot->right;
    if (pivot->right != NULL)
    {
        pivot->right->parent = node;
    }

    node->parent = pivot;
    pivot->right = node;
}

void _left_rotate(tree_node *node)
{
    tree_node *pivot = node->parent;

    pivot->parent = node->parent;
    if (node->parent != NULL)
    {
        if (node->parent->left == node)
        {
            node->parent->left = pivot;
        }
        else
        {
            node->parent->right = pivot;
        }
    }

    node->right = pivot->left;
    if (pivot->left != NULL)
    {
        pivot->left->parent = node;
    }

    node->parent = pivot;
    pivot->left = node;
}

static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    if (node == NULL)
    {
        return;
    }
    _print_tree_as_list(node->left, tkey, tvalue);
    _print_tree_elem(node, tkey, tvalue);
    _print_tree_as_list(node->right, tkey, tvalue);
}

void _balance_tree(Tree *tree, tree_node *node)
{
    if (node == tree->node){
        _insert_case1(node);
        return;
    }
    if (node->parent->color == BLACK){
        _insert_case2(node);
        return;
    }
    tree_node *uncle = _get_uncle(node);
    if ((node->parent->color == RED) && (uncle->color == RED)){
        _insert_case3(node);
        return;
    }
    tree_node *grandparent = _get_grandparent(node);
    if ((node->parent->color == RED) && (uncle->color == BLACK) && (node == node->parent->right) && (node->parent == grandparent->left)){
        _insert_case4(node);
    }
    if ((node->parent->color == RED) && (uncle->color == BLACK) && (node == node->parent->left) && (node->parent == grandparent->left)){
        _insert_case5(node);
    }
}

void _insert_case1(tree_node *node)
{
    if (node->parent == NULL)
    {
        node->color = BLACK;
    }
    else
    {
        _insert_case2(node);
    }
}

void _insert_case2(tree_node *node)
{
    if (node->parent->color == BLACK)
    {
        return;
    }
    else
    {
        _insert_case3(node);
    }
}

void _insert_case3(tree_node *node)
{
    tree_node *uncle = _get_uncle(node), *grandparent;

    if ((uncle != NULL) && (uncle->color == RED))
    {
        node->parent->color = BLACK;
        uncle->color = BLACK;
        grandparent = _get_grandparent(node);
        grandparent->color = RED;
        _insert_case1(grandparent);
    }
    else
    {
        _insert_case4(node);
    }
}

void _insert_case4(tree_node *node)
{
    tree_node *grandparent = _get_grandparent(node);

    if ((node == node->parent->right) && (node->parent == grandparent->left))
    {
        _left_rotate(node->parent);
        node = node->left;
    }
    else if ((node == node->parent->left) && (node->parent == grandparent->right))
    {
        _right_rotate(node->parent);
        node = node->right;
    }

    _insert_case5(node);
}

void _insert_case5(tree_node *node)
{
    tree_node *grandparent = _get_grandparent(node);

    node->parent->color = BLACK;
    grandparent->color = RED;
    if ((node == node->parent->left) && (node->parent == grandparent->left))
    {
        _right_rotate(grandparent);
    }
    else
    {
        _left_rotate(grandparent);
    }
}

static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    switch (tkey)
    {
    case DECIMAL_ELEM:
        printf(" [%d => ", node->data.key.decimal);
        switch (tvalue)
        {
        case DECIMAL_ELEM:
            printf("%d] ", node->data.value.decimal);
            break;
        case REAL_ELEM:
            printf("%lf] ", node->data.value.real);
            break;
        case STRING_ELEM:
            printf("%s] ", node->data.value.string);
            break;
        }
        break;
    case STRING_ELEM:
        printf(" ['%s' => ", node->data.key.string);
        switch (tvalue)
        {
        case DECIMAL_ELEM:
            printf("%d] ", node->data.value.decimal);
            break;
        case REAL_ELEM:
            printf("%lf] ", node->data.value.real);
            break;
        case STRING_ELEM:
            printf("%s] ", node->data.value.string);
            break;
        }
        break;
    }
}

static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    if (node == NULL)
    {
        printf("null");
        return;
    }
    putchar('(');
    _print_tree(node->left, tkey, tvalue);
    _print_tree_elem(node, tkey, tvalue);
    _print_tree(node->right, tkey, tvalue);
    putchar(')');
}

//*
static tree_node *set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
    tree_node *inserted_node = node;
    switch (tkey)
    {
    case DECIMAL_ELEM:
        if ((int64_t)key > node->data.key.decimal)
        {
            if (node->right == NULL)
            {
                node->right = _new_node(tkey, tvalue, key, value);
                node->right->parent = node;
            }
            else
            {
                _set_tree(node->right, tkey, tvalue, key, value);
            }
        }
        else if ((uint64_t)key < node->data.key.decimal)
        {
            if (node->left == NULL)
            {
                node->left = _new_node(tkey, tvalue, key, value);
                node->left->parent = node;
            }
            else
            {
                _set_tree(node->left, tkey, tvalue, key, value);
            }
        }
        else
        {
            _set_value(node, tvalue, value);
        }
        break;
    case STRING_ELEM:
        int condition = strcmp((uint8_t *)key, node->data.key.string);
        if (condition > 0)
        {
            if (node->right == NULL)
            {
                node->right = _new_node(tkey, tvalue, key, value);
                node->right->parent = node;
            }
            else
            {
                _set_tree(node->right, tkey, tvalue, key, value);
            }
        }
        else if (condition < 0)
        {
            if (node->left == NULL)
            {
                node->left = _new_node(tkey, tvalue, key, value);
                node->left->parent = node;
            }
            else
            {
                _set_tree(node->left, tkey, tvalue, key, value);
            }
        }
        else
        {
            _set_value(node, tvalue, value);
        }
        break;
    }

    return inserted_node;
}

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
    tree_node *node = (tree_node *)malloc(sizeof(tree_node));
    node->color = RED;
    _set_key(node, tkey, key);
    _set_value(node, tvalue, value);
    node->left = NULL;
    node->left = NULL;
    node->left = NULL;
    return node;
}

static void _set_key(tree_node *node, vtype_tree_t tkey, void *key)
{
    switch (tkey)
    {
    case DECIMAL_ELEM:
        node->data.key.decimal = (uint64_t)key;
        break;
    case STRING_ELEM:
        node->data.key.string = (uint8_t *)key;
        break;
    }
}

static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value)
{
    switch (tvalue)
    {
    case DECIMAL_ELEM:
        node->data.value.decimal = (uint64_t)value;
        break;
    case REAL_ELEM:
        node->data.value.real = *(double *)value;
        free((double *)value);
        break;
    case STRING_ELEM:
        node->data.value.string = (uint8_t *)value;
        break;
    }
}