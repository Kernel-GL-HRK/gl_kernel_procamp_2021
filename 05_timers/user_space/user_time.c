// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko
 */

#include <stdio.h>
#include <time.h>

void print_clock_resolution(void)
{
    struct timespec real_res;
    struct timespec monolitic_res;
    struct timespec proc_cpu_res;
    struct timespec thread_cpu_res;
    struct timespec monotonic_raw_res;
    struct timespec realtime_coarse_raw_res;
    struct timespec monotonic_coarse_raw_res;
    struct timespec boottime_res;
    struct timespec realtime_alarm_res;
    struct timespec boottime_alarm_res;
    struct timespec clock_tai_res;

    clock_getres(CLOCK_REALTIME, &real_res);
    clock_getres(CLOCK_MONOTONIC, &monolitic_res);
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &proc_cpu_res);
    clock_getres(CLOCK_THREAD_CPUTIME_ID, &thread_cpu_res);
    clock_getres(CLOCK_MONOTONIC_RAW, &monotonic_raw_res);
    clock_getres(CLOCK_REALTIME_COARSE, &realtime_coarse_raw_res);
    clock_getres(CLOCK_MONOTONIC_COARSE, &monotonic_coarse_raw_res);
    clock_getres(CLOCK_BOOTTIME, &boottime_res);
    clock_getres(CLOCK_REALTIME_ALARM, &realtime_alarm_res);
    clock_getres(CLOCK_BOOTTIME_ALARM, &boottime_alarm_res);
    clock_getres(CLOCK_TAI, &clock_tai_res);

    printf("\nClock resolutin of different sources\n");
    printf("===============================================+===========\n");
    printf("|%25s|%15s|%15s|\n","CLOCK SOURCE","Seconds","Nano Seconds");
    printf("==============================================+============\n");

    printf("|%25s|%15lu|%15lu|\n", "CLOCK_REALTIME" ,real_res.tv_sec, real_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_MONOTONIC" ,monolitic_res.tv_sec, monolitic_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_PROCESS_CPUTIME_ID" ,proc_cpu_res.tv_sec, proc_cpu_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_THREAD_CPUTIME_ID" ,thread_cpu_res.tv_sec, thread_cpu_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_MONOTONIC_RAW" ,monotonic_raw_res.tv_sec, monotonic_raw_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_REALTIME_COARSE" ,realtime_coarse_raw_res.tv_sec, realtime_coarse_raw_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_MONOTONIC_COARSE" , monotonic_coarse_raw_res.tv_sec, monotonic_coarse_raw_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_BOOTTIME" , boottime_res.tv_sec, boottime_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_REALTIME_ALARM" , realtime_alarm_res.tv_sec, realtime_alarm_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_BOOTTIME_ALARM" , boottime_alarm_res.tv_sec, boottime_alarm_res.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_TAI" , clock_tai_res.tv_sec, clock_tai_res.tv_nsec);
}

void print_absolute_time(void)
{
    struct timespec real_time;
    struct timespec monolitic_time;
    struct timespec proc_cpu_time;
    struct timespec thread_cpu_time;
    struct timespec monotonic_raw_time;
    struct timespec realtime_coarse_raw_time;
    struct timespec monotonic_coarse_raw_time;
    struct timespec boottime_time;
    struct timespec realtime_alarm_time;
    struct timespec boottime_alarm_time;
    struct timespec clock_tai_time;

    clock_gettime(CLOCK_REALTIME, &real_time);
    clock_gettime(CLOCK_MONOTONIC, &monolitic_time);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &proc_cpu_time);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &thread_cpu_time);
    clock_gettime(CLOCK_MONOTONIC_RAW, &monotonic_raw_time);
    clock_gettime(CLOCK_REALTIME_COARSE, &realtime_coarse_raw_time);
    clock_gettime(CLOCK_MONOTONIC_COARSE, &monotonic_coarse_raw_time);
    clock_gettime(CLOCK_BOOTTIME, &boottime_time);
    clock_gettime(CLOCK_REALTIME_ALARM, &realtime_alarm_time);
    clock_gettime(CLOCK_BOOTTIME_ALARM, &boottime_alarm_time);
    clock_gettime(CLOCK_TAI, &clock_tai_time);

    printf("\nAbsolute time of different sources\n");
    printf("===============================================+===========\n");
    printf("|%25s|%15s|%15s|\n","CLOCK SOURCE","Seconds","Nano Seconds");
    printf("==============================================+============\n");

    printf("|%25s|%15lu|%15lu|\n", "CLOCK_REALTIME" ,real_time.tv_sec, real_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_MONOTONIC" ,monolitic_time.tv_sec, monolitic_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_PROCESS_CPUTIME_ID" ,proc_cpu_time.tv_sec, proc_cpu_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_THREAD_CPUTIME_ID" ,thread_cpu_time.tv_sec, thread_cpu_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_MONOTONIC_RAW" ,monotonic_raw_time.tv_sec, monotonic_raw_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_REALTIME_COARSE" ,realtime_coarse_raw_time.tv_sec, realtime_coarse_raw_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_MONOTONIC_COARSE" , monotonic_coarse_raw_time.tv_sec, monotonic_coarse_raw_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_BOOTTIME" , boottime_time.tv_sec, boottime_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_REALTIME_ALARM" , realtime_alarm_time.tv_sec, realtime_alarm_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_BOOTTIME_ALARM" , boottime_alarm_time.tv_sec, boottime_alarm_time.tv_nsec);
    printf("|%25s|%15lu|%15lu|\n", "CLOCK_TAI" , clock_tai_time.tv_sec, clock_tai_time.tv_nsec);
}

int main(void)
{
    print_clock_resolution();
    print_absolute_time();
    
    return 0;
}
