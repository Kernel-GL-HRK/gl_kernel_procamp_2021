#include <time.h>
#include <stdio.h>




int main()
{
    struct timespec tp1, tp2;

    clock_gettime( CLOCK_REALTIME, &tp1 );
    clock_getres( CLOCK_REALTIME, &tp2 );
    printf ( "%s| CLOCK_REALTIME = %ld| resolution =%ld (Nanoseconds)\n",ctime(&tp1.tv_sec), tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_MONOTONIC, &tp1 );
    clock_getres( CLOCK_MONOTONIC, &tp2 );
    printf( "CLOCK_MONOTONIC = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &tp1 );
    clock_getres( CLOCK_PROCESS_CPUTIME_ID, &tp2 );
    printf( "CLOCK_PROCESS_CPUTIME_ID = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_THREAD_CPUTIME_ID, &tp1 );
    clock_getres( CLOCK_THREAD_CPUTIME_ID, &tp2 );
    printf( "CLOCK_THREAD_CPUTIME_ID = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_MONOTONIC_RAW, &tp1 );
    clock_getres( CLOCK_MONOTONIC_RAW, &tp2 );
    printf( "CLOCK_MONOTONIC_RAW = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_REALTIME_COARSE, &tp1 );
    clock_getres( CLOCK_REALTIME_COARSE, &tp2 );
    printf( "CLOCK_REALTIME_COARSE = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_MONOTONIC_COARSE, &tp1 );
    clock_getres( CLOCK_MONOTONIC_COARSE, &tp2 );
    printf( "CLOCK_MONOTONIC_COARSE = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_BOOTTIME, &tp1 );
    clock_getres( CLOCK_BOOTTIME, &tp2 );
    printf( "CLOCK_BOOTTIME = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_REALTIME_ALARM, &tp1 );
    clock_getres( CLOCK_REALTIME_ALARM, &tp2 );
    printf( "CLOCK_BOOTTIME = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );

    clock_gettime( CLOCK_TAI, &tp1 );
    clock_getres( CLOCK_TAI, &tp2 );
    printf( "CLOCK_TAI = %ld| resolution =%ld (Nanoseconds)\n", tp1.tv_sec, tp2.tv_nsec );



}







