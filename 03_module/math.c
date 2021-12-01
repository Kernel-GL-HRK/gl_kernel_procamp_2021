#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Serhii Mykhailov");

static int left;
module_param(left, int, 0660);
MODULE_PARM_DESC(left, "Left value");

static int right;
module_param(right, int, 0660);
MODULE_PARM_DESC(right, "Right value");

int init_module(void)
{
    int sum_result = left + right;
    printk(KERN_INFO "Sum result: %d!\n", sum_result);
    return 0;
}

void cleanup_module(void)
{
    int subtract_result = left - right;
    printk(KERN_INFO "Subtraction result: %d!\n", subtract_result);
}
