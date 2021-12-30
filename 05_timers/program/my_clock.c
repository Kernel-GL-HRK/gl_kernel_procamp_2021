#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE(X)		(sizeof(X) / sizeof(X[0]))
#define SYS_CLOCK(_ID)		{.id = _ID, .name = #_ID}

struct system_clocks {
	const int id;
	const char *name;
};

const struct system_clocks system_clocks[] = {
		SYS_CLOCK(CLOCK_REALTIME),
		SYS_CLOCK(CLOCK_MONOTONIC),
		SYS_CLOCK(CLOCK_PROCESS_CPUTIME_ID),
		SYS_CLOCK(CLOCK_THREAD_CPUTIME_ID),
		SYS_CLOCK(CLOCK_MONOTONIC_RAW),
		SYS_CLOCK(CLOCK_REALTIME_COARSE),
		SYS_CLOCK(CLOCK_MONOTONIC_COARSE),
		SYS_CLOCK(CLOCK_BOOTTIME),
		SYS_CLOCK(CLOCK_REALTIME_ALARM),
		SYS_CLOCK(CLOCK_BOOTTIME_ALARM),
};

int main(int argc, char **argv)
{
	int i, ret;
	struct timespec tp;

	for (i = 0; i < ARRAY_SIZE(system_clocks); i++) {
		printf("%s:\n", system_clocks[i].name);

		ret = clock_getres(system_clocks[i].id , &tp);
		if (ret == -1) {
			printf("\tstatus: NOT SUPPORTED\n");
			continue;
		}

		printf("\tstatus: OK\n");
		printf("\tresolution   : %lu sec, %lu nsec\n", tp.tv_sec,  tp.tv_nsec);

		clock_gettime(system_clocks[i].id , &tp);
		printf("\tcurrent value: %lu sec, %lu nsec\n", tp.tv_sec,  tp.tv_nsec);
	}

	return 0;
}
