#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static const char* module_name = "mymodule";
static int a = 0;
static int b = 0;
module_param(a, int, 0);
module_param(b, int, 0);

static int __init hello_init(void)
{
	printk(KERN_INFO "%s: Hello world!\n", module_name);
	printk("%s: a = %i\n", module_name, a);
	printk("%s: b = %i\n", module_name, b);
	printk("%s: a + b = %i\n", module_name, a+b);
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "%s: Goodbye, world!\n", module_name);
	printk("%s: a - b = %i\n", module_name, a-b);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ViacheslavHolubnychyi");
MODULE_DESCRIPTION("simple description");
MODULE_VERSION("0.1");

MODULE_INFO(intree, "Y");
