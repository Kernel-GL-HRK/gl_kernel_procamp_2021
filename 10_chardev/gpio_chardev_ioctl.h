#ifndef GPIO_CHARDEV_IOCTL_H
#define GPIO_CHARDEV_IOCTL_H

// https://elixir.bootlin.com/linux/latest/source/arch/m68k/include/asm/dsp56k.h#L31

struct gpio_chardev_set_data {
	int gpio_num;
	int gpio_value;
};

struct gpio_chardev_get_data {
	int gpio_num;
	int __user *gpio_value;
};

#define GPIO_CHARDEV_SET_VALUE 1
#define GPIO_CHARDEV_READ_BUTTON_VALUE 2

#endif