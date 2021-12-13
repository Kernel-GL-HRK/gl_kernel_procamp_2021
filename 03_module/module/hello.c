#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static int a=0;
module_param(a,int,0660);

static int b=0;
module_param(b,int,0660);

int init_module(void)
{
	printk(KERN_INFO "Hi world! %d\n", a + b);
	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "Bye world! %d\n", a - b);
}

