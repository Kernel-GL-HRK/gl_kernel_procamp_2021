// SPDX-License-Identifier: 0BSD
/**
 * Description: My Time
 * Author:      Nazarii Kurylko
 * Created:     09.12.2021
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// see https://man7.org/linux/man-pages/man3/clock_gettime.3.html for details

clockid_t clk_id_arr[] = { CLOCK_REALTIME,	  CLOCK_REALTIME_ALARM,
			   CLOCK_REALTIME_COARSE,   CLOCK_TAI,
			   CLOCK_MONOTONIC,	 CLOCK_MONOTONIC_RAW,
			   CLOCK_MONOTONIC_COARSE,  CLOCK_PROCESS_CPUTIME_ID,
			   CLOCK_THREAD_CPUTIME_ID, CLOCK_BOOTTIME,
			   CLOCK_BOOTTIME_ALARM };

char *clk_id_arr_str[] = {
	"CLOCK_REALTIME",	       "CLOCK_REALTIME_ALARM",
	"CLOCK_REALTIME_COARSE",   "CLOCK_TAI",
	"CLOCK_MONOTONIC",	       "CLOCK_MONOTONIC_RAW",
	"CLOCK_MONOTONIC_COARSE",  "CLOCK_PROCESS_CPUTIME_ID",
	"CLOCK_THREAD_CPUTIME_ID", "CLOCK_BOOTTIME",
	"CLOCK_BOOTTIME_ALARM"
};

int timespec2str(char *buf, uint len, struct timespec *ts)
{
	int ret;
	struct tm t;

	tzset();
	if (localtime_r(&(ts->tv_sec), &t) == NULL)
		return 1;

	ret = strftime(buf, len, "%F %T", &t);
	if (ret == 0)
		return 2;
	len -= ret - 1;

	ret = snprintf(&buf[strlen(buf)], len, ".%09ld", ts->tv_nsec);
	if (ret >= len)
		return 3;

	return 0;
}

/*main algorithm*/
int main(int argc, char **argv)
{
	static int i;
	static struct timespec mytime, mytimeres;
	char tmp[100];

	for (i = 0; i < ARRAY_SIZE(clk_id_arr); ++i) {
		//print clock output
		if (!clock_gettime(clk_id_arr[i], &mytime))
			printf("%s: time: %ld sec, %ld nsec\n",
			       clk_id_arr_str[i], mytime.tv_sec,
			       mytime.tv_nsec);
		//print time in formated string
		timespec2str(tmp, sizeof(tmp), &mytime);
		printf("%s: %s\n", clk_id_arr_str[i], tmp);

		//print clock resolution
		if (!clock_getres(clk_id_arr[i], &mytimeres))
			printf("%s: resolution: %ld sec, %ld nsec\n",
			       clk_id_arr_str[i], mytimeres.tv_sec,
			       mytimeres.tv_nsec);
		printf("==================================\n");
	}

	return 0;
}
