// SPDX-License-Identifier: GPL-2.0
/* Implement program which return absolute time in user space.
 * Use clock_gettime() from time.h. Try different clock id. Find the difference.
 * Show possible clock resolution provided by clock_getres().
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    struct timespec real_ts, mono_ts, pcpu_ts, tcpu_ts, boot_ts;
    struct timespec real_tp, mono_tp, pcpu_tp, tcpu_tp, boot_tp;

    printf("getting clock resolution...\n");
    if (clock_getres(CLOCK_REALTIME, &real_ts))
	perror(strerror(errno));
    if (clock_getres(CLOCK_MONOTONIC, &mono_ts))
	perror(strerror(errno));
    if (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &pcpu_ts))
	perror(strerror(errno));
    if (clock_getres(CLOCK_THREAD_CPUTIME_ID, &tcpu_ts))
	perror(strerror(errno));
    if (clock_getres(CLOCK_BOOTTIME, &boot_ts))
	perror(strerror(errno));

    printf("----------------------------------------------------------------\n");
    printf("|%-20.20s|%-20.20s|%-20.20s|\n", "clock_id", "sec", "nsec");
    printf("----------------------------------------------------------------\n");
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_REALTIME", real_ts.tv_sec, real_ts.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_MONOTONIC", mono_ts.tv_sec, mono_ts.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_PROCESS_CPUTIME_ID", pcpu_ts.tv_sec, pcpu_ts.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_THREAD_CPUTIME_ID", tcpu_ts.tv_sec, tcpu_ts.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_BOOTTIME", boot_ts.tv_sec, boot_ts.tv_nsec);
    printf("\n");

    printf("getting actual clock values...\n");
    if (clock_gettime(CLOCK_REALTIME, &real_tp))
	perror(strerror(errno));
    if (clock_gettime(CLOCK_MONOTONIC, &mono_tp))
	perror(strerror(errno));
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &pcpu_tp))
	perror(strerror(errno));
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tcpu_tp))
	perror(strerror(errno));
    if (clock_gettime(CLOCK_BOOTTIME, &boot_tp))
	perror(strerror(errno));

    printf("----------------------------------------------------------------\n");
    printf("|%-20.20s|%-20.20s|%-20.20s|\n", "clock_id", "sec", "nsec");
    printf("----------------------------------------------------------------\n");
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_REALTIME", real_tp.tv_sec, real_tp.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_MONOTONIC", mono_tp.tv_sec, mono_tp.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_PROCESS_CPUTIME_ID", pcpu_tp.tv_sec, pcpu_tp.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_THREAD_CPUTIME_ID", tcpu_tp.tv_sec, tcpu_tp.tv_nsec);
    printf("|%-20.20s|%-20li|%-20li|\n", "CLOCK_BOOTTIME", boot_tp.tv_sec, boot_tp.tv_nsec);
    printf("\n");

    return 0;
}
