/**
 * @file memtime.c
 * @brief User space program to measure time spent for different type of memory allocators
 */

#include <assert.h>
#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* Max n for 2^n to allocate the buffer */
#define KMEMTIME_TESTS_MAX_N   23

enum kmem_type
{
    e_malloc,
    e_calloc,
    e_alloca,
};

struct kmemtime
{
    enum kmem_type type;
    const char *alloc_name;
    const char *free_name;
    long buff_size[KMEMTIME_TESTS_MAX_N];
    long time_alloc[KMEMTIME_TESTS_MAX_N];
    long time_free[KMEMTIME_TESTS_MAX_N];
};

static struct kmemtime memtime_test[] =
{
    { .type = e_malloc, .alloc_name = "malloc", .free_name = "free" },
    { .type = e_calloc, .alloc_name = "calloc", .free_name = "free" },
    { .type = e_alloca, .alloc_name = "alloca", .free_name = "-   " },
};

static void *memtime_get_mem(enum kmem_type type, long buff_size, long *time)
{
    struct timespec alloc_start;
    struct timespec alloc_end;
    void *mem_ptr = NULL;

    switch (type)
    {
    case e_malloc:
        clock_gettime(CLOCK_MONOTONIC, &alloc_start);
        mem_ptr = malloc(buff_size);
        clock_gettime(CLOCK_MONOTONIC, &alloc_end);
        break;

    case e_alloca:
        clock_gettime(CLOCK_MONOTONIC, &alloc_start);
        mem_ptr = alloca(buff_size);
        clock_gettime(CLOCK_MONOTONIC, &alloc_end);
        break;

    case e_calloc:
        clock_gettime(CLOCK_MONOTONIC, &alloc_start);
        mem_ptr = calloc(buff_size, sizeof(char));
        clock_gettime(CLOCK_MONOTONIC, &alloc_end);
        break;

    default:
        assert(0);
        break;
    }

    *time = (mem_ptr != NULL) ? (alloc_end.tv_nsec - alloc_start.tv_nsec) : -1;

    return mem_ptr;
}

static void memtime_free(enum kmem_type type, void *mem_ptr, long *time)
{
    struct timespec free_start;
    struct timespec free_end;

    if (mem_ptr == NULL)
    {
        *time = -1;
        return;
    }

    switch (type)
    {
    case e_malloc:
        clock_gettime(CLOCK_MONOTONIC, &free_start);
        free(mem_ptr);
        clock_gettime(CLOCK_MONOTONIC, &free_end);
        *time = free_end.tv_nsec - free_start.tv_nsec;
        break;

    case e_alloca:
        // Freed automatically
        *time = 0;
        break;

    case e_calloc:
        clock_gettime(CLOCK_MONOTONIC, &free_start);
        free(mem_ptr);
        clock_gettime(CLOCK_MONOTONIC, &free_end);
        *time = free_end.tv_nsec - free_start.tv_nsec;
        break;

    default:
        assert(0);
        break;
    }
}

static void memtime_get_result(struct kmemtime *result)
{
    int *test_ptr;
    int buff_size;

    for (int i = 0; i < KMEMTIME_TESTS_MAX_N; ++i)
    {
        buff_size            = pow(2, i);
        result->buff_size[i] = buff_size;

        test_ptr = memtime_get_mem(result->type, buff_size, &result->time_alloc[i]);
        memtime_free(result->type, test_ptr, &result->time_free[i]);
    }
}

int main(void)
{
    const int test_struct_size = sizeof(memtime_test) / sizeof(memtime_test[0]);

    for (int i = 0; i < test_struct_size; ++i)
    {
        memtime_get_result(&memtime_test[i]);
    }

    printf("Buffer size, bytes | Function name | Allocation time, ns | Deallocation time, ns |\n");

    for (int i = 0; i < KMEMTIME_TESTS_MAX_N; ++i)
    {
        for (int j = 0; j < test_struct_size; ++j)
        {
            printf("%18ld | %13s | %19ld | %21ld |\n",
                memtime_test[j].buff_size[i],
                memtime_test[j].alloc_name,
                memtime_test[j].time_alloc[i], memtime_test[j].time_free[i]);
        }
    }

    return 0;
}
