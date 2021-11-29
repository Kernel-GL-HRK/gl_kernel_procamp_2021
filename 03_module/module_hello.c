#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int x=0;
module_param(x,int,0660);

static int y=0;
module_param(y,int,0660);

static int __init hello_start(void)
{
    printk(KERN_INFO "Loading hello module...\n");
    printk(KERN_INFO "x + y = %d\n", x + y);
    return 0;
}

static void __exit hello_end(void)
{
    printk(KERN_INFO "Goodbye hello module...\n");
    printk(KERN_INFO "x - y = %d\n", x - y);
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Vasyl Yosypenko");
