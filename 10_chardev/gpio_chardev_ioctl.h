#ifndef GPIO_CHARDEV_IOCTL_H
#define GPIO_CHARDEV_IOCTL_H

#if defined(__cplusplus)
extern "C" {
#endif


// https://elixir.bootlin.com/linux/latest/source/arch/m68k/include/asm/dsp56k.h#L31

struct gpio_chardev_set_data {
	int gpio_num;
	int gpio_value;
};

struct gpio_chardev_get_data {
	int gpio_num;
#if defined(__cplusplus)
	int *gpio_value;
#else
	int __user *gpio_value;
#endif
};

#define GPIO_CHARDEV_IOCTL_BASE 'f'

#define GPIO_CHARDEV_SET_VALUE _IOW(GPIO_CHARDEV_IOCTL_BASE,0x00,struct gpio_chardev_set_data)
#define GPIO_CHARDEV_READ_BUTTON_VALUE _IOR(GPIO_CHARDEV_IOCTL_BASE,0x01,struct gpio_chardev_get_data)


#if defined(__cplusplus)
}
#endif

#endif