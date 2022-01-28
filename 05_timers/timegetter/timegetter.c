/**
 * @file timegetter.c
 * @brief User space program to get absolute time in seconds
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <linux/types.h>
#include <unistd.h>
#include <string.h>

struct ClockGetter
{
    char option;
    const char *name;
    clockid_t clock_id;
};

#define MAX_CLOCK  (32u)

static const struct ClockGetter clock_getter[] =
{
    { .option = 'r', .name = "CLOCK_REALTIME",           .clock_id = CLOCK_REALTIME           },
    { .option = 'm', .name = "CLOCK_MONOTONIC",          .clock_id = CLOCK_MONOTONIC          },
    { .option = 'p', .name = "CLOCK_PROCESS_CPUTIME_ID", .clock_id = CLOCK_PROCESS_CPUTIME_ID },
    { .option = 't', .name = "CLOCK_THREAD_CPUTIME_ID",  .clock_id = CLOCK_THREAD_CPUTIME_ID  },
};

static const size_t clock_num = (sizeof(clock_getter) / sizeof(clock_getter[0]));

/* MAX_CLOCK - 2 because we need to create an argument string with 'h' option an '\0' */
_Static_assert((sizeof(clock_getter) / sizeof(clock_getter[0]) < MAX_CLOCK - 2), "Size error");

int main(int argc, char* argv[])
{
    char *input_arg;
    int c;
    int i;
    char args[MAX_CLOCK];

    for (i = 0; i < clock_num; ++i)
    {
        args[i] = clock_getter[i].option;
    }
    /* Put help option h and \0 */
    args[i]     = 'h';
    args[i + 1] = '\0';

    while ((c = getopt(argc, argv, args)) != -1)
    {
        if (c == 'h')
        {
            puts("Options:");
            
            for (int j = 0; j < clock_num; ++j)
            {
                printf("  -%c for %s\n", clock_getter[j].option, clock_getter[j].name);
            }
        }
        else if ((input_arg = strchr(args, c)) != NULL)
        {
            int j = 0;
            for (; j < clock_num; ++j)
            {
                if (clock_getter[j].option == *input_arg)
                    break;
            }
            
            struct timespec tp = { 0 };

            if (clock_gettime(clock_getter[j].clock_id, &tp) != 0)
                exit(EXIT_FAILURE);
 
            printf("%s: %ld.%ld\n", clock_getter[j].name, tp.tv_sec, tp.tv_nsec);

            if (clock_getres(clock_getter[j].clock_id, &tp) != 0)
                exit(EXIT_FAILURE);

            printf("Resolution of %s: %ld.%ld\n", clock_getter[j].name, tp.tv_sec, tp.tv_nsec);
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
