#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>

#include "hash-table.h"

typedef struct 
{
    char* array;
    size_t array_size;
    size_t n_words;
} input_data_t;

void HashTableBenchmark (const char* words_for_insert_file_name, const char* words_for_find_file_name);

void HashTableInsertTest (hash_table_t* hash_table, input_data_t* words_for_insert);
void HashTableFindTest (hash_table_t* hash_table, input_data_t* words_for_find);

input_data_t* ReadFile (const char* input_file_name, input_data_t* input_data);

#endif // BENCHMARK_H
