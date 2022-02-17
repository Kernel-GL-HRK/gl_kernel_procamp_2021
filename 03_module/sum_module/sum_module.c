// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME

#include <linux/module.h>
#include <linux/kernel.h>

static int first_op, second_op;
module_param(first_op, int, 0660);
module_param(second_op, int, 0660);

int init_module(void)
{
	int sum;

	sum = first_op + second_op;
	pr_info("sum_module: Sum module loaded.\n");
	pr_info("sum_module: Sum of parameters: %d\n", sum);
	return 0;
}

void cleanup_module(void)
{
	int sub;

	sub = first_op - second_op;
	pr_info("sum_module: Substraction of parameters: %d\n", sub);
	pr_info("sum_module: Sum module exited.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Shyshko");
