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

static const int HASH_TABLE_LISTS_CAPACITY_AFTER_RESIZE = 2;
static const int HASH_TABLE_MAX_LOAD_FACTOR = 15;
static const int HASH_TABLE_GROWTH_FACTOR = 2;

static void HashTableReinsert (hash_table_t* hash_table, hash_table_elem_t* elem);
static void HashTableResize (hash_table_t* hash_table, int hash_table_new_capacity);
static hash_table_elem_t* HashTableFindStringByHash (hash_table_t* hash_table, const char* string, uint32_t string_hash);

//-------------------------------------------------

void HashTableInit (hash_table_t* hash_table, int hash_table_initial_capacity, int lists_initial_capacity, int elem_size, hash_func_t HashFunc)
{
    assert (hash_table != NULL);
    assert (hash_table_initial_capacity > 0);
    assert (lists_initial_capacity > 0);
    assert (elem_size > 0);
    assert (HashFunc != NULL);

    hash_table->table = (list_t*) calloc ((size_t) hash_table_initial_capacity, sizeof (list_t));
    CUSTOM_ASSERT (hash_table->table != NULL);
    hash_table->capacity = hash_table_initial_capacity;
    hash_table->elem_size = elem_size;
    hash_table->elems_in_table = 0;
    hash_table->load_factor = 0.0;
    hash_table->HashFunc = HashFunc;

    for (int i = 0; i < hash_table_initial_capacity; ++i)
    {
        ListInit (&hash_table->table[i], lists_initial_capacity, elem_size);
    }
}

void HashTableDestroy (hash_table_t* hash_table)
{
    assert (hash_table != NULL);

    for (int i = 0; i < hash_table->capacity; ++i)
        ListDestroy (&hash_table->table[i]);

    free (hash_table->table);
    memset (hash_table, 0, sizeof (hash_table_t));
}

//-------------------------------------------------

void HashTableInsert (hash_table_t* hash_table, const char* string_ptr)
{
    assert (hash_table != NULL);
    assert (string_ptr != NULL);

    uint32_t string_hash = hash_table->HashFunc (string_ptr);

    hash_table_elem_t* found_elem = HashTableFindStringByHash (hash_table, string_ptr, string_hash);
    if (found_elem != NULL)
    {
        found_elem->repetitions_number += 1;
        return;
    }

    hash_table->elems_in_table += 1;
    hash_table->load_factor = (float) hash_table->elems_in_table / (float) hash_table->capacity;

    if (hash_table->load_factor > HASH_TABLE_MAX_LOAD_FACTOR)
    {
        HashTableResize (hash_table, hash_table->capacity * HASH_TABLE_GROWTH_FACTOR);
    }

    hash_table_elem_t new_elem = {};
    new_elem.string = string_ptr;
    new_elem.repetitions_number = 1;
    new_elem.string_hash = string_hash;

    list_t* list = &hash_table->table[string_hash % (uint32_t) hash_table->capacity];
    ListInsertEnd (list, &new_elem);
}

static void HashTableReinsert (hash_table_t* hash_table, hash_table_elem_t* elem)
{
    assert (hash_table != NULL);
    assert (elem != NULL);

    list_t* list = &hash_table->table[elem->string_hash % (uint32_t) hash_table->capacity];
    ListInsertEnd (list, elem);
}

//-------------------------------------------------

hash_table_elem_t* HashTableFindString (hash_table_t* hash_table, const char* string)
{
    assert (hash_table != NULL);
    assert (string != NULL);

    uint32_t string_hash = hash_table->HashFunc (string);

    return HashTableFindStringByHash (hash_table, string, string_hash);
}

static hash_table_elem_t* HashTableFindStringByHash (hash_table_t* hash_table, const char* string, uint32_t string_hash)
{
    assert (hash_table != NULL);
    assert (string != NULL);

    int list_index_in_hash_table = (int) (string_hash % (uint32_t) hash_table->capacity);
    list_t* list = &hash_table->table[list_index_in_hash_table];
    hash_table_elem_t* list_data = (hash_table_elem_t*) (list->data);

    for (int i = 0; i < list->size; ++i)
    {
        if (list_data[i].string_hash == string_hash)
        {
            if (strcmp (string, list_data[i].string) == 0)
                return (list_data + i);
        }
    }

    return NULL;
}

//-------------------------------------------------

static void HashTableResize (hash_table_t* hash_table, int hash_table_new_capacity)
{
    assert (hash_table != NULL);
    assert (hash_table_new_capacity > 0);

    hash_table_t new_hash_table = {};

    HashTableInit (&new_hash_table, hash_table_new_capacity, HASH_TABLE_LISTS_CAPACITY_AFTER_RESIZE, hash_table->elem_size, hash_table->HashFunc);

    for (int i = 0; i < hash_table->capacity; ++i)
    {
        list_t* list = &hash_table->table[i];
        hash_table_elem_t* list_data = (hash_table_elem_t*) (list->data);
        int current = list->head;

        if (current != -1)
        {
            do
            {
                HashTableReinsert (&new_hash_table, &list_data[current]);
                current = list->next[current];
            } while (list->next[current] != list->head);
        }
    }

    new_hash_table.elems_in_table = hash_table->elems_in_table;
    new_hash_table.load_factor = (float) new_hash_table.elems_in_table / (float) new_hash_table.capacity;

    HashTableDestroy (hash_table);

    memcpy (hash_table, &new_hash_table, sizeof (hash_table_t));
}

//-------------------------------------------------

uint32_t HashCrc32Intrinsic (const char* str)
{
    uint32_t crc = 0;
    
    int shift = 0;
    while (*(str + shift) != 0) 
    {
        crc = _mm_crc32_u8(crc, (unsigned char) *(str + shift));
        shift++;
    }
    
    return crc;
}

//-------------------------------------------------
