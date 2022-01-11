#include <linux/init.h>               // Macros used to mark up functions  __init __exit
#include <linux/module.h>            // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/moduleparam.h>

static int int_param1;
static int int_param2;
module_param(int_param1, int, 0);
module_param(int_param2, int, 0);

MODULE_LICENSE( "GPL" );
MODULE_DESCRIPTION("A simple Linux driver.");  
MODULE_VERSION("0.1");

static int __init hello_init( void ) { 
    printk( KERN_NOTICE "Hello, world!" ); 

    printk( KERN_NOTICE "int1 + int2 = %d", int_param1 + int_param2 ); 
    
    return -1; 
} 

// static void __exit hello_exit( void ) { 
//       printk( KERN_DEBUG "Goodbye, world!" ); 
// } 


module_init( hello_init );  
//module_exit( hello_exit );



