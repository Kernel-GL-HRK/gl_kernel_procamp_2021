
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUF_LENGTH		(99)
#define TO_STRING(x)	(#x)
#define CLOCK_ID_MAX	(8)

struct clock_id {
	int clock_id_num;
	const char * const clock_id_name;
};

int main(int argc, char *argv[])
{
	struct timespec time;
	int result = 0;
	static const struct clock_id clocks[CLOCK_ID_MAX] = {
				{CLOCK_REALTIME, TO_STRING(CLOCK_REALTIME)},
				{CLOCK_REALTIME_COARSE, TO_STRING(CLOCK_REALTIME_COARSE)},
				{CLOCK_MONOTONIC, TO_STRING(CLOCK_MONOTONIC)},
				{CLOCK_MONOTONIC_COARSE, TO_STRING(CLOCK_MONOTONIC_COARSE)},
				{CLOCK_MONOTONIC_RAW, TO_STRING(CLOCK_MONOTONIC_RAW)},
				{CLOCK_BOOTTIME, TO_STRING(CLOCK_BOOTTIME)},
				{CLOCK_PROCESS_CPUTIME_ID, TO_STRING(CLOCK_PROCESS_CPUTIME_ID)},
				{CLOCK_THREAD_CPUTIME_ID, TO_STRING(CLOCK_THREAD_CPUTIME_ID)},
			};

	for (int i = 0; i < CLOCK_ID_MAX; i++) {
		result = clock_gettime(clocks[i].clock_id_num, &time);
		if (result != 0)
			printf("Error %d: can't get current time\n", result);

		printf("%s time is: %ld.%ld seconds\n", clocks[i].clock_id_name,
				time.tv_sec,
				time.tv_nsec);
		result = clock_getres(clocks[i].clock_id_num, &time);
		if (result != 0)
			printf("Error %d: can't get current time", result);

		printf("Resolution of %s is %ld nanosec\n\n", clocks[i].clock_id_name,
				time.tv_nsec);
	}

	return 0;
}

