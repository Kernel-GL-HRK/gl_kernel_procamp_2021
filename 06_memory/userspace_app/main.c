#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum profiling_alloc
{
    malloc_allocator,
    calloc_allocator,
} profiling_alloc_t;


void profile_alloc(profiling_alloc_t profiling_mode)
{

    uint64_t alloc_request = 0;
    do
    {
        clock_t begin_alloc = clock();
        void* p_allocated_mem = NULL;
        switch(profiling_mode)
        {
            case malloc_allocator:
            p_allocated_mem = malloc(alloc_request);
            break;
            case calloc_allocator:
            p_allocated_mem = calloc(alloc_request,1);
            break;
            default:
            break;
        }
        clock_t end_alloc = clock();

        double time_spent = (double)(end_alloc - begin_alloc) / CLOCKS_PER_SEC;

        if(!p_allocated_mem){
            printf("Allocation of %ld bytes failed, return. Time spent:%lf \n", alloc_request, time_spent);
            return;
        }

        clock_t begin_free = clock();
        free(p_allocated_mem);
        clock_t end_free = clock();
        double time_spent_free = (double)(end_alloc - begin_alloc) / CLOCKS_PER_SEC;

        printf("Allocation of %ld bytes completed. Time spent:%lf \n",alloc_request,time_spent);
        printf("De-Allocation of %ld bytes completed. Time spent:%lf \n",alloc_request,time_spent_free);

    alloc_request = alloc_request == 0 ? 1: alloc_request* 2;
    }
    while(true);
}

bool profile_alloca_impl(uint64_t request_mem)
{
    int stack_impl_protector = 24;
    void* p_allocated = alloca(request_mem);
    if(p_allocated)
        return true;
    return false;
}
void profile_alloca()
{
    int stack_guard = 42;
    uint64_t alloc_size = 0;
    do{
        clock_t begin_alloc = clock();
        if(!profile_alloca_impl(alloc_size))
        {
            break;
        }
        else{
            clock_t end_alloc = clock();
            double time_spent = (double)(end_alloc - begin_alloc) / CLOCKS_PER_SEC;


            printf("alloca request completed. Bytes: %ld, Time:%lf\n", alloc_size,time_spent);
            alloc_size = alloc_size == 0? 1: alloc_size*2;
        }
    }while(true);
}

int main()
{
    printf("PROFILE MALLOC BEGIN\n");
    profile_alloc(malloc_allocator);
    printf("PROFILE MALLOC END\n");

    printf("PROFILE CALLOC BEGIN\n");
    profile_alloc(calloc_allocator);
    printf("PROFILE CALLOC END\n");


    printf("PROFILE ALLOCA BEGIN\n");
    profile_alloca();
    return 0;
}
