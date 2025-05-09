#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"

int main (int argc, const char* argv[])
{
    if (argc != 3)
    {
        fprintf (stderr, "Expected input data for tests\n");
        exit (EXIT_FAILURE);
    }

    HashTableBenchmark (argv[1], argv[2]);
}
