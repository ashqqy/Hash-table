#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>

void HashTableBenchmark (const char* input_file_name);

size_t ReadFile (FILE* input_file, char** array);

#endif // BENCHMARK_H
