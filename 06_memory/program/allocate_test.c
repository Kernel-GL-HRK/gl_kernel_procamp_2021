#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

static inline void timespec_sub(struct timespec *diff, const struct timespec *left,
			 const struct timespec *right)
{
	diff->tv_sec = left->tv_sec - right->tv_sec;
	diff->tv_nsec = left->tv_nsec - right->tv_nsec;
	if (diff->tv_nsec < 0) {
		--diff->tv_sec;
		diff->tv_nsec += 1000000000;
	}
}

static void test_malloc(int iteretion)
{
	int i;
	size_t mem_size;
	void *mem_ptr;
	struct timespec tp_begin, tp_end;
	struct timespec alloc_time, free_time;

	printf("|---------------------|---------------|--------------|\n");
	printf("|        Size         | malloc Time   | free Time    |\n");
	printf("|---------------------|---------------|--------------|\n");

	for (i = 0; i <= iteretion; i++) {
		mem_size = pow(2, i);
		if (mem_size == 0) {
			break;
		}

		clock_gettime(CLOCK_MONOTONIC, &tp_begin);
		mem_ptr = malloc(mem_size);
		clock_gettime(CLOCK_MONOTONIC, &tp_end);
		timespec_sub(&alloc_time, &tp_end, &tp_begin);

		if (mem_ptr) {
			clock_gettime(CLOCK_MONOTONIC, &tp_begin);
			free(mem_ptr);
			clock_gettime(CLOCK_MONOTONIC, &tp_end);
			timespec_sub(&free_time, &tp_end, &tp_begin);
		}

		printf("|%20lu | %lu.%09lu   | %lu.%09lu  |\n", mem_size, alloc_time.tv_sec, alloc_time.tv_nsec,
				free_time.tv_sec, free_time.tv_nsec);
	}
	printf("|---------------------|---------------|--------------|\n");
}

static void test_calloc(int iteretion)
{
	int i;
	size_t mem_size;
	void *mem_ptr;
	struct timespec tp_begin, tp_end;
	struct timespec alloc_time, free_time;

	printf("|---------------------|---------------|--------------|\n");
	printf("|        Size         | calloc Time   | free Time    |\n");
	printf("|---------------------|---------------|--------------|\n");

	for (i = 0; i <= iteretion; i++) {
		mem_size = pow(2, i);
		if (mem_size == 0) {
			break;
		}

		clock_gettime(CLOCK_MONOTONIC, &tp_begin);
		mem_ptr = calloc(10, mem_size);
		clock_gettime(CLOCK_MONOTONIC, &tp_end);
		timespec_sub(&alloc_time, &tp_end, &tp_begin);

		if (mem_ptr) {
			*((char*)mem_ptr) = 1;
			clock_gettime(CLOCK_MONOTONIC, &tp_begin);
			free(mem_ptr);
			clock_gettime(CLOCK_MONOTONIC, &tp_end);
			timespec_sub(&free_time, &tp_end, &tp_begin);
		}

		printf("|%20lu | %lu.%09lu   | %lu.%09lu  |\n", mem_size, alloc_time.tv_sec, alloc_time.tv_nsec,
				free_time.tv_sec, free_time.tv_nsec);
	}
	printf("|---------------------|---------------|--------------|\n");
}

static void * _alloca(size_t mem_size, struct timespec *tp_begin, struct timespec *tp_end)
{
	void *mem_ptr;

	clock_gettime(CLOCK_MONOTONIC, tp_begin);
	mem_ptr = alloca(mem_size);
	clock_gettime(CLOCK_MONOTONIC, tp_end);
	return mem_ptr;
}

static void test_alloca(int iteretion)
{
	int i;
	size_t mem_size;
	void *mem_ptr;
	struct timespec tp_begin, tp_end, tp_ret;
	struct timespec alloc_time, free_time;

	printf("|---------------------|---------------|--------------|\n");
	printf("|        Size         | alloca Time   | free Time    |\n");
	printf("|---------------------|---------------|--------------|\n");

	for (i = 0; i <= iteretion; i++) {
		mem_size = pow(2, i);
		if (mem_size == 0) {
			break;
		}

		if (8388608 == mem_size) {
			mem_size+=1;
			mem_size-=1;
		}

		mem_ptr = _alloca(mem_size, &tp_begin, &tp_end);
		clock_gettime(CLOCK_MONOTONIC, &tp_ret);
		timespec_sub(&alloc_time, &tp_end, &tp_begin);
		timespec_sub(&free_time, &tp_ret, &tp_end);

		if (mem_ptr) {
			*((char*)mem_ptr) = 1; // check that we can write to memory
		}

		printf("|%20lu | %lu.%09lu   | %lu.%09lu  |\n", mem_size, alloc_time.tv_sec, alloc_time.tv_nsec,
				free_time.tv_sec, free_time.tv_nsec);
	}
	printf("|---------------------|---------------|--------------|\n");
}

int main(int argc, char **argv)
{
	test_malloc(100);
	test_calloc(100);
	test_alloca(100);

	return 0;
}
