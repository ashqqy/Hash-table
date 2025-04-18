#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>

#include "linked-list.h"

typedef uint32_t (*hash_func_t)(const char*);

typedef struct
{
    list_t* table;
    int capacity;
    int elems_in_table;
    int elem_size;
    float load_factor;
    hash_func_t HashFunc;
} hash_table_t;

typedef struct
{
    const char* string;
    int repetitions_number;
    uint32_t string_hash;
} hash_table_elem_t;


void HashTableInit (hash_table_t* hash_table, int hash_table_initial_capacity, int lists_initial_capacity, int elem_size, hash_func_t HashFunc);
void HashTableDestroy (hash_table_t* hash_table);

void HashTableInsert (hash_table_t* hash_table, const char* string);

hash_table_elem_t* HashTableFindString (hash_table_t* hash_table, const char* string);
hash_table_elem_t* HashTableFindStringByHash (hash_table_t* hash_table, const char* string, uint32_t string_hash);

uint32_t HashCrc32Intrinsic (const char* str);

#endif // HASH_TABLE_H
