#include <linux/kernel.h>
#include <linux/module.h>

static int param_1 = 0;
static int param_2 = 0;

module_param(param_1, int, S_IRUSR | S_IRGRP);
MODULE_PARM_DESC(param_1, "the first module's parameter");
module_param(param_2, int, S_IRUSR | S_IRGRP);
MODULE_PARM_DESC(param_2, "the second module's parameter");

int init_module(void)
{
	printk(KERN_INFO "param_1 + param_2 = %d\n", param_1 + param_2);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "param_1 - param_2 = %d\n", param_1 - param_2);
}

MODULE_DESCRIPTION("My simple kernel module");
MODULE_AUTHOR("Michael N.");
MODULE_LICENSE("GPL");
