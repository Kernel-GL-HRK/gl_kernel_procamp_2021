/**
 * @file ioctrl_usrspace.c
 * @brief User space appliation to control builtin Orange PI leds
 */

#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "dev.h"

static int cmd_run = 1;

static int blink(int dfile, enum e_led_type type);
static void signal_handler(int signal);

int main(void)
{
	int dfd;

	signal(SIGINT, signal_handler);

	if ((dfd = open(CHRDEV_PATH, O_RDWR)) < 0) {
		fprintf(stderr, "Error %d while opening the device\n", dfd);
		exit(EXIT_FAILURE);
	}

	while (cmd_run) {
		if (blink(dfd, e_led_type_green))
			break;

		if (blink(dfd, e_led_type_red))
			break;

		usleep(100000);
	}

	close(dfd);
	signal(SIGINT, SIG_DFL);

	return EXIT_SUCCESS;
}

static int blink(int dfile, enum e_led_type type)
{
	struct led_control led_ctrl = { .e_type = type };

	if (ioctl(dfile, CHRDEV_IOCTRL_GET_VAL, &led_ctrl)) {
		fprintf(stderr, "Error %d while ioctl get operation\n", dfile);
		return dfile;
	}

	if (!chrdev_led_cmd_is_ok(&led_ctrl)) {
		assert(0);
		return -1;
	}

	/* Invert LED state */
	led_ctrl.e_state =
		(led_ctrl.e_state == e_led_state_off) ? e_led_state_on : e_led_state_off;

	if (ioctl(dfile, CHRDEV_IOCTRL_SET_VAL, &led_ctrl)) {
		fprintf(stderr, "Error %d while ioctl set operation\n", dfile);
		return dfile;
	}

	return 0;
}

static void signal_handler(int signal)
{
	cmd_run = 0;
}
