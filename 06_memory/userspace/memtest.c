// SPDX-License-Identifier: GPL-2.0-only

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static int pow2(unsigned int y)
{
	if (y == 0)
		return 1;
	else if (y % 2 == 0)
		return pow2(y / 2) * pow2(y / 2);
	else
		return 2 * pow2(y / 2) * pow2(y / 2);
}

static double timediff(struct timespec *time_stamp1,
		       struct timespec *time_stamp2)
{
	//todo add check for NULL pointer
	return (time_stamp2->tv_sec - time_stamp1->tv_sec) * 1e9 +
	       (time_stamp2->tv_nsec - time_stamp1->tv_nsec); // nanosec
}

static void alloca_test(size_t size, double *ptr_alloca_time,
			struct timespec *ptr_time_stamp_free_start)
{
	void *ptr = NULL;
	struct timespec time_stamp1, time_stamp2;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp1);
	ptr = alloca(size);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, ptr_time_stamp_free_start);

	if (ptr_alloca_time && ptr_time_stamp_free_start)
		*ptr_alloca_time =
			timediff(&time_stamp1, ptr_time_stamp_free_start);
}

/*main algorithm*/
int main(int argc, char **argv)
{
	struct timespec time_stamp1, time_stamp2;
	void *ptr = NULL;
	double malloc_time, malloc_free_time, calloc_free_time, calloc_time,
		alloca_time, alloca_free_time;
	int i = 0;

    printf("table with allocation/deallocation time:\n");
	printf("bytes       | malloc() | calloc() | alloca() | malloc_free() | calloc_free() | alloca_free()\n");
	printf("--------------------------------------------------------------------------------------------\n");
	for (i = 0; i < 32; ++i) {
		//malloc
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp1);
		ptr = malloc(pow2(i));
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp2);
		malloc_time = timediff(&time_stamp1, &time_stamp2);

		//malloc_free
		if (ptr) {
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp1);
			free(ptr);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp2);
			malloc_free_time = timediff(&time_stamp1, &time_stamp2);
		}

		//calloc
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp1);
		ptr = calloc(pow2(i), 1);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp2);
		calloc_time = timediff(&time_stamp1, &time_stamp2);

		//calloc free
		if (ptr) {
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp1);
			free(ptr);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp2);
			calloc_free_time = timediff(&time_stamp1, &time_stamp2);
		}
		if (i < 23) { //cant alocate more memory
			alloca_test(pow2(i), &alloca_time, &time_stamp1);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stamp2);
			alloca_free_time = timediff(&time_stamp1, &time_stamp2);
		} else {
			alloca_time = 0;
			alloca_free_time = 0;
		}
		printf("  %10u|%8.0f  |%8.0f  | %8.0f | %8.0f      | %8.0f      | %8.0f\n",
			pow2(i), malloc_time, calloc_time, alloca_time,
			malloc_free_time, calloc_free_time, alloca_free_time);
	}
	printf("* time is in nano seconds\n");
	return 0;
}
