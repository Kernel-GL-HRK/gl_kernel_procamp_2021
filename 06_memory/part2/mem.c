//#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

static int hello_init(void)
{
    int i = 0;
    char *p_mem;
    cycles_t my_c1, my_c2;

    int arr_mem_s[ 6 ] = { 1, 1000, 10000, 100000, 1000000, 4000000 };
    int arr_order[ 6 ] = { 1, 3, 4, 6, 8, 10 };

    for(i = 0; i < 6; i++)
    {
        my_c1 = get_cycles();

        p_mem = kmalloc( arr_mem_s[ i ], GFP_KERNEL );
        if( !p_mem )
        {
            printk( KERN_ALERT "~~~Eror allocating %d bytes", arr_mem_s[ i ] );
        }
        p_mem[0] = 'a';
        p_mem[ i - 1 ] = 'a';
        kfree( p_mem );

        my_c2 = get_cycles();

        printk( KERN_ALERT "***Allocating %d bytes by kmalloc in %llu cycles|", arr_mem_s[i], my_c2 - my_c1 );
    }

    for(i = 0; i < 6; i++)
    {
        my_c1 = get_cycles();

        p_mem = vmalloc( arr_mem_s[ i ] );
        if( !p_mem )
        {
            printk( KERN_ALERT "~~~Eror allocating %d bytes", arr_mem_s[ i ] );
        }
        p_mem[0] = 'a';
        p_mem[ arr_mem_s[ i ] - 1 ] = 'a';
        vfree( p_mem );

        my_c2 = get_cycles();

        printk( KERN_ALERT "***Allocating %d bytes by vmalloc in %llu cycles|", arr_mem_s[i], my_c2 - my_c1 );
    }

    for(i = 0; i < 6; i++)
    {
        my_c1 = get_cycles();

        p_mem = (char *)__get_free_pages( GFP_KERNEL, arr_order[ i ] );
        if( !p_mem )
        {
            printk( KERN_ALERT "~~~Eror allocating %d bytes", arr_order[ i ] );
        }
        p_mem[0] = 'a';
        p_mem[ arr_order[ i ] - 1 ] = 'a';
        free_pages( (unsigned long)p_mem, arr_order[ i ] );

        my_c2 = get_cycles();

        printk( KERN_ALERT "***Allocating %d order by __get_free_page in %llu cycles|", arr_order[i], my_c2 - my_c1 );
    }

    printk( KERN_ALERT "Hi!!!!" );


    return -1;
}


module_init(hello_init);


MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
