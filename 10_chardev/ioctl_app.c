#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "dev.h"


#define SYMB_BUTTON		'b'
#define SYMB_EXIT		'e'
#define SYMB_GREENLED	'g'
#define SYMB_REDLED		'r'

int main(void)
{
	char sdevice[256], device, action;
	char bstate;
	int fdev;

	device = action = 0;
	bstate = 0;

	for (;;) {
		// choise device
		for (;;) {
			printf("\nPlease, choose Green LED (%c) - Red LED(%c)"\
					" - Button(%c) exit - (%c)\n",
					SYMB_GREENLED, SYMB_REDLED, SYMB_BUTTON, SYMB_EXIT);

			if (fgets((char *)sdevice, sizeof(sdevice), stdin) == NULL) {
				printf("\nInput error");
				return 1;
			}

			if (strlen((char *)sdevice) > 2) {
				printf("\n too much symbols (%li) %s",
						strlen((char *)sdevice)-1, sdevice);
				continue;
			}

			device = sdevice[0];

			if (device == SYMB_EXIT)
				return 0;

			if ((device == SYMB_GREENLED) || (device == SYMB_REDLED)
				|| (device == SYMB_BUTTON))
				break;

			printf("\n\tWrong choise  - (%c)\n\t", device);
		}

		if ((device == SYMB_GREENLED) || (device == SYMB_REDLED)) {
			for (;;) {
				printf(
					"Please, choose action switch on/off (1/0), exit - (%c)\n",
					SYMB_EXIT);

				if (fgets((char *)sdevice, sizeof(sdevice), stdin) == NULL) {
					printf("\n\tInput error");
					return 1;
				}

				if (strlen((char *)sdevice) > 2) {
					printf("\n\t too much symbols (%li) %s",
							strlen((char *)sdevice)-1, sdevice);
					continue;
				}

				action = sdevice[0];

				if (action == SYMB_EXIT)
					return 0;
				if ((action == '1') || (action == '0'))
					break;
				printf("\n\tWrong choise  - (%c)\n\t", device);
			}
		}

		fdev = open(DEVPATH, O_RDWR);
		if (fdev >= 0) {
			switch (device) {
			default:
			case SYMB_BUTTON:
				ioctl(fdev, IOCTL_BUTTGET_VAL, &bstate);
				printf("\n Button state %d", bstate);
				break;
			case SYMB_GREENLED:
				if (action == '1') {
					ioctl(fdev, IOCTL_LEDGSET_VAL, 1);
					printf("\n Green led is switched on");
				} else {
					ioctl(fdev, IOCTL_LEDGSET_VAL, 0);
					printf("\n Green led is switched off");
				}
				break;
			case SYMB_REDLED:
				if (action == '1') {
					ioctl(fdev, IOCTL_LEDRSET_VAL, 1);
					printf("\n Red led is switched on");
				} else {
					ioctl(fdev, IOCTL_LEDRSET_VAL, 0);
					printf("\n Red led is switched off");
				}
				break;
			}
			close(fdev);
		} else {
			printf("\n Driver is not ready");
		}
	}
	return 0;
}
