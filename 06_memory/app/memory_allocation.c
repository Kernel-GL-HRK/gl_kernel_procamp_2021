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

int measure(size_t i, enum allocator alloc) {
	void *p;
    // t0 - start of allocation
    // t1 - end of allocation and start of free
    // t2 - end of free
	struct timespec t[3];
	int ret;

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

	printf(" %0.9f(%0.9f) ", to_nsec(t[1]) - to_nsec(t[0]), to_nsec(t[2]) - to_nsec(t[1]));

	return 0;
}

int main() {
	size_t i;
	int ret;

	printf("	  bytes		time to allocate(time to free)\n");
	printf("			  malloc	   calloc	            alloca   \n");
	for (i = 2; i < SIZE_MAX; i *= 2) {
		printf("%11ld ", i);

		ret = measure(i, MALLOC);
		if (ret != 0)
			break;

		ret = measure(i, CALLOC);
		if (ret != 0)
			break;

		if ( i < (8*1024*1024)) {
			ret = measure(i, ALLOCA);
			if (ret != 0)
				break;

			printf("\n");
		} else {
			printf(" ----\n");
		}
	}

	if (ret != 0)
		printf("Error to allocate memory\n");

	return 0;
}
