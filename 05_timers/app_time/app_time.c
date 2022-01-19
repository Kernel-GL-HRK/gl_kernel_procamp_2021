#include <time.h>
#include <stdio.h>

int main() {
    struct timespec tp, tr;
    int ret;

    ret = clock_gettime(CLOCK_REALTIME, &tp);
    if (ret < 0)
        return ret;

    ret = clock_getres(CLOCK_REALTIME, &tr);
    if (ret < 0)
        return ret;

    unsigned int us = tp.tv_nsec / 1000; // cut to us, as double can hold with precision 1us. 32bit for integer and 20bit for fractional
    double s = us / 1000000.0;
    double abs_time = tp.tv_sec + s;
    printf("Time is %.6f seconds, possible resolution is %ld nsec. Time local: %s", abs_time, tr.tv_nsec, ctime(&tp.tv_sec));

    return 0;
}
