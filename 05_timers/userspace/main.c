#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

void print_clock_details(clockid_t id, char *label) {
    printf("Clock - %s\n", label);
    struct timespec tp;
    int result = 0;
    result = clock_gettime(id, &tp);
    if (result != 0) {
        printf("Error clock_gettime: %s", strerror(errno));
    }
    printf("\ttime sec  %ld\n", tp.tv_sec);
    printf("\ttime nsec  %ld\n", tp.tv_nsec);
    
    result = clock_getres(id, &tp);
    if (result != 0) {
        printf("Error clock_getres: %s", strerror(errno));
    }
    printf("\tres sec  %ld\n", tp.tv_sec);
    printf("\trs nsec  %ld\n", tp.tv_nsec);
}

// immitation of calculations, to show THREAD_CPUTIME and PROCESS_CPUTIME
void calculate() {
    int i = 1;
    while (i > 0)
    {
        i++;
    }
}

int main() {
    calculate();
    
    clockid_t clockid_array[] = {
        CLOCK_REALTIME,
        CLOCK_REALTIME_ALARM,
        CLOCK_REALTIME_COARSE,
        CLOCK_TAI,
        CLOCK_MONOTONIC,
        CLOCK_MONOTONIC_COARSE,
        CLOCK_MONOTONIC_RAW,
        CLOCK_BOOTTIME,
        CLOCK_BOOTTIME_ALARM,
        CLOCK_PROCESS_CPUTIME_ID,
        CLOCK_THREAD_CPUTIME_ID
    };
    char* clockid_labels_array[] = {
        "CLOCK_REALTIME",
        "CLOCK_REALTIME_ALARM",
        "CLOCK_REALTIME_COARSE",
        "CLOCK_TAI",
        "CLOCK_MONOTONIC",
        "CLOCK_MONOTONIC_COARSE",
        "CLOCK_MONOTONIC_RAW",
        "CLOCK_BOOTTIME",
        "CLOCK_BOOTTIME_ALARM",
        "CLOCK_PROCESS_CPUTIME_ID",
        "CLOCK_THREAD_CPUTIME_ID"
    };

    int size = sizeof clockid_array / sizeof clockid_array[0];
    for (int i = 0; i < size; i++) {
        print_clock_details(clockid_array[i], clockid_labels_array[i]);
    }

    return 0;
}