#include <alloca.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum allocator {
	MALLOC = 0,
	CALLOC,
	ALLOCA,
};

double to_nsec(struct timespec t) {
	double ns = t.tv_nsec / 1000000000.0;
	return t.tv_sec + ns;
}

void *call_alloc(enum allocator alloc, size_t i, struct timespec *t) {
	void *p = NULL;
	int ret;

	switch(alloc) {
	case MALLOC:
		p = malloc(i);
		break;
	case CALLOC:
		p = calloc(i, sizeof(char));
		break;
	case ALLOCA:
		p = alloca(i);
		break;
	default:
		return NULL;
	}

	// need to measure time here, as alloca will free the memory on exit of the functions
	if (p != NULL) {
		ret = clock_gettime(CLOCK_MONOTONIC, t);
		if (ret < 0)
			return NULL;
	}

	return p;
}

int measure(enum allocator alloc, size_t size_max) {
	void *p;
	// t0 - start of allocation
	// t1 - end of allocation and start of free
	// t2 - end of free
	struct timespec t[3];
	int ret;
	size_t i;

	printf(" Buffer size  allocation time  freeing time\n");
	for (i = 2; i < size_max; i *= 2) {
		ret = clock_gettime(CLOCK_MONOTONIC, &t[0]);
		if (ret < 0)
			return ret;

		p = call_alloc(alloc, i, &t[1]);
		if (p == NULL)
			return 1;

		if (alloc != ALLOCA)
			free(p);

		ret = clock_gettime(CLOCK_MONOTONIC, &t[2]);
		if (ret < 0)
			return ret;

		printf("%12ld   %0.9f	  %0.9f\n", i, to_nsec(t[1]) - to_nsec(t[0]), to_nsec(t[2]) - to_nsec(t[1]));
	}

	return 0;
}



int main() {
	int ret;

	printf("***** MALLOC *****\n");
	measure(MALLOC, SIZE_MAX);

	printf("\n***** CALLOC *****\n");
	measure(CALLOC, SIZE_MAX);

	printf("\n***** ALLOCA *****\n");
	measure(ALLOCA, 8 * 1024 * 1024);

	return 0;
}
