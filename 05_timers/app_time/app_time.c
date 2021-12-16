#include <time.h>
#include <stdio.h>

int print_time(clockid_t id, char *name) {
    struct timespec tp;
    int ret;

    ret = clock_gettime(id, &tp);
    if (ret < 0)
        return ret;

    printf("Get time with id %s: %ld.%ld seconds", name, tp.tv_sec, tp.tv_nsec);

    ret = clock_getres(id, &tp);
    if (ret < 0)
        return ret;

    printf(" and time resolution is %ld nsec\n", tp.tv_nsec);

    return 0;
}

int main() {
    int ret = 0;
    
    ret = print_time(CLOCK_REALTIME, "CLOCK_REALTIME");
    if (ret < 0)
        return ret;

    ret = print_time(CLOCK_REALTIME_COARSE, "CLOCK_REALTIME_COARSE");
    if (ret < 0)
        return ret;

    ret = print_time(CLOCK_MONOTONIC, "CLOCK_MONOTONIC");
    if (ret < 0)
        return ret;

    ret = print_time(CLOCK_MONOTONIC_COARSE, "CLOCK_MONOTONIC_COARSE");
    if (ret < 0)
        return ret;

    ret = print_time(CLOCK_MONOTONIC_RAW, "CLOCK_MONOTONIC_RAW");
    if (ret < 0)
        return ret;

    ret = print_time(CLOCK_BOOTTIME, "CLOCK_BOOTTIME");

    return ret;
}
