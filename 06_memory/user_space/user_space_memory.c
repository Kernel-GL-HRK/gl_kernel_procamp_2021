// SPDX-License-Identifier: GPL-2.0
/*
 * Create user-space C or C++ program which tries to allocate
 * buffers with sizes 2^x for x in range from 0 to maximium possible
 * value using functions: **malloc, calloc, alloca,
 * (optional for C++) new **.
 * Measure time of each allocation/freeing.
 *
 * 2^x means x power of 2 in this task.
 *
 * Pull request should contains program source code and program output in text format.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <alloca.h>

#ifndef CALL_ALLOCATOR
#define CALL_ALLOCATOR 1
#endif

#ifndef CALL_FREE
#define CALL_FREE 1
#endif


#if CALL_ALLOCATOR == 3
#define MAX_POW 23
#else
#define MAX_POW 100
#endif

char * alloca_wrapper(size_t size){
    return (char *)alloca(size);
}


int main(void)
{
    unsigned long allocate_start, free_start, allocate_took, free_took, allocate_total, free_total;
    int power = 0;
    size_t allocate_size = 0;
    char *ptr = NULL;

    allocate_total = 0;
    free_total = 0;

    printf("----------------------------------------------------------------------\n");
#if CALL_ALLOCATOR == 1
    printf("use malloc, call free=%d\n", CALL_FREE);
#elif CALL_ALLOCATOR == 2
    printf("use calloc, call free=%d\n", CALL_FREE);
#elif CALL_ALLOCATOR == 3
    printf("use alloca, should not call free\n");
#endif
    printf("----------------------------------------------------------------------\n");
    printf("|%-5s|%-20s|%-20s|%-20s|\n", "pow", "size", "allocate took", "free took");
    printf("----------------------------------------------------------------------\n");
    for (power=1, allocate_size=1; power<=MAX_POW; allocate_size *= 2, power++){
        allocate_start = clock();
#if CALL_ALLOCATOR == 1
        ptr = (char *)malloc(allocate_size);
#elif CALL_ALLOCATOR == 2
        ptr = (char *)calloc(allocate_size, 1);
#elif CALL_ALLOCATOR == 3
        ptr = alloca_wrapper(allocate_size);
#endif
        if (ptr == NULL){
            break;
        }
        allocate_took = clock() - allocate_start;
        allocate_total += allocate_took;
        free_start = clock();
#if CALL_FREE == 1 && CALL_ALLOCATOR != 3
        free(ptr);
#endif
        free_took = clock() - free_start;
        free_total += free_took;
        printf("|%-5i|%-20lu|%-20lu|%-20lu|\n", power, allocate_size, allocate_took, free_took);
    }
    printf("|%-5s|%-20s|%-20lu|%-20lu|\n", "total", "-", allocate_total, free_total);
    printf("----------------------------------------------------------------------\n");
    return 0;
}
