// SPDX-License-Identifier: 0BSD

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int in1;
module_param(in1, int, 0660);

static int in2;
module_param(in2, int, 0660);

static int kmodule_init(void)
{
	printk(KERN_INFO "kmodule: in2=%d in2=%d\n", in1, in2);
	printk(KERN_INFO "kmodule: in1+in2=%d\n", (in1 + in2));
	printk(KERN_INFO "kmodule: in1-in2=%d\n", (in1 - in2));
	return 0;
}

static void kmodule_cleanup(void)
{
	printk(KERN_INFO "bye... kmodule\n");
}

module_init(kmodule_init);
module_exit(kmodule_cleanup);
