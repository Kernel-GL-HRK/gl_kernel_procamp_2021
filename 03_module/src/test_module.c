#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/jiffies.h>
#include <linux/types.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR( "Vitaliy Zhyrko vitaliyzh@gmail.com" );
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static int val1 = 0;
static int val2 = 0;

module_param( val1, int, S_IRUGO | S_IWUSR );
module_param( val2, int, S_IRUGO | S_IWUSR );

static int __init mod_init( void )
{
    printk(KERN_INFO "module: (val1: %d + val2: %d) = %d", val1, val2, val1+val2 );
    return 0;
}

static void __exit mod_exit(void)
{
    printk(KERN_INFO "module: (val1: %d - val2: %d) = %d", val1, val2, val1-val2 );
}

module_init( mod_init );
module_exit( mod_exit );
