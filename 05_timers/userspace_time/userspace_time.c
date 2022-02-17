// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define REALTIME 1
#define BOOTTIME 2
#define PROCESS_CPUTIME_ID 3
#define THREAD_CPUTIME_ID 4

int main(int argc, char *argv[])

	struct timespec timer;
	struct timespec precision;

	if (argc > 1) {
		int arg = atoi(argv[1]);

		switch (arg) {
		case REALTIME:
			clock_gettime(CLOCK_REALTIME, &timer);
			clock_getres(CLOCK_REALTIME, &precision);
			break;
		case BOOTTIME:
			clock_gettime(CLOCK_BOOTTIME, &timer);
			clock_getres(CLOCK_BOOTTIME, &precision);
			break;
		case PROCESS_CPUTIME_ID:
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer);
			clock_getres(CLOCK_PROCESS_CPUTIME_ID, &precision);
			break;
		case THREAD_CPUTIME_ID:
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &timer);
			clock_getres(CLOCK_THREAD_CPUTIME_ID, &precision);
			break;
		default:
			perror("Uncompatible argument");
			return 1;
		}
		printf("%ld.%ld\n", timer.tv_sec, timer.tv_nsec);
		printf("%ld.%ld\n", precision.tv_sec, precision.tv_nsec);
	} else {
		perror("No arguments");
		return 1;
	}
	return 0;
}
