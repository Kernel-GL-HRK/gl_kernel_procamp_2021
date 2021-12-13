#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int a = 0; 
static int b = 0;

module_param(a, int, 0660);
module_param(b, int, 0660);

int hello_init(void)
{
	printk(KERN_WARNING "\na + b = %i\n", a + b);
	return 0;
}

void hello_exit(void)
{
	printk(KERN_WARNING "\na - b = %i\n", a - b);

}

MODULE_LICENSE("GPL");

module_init(hello_init);
module_exit(hello_exit);
