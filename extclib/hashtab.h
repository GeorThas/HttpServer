#include "bintree.h"

typedef struct Hashtab
{
    struct {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    size_t size;
    Tree **table;
} Hashtab;

extern Hashtab *new_hashtab(size_t size, vtype_tree_t key, vtype_tree_t value);
extern void free_hashtab(Hashtab *hashtab);

extern value_tree_t get_hashtab(Hashtab *hashtab, void *key);
extern void set_hashtab(Hashtab *hashtab, void *key, void *value);
extern void del_hashtab(Hashtab *hashtab, void *key);
extern _Bool in_hashtab(Hashtab *hashtab, void *key); // contains(...);

extern void print_hashtab(Hashtab *hashtab);

static uint32_t _strhash(uint8_t *s, size_t size);



uint32_t _strhash(uint8_t *s, size_t size) //?!
{
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s){
        hashval = *s + 31 * hashval;
    }
    //printf("\n{%d}\n", hashval  % size);
    return hashval % size;
}