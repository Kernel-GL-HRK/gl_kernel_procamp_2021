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

void *call_alloc(enum allocator alloc, size_t i) {
	void *p = NULL;

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

	return p;
}

int measure(size_t i, enum allocator alloc) {
	void *p;
	struct timespec tp, tn;
	int ret;

	ret = clock_gettime(CLOCK_MONOTONIC, &tp);
	if (ret < 0)
		return ret;

	p = call_alloc(alloc, i);
	if (p == NULL)
		return 1;

	ret = clock_gettime(CLOCK_MONOTONIC, &tn);
	if (ret < 0)
		return ret;

	if (alloc != ALLOCA)
		free(p);

	printf(" %0.9f ", to_nsec(tn) - to_nsec(tp));

	return 0;
}

int main() {
	size_t i;
	int ret;

	printf("	  bytes				 time				 \n");
	printf("			  malloc	   calloc	   alloca   \n");
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
