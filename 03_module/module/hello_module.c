#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static int a = 100;
module_param(a, int, 0660);

static int b = 50;
module_param(b, int, 0660);

int init_module(void)
{
    printk(KERN_INFO "Parameter %d and %d sum: %d\n", a, b, a + b);
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Parameter %d and %d substration: %d \n", a, b, a - b);
}
