/**
 * @file sum_sub.c
 * @brief Sum and substaction Linux kernel module.
 *   This module takes two integers var1 and var2 as arguments
 *   and gives their sum on module initialization then gives their substraction on module cleanup stage
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int var1 = 0;
module_param(var1, int, 0);
MODULE_PARM_DESC(var1, "Integer #1");

static int var2 = 0;
module_param(var2, int, 0);
MODULE_PARM_DESC(var2, "Integer #2");

int sum_sub_init(void)
{
    printk(KERN_INFO "The sum of arguments var1 and var2 is %d\n", var1 + var2);
    return 0;
}

void sum_sub_cleanup(void)
{
    printk(KERN_INFO "The substraction of arguments var1 and var2 is %d\n", var1 - var2);
}

module_init(sum_sub_init);
module_exit(sum_sub_cleanup);

MODULE_DESCRIPTION("SUM SUB kernel module");
MODULE_AUTHOR("Sergey D.");
MODULE_LICENSE("GPL");
