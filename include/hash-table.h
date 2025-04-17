#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked-list.h"

typedef struct
{
    list_t* table;
    int capacity;
    int elem_size;
    float load_factor;
} hash_table_t;

void HashTableInit (hash_table_t* hash_table, int hash_table_initial_capacity, int lists_initial_capacity, int elem_size);
void HashTableDestroy (hash_table_t* hash_table);

#endif // HASH_TABLE_H
