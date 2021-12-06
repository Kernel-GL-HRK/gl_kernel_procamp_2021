// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int param_1 = 2;
module_param(param_1, int, 0660);

static int param_2 = 1;
module_param(param_2, int, 0660);

static int simple_init(void)
{
	pr_info("Start: param_1=%d, param_2=%d, param_1+param_2=%d\n", param_1, param_2, param_1+param_2);
	return 0;
}

static void simple_cleanup(void)
{
	pr_info("Stop: param_1=%d, param_2=%d, param_1-param_2=%d\n", param_1, param_2, param_1-param_2);
}

module_init(simple_init);
module_exit(simple_cleanup);

MODULE_AUTHOR("Vadym Hutovskyi <vadym.hutovskyi@globallogic.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
