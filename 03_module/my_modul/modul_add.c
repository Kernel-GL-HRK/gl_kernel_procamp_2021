#include <linux/kernel.h>
#include <linux/module.h>

static int znach_1 = 5;
static int znach_2 = 10;

module_param(znach_1, int, S_IRUSR | S_IRGRP);
MODULE_PARM_DESC(znach_1, "the first module's parameter");
module_param(znach_2, int, S_IRUSR | S_IRGRP);
MODULE_PARM_DESC(znach_2, "the second module's parameter");

int init_module(void)
{
	printk(KERN_INFO "znach_1 + znach_2 = %d\n", znach_1 + znach_2);
    
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "znach_1 - znach_2 = %d\n", znach_1 - znach_2);
}

MODULE_DESCRIPTION("My simple kernel module");
MODULE_AUTHOR("Valentin S.");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");