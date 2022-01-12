#ifndef __DEVICE_H__
#define __DEVICE_H__

#define IOC_MAGIC   'h'
#define IOCTL_LEDGSET_VAL   _IOW(IOC_MAGIC, 1, unsigned int)
#define IOCTL_LEDRSET_VAL   _IOW(IOC_MAGIC, 2, unsigned int)
#define IOCTL_BUTTGET_VAL   _IOR(IOC_MAGIC, 3, unsigned int)

#define DEVICE_NAME	"ledioctl"
#define DEVPATH "/dev/ledioctl"

#endif
