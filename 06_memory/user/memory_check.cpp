#include <stdio.h>
#include <limits>
#include <chrono>
#include <cstdlib>
#include <new>

void test_alloca() 
{
    unsigned long x = 0;
    for (unsigned long i = 0; i < std::numeric_limits<unsigned long>::max(); i++) {
        x = x == 0 ? 1 : x * 2;
        auto start = std::chrono::high_resolution_clock::now();
        char *mem = (char*)alloca(x);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        if (mem == NULL) {
            printf("%lu\tAlloca\t%lu\t\tAllocation error\t%lu\n", i, duration.count(), x);
            break;
        }
        mem[0] = '!';
        if (mem[0] != '!') {
            printf("%lu\tAlloca\t%lu\t\t%lu\tMemory check error\n", i, duration.count(), x);
            break;
        }

        // free is not needed for alloca
        // auto start_free = std::chrono::high_resolution_clock::now();
        // free(mem);
        // auto stop_free = std::chrono::high_resolution_clock::now();
        // auto duration_free = std::chrono::duration_cast<std::chrono::microseconds>(stop_free - start_free);

        printf("%lu\tAlloca\t%lu\t\t-\t%lu\n", i, duration.count(), x);
    }
}

void test_calloc() 
{
    unsigned long x = 0;
    for (unsigned long i = 0; i < std::numeric_limits<unsigned long>::max(); i++) {
        x = x == 0 ? 1 : x * 2;
        auto start = std::chrono::high_resolution_clock::now();
        char *mem = (char*)calloc(x, sizeof(char));
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        if (mem == NULL) {
            printf("%lu\tCalloc\t%lu\t\tAllocation error\t%lu\n", i, duration.count(), x);
            break;
        }
        mem[0] = '!';
        if (mem[0] != '!') {
            printf("%lu\tCalloc\t%lu\t\t%lu\tMemory check error\n", i, duration.count(), x);
            break;
        }

        auto start_free = std::chrono::high_resolution_clock::now();
        free(mem);
        auto stop_free = std::chrono::high_resolution_clock::now();
        auto duration_free = std::chrono::duration_cast<std::chrono::microseconds>(stop_free - start_free);

        printf("%lu\tCalloc\t%lu\t\t%lu\t%lu\n", i, duration.count(), duration_free.count(), x);
    }
}

void test_malloc() 
{
    unsigned long x = 0;
    for (unsigned long i = 0; i < std::numeric_limits<unsigned long>::max(); i++) {
        x = x == 0 ? 1 : x * 2;
        auto start = std::chrono::high_resolution_clock::now();
        char *mem = (char*)malloc(x);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        if (mem == NULL) {
            printf("%lu\tMalloc\t%lu\t\tAllocation error\t%lu\n", i, duration.count(), x);
            break;
        }
        mem[0] = '!';
        if (mem[0] != '!') {
            printf("%lu\tMalloc\t%lu\t\t%lu\tMemory check error\n", i, duration.count(), x);
            break;
        }

        auto start_free = std::chrono::high_resolution_clock::now();
        free(mem);
        auto stop_free = std::chrono::high_resolution_clock::now();
        auto duration_free = std::chrono::duration_cast<std::chrono::microseconds>(stop_free - start_free);

        printf("%lu\tMalloc\t%lu\t\t%lu\t%lu\n", i, duration.count(), duration_free.count(), x);
    }
}

void test_new() 
{
    unsigned long x = 0;
    for (unsigned long i = 0; i < std::numeric_limits<unsigned long>::max(); i++) {
        x = x == 0 ? 1 : x * 2;
        auto start = std::chrono::high_resolution_clock::now();
        char *mem;
        try {
            mem = new char[x];
        } catch(std::bad_alloc& ba) {
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            printf("%lu\tNew\t%lu\t\tAllocation error\t%lu\n", i, duration.count(), x);
            break;
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        if (mem == NULL) {
            printf("%lu\tNew\t%lu\t\tAllocation error\t%lu\n", i, duration.count(), x);
            break;
        }
        mem[0] = '!';
        if (mem[0] != '!') {
            printf("%lu\tNew\t%lu\t\t%lu\tMemory check error\n", i, duration.count(), x);
            break;
        }

        auto start_free = std::chrono::high_resolution_clock::now();
        delete[] mem;
        auto stop_free = std::chrono::high_resolution_clock::now();
        auto duration_free = std::chrono::duration_cast<std::chrono::microseconds>(stop_free - start_free);

        printf("%lu\tNew\t%lu\t\t%lu\t%lu\n", i, duration.count(), duration_free.count(), x);
    }
}

int main(int argc, char const *argv[])
{
    printf("#\tType\tAllocation\tFree\tBuf size\n");
    test_malloc();
    
    printf("\n");
    printf("#\tType\tAllocation\tFree\tBuf size\n");
    test_calloc();
   
    printf("\n");
    printf("#\tType\tAllocation\tFree\tBuf size\n");
    test_new();

    printf("\n");
    printf("#\tType\tAllocation\tFree\tBuf size\n");
    test_alloca();

    printf("The End :)\n");
    return 0;
}
