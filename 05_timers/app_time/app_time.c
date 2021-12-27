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

	double ns = tp.tv_nsec / 1000000000.0;
	double abs_time = tp.tv_sec + ns;
    printf("Time is %.9f seconds, resolution is %ld nsec. Time local: %s", abs_time, tr.tv_nsec, ctime(&tp.tv_sec));

    return 0;
}
