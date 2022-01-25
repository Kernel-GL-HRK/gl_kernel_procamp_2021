// SPDX-License-Identifier: GPL-2.0

#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <asm-generic/bug.h>

#include "dev.h"

#define CHRDEV_MODULE_AUTHOR "Sergey Dubyna <serj.dub1na@gmail.com>"

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))

/*
 *  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *	LED GREEN (PL10): GPIO_11_10
 *	LED RED   (PA17): GPIO_0_17
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)

#define CHRDEV_DEV_NAME "chrdev_led"
#define CHRDEV_CLASS_NAME "chrdev"

#define BUFFER_SIZE 1024u

static int data_size;
static u8 data_buffer[BUFFER_SIZE];

static int ledg_gpio = -1;
static int ledr_gpio = -1;

static struct class *pclass;
static struct device *pdev;
static int major;
static int is_open;

static int led_gpio_init(int gpio, int *led_gpio)
{
	int res;

	res = gpio_direction_output(gpio, 0);
	if (res != 0)
		return res;

	*led_gpio = gpio;
	return 0;
}

static int chrdev_open(struct inode *pinode, struct file *pfile)
{
	(void)pinode;
	(void)pfile;

	if (is_open) {
		pr_err("device is already opened\n");
		return -EBUSY;
	}

	is_open = 1;
	pr_info("device opened\n");
	return 0;
}

static int chrdev_release(struct inode *pinode, struct file *pfile)
{
	(void)pinode;
	(void)pfile;

	is_open = 0;
	pr_info("device closed\n");
	return 0;
}

static ssize_t chrdev_read(struct file *pfile, char *buf, size_t len, loff_t *offset)
{
	int ret;

	pr_info("read from file %s\n", pfile->f_path.dentry->d_iname);
	pr_info("read from device %d:%d\n", imajor(pfile->f_inode), iminor(pfile->f_inode));

	if (len > data_size)
		len = data_size;

	ret = copy_to_user(buf, data_buffer, len);
	if (ret) {
		pr_err("copy_to_user failed: %d\n", ret);
		return -EFAULT;
	}

	data_size = 0;
	pr_info("%zu bytes read\n", len);
	return len;
}

static ssize_t chrdev_write(struct file *pfile, const char *buf, size_t len, loff_t *offset)
{
	int ret;

	pr_info("write to file %s\n", pfile->f_path.dentry->d_iname);
	pr_info("write to device %d:%d\n", imajor(pfile->f_inode), iminor(pfile->f_inode));

	data_size = len;
	if (data_size > BUFFER_SIZE)
		data_size = BUFFER_SIZE;

	ret = copy_from_user(data_buffer, buf, data_size);
	if (ret) {
		pr_err("copy_from_user failed: %d\n", ret);
		return -EFAULT;
	}

	pr_info("%d bytes written\n", data_size);
	return data_size;
}

static long chrdev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	struct led_control led_cmd = {};

	(void)f;

	if (_IOC_TYPE(cmd) != CHRDEV_IOCTRL_MAGIC)
		return -ENOTTY;

	if (copy_from_user(&led_cmd, (const void *)arg, sizeof(struct led_control)))
		return -EFAULT;

	if (!chrdev_led_cmd_is_ok(&led_cmd)) {
		BUG();
		return -EINVAL;
	}

	const int gpio_num =
		(led_cmd.e_type == e_led_type_green) ? ledg_gpio : ledr_gpio;

	switch (cmd) {
	case CHRDEV_IOCTRL_SET_VAL:
		gpio_set_value(gpio_num, led_cmd.e_state);
		break;

	case CHRDEV_IOCTRL_GET_VAL:
		led_cmd.e_state =
			gpio_get_value(gpio_num) ? e_led_state_on : e_led_state_off;
		if (copy_to_user((void *)arg, &led_cmd, sizeof(struct led_control)))
			return -EFAULT;
		break;

	default:
		BUG();
		return -EINVAL;
	}

	return 0;
}

static const struct file_operations chrdev_fops = {
	.open           = chrdev_open,
	.write          = chrdev_write,
	.release        = chrdev_release,
	.read           = chrdev_read,
	.unlocked_ioctl = chrdev_ioctl,
};

static int __init chrdev_mod_init(void)
{
	int ret;

	is_open = 0;
	data_size = 0;

	major = register_chrdev(0, CHRDEV_DEV_NAME, &chrdev_fops);
	if (major < 0) {
		pr_err("Error %d. Cannot register char device\n", major);
		return major;
	}
	pr_info("Chrdev registered, major = %d\n", major);

	pclass = class_create(THIS_MODULE, CHRDEV_CLASS_NAME);
	if (IS_ERR(pclass)) {
		unregister_chrdev(major, CHRDEV_DEV_NAME);
		pr_err("Cannot create class\n");
		return PTR_ERR(pclass);
	}
	pr_info("Class successfully created\n");

	pdev = device_create(pclass, NULL, MKDEV(major, 0), NULL, CHRDEV_CLASS_NAME "0");
	if (IS_ERR(pdev)) {
		class_destroy(pclass);
		unregister_chrdev(major, CHRDEV_DEV_NAME);
		pr_err("Cannot create device\n");
		return PTR_ERR(pdev);
	}
	pr_info("Device node created successfully\n");

	ret = led_gpio_init(LED_GREEN, &ledg_gpio);
	if (ret != 0) {
		pr_err("Cannot set GPIO%d for output\n", LED_GREEN);
		return ret;
	}

	ret = led_gpio_init(LED_RED, &ledr_gpio);
	if (ret != 0) {
		pr_err("Can't set GPIO%d for output\n", LED_RED);
		return ret;
	}

	gpio_set_value(ledg_gpio, 0);
	gpio_set_value(ledr_gpio, 0);

	pr_info("module loaded\n");

	return 0;
}

static void __exit chrdev_mod_exit(void)
{
	device_destroy(pclass, MKDEV(major, 0));
	class_destroy(pclass);
	unregister_chrdev(major, CHRDEV_DEV_NAME);

	gpio_set_value(ledg_gpio, 0);
	gpio_set_value(ledr_gpio, 0);
	gpio_free(ledg_gpio);
	gpio_free(ledr_gpio);

	pr_info("module exited\n");
}

module_init(chrdev_mod_init);
module_exit(chrdev_mod_exit);

MODULE_AUTHOR(CHRDEV_MODULE_AUTHOR);
MODULE_DESCRIPTION("Char device module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
