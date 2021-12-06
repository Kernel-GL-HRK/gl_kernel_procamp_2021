// SPDX-License-Identifier: MIT

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int a;
module_param(a, int, 0660);

static int b;
module_param(b, int, 0660);

static int module_open(void)
{
	pr_info("Addition process\n");
	pr_info("Hi...P1=%d P2=%d Summ=%d\n", a, b, a+b);
	return 0;
}
static void module_end(void)
{
	pr_info("Processing substration\n");
	pr_info("bye...P1=%d P2=%d Subst=%d\n", a, b, a-b);
}
module_init(module_open);
module_exit(module_end);
