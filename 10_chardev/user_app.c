// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <pthread.h>

#include "ioctl_common.h"

void *threadBlinkLed_Green(void *arg)
{
    static leds_cont_t led_control = {
        .led_color = GREEN,
        .led_state = LED_OFF
    };

    int *p_fdev = (int*)arg;

    for(;;)
    {
        led_control.led_state = LED_ON;
        ioctl(*p_fdev, WR_VALUE, &led_control);
        printf("Turn on LED Green\n");
        sleep(1);

        led_control.led_state = LED_OFF;
        ioctl(*p_fdev, WR_VALUE, &led_control);
        printf("Turn off LED Green\n");
        sleep(1);
    }

    return NULL;
}

void *threadBlinkLed_Red(void *arg)
{
    static leds_cont_t led_control = {
        .led_color = RED,
        .led_state = LED_OFF
    };

    int *p_fdev = (int*)arg;

    for(;;)
    {
        led_control.led_state = LED_ON;
        ioctl(*p_fdev, WR_VALUE, &led_control);
        printf("Turn on LED Red\n");
        sleep(2);

        led_control.led_state = LED_OFF;
        ioctl(*p_fdev, WR_VALUE, &led_control);
        printf("Turn off LED Red\n");
        sleep(2);
    }

    return NULL;
}

int main()
{
    pthread_t thread_led_red;
    pthread_t thread_led_green;

    printf("This application interucts wit led kernl driver\n");
    printf("Press Ctrl+z to stop\n");
    sleep(2);

    int fdev = open("/dev/led_chardev", O_RDWR);
    if (fdev == -1) {
        printf("Can't open /dev/led_chardev device\n");
        return -1;
    }

    pthread_create(&thread_led_red, NULL, threadBlinkLed_Red, &fdev);
    pthread_create(&thread_led_green, NULL, threadBlinkLed_Green, &fdev);

    pthread_join(thread_led_red, NULL);
    pthread_join(thread_led_green, NULL);

    return 0;
}
