// SPDX-License-Identifier: GPL-2.0-only
/* Custom kernel module */
/* Prints dmesg messages on load and exit */


#include <linux/init.h> // Macros used to mark up functions  __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Aleksieiev <oleksandr.aleksieiev@globallogic.com>");
MODULE_DESCRIPTION("A simple Linux driver.");
MODULE_VERSION("0.1");

static int param_one = 1;
static int param_two = 1;
module_param(param_one, int, 0);
module_param(param_two, int, 0);

static int __init kpc_init(void)
{
	pr_info("custom_printk: kernel params sum.\n");
	pr_info("custom_printk: %d + %d = %d.\n", param_one, param_two, param_one + param_two);
	return 0;
}

static void __exit kpc_exit(void)
{
	pr_info("custom_printk: kernel params diff.\n");
	pr_info("custom_printk: %d - %d = %d.\n", param_one, param_two, param_one - param_two);

}


module_init(kpc_init);
module_exit(kpc_exit);
