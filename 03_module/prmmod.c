/*
* Simple module as example of usage modules with parameters
*/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/init.h>

//first parameter initialisation
static int fprm = 1;
module_param(fprm,int,0660);

//second parameter initialisation
static int sprm = 2;
module_param(sprm,int,0660);


int prmmod_init(void)
{
	printk(KERN_INFO "Module with parameters fprm=%d sprm=%d initialisation!\n",fprm,sprm);
	printk(KERN_INFO "The sum of parameters fprm and sprm is %d\n",fprm+sprm);
	return 0;
}


void prmmod_exit(void) {
	printk(KERN_INFO "Module with parameters fprm=%d sprm=%d initialisation!\n",fprm,sprm);
	printk(KERN_INFO "The substraction of parameters fprm and sprm is %d\n",fprm-sprm);
	printk(KERN_INFO "Bye Bye...\n");
}

module_init(prmmod_init);
module_exit(prmmod_exit);
