#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// https://linux.die.net/man/3/clock_gettime

const char* clockid_to_string(clockid_t id)
{
    switch(id){

        case CLOCK_REALTIME:
            return "CLOCK_REALTIME";
        break;
        case CLOCK_MONOTONIC:
            return "CLOCK_MONOTONIC";
        break;
        case CLOCK_PROCESS_CPUTIME_ID:
            return "CLOCK_PROCESS_CPUTIME_ID";
        break;
        case CLOCK_THREAD_CPUTIME_ID:
            return "CLOCK_THREAD_CPUTIME_ID";
        break;
        default:
            return "UNDEFINED";
        break;
    }
    
}

void show_time_source_info(clockid_t clock_id)
{
    struct timespec timespec;
    memset(&timespec,0,sizeof(struct timespec));

    int result = clock_getres(clock_id,&timespec);
    assert(!result);

    printf("Clocksource: %s, seconds:%ld, nanoseconds:%ld \n",clockid_to_string(clock_id), timespec.tv_sec, timespec.tv_nsec);

    struct tm* absolute_time = localtime(&timespec.tv_sec);
    printf("ABSOLUTE TIME: %d:%d:%d %d/%d/%d \n",absolute_time->tm_hour, absolute_time->tm_min, absolute_time->tm_sec, absolute_time->tm_mday,absolute_time->tm_mon,absolute_time->tm_year+1900);
}

int main()
{
    show_time_source_info(CLOCK_REALTIME);
    show_time_source_info(CLOCK_MONOTONIC);
    show_time_source_info(CLOCK_PROCESS_CPUTIME_ID);
    show_time_source_info(CLOCK_THREAD_CPUTIME_ID);

    return 0;
}