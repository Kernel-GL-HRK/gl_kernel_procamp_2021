// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <malloc.h>

#define MALLOC 1
#define CALLOC 2
#define ALLOCA 3

int get_nsec(struct timespec ts)
{
	int time;

	clock_gettime(CLOCK_REALTIME, &ts);
	time = ts.tv_nsec;
	return time;
}

int main(int argc, char *argv[])
{
	struct timespec ts;
	int start, stop, diff_alloc, diff_free;
	float power;

	if (argc > 1) {
		int arg = atoi(argv[1]);

		if (arg != ALLOCA) {
			for (int x = 1; x <= 30; x++) {
				int *x_ptr = NULL;

				power = pow(2, x);
				start = get_nsec(ts);  //for allocating
				switch (arg) {
				case MALLOC:
					x_ptr = malloc(power);
					break;
				case CALLOC:
					x_ptr = calloc(power, sizeof(int));
					break;
				default:
					perror("Uncompatible argument");
					return 1;
				}
				stop = get_nsec(ts);
				diff_alloc = stop - start;
				start = get_nsec(ts);  //for freeing
				free(x_ptr);
				stop = get_nsec(ts);
				diff_free = stop - start;
				printf("%d ns for allocating and %d ns for freeing %.0f bytes\n", diff_alloc, diff_free, power);
			}
		} else {	//if alloca chosen
			for (int x = 1; x <= 21; x++) {
				int *x_ptr = NULL;

				power = pow(2, x);
				start = get_nsec(ts);
				x_ptr = alloca(power);
				stop = get_nsec(ts);
				diff_alloc = stop - start;
				printf("%d ns for allocating %.0f bytes\n", diff_alloc, power);
			}
			printf("alloca frees automatically\n");
		}
	} else {
		perror("No arguments");
		return 1;
	}
	return 0;
}
