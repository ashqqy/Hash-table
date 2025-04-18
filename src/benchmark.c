#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"
#include "hash-table.h"

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

void HashTableBenchmark (const char* words_for_insert_file_name, const char* words_for_find_file_name)
{
    assert (words_for_insert_file_name != NULL);
    assert (words_for_find_file_name != NULL);

    input_data_t words_for_insert = {};
    ReadFile (words_for_insert_file_name, &words_for_insert);

    input_data_t words_for_find = {};
    ReadFile (words_for_find_file_name, &words_for_find);

    //---------------------------------------------

    hash_table_t* hash_table = (hash_table_t*) calloc (1, sizeof (hash_table_t));
    HashTableInit (hash_table, 10, 10, sizeof (hash_table_elem_t), HashCrc32Intrinsic);

    //---------------------------------------------

    HashTableInsertTest (hash_table, &words_for_insert);
    HashTableFindTest (hash_table, &words_for_find);

    //---------------------------------------------

    HashTableDestroy (hash_table);
    free (hash_table);
    free (words_for_insert.array); words_for_insert.array = NULL;
    free (words_for_find.array); words_for_find.array = NULL;
}

//-------------------------------------------------

void HashTableInsertTest (hash_table_t* hash_table, input_data_t* words_for_insert)
{
    assert (hash_table != NULL);
    assert (words_for_insert != NULL);

    char* current_word_ptr = words_for_insert->array;    

    for (size_t i = 0; i < words_for_insert->n_words; ++i)
    {
        HashTableInsert (hash_table, current_word_ptr);
        current_word_ptr = strchr(current_word_ptr, '\0') + 1;
    }
}

void HashTableFindTest (hash_table_t* hash_table, input_data_t* words_for_find)
{
    assert (hash_table != NULL);
    assert (words_for_find != NULL);

    char* current_word_ptr = words_for_find->array;    

    for (size_t i = 0; i < words_for_find->n_words; ++i)
    {
        hash_table_elem_t* elem = HashTableFindString (hash_table, current_word_ptr);
        current_word_ptr = strchr(current_word_ptr, '\0') + 1;
        if (elem != NULL)
            printf ("string: %s; n_repeats = %d\n", elem->string, elem->repetitions_number);
        else
            printf ("Not found\n");
    }
}

//-------------------------------------------------

input_data_t* ReadFile (const char* input_file_name, input_data_t* input_data)
{
    assert (input_file_name != NULL);
    assert (input_data != NULL);

    FILE* input_file = fopen (input_file_name, "rb");
    CUSTOM_ASSERT (input_file != NULL);
    
    // Find file size
    fseek (input_file, 0L, SEEK_END);
    size_t size_input_file = (size_t) ftell(input_file);
    fseek(input_file, 0L, SEEK_SET);

    input_data->array = (char*) calloc (size_input_file, sizeof(char));
    CUSTOM_ASSERT (input_data->array!= NULL);

    input_data->array_size = fread (input_data->array, sizeof (char), size_input_file, input_file);
    CUSTOM_ASSERT (size_input_file == input_data->array_size);

    fclose (input_file);

    // Set '\n' to '\0' and count n_words
    char* current_word = input_data->array;
    while ((current_word != NULL) && (current_word < input_data->array + input_data->array_size))
    {
        current_word = strchr (current_word, '\n');
        if (current_word == NULL)
            break;

        *current_word = '\0';
        input_data->n_words++;
        current_word++;
    }

    return input_data;
}

//-------------------------------------------------
