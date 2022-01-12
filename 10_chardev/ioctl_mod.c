#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include "dev.h"

MODULE_AUTHOR("Volodymyr Kniazkyi <kniazkyi@gmail.com>");
MODULE_DESCRIPTION("Character device example driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#define CLASS_NAME	"chrdev"
#define BUFFER_SIZE	1024

static struct class *pclass;
static struct device *pdev;

static int major;

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))

/*
 *  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *	LED GREEN (PL10): GPIO_11_10
 *	LED RED   (PA17): GPIO_0_17
 *	BUTTON    (PG7) : GPIO_6_7
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)
#define BUTTON GPIO_NUMBER(6, 7)

static int ledg_gpio;
static int ledr_gpio;
static int button_gpio;

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

	res = gpio_request(gpio, "User button");
	if (res != 0)
		return res;

	res = gpio_direction_input(gpio);
	if (res != 0) {
		gpio_free(gpio);
		return res;
	}

	button_gpio = gpio;
	pr_info("Init GPIO%d OK\n", button_gpio);
	return 0;
}

static void button_gpio_deinit(void)
{
	if (button_gpio >= 0) {
		gpio_free(button_gpio);
		pr_info("Deinit GPIO%d\n", button_gpio);
	}
}

static int is_open;
static int data_size;
static unsigned char data_buffer[BUFFER_SIZE];

static int dev_open(struct inode *inodep, struct file *filep)
{
	if (is_open) {
		pr_err("chrdev: already open\n");
		return -EBUSY;
	}

	is_open = 1;
	//pr_info("chrdev: device opened\n");
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	is_open = 0;
	//pr_info("chrdev: device closed\n");
	return 0;
}


static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int ret;

	pr_info("chrdev: read from file %s\n", filep->f_path.dentry->d_iname);
	pr_info("chrdev: read from device %d:%d\n", imajor(filep->f_inode), iminor(filep->f_inode));

	if (len > data_size)
		len = data_size;

	ret = copy_to_user(buffer, data_buffer, len);
	if (ret) {
		pr_err("chrdev: copy_to_user failed: %d\n", ret);
		return -EFAULT;
	}
	data_size = 0; /* eof for cat */

	pr_info("chrdev: %zu bytes read\n", len);
	return len;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	int ret;

	pr_info("chrdev: write to file %s\n", filep->f_path.dentry->d_iname);
	pr_info("chrdev: write to device %d:%d\n", imajor(filep->f_inode), iminor(filep->f_inode));

	data_size = len;
	if (data_size > BUFFER_SIZE)
		data_size = BUFFER_SIZE;

	ret = copy_from_user(data_buffer, buffer, data_size);
	if (ret) {
		pr_err("chrdev: copy_from_user failed: %d\n", ret);
		return -EFAULT;
	}

	pr_info("chrdev: %d bytes written\n", data_size);
	return data_size;
}

static long dev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	if (_IOC_TYPE(cmd) != IOC_MAGIC)
		return -ENOTTY;
	switch (cmd) {
	case IOCTL_BUTTGET_VAL:
		{
			char bstate;

			if (gpio_get_value(button_gpio) == 0)
				bstate = 0;
			else
				bstate = 1;
			if (copy_to_user((char *)arg, &bstate, sizeof(bstate)))
				return -EFAULT;
			//pr_info("IOCTL_BUTTGET_VAL: %d\n", bstate);
		}
		break;
	case IOCTL_LEDGSET_VAL:
		//pr_info("IOCTL_LEDGSET_VAL: %ld\n", arg);
		if (arg == 0)
			gpio_set_value(ledg_gpio, 0);
		else
			gpio_set_value(ledg_gpio, 1);
		break;
	case IOCTL_LEDRSET_VAL:
		//pr_info("IOCTL_LEDRSET_VAL: %ld\n", arg);
		if (arg == 0)
			gpio_set_value(ledr_gpio, 0);
		else
			gpio_set_value(ledr_gpio, 1);
		break;
	default:
		return -ENOTTY;
	}
	return 0;
}
static struct file_operations fops = {
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
	.write = dev_write,
	.unlocked_ioctl = dev_ioctl
};

static int chrdev_init(void)
{
	int ret;

	is_open = 0;
	data_size = 0;

	major = register_chrdev(0, DEVICE_NAME, &fops);
	if (major < 0) {
		pr_err("register_chrdev failed: %d\n", major);
		return major;
	}
	pr_info("chrdev: register_chrdev ok, major = %d\n", major);

	pclass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(pclass)) {
		unregister_chrdev(major, DEVICE_NAME);
		pr_err("chrdev: class_create failed\n");
		return PTR_ERR(pclass);
	}
	pr_info("chrdev: device class created successfully\n");

	pdev = device_create(pclass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	if (IS_ERR(pdev)) {
		class_destroy(pclass);
		unregister_chrdev(major, DEVICE_NAME);
		pr_err("chrdev: device_create failed\n");
		return PTR_ERR(pdev);
	}
	pr_info("chrdev: device node created successfully\n");

	pr_info("GPIO Init\n");

	ret = led_gpio_init(LED_GREEN, &ledg_gpio);
	if (ret == 0) {
		pr_info("Set GPIO%d for output\n", ledg_gpio);
		gpio_set_value(ledg_gpio, 0);

		ret = led_gpio_init(LED_RED, &ledr_gpio);
		if (ret == 0) {
			pr_info("Set GPIO%d for output\n", ledr_gpio);
			gpio_set_value(ledr_gpio, 0);

			ret = button_gpio_init(BUTTON);
			if (ret != 0)
				pr_err("Can't set GPIO%d for button\n", BUTTON);
		} else
			pr_err("Can't set GPIO%d for output\n", LED_RED);
	} else
		pr_err("Can't set GPIO%d for output\n", LED_GREEN);

	if (ret != 0) {
		device_destroy(pclass, MKDEV(major, 0));
		class_destroy(pclass);
		unregister_chrdev(major, DEVICE_NAME);
		return ret;
	}

	pr_info("chrdev: module loaded\n");
	return 0;
}

static void chrdev_exit(void)
{
	gpio_set_value(ledg_gpio, 0);
	gpio_set_value(ledr_gpio, 0);

	device_destroy(pclass, MKDEV(major, 0));
	class_destroy(pclass);
	unregister_chrdev(major, DEVICE_NAME);

	button_gpio_deinit();

	pr_info("chrdev: module exited\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);
