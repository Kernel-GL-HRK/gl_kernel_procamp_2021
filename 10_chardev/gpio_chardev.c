#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#include "gpio_chardev_ioctl.h"

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))

//https://elixir.bootlin.com/linux/latest/source/drivers/char/dsp56k.c#L503

/*	https://linux-sunxi.org/Xunlong_Orange_Pi_PC#LEDs
 *  Board config for OPI-PC:
 *	LED GREEN (PL10): GPIO_11_10
 *	LED RED   (PA15): GPIO_0_15  
 *	BUTTON    (PG7) : GPIO_6_7
 *
 *  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *	LED GREEN (PL10): GPIO_11_10 
 *	LED RED   (PA17): GPIO_0_17 
 *	BUTTON    (PA6) : GPIO_0_6
 *  
 */

#ifdef ORANGE_PI_PC
#error "Implement the necessary port mappings"
#endif
#ifdef ORANGE_PI_ZERO
#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)
#define BUTTON GPIO_NUMBER(0, 6)
#endif

#define CLASS_NAME "gpio_chardev"
#define DEVICE_NAME "gpio_dev"
#define BUFFER_SIZE 1024

static struct class *pdevice_class;
static struct device *p_chardev;

static int major;
static int is_open;

static int data_size;
static unsigned char data_buffer[BUFFER_SIZE];

static int led_red_gpio;
static int led_green_gpio;

static int dev_open(struct inode *inodep, struct file *filep)
{
	if (is_open) {
		pr_err("gpio_chardev: already open\n");
		return -EBUSY;
	}

	is_open = 1;
	pr_info("gpio_chardev: device opened\n");
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	is_open = 0;
	pr_info("gpio_chardev: device closed\n");
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len,
			loff_t *offset)
{
	int ret;

	pr_info("gpio_chardev: read from file %s\n",
		filep->f_path.dentry->d_iname);
	pr_info("gpio_chardev: read from device %d:%d\n",
		imajor(filep->f_inode), iminor(filep->f_inode));

	if (len > data_size)
		len = data_size;

	ret = copy_to_user(buffer, data_buffer, len);
	if (ret) {
		pr_err("gpio_chardev: copy_to_user failed: %d\n", ret);
		return -EFAULT;
	}
	data_size = 0; /* eof for cat */

	pr_info("gpio_chardev: %zu bytes read\n", len);
	return len;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len,
			 loff_t *offset)
{
	int ret;

	pr_info("gpio_chardev: write to file %s\n",
		filep->f_path.dentry->d_iname);
	pr_info("gpio_chardev: write to device %d:%d\n", imajor(filep->f_inode),
		iminor(filep->f_inode));

	data_size = len;
	if (data_size > BUFFER_SIZE)
		data_size = BUFFER_SIZE;

	ret = copy_from_user(data_buffer, buffer, data_size);
	if (ret) {
		pr_err("gpio_chardev: copy_from_user failed: %d\n", ret);
		return -EFAULT;
	}

	pr_info("gpio_chardev: %d bytes written\n", data_size);
	return data_size;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;

	switch (cmd) {
	case GPIO_CHARDEV_SET_VALUE: {
		struct gpio_chardev_set_data __user *gpio_data = argp;

		if (gpio_data->gpio_num == led_green_gpio ||
		    gpio_data->gpio_num == led_red_gpio) {
			gpio_set_value(gpio_data->gpio_num,
				       gpio_data->gpio_value);
			return 0;
		}
		return -EINVAL;
	} break;
	case GPIO_CHARDEV_READ_BUTTON_VALUE: {
		struct gpio_chardev_get_data __user *gpio_data = argp;

		if (gpio_data->gpio_num == BUTTON) {
			int button_value = gpio_get_value(BUTTON);

			return put_user(button_value, gpio_data->gpio_value);
		}
		return -EINVAL;
	} break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int led_gpio_init(int gpio, int *led_gpio)
{
	int res;

	res = gpio_direction_output(gpio, 0);
	if (res != 0)
		return res;

	*led_gpio = gpio;
	return 0;
}
static int button_gpio_init(int gpio)
{
	int res;

	res = gpio_request(gpio, "Onboard user button");
	if (res != 0)
		return res;

	res = gpio_direction_input(gpio);
	if (res != 0)
		goto err_input;

	return 0;

err_input:
	gpio_free(gpio);
	return res;
}

static void button_gpio_deinit(int button_gpio)
{
	gpio_free(button_gpio);
	pr_info("Deinit GPIO%d\n", button_gpio);
}
static struct file_operations fops = {
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
	.write = dev_write,
	.unlocked_ioctl = dev_ioctl,
};

static int __init charedev_mod_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if (major < 0) {
		pr_err("register_chrdev failed: %d\n", major);
		return major;
	}
	pr_info("gpio_chardev: register_chrdev ok, major = %d\n", major);

	pdevice_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(pdevice_class)) {
		unregister_chrdev(major, DEVICE_NAME);
		pr_err("gpio_chardev: class_create failed\n");
		return PTR_ERR(pdevice_class);
	}
	pr_info("gpio_chardev: device class created successfully\n");

	p_chardev = device_create(pdevice_class, NULL, MKDEV(major, 0), NULL,
				  CLASS_NAME "0");
	if (IS_ERR(p_chardev)) {
		class_destroy(pdevice_class);
		unregister_chrdev(major, DEVICE_NAME);
		pr_err("chrdev: device_create failed\n");
		return PTR_ERR(p_chardev);
	}
	pr_info("gpio_chardev: device node created successfully\n");

	led_gpio_init(LED_GREEN, &led_green_gpio);
	led_gpio_init(LED_RED, &led_red_gpio);
	button_gpio_init(BUTTON);

	pr_info("gpio_chardev: module loaded\n");
	return 0;
}

static void __exit chardev_mod_exit(void)
{
	button_gpio_deinit(BUTTON);
	gpio_free(led_red_gpio);
	gpio_free(led_green_gpio);

	device_destroy(pdevice_class, MKDEV(major, 0));
	class_destroy(pdevice_class);
	unregister_chrdev(major, DEVICE_NAME);

	pr_info("gpio_chardev: module exited\n");
}

module_init(charedev_mod_init);
module_exit(chardev_mod_exit);

MODULE_AUTHOR("Valentyn Korniienko <valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("Character device with IOCTL support for GPIOs");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");