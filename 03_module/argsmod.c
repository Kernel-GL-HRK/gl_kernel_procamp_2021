// SPDX-License-Identifier: GPL-2.0-only

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Krasnoshchok <ct3da21164@protonmail.ch>");
MODULE_DESCRIPTION("Compute sum & diff of mod's args upon init/exit.");
MODULE_VERSION("0.1");

static int first_arg;
static int second_arg;

module_param(first_arg, int, 0444); /* S_IRUGO */
MODULE_PARM_DESC(first_arg, "The first int arg.");

module_param(second_arg, int, 0444);
MODULE_PARM_DESC(second_arg, "The second int arg.");

static int __init args_test_init(void)
{
	pr_info("%d + %d = %d\n", first_arg, second_arg, (first_arg + second_arg));
	return 0;
}

static void __exit args_test_exit(void)
{
	pr_info("%d - %d = %d\n", first_arg, second_arg, (first_arg - second_arg));
}

module_init(args_test_init);
module_exit(args_test_exit);

