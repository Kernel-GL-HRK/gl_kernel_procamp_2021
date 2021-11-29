#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>


static int n1=0;
module_param(n1, int, 0660);

static int n2=0;
module_param(n2, int, 0660);

int init_module(void)
{
    printk(KERN_INFO "hello add n1: %d and n2: %d result: %d\n", n1, n2, n1+n2);
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Bye substract n1: %d and n2: %d result: %d\n", n1, n2, n1-n2);
}

MODULE_AUTHOR("Ivan Stepanenko <istepanenko@gmail.com>");
MODULE_DESCRIPTION("Hello world module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
