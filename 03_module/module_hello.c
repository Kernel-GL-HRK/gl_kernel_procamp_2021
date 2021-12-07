// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko <v.v.yosypenko@gmail.com>
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int x;
module_param(x, int, 0660);

static int y;
module_param(y, int, 0660);

static int __init hello_start(void)
{
    printk(KERN_INFO "Loading hello module...\n");
    printk(KERN_INFO "x + y = %d\n", x + y);
    return 0;
}

static void __exit hello_end(void)
{
    printk(KERN_INFO "Goodbye hello module...\n");
    printk(KERN_INFO "x - y = %d\n", x - y);
}

module_init(hello_start);
module_exit(hello_end);

MODULE_AUTHOR("Vasyl Yosypenko <v.v.yosypenko@gmail.com>");
MODULE_DESCRIPTION("The simple kernel module with cli support");
MODULE_LICENSE("GPL v2");
