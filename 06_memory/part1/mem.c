#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <alloca.h>






int main() {



    struct timespec tp1, tp2, tp_res;

    unsigned long int size;
    char *p_alloc;
    struct tm *my_tm;   
//malloc
    for( size = 1; ; size *= 2 )
    {
        clock_gettime( CLOCK_REALTIME, &tp1 );
        p_alloc = (char *)malloc( size * sizeof(char));
        if( !p_alloc )
        {
            printf("~~~~~Error in allocating %ld bites using %s\n", size, "malloc");
            break;
        }
        p_alloc[ 0 ] = 'a';
        p_alloc[ size - 1 ] = 'a';
        clock_gettime( CLOCK_REALTIME, &tp2 );
        free( p_alloc );

        tp_res.tv_sec = tp2.tv_sec - tp1.tv_sec;
        tp_res.tv_nsec = tp2.tv_nsec - tp1.tv_nsec;
        my_tm = localtime(&tp_res.tv_sec); 

        printf("allocate %ld bites in %d:%ld sec:nsec using %s\n", size, my_tm->tm_sec, tp_res.tv_nsec, "malloc");
    } 
//calloc
    for( size = 1; ; size *= 2 )
    {
        clock_gettime( CLOCK_REALTIME, &tp1 );
        p_alloc = (char *)calloc( size, sizeof(char));
        if( !p_alloc )
        {
            printf("~~~~~Error in allocating %ld bites using %s\n", size, "calloc");
            break;
        }
        p_alloc[ 0 ] = 'a';
        p_alloc[ size - 1 ] = 'a';
        clock_gettime( CLOCK_REALTIME, &tp2 );
        free( p_alloc );

        tp_res.tv_sec = tp2.tv_sec - tp1.tv_sec;
        tp_res.tv_nsec = tp2.tv_nsec - tp1.tv_nsec;
        my_tm = localtime(&tp_res.tv_sec); 

        printf("allocate %ld bites in %d:%ld sec:nsec using %s\n", size, my_tm->tm_sec, tp_res.tv_nsec, "calloc");
    } 

//alloca
    for( size = 1; ; size *= 2 )
    {
        clock_gettime( CLOCK_REALTIME, &tp1 );
        p_alloc = (char *)alloca( size * sizeof(char));
        if( !p_alloc )
        {
            printf("~~~~~Error in allocating %ld bites using %s\n", size, "calloc");
            break;
        }
        p_alloc[ 0 ] = 'a';
        p_alloc[ size - 1 ] = 'a';
        clock_gettime( CLOCK_REALTIME, &tp2 );

        tp_res.tv_sec = tp2.tv_sec - tp1.tv_sec;
        tp_res.tv_nsec = tp2.tv_nsec - tp1.tv_nsec;
        my_tm = localtime(&tp_res.tv_sec); 

        printf("allocate %ld bites in %d:%ld sec:nsec using %s\n", size, my_tm->tm_sec, tp_res.tv_nsec, "calloc");
    } 

}






