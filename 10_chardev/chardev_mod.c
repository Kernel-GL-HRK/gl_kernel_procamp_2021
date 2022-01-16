// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/delay.h>

#include "ioctl_common.h"

#define GPIO_NUMBER(port, bit) (32 * (port) + (bit))


/*  https://linux-sunxi.org/Xunlong_Orange_Pi_Zero#LEDs
 *  Board config for OPI-Zero:
 *  LED GREEN (PL10): GPIO_11_10
 *  LED RED   (PA17): GPIO_0_17
 *
 */

#define LED_GREEN GPIO_NUMBER(11, 10)
#define LED_RED GPIO_NUMBER(0, 17)

#define DEVICE_NAME "led_chardev"

static int major;
static struct class *dev_class;
static struct device *my_dev;

static leds_cont_t led_control;

static int gpio_led_green = -1;
static int gpio_led_red = -1;


static int device_open(struct inode *inode, struct file *finst)
{
    pr_info("device_open()\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *finst)
{
    pr_info("device_release()\n");
    return 0;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    pr_info("device_write()\n");
    return 0;
}

static long int led_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{
    switch(cmd)
    {
        case WR_VALUE:
            if (copy_from_user(&led_control, (leds_cont_t *)args, sizeof(led_control)))
                pr_err("led_ioctl: error copying data from user\n");
            else
                pr_info("led_ioctl: led_state was updated to %d\n", (uint32_t)led_control.led_state);

                switch(led_control.led_color)
                {
                    case RED:
                        gpio_set_value(gpio_led_red, led_control.led_state);
                        pr_info("LED_RED: status %d\n", led_control.led_state);
                        break;

                    case GREEN:
                        gpio_set_value(gpio_led_green, led_control.led_state);
                        pr_info("LED_GREEN: status %d\n", led_control.led_state);
                        break;

                    default:
                        pr_err("Incorrect LED color\n");
                        break;
                }
            break;

        default:
            break;
    }
    return 0;
}



static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = led_ioctl
};

static int led_gpio_init(int gpio, int *led_gpio)
{
    int res;

    res = gpio_direction_output(gpio, 0);
    if (res != 0)
        return res;

    *led_gpio = gpio;
    return 0;
}


/* Module entry/exit points */
static int __init chardev_init(void)
{
    int res;
    int blink_counter;
    pr_info("chardev_init\n");

    // this name will be in /proc/devices
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if(major < 0) {
        pr_err("Failed to register device %d\n", major);
        return major;
    }
    pr_info("register chrdev, major = %d\n", major);

    // the name will appear in /sys/class/<name>.
    dev_class = class_create(THIS_MODULE, "my_chdev_led");
    if (dev_class == NULL)
    {
        pr_err("Device class can't be created\n");
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(dev_class);
    }

    pr_info("chrdev: device class created successfully\n");

    // create device file
    // device name will appear in /sys/devices/virtual/<class name>/<device name>
    // and /dev/<device name>
    my_dev = device_create(dev_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (my_dev == NULL)
    {
        pr_err("Device file can't be created\n");
        class_destroy(dev_class);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(my_dev);
    }

    pr_info("chrdev: device node created successfully\n");

    res = led_gpio_init(LED_GREEN, &gpio_led_green);
    if (res != 0) {
        pr_err("Can't set GPIO%d for output\n", LED_GREEN);
        return res;
    }

    res = led_gpio_init(LED_RED, &gpio_led_red);
    if (res != 0) {
        pr_err("Can't set GPIO%d for output\n", LED_RED);
        return res;
    }

    // Led blinks 3 times
    // to demostrate that LED kernel model was loaded
    pr_info("Both LEDs will be blining 3 times\n");
    for(blink_counter = 3; blink_counter > 0; blink_counter--)
    {
        gpio_set_value(gpio_led_green, 1);
        gpio_set_value(gpio_led_red, 1);
        mdelay(100);

        gpio_set_value(gpio_led_green, 0);
        gpio_set_value(gpio_led_red, 0);
        mdelay(100);
    }

    pr_info("chrdev: module loaded\n");

    return 0;

}

static void __exit chardev_exit(void)
{
    gpio_set_value(gpio_led_green, 0);
    gpio_set_value(gpio_led_red, 0);

    device_destroy(dev_class, MKDEV(major, 0));
    class_destroy(dev_class);
    unregister_chrdev(major, DEVICE_NAME);

    pr_info("chrdev: module exited\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_AUTHOR("Vasyl Yosypenko");
MODULE_DESCRIPTION("My char device module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
