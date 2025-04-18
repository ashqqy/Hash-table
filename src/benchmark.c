#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

void HashTableBenchmark (const char* input_file_name)
{
    assert (input_file_name != NULL);

    FILE* input_file = fopen (input_file_name, "rb");
    CUSTOM_ASSERT (input_file != NULL);

    char* array = NULL;
    size_t array_size = ReadFile (input_file, &array);

    fclose (input_file);

    //---------------------------------------------

    hash_table_t* hash_table = (hash_table_t*) calloc (1, sizeof (hash_table_t));
    HashTableInit (hash_table, 10, 10, sizeof (hash_table_elem_t), HashCrc32Intrinsic);

    //---------------------------------------------

    HashTableInsertsTest (hash_table, array);

    //---------------------------------------------

    free (array); array = NULL;
    HashTableDestroy (hash_table);
}

//-------------------------------------------------

// void HashTableInsertsTest (hash_table_t* hash_table, const char* array, size_t array_size)
// {
//     assert (hash_table != NULL);
//     assert (array != NULL);
//     assert (array_size > 0);


// }

//-------------------------------------------------

size_t ReadFile (FILE* input_file, char** array)
{
    assert (input_file != NULL);
    assert (array != NULL);
    
    // Find file size
    fseek (input_file, 0L, SEEK_END);
    size_t size_input_file = (size_t) ftell(input_file);
    fseek(input_file, 0L, SEEK_SET);

    *array = (char*) calloc (size_input_file, sizeof(char));
    CUSTOM_ASSERT (*array != NULL);

    size_t n_readen = fread (*array, sizeof (char), size_input_file, input_file);
    CUSTOM_ASSERT (size_input_file == n_readen);

    return size_input_file;
}

//-------------------------------------------------

