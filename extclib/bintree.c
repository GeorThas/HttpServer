#include "bintree.h"

#include <string.h>
#include <stdio.h>

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static void _set_key(tree_node *node, vtype_tree_t tkey, void *key);
static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value);
/*changed void->tree_node* */ static tree_node *_set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static tree_node *_get_tree(tree_node *node, vtype_tree_t tkey, void *key);

static tree_node *del1_tree(Tree *tree, vtype_tree_t tkey, void *key);
static void _del2_tree(Tree *tree, tree_node *node);
static void _del3_tree(tree_node *node);
static void _free_tree(tree_node *node);

static tree_node *_get_grandparent(tree_node *node);
static tree_node *_get_uncle(tree_node *node);

static void _right_rotate(tree_node *node);
static void _left_rotate(tree_node *node);

static void _balance_tree(Tree *tree, tree_node *node);
static void _insert_case1(tree_node *node);
static void _insert_case2(tree_node *node);
static void _insert_case3(tree_node *node);
static void _insert_case4(tree_node *node);
static void _insert_case5(tree_node *node);

static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);

void printTreeUtil(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, int space);
void printTree(Tree *root);

//================================
/** 
typedef struct Hashtab
{
    struct {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    size_t size;
    Tree **table;
} Hashtab;

Hashtab *new_hashtab(size_t size, vtype_tree_t key, vtype_tree_t value) {
    switch(key){
        case DECIMAL_ELEM: case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "key type not supported");
            return NULL;
    }
    switch(value) {
        case DECIMAL_ELEM: case REAL_ELEM: case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "value type not supported");
            return NULL;
    }
    Hashtab *hashtab = (Hashtab*)malloc(sizeof(Hashtab));
    hashtab->table = (Tree**)malloc(size * sizeof(Tree));
    for (size_t i = 0; i < size; ++i) {
        hashtab->table[i] = new_tree(key, value);
    }
    hashtab->size = size;
    hashtab->type.key = key;
    hashtab->type.value = value;
    return hashtab;
}


uint32_t _strhash(uint8_t *s, size_t size)
{
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s){
        hashval = *s + 31 * hashval;
    }
    printf("\n{%d}\n", hashval  % size);
    return hashval % size;
}

void set_hashtab(Hashtab *hashtab, void *key, void *value)
{
    uint32_t hash;
    switch (hashtab->type.key)
    {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;
        break;

    case STRING_ELEM:
        hash = _strhash((uint8_t *)key, hashtab->size);
        break;
    }
    // printf("%d",hashtab->size);
    set_tree(hashtab->table[hash], key, value);
}

extern void print_hashtab(Hashtab *hashtab)
{

    printf("{\n");
    for (size_t i = 0; i < hashtab->size; ++i)
    {
        if (hashtab->table[i]->node == NULL)
        {
            continue;
        }
        // switch(hashtab->type.key) {
        //  case DECIMAL_ELEM:
        //      hash = hashtab->table[i]->node->data.key.decimal % hashtab->size;
        //  break;
        //  case STRING_ELEM:
        //      hash = _strhash(hashtab->table[i]->node->data.key.string, hashtab->size);
        //      printf("%d - %d\n", i, hash);
        //  break;
        // }
        printf("\t%d => ", i);
        print_tree_as_list(hashtab->table[i]);
    }
    printf("}\n");
}

//================================

int main(void)
{

    printf("Start\n");

    Hashtab *hashtab = new_hashtab(10, DECIMAL_ELEM, STRING_ELEM);
    //(Hashtab*)malloc(sizeof(Hashtab));
    //hashtab->table = (Tree**)malloc(2 * sizeof(Tree));
    
    //hashtab->size = size;
    //hashtab->type.key = key;
    //hashtab->type.value = value;

    //Tree** table = (Tree**)malloc(2 * sizeof(Tree));
    //for (size_t i = 0; i < 2; ++i) {
    //    hashtab->table[i] = new_tree(DECIMAL_ELEM, STRING_ELEM);
    //}

    //Tree *tree = new_tree(DECIMAL_ELEM, STRING_ELEM);

    printf("Tree init - \n");

    
    set_tree(hashtab->table[0], decimal(50), string("F"));
    set_tree(hashtab->table[1], decimal(55), string("I"));
    set_tree(hashtab->table[0], decimal(40), string("O"));
    set_tree(hashtab->table[1], decimal(70), string("N"));
    set_tree(hashtab->table[0], decimal(65), string("A"));
    set_tree(hashtab->table[1], decimal(75), string("R"));
    set_tree(hashtab->table[0], decimal(60), string("U"));
    set_tree(hashtab->table[1], decimal(59), string("V"));

    set_hashtab(hashtab, decimal(50), string("F"));
    set_hashtab(hashtab, decimal(55), string("I"));
    set_hashtab(hashtab, decimal(40), string("O"));
    set_hashtab(hashtab, decimal(70), string("N"));
    set_hashtab(hashtab, decimal(65), string("A"));
    set_hashtab(hashtab, decimal(75), string("R"));
    set_hashtab(hashtab, decimal(60), string("U"));
    set_hashtab(hashtab, decimal(59), string("V"));
    //printf("Tree init\n");

    //value_tree_t v = get_tree(table[0], decimal(50));

    //printf("%d\n", in_tree(table[0], decimal(50)));

    // print_tree(tree);
    // print_tree_as_list(tree);
    //printTree(hashtab->table[0]);
    //printTree(hashtab->table[1]);

    print_hashtab(hashtab);

    free_tree(hashtab->table[0]);
    system("pause");
    return 0;
}

*/
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
    //printf("bt.c120");
    if (tree->node == NULL)
    {
        tree->node = _new_node(tree->type.key, tree->type.value, key, value);
        _balance_tree(tree, tree->node);
        return;
    }
    tree_node *inserted_node = _set_tree(tree->node, tree->type.key, tree->type.value, key, value);
    _balance_tree(tree, inserted_node); // not checked!!!
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
        _del3_tree(node); //
        return;
    }
    _del2_tree(tree, node);
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

