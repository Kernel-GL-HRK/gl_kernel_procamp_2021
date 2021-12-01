#include <linux/module.h>
#include <linux/kernel.h>

static int param_a;
module_param(param_a, int, 0660);

static int param_b;
module_param(param_b, int, 0660);

int init_module(void)
{
	pr_info("Hi world! Sum: %d\n", param_a + param_b);
	return 0;
}

void cleanup_module(void)
{
	pr_info("Bye world! Subtraction:%d\n", param_a - param_b);
}
