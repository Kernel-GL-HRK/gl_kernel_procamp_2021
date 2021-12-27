//#include <stdio.h>
//#include <stdlib.h>
#include <malloc.h>
#include <ratio>
#include <chrono>
#include <iostream>

using namespace std::chrono;

static size_t size = 1;

static std::chrono::time_point<std::chrono::steady_clock> start =
    std::chrono::steady_clock::now();

void StartTimer()
{
    start = std::chrono::steady_clock::now();
}

void StopTimer(const char * name_timer)
{
    const auto end = std::chrono::steady_clock::now();
    std::cout
          << name_timer << ": It took  "
          << (end - start) / 1s << " s, "
          << (end - start) / 1ms << " ms,  "
          << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs, "
          << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns, "
          << " size " << size << "\n";
}


int main(void)
{
    int counter=1000;

    while(1)
    {
        StartTimer();
        char *buf = (char*)malloc(size);
        StopTimer("malloc");

        if(!buf)
        {
            std::cout << "Error: size - " << size << std::endl;
            break;
        }
        else {
            //printf( "malloc size - %ld\n" , size);
            free(buf);
        }


        StartTimer();
        buf = (char*)calloc(1, size);
        StopTimer("calloc");

        if(!buf)
        {
            std::cout << "Error: size - " << size << std::endl;
            break;
        }
        else {
            //printf( "calloc size - %ld\n" , size);
            free(buf);
        }

        StartTimer();
        buf = (char *)alloca(size);
        StopTimer("alloca");

        if(!buf)
        {
            std::cout << "Error: size - " << size << std::endl;
            break;
        }
        else {
            //printf( "alloca size - %ld\n" , size);
        }

        size*=2;

        if(!counter--)
            break;
    }
    return 0;
}