static void _del2_tree(Tree *tree, tree_node *node)
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
    tree_node *grandparent = _get_grandparent(node);
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
    tree_node *pivot = node->right;

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
    //printf(" [%s => ", "bt.c_394");
    if (node == NULL)
    {
        return;
    }
    //printf(" [%s => ", "bt.c_399");
    _print_tree_as_list(node->left, tkey, tvalue);
    //printf(" [%s => ", "bt.c_400");
    _print_tree_elem(node, tkey, tvalue);
    //printf(" [%s => ", "bt.c_400");
    _print_tree_as_list(node->right, tkey, tvalue);
}

void _balance_tree(Tree *tree, tree_node *node)
{
    if (node == tree->node)
    {
        _insert_case1(node);
        return;
    }

    if (node->parent->color == BLACK)
    {
        _insert_case2(node);
        return;
    }
    tree_node *uncle = _get_uncle(node);
    if (uncle != NULL)
    {
        if ((node->parent->color == RED) && (uncle->color == RED))
        { // проверка uncle на NULL?
            _insert_case3(node);
            return;
        }
    }
    tree_node *grandparent = _get_grandparent(node);
    // printf(" -4if%d-\n ", uncle->color);
    if ((node->parent->color == RED) && (uncle == NULL || uncle->color == BLACK) && (node == node->parent->right) && (node->parent == grandparent->left))
    {
        // printf(" -4if%d-\n ", node->color);
        _insert_case4(node);
        return;
    }

    if ((node->parent->color == RED) && (uncle == NULL || uncle->color == BLACK) && (node == node->parent->left) && (node->parent == grandparent->left))
    {
        _insert_case5(node);
        return;
    }
}

void _insert_case1(tree_node *node)
{
    ///printf(" -1b%d-\n ", node->color);
    if (node->parent == NULL)
    {
        node->color = BLACK;
    }
    else
    {
        _insert_case2(node);
    }
    //printf(" -1%d-\n ", node->color);
}

void _insert_case2(tree_node *node)
{
    //printf(" -2b%d-\n ", node->color);
    //printf("bt.c455");

    if(node == NULL){
        //printf("bt.c458 nodeIsNULL");
    }
    if (node->parent->color == BLACK)
    {
        return;
    }
    else
    {
        _insert_case3(node);
    }
    //printf(" -2%d-\n ", node->color);
}

void _insert_case3(tree_node *node)
{
    //printf(" -3b%d-\n ", node->color);
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
    //printf(" -3%d-\n ", node->color);
}

void _insert_case4(tree_node *node)
{
    //printf(" -4b%d-\n ", node->color);
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
    //printf(" -4%d-\n ", node->color);
}

void _insert_case5(tree_node *node)
{
    //printf(" -5b%d-\n ", node->color);
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
    //printf(" -5%d-\n ", node->color);
}

static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    //printf(" [%s => ", "bt.c_563");
    switch (tkey)
    {
        
    case DECIMAL_ELEM:
        printf(" [%d => ", node->data.key.decimal);
        //printf(" -%d- ", node->color);
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
        //printf(" -%d- ", node->color);
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
void printTreeUtil(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, int space)
{

    if (node == NULL)
        return;
    int spacing = 5;
    space += spacing;

    printTreeUtil(node->right, tkey, tvalue, space);
    printf("\n");
    for (int i = spacing; i < space; i++)
    {
        printf(" ");
    }
    // printf("%d\n", root->data);
    _print_tree_elem(node, tkey, tvalue);

    printTreeUtil(node->left, tkey, tvalue, space);
}

void printTree(Tree *tree)
{
    if (tree->node == NULL)
    {
        printf("Tree is empty!\n");
        return;
    }
    printf("Vertical visualisaton:\n");
    printTreeUtil(tree->node, tree->type.key, tree->type.value, 10);
    printf("\n");
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
static tree_node *_set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
    tree_node *inserted_node; // = node;
    switch (tkey)
    {
    case DECIMAL_ELEM:
        if ((int64_t)key > node->data.key.decimal)
        {
            if (node->right == NULL)
            {
                node->right = _new_node(tkey, tvalue, key, value);
                node->right->parent = node;
                inserted_node = node->right;
            }
            else
            {
                inserted_node = _set_tree(node->right, tkey, tvalue, key, value);
            }
        }
        else if ((uint64_t)key < node->data.key.decimal)
        {
            if (node->left == NULL)
            {
                node->left = _new_node(tkey, tvalue, key, value);
                node->left->parent = node;
                inserted_node = node->left;
            }
            else
            {
                inserted_node = _set_tree(node->left, tkey, tvalue, key, value);
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
                inserted_node = node->right;
            }
            else
            {
                inserted_node = _set_tree(node->right, tkey, tvalue, key, value);
            }
        }
        else if (condition < 0)
        {
            if (node->left == NULL)
            {
                node->left = _new_node(tkey, tvalue, key, value);
                node->left->parent = node;
                inserted_node = node->left;
            }
            else
            {
                inserted_node = _set_tree(node->left, tkey, tvalue, key, value);
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
    node->parent = NULL;
    node->right = NULL;
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