#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	struct timespec	ts, ts_loc;
	struct tm	*loctime;

	printf("\n  Absolute time            Local time\n");
	printf(" tv_nsec     tv_sec        date      time\n");
	//switch off cursor
	printf("\033[?25l");

	while (1) {
		clock_gettime(CLOCK_REALTIME, &ts);
		// clock_gettime(CLOCK_MONOTONIC, &ts);
		//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
		//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
		printf("%10ld  %10ld  ", ts.tv_nsec, ts.tv_sec);

		clock_gettime(CLOCK_REALTIME, &ts_loc);
		loctime = localtime((time_t *)&ts_loc.tv_sec);

		printf("%02d-%02d-%04d %02d:%02d:%02d",
					loctime->tm_mday,
					loctime->tm_mon+1,
					loctime->tm_year+1900,
					loctime->tm_hour,
					loctime->tm_min,
					loctime->tm_sec);

		usleep(1000);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	}
	printf("\n");
	return 0;
}