// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>

MODULE_AUTHOR("Vladyslav Andrishko <vladyslav.andrishko@globallogic.com>");
MODULE_DESCRIPTION("Third Task in Linux Kernel Training Course");
MODULE_LICENSE("Dual BSD/GPL");

static int task = 1;
module_param(task, int, 0);
MODULE_PARM_DESC(task, "Task number");

static char *user;
module_param(user, charp, 0);
MODULE_PARM_DESC(user, "User name");

static int __init kernel_init(void)
{
	if (user != NULL)
		pr_warn("\nHello, %s!\n", user);
	else
		pr_info("\nOps..., I don`t know who u are?\n");
	if (task == 3)
		pr_info("\nHmm, yeah this is third task in this course\n");
	else {
		pr_info("\nYou work too much. You even don`t know what is it\n");
		return -EINVAL;
	}
	return 0;
}

static void __exit kernel_exit(void)
{
	pr_info("\nOkay, congatulations. Task 03 done.Byee...%s\n", user);
}

module_init(kernel_init);
module_exit(kernel_exit);