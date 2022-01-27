#ifndef PCI_IOCTL_H
#define PCI_IOCTL_H
#include <linux/ioctl.h>

#define GREEN_LEN_ON _IO('q', 1)
#define GREEN_LEN_OFF _IO('q', 2)

#define RED_LEN_ON _IO('q', 3)
#define RED_LEN_OFF _IO('q', 4)

#define DEVICE_NAME "/dev/led"

#endif // MY_IOCTL_H
