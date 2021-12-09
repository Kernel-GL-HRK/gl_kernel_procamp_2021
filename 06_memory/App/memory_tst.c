/*
 * Application for test timing of memory
 * allocation/deallocation functions
 *
 */
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <alloca.h>
#include <limits.h>
#include <stdint.h>


#define MAX_POWER_2	34

#define FUNC_ALLOCA	0
#define FUNC_CAALOC	1
#define FUNC_MAALOC	2

#define FUNC_MAX_ID	3

#define FUNC_MAX_NAME	7

char func_name[FUNC_MAX_ID][FUNC_MAX_NAME] = {
		"alloca",
		"calloc",
		"malloc"};

int64_t calc_delta_time(struct timespec *start, struct timespec *stop)
{
	return (int64_t)(stop->tv_sec - start->tv_sec) * (int64_t)1000000000UL
				+ (int64_t)(stop->tv_nsec - start->tv_nsec);
}

void *get_memory(int8_t func_id, int64_t size, int64_t *time)
{
	char *buf = NULL;

	struct timespec start;
	struct timespec stop;

	if (time != NULL)
		*time = 0;

	//clock_gettime(CLOCK_MONOTONIC, &start);

	switch (func_id) {
	case FUNC_ALLOCA:
		//I am a beginner in Linux programming and
		//I do not know how check maximum memory size
		//that can be allocated safely by alloca()
		if (size > (1ul<<20))
			break;
		clock_gettime(CLOCK_MONOTONIC, &start);
		buf = alloca(size);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		break;
	case FUNC_CAALOC:
		clock_gettime(CLOCK_MONOTONIC, &start);
		buf = calloc(size, sizeof(char));
		clock_gettime(CLOCK_MONOTONIC, &stop);
		break;
	case FUNC_MAALOC:
		clock_gettime(CLOCK_MONOTONIC, &start);
		buf = malloc(size);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		break;
	default:
		return NULL;
	}

	//clock_gettime(CLOCK_MONOTONIC, &stop);

	if (time != NULL) {
		if (buf == NULL)
			*time = -1;// error happened
		else
			*time = calc_delta_time(&start, &stop);
	}

	return (void *)buf;
}

void free_memory(void *buf, int8_t func_id, int64_t *time)
{
	struct timespec start = {0};
	struct timespec stop = {0};

	if (time != NULL)
		*time = 0;

	//clock_gettime(CLOCK_MONOTONIC, &start);

	switch (func_id) {
	case FUNC_ALLOCA:
		// memory was freed at the moment of exit from get_memory automatically
		break;
	case FUNC_CAALOC:
		clock_gettime(CLOCK_MONOTONIC, &start);
		free(buf);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		break;
	case FUNC_MAALOC:
		clock_gettime(CLOCK_MONOTONIC, &start);
		free(buf);
		clock_gettime(CLOCK_MONOTONIC, &stop);
		break;
	default:
		return;
	}

	//clock_gettime(CLOCK_MONOTONIC, &stop);

	if (time != NULL)
		*time = calc_delta_time(&start, &stop);
}


int main(void)
{
	int8_t func_id;
	int8_t s = 0;
	int64_t size = 0;
	int64_t tim_alloc;
	int64_t tim_dealloc;

	char *buf = NULL;

	printf("\n\tMemory allocation/deallocation test\n\n");

	printf("Size\t\tFunction\tAlloc(ns)\tDealloc(ns)\n");

	for (s = 0; s < MAX_POWER_2; s++) {

		size = 1ull << s;

		for (func_id = 0; func_id < FUNC_MAX_ID; func_id++) {
			buf = get_memory(func_id, size, &tim_alloc);
			if (buf != NULL)
				free_memory(buf, func_id, &tim_dealloc);
			if (tim_alloc == -1)
				printf("2^%i\t\t%s\t\terror\n",
					s, &func_name[func_id][0]);
			else
				printf("2^%i\t\t%s\t\t%lu\t\t%lu\n",
					s, &func_name[func_id][0], tim_alloc, tim_dealloc);

		}
	}
}
