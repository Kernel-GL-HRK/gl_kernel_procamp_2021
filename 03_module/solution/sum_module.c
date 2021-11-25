
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */

static int first_param = 0;
static int second_param = 0;
module_param( first_param, int, 0 ); 
module_param( second_param, int, 0 ); 


int init_module(void)
{
   printk(KERN_INFO"Sum of first and second param is: %d\n",first_param + second_param);
	
   return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "Substract of parameters is: %d\n", first_param - second_param);
}  

MODULE_LICENSE("GPL");
