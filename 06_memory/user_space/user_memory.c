// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2022 Vasyl Yosypenko
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <time.h>

typedef enum alloc_modes{
    MALLOC_MODE = 0,
    CALLOC_MODE,
    ALLOCA_MODE
}alloc_modes_t;

// Implemented 2^N
uint64_t pow2(uint8_t n)
{
    return 1<<n;
}


void check_memory_allocation(alloc_modes_t alloc_mode)
{
    char* test_ptr = NULL;
    clock_t start_time;
    clock_t end_time;

    uint8_t order = 0;
    uint64_t bytes_numbers = 0;

    uint64_t alloc_time  = 0;
    uint64_t free_time  = 0;

    printf("\n========================================================\n");
    printf("|%5s|%15s|%15s|%15s|\n","N","Bytes","Alloc time(us)","Free time(us)");
    printf("========================================================\n");

    do
    {
        bytes_numbers = pow2(order);

        switch(alloc_mode)
        {
        case MALLOC_MODE:
            start_time = clock();
            test_ptr = (char*)malloc(bytes_numbers * sizeof(char));
            end_time = clock();
            alloc_time = end_time - start_time;
            break;

        case CALLOC_MODE:
            start_time = clock();
            test_ptr = (char*)calloc(bytes_numbers, sizeof(char));
            end_time = clock();
            alloc_time = end_time - start_time;
            break;

        case ALLOCA_MODE:
            start_time = clock();
            test_ptr = (char*)alloca(bytes_numbers * sizeof(char));
            end_time = clock();
            alloc_time = end_time - start_time;
            break;
			
		default:
			printf("Incorrect memory allocation mode\n");
			break;
        }

        // Can't allocate memory, so exit from loop
		if (!test_ptr)
            break;
    
        // There is no standard way to clean up memory allocated by alloca
        if (alloc_mode != ALLOCA_MODE)
        {
            start_time = clock();
            free(test_ptr);
            end_time = clock();
            free_time = end_time - start_time;
        }

        printf("|%5d|%15lu|%15lu|%15lu|\n", order, bytes_numbers, alloc_time, free_time);

        order++;

    } while (test_ptr);
}

int main()
{

    printf("\nCheck memory allocation with malloc:");
    check_memory_allocation(MALLOC_MODE);

    printf("\nCheck memory allocation with calloc:");
    check_memory_allocation(CALLOC_MODE);

    printf("\nCheck memory allocation with alloca:");
    check_memory_allocation(ALLOCA_MODE);

    return 0;
}
