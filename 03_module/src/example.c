#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yurii Bondarenko <iuriy.bondarenko@gmail.com>");
MODULE_DESCRIPTION("Simple loadable module with parameters");

static int first = 0;
static int second = 0;

module_param(first, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(first, "First term");
module_param(second, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(second, "Second term");

int init_module(void)
{
	// WARNING: on 32-bit platforms might cause overflow, but idc
	pr_info("%d + %d = %ld\n", first, second, (long)first + second);
	return 0;
}

void cleanup_module(void)
{
	// WARNING: on 32-bit platforms might cause overflow, but idc
	pr_info("%d - %d = %ld\n", first, second, (long)first - second);
}
