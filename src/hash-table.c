#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nmmintrin.h>

#include "hash-table.h"
#include "linked-list.h"

//-------------------------------------------------

#define CUSTOM_ASSERT(expression) do                                                                             \
{                                                                                                                \
    if (!(expression))                                                                                           \
    {                                                                                                            \
        fprintf (stderr, "%s: %s:%d: Assertion `%s' failed.\n", __FUNCTION__, __FILE__, __LINE__, #expression);  \
        fprintf (stderr, "Program aborting\n");                                                                  \
        exit(EXIT_FAILURE);                                                                                      \
    }                                                                                                            \
}                                                                                                                \
while (0)

//-------------------------------------------------

static const int LISTS_CAPACITY_AFTER_RESIZE = 2;

static void HashTableResize (hash_table_t* hash_table, int hash_table_new_capacity)
{
    assert (hash_table != NULL);
    assert (hash_table_new_capacity > 0);

    hash_table_t* new_hash_table = (hash_table_t*) calloc (1, sizeof (hash_table_t));
    CUSTOM_ASSERT (new_hash_table != NULL);

    HashTableInit (new_hash_table, hash_table_new_capacity, LISTS_CAPACITY_AFTER_RESIZE, hash_table->elem_size);

    // TODO
}

static uint32_t HashCrc32Intrinsic (const char* str)
{
    uint32_t crc = 0;
    
    int shift = 0;
    while (*(str+shift) != 0) 
    {
        crc = _mm_crc32_u8(crc, (unsigned char) *(str + shift));
        shift++;
    }
    
    return crc;
}

//-------------------------------------------------

void HashTableInit (hash_table_t* hash_table, int hash_table_initial_capacity, int lists_initial_capacity, int elem_size)
{
    assert (hash_table != NULL);
    assert (hash_table_initial_capacity > 0);
    assert (lists_initial_capacity > 0);
    assert (elem_size > 0);

    hash_table->table = (list_t*) calloc ((size_t) hash_table_initial_capacity, sizeof (list_t));
    CUSTOM_ASSERT (hash_table->table != NULL);
    hash_table->capacity = hash_table_initial_capacity;
    hash_table->elem_size = elem_size;
    hash_table->load_factor = 0.0;

    for (int i = 0; i < hash_table_initial_capacity; ++i)
    {
        ListInit (&hash_table->table[i], lists_initial_capacity, elem_size);
    }
}

void HashTableDestroy (hash_table_t* hash_table)
{
    assert (hash_table != NULL);

    for (int i = 0; i < hash_table->capacity; ++i)
    {
        ListDestroy (&hash_table->table[i]);
    }

    memset (hash_table, 0, sizeof (hash_table_t));
}

//-------------------------------------------------

// // void HashTableInsert ()
// {

// }

// // void HashTableFind ()
// {

// }

