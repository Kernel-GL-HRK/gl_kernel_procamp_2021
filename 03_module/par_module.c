#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

static int param1 = 0;
module_param(param1, int, 0660);

static int param2 = 0;
module_param(param2, int, 0660);

int init_module(void)
{
	printk(KERN_WARNING "Hello!\nSum: %d + %d = %d\n", param1, param2, param1 + param2);
	printk(KERN_WARNING "Subtruction: %d - %d = %d\n", param1, param2, param1 - param2);
	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "Good buy!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Serhii.Sherepera");
MODULE_DESCRIPTION("Lesson_3 \"Module\" homework");
MODULE_VERSION("0.1");
