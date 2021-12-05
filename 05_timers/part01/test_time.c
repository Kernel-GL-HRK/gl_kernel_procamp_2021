#include <stdio.h>
#include <time.h>

#define SECS_IN_DAY (24 * 60 * 60)

void show_time(clockid_t clock, const char *name)
{
    struct timespec ts, ts2;
    clock_gettime(clock, &ts);
    clock_getres(clock, &ts2);
    long days = ts.tv_sec / SECS_IN_DAY;
    printf("%s:\n\t\t %10ld.%ld\t resolution: %10jd.%09ld\t", name, ts.tv_sec, ts.tv_nsec, ts2.tv_sec, ts2.tv_nsec);
    printf("( %ld days + %2dh %2dm %2ds )\n",  days,
                                            (int) (ts.tv_sec % SECS_IN_DAY) / 3600,
                                            (int) (ts.tv_sec % 3600) / 60,
                                            (int) ts.tv_sec % 60);

}

int main (void)
{
    show_time(CLOCK_REALTIME, "CLOCK_REALTIME");
    show_time(CLOCK_TAI, "CLOCK_TAI");
    show_time(CLOCK_MONOTONIC, "CLOCK_MONOTONIC");
    show_time(CLOCK_BOOTTIME, "CLOCK_BOOTTIME");

    return 0;
}
