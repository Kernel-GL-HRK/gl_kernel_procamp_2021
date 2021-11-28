#include <linux/module.h>
#include <linux/kernel.h>

static int param_a=0;
module_param(param_a,int,0660);

static int param_b=0;
module_param(param_b,int,0660);

int init_module(void)
{
    printk(KERN_INFO "Hi world! Sum: %d\n", param_a+param_b);
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Bye world! Subtraction:%d\n", param_a-param_b);
}
