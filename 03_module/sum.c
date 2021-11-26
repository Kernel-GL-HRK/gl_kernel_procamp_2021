// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Komyshan");

static int firstNum;
static int secondNum;

module_param(firstNum, int, 0);
module_param(secondNum, int, 0);

int init_module(void)
{
	int sum = firstNum + secondNum;

	pr_info("Sum: %d\n", sum);

	return 0;
}

void cleanup_module(void)
{
	int substration = firstNum - secondNum;

	pr_info("Substration: %d\n", substration);
}

