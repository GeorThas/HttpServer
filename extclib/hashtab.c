#include <stdio.h>
#include "hashtab.h"

/*
int main(void)
{
    printf("Start\n");

    Hashtab *hashtab = new_hashtab(3, STRING_ELEM, DECIMAL_ELEM);

    printf("Tree init - \n");

    set_hashtab(hashtab, string("F"), decimal(1));
    set_hashtab(hashtab, string("I"), decimal(8));
    set_hashtab(hashtab, string("O"), decimal(3));
    set_hashtab(hashtab, string("N"), decimal(7));
    set_hashtab(hashtab, string("A"), decimal(4));
    set_hashtab(hashtab, string("R"), decimal(6));
    set_hashtab(hashtab, string("U"), decimal(9));
    set_hashtab(hashtab, string("V"), decimal(0));

    print_hashtab(hashtab);

    // printf("%d\n",hashtab->table[0]->node->right->data.key.decimal);

    // print_tree_as_list(hashtab->table[1]);

    // free_hashtab(hashtab);
    system("pause");
    return 0;
}
*/

Hashtab *new_hashtab(size_t size, vtype_tree_t key, vtype_tree_t value)
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
    Hashtab *hashtab = (Hashtab *)malloc(sizeof(Hashtab));
    hashtab->table = (Tree **)malloc(size * sizeof(Tree));
    for (size_t i = 0; i < size; ++i)
    {
        hashtab->table[i] = new_tree(key, value);
    }
    hashtab->size = size;
    hashtab->type.key = key;
    hashtab->type.value = value;
    return hashtab;
}

void free_hashtab(Hashtab *hashtab)
{
    for (size_t i = 0; i < hashtab->size; ++i)
    {
        free_tree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

value_tree_t get_hashtab(Hashtab *hashtab, void *key)
{
    uint32_t hash;
    value_tree_t result;
    switch (hashtab->type.key)
    {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;

        break;

    case STRING_ELEM:
        hash = _strhash((uint8_t *)key, hashtab->size);
        break;
    }
    result = get_tree(hashtab->table[hash], key);
    return result;
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

_Bool in_hashtab(Hashtab *hashtab, void *key)
{
    uint32_t hash;
    _Bool result;
    switch (hashtab->type.key)
    {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;

        break;

    case STRING_ELEM:
        hash = _strhash((uint8_t *)key, hashtab->size);
        break;
    }
    result = in_tree(hashtab->table[hash], key);
    return result;
}

extern void print_hashtab(Hashtab *hashtab)
{

    printf("{\n");
    for (size_t i = 0; i < hashtab->size; ++i)
    {
        //
        if (hashtab->table[i]->node == NULL)
        {
            // printf("\t NUL %d", hashtab->table[i]);
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
        // if (hashtab->table[i]->node != NULL)
        // printf("\t%s\t%d\t%d", hashtab->table[i]->node->data.value.string, hashtab->table[i]->node->right,  hashtab->table[i]->node->left);
        printf("\t%d => ", i);
        print_tree_as_list(hashtab->table[i]);
        // print_tree(hashtab->table[i]);
    }
    printf("}\n");
}
