#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int a=10;
module_param(a,int,0660);

static int b=15;
module_param(b,int,0660);

static int m;


static int module_open(void)
{
	printk(KERN_INFO "Operetion summary\n");
    printk(KERN_INFO "Parameter1=%d\n", a);
    printk(KERN_INFO "Parameter2=%d\n", b);

    m = a + b;

    printk(KERN_INFO "Summ=%d\n", m);

	return 0;
}

static void module_end(void) 
{
	printk(KERN_INFO "Operetion substration\n");
    printk(KERN_INFO "Parameter1=%d\n", a);
    printk(KERN_INFO "Parameter2=%d\n", b);
    
	m = a - b;

    printk(KERN_INFO "Subst=%d\n", m);
}


module_init (module_open);
module_exit (module_end);