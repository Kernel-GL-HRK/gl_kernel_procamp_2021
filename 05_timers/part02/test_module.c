#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/jiffies.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/timex.h>
#include <linux/cdev.h>

#define MODULE_TAG      "example_module "
#define PROC_DIRECTORY  "example"
#define PROC_FILENAME   "buffer"
#define BUFFER_SIZE     32
#define LEN_MSG 160

u64 abs_time=0;
struct timespec64 abs_time_ts={0,0};
cycles_t relative_time=0;

static ssize_t x_show( struct class *class, struct class_attribute *attr,
                       char *buf )
{
    cycles_t cl = get_cycles();

    //unsigned long us = cl / CLOCK_TICK_RATE;
    printk(KERN_INFO "module: relative_time in cycles: %llu",
           cl-relative_time);
    relative_time = cl;

    printk(KERN_INFO "module: prev absolute time in sec: %llu.%lu",
           abs_time_ts.tv_sec, abs_time_ts.tv_nsec);
    ktime_get_ts64(&abs_time_ts);

    return 0;
}

static ssize_t x_store( struct class *class, struct class_attribute *attr,
                        const char *buf, size_t count )
{
    return 0;
}

#define CLASS_ATTR(_name, _mode, _show, _store) \
struct class_attribute class_attr_##_name = \
__ATTR(_name, _mode, _show, _store)
CLASS_ATTR( xxx, ( S_IWUSR | S_IRUGO ), &x_show, &x_store );

static struct class *x_class=NULL;


static int create_sysclass(void)
{
    int res;
    x_class = class_create( THIS_MODULE, "example-class" );
    if( IS_ERR( x_class ) ) printk( "bad class create\n" );
    res = class_create_file( x_class, &class_attr_xxx );

    return res;
}

void x_cleanup(void)
{
    if(x_class)
    {
        class_remove_file( x_class, &class_attr_xxx );
        class_destroy( x_class );
    }
}

static char *proc_buffer;
static size_t proc_msg_length;


static int create_buffer(void)
{
    int res = 0;

    proc_buffer = kmalloc(BUFFER_SIZE+1, GFP_KERNEL);
    if (NULL == proc_buffer)
        return -ENOMEM;
    proc_msg_length = 0;

    return res;
}

static void cleanup_buffer(void)
{
    if (proc_buffer)
    {
        kfree(proc_buffer);
        proc_buffer = NULL;
    }
    proc_msg_length = 0;
}

static int __init mod_init( void )
{
    int err;

    err = create_buffer();
    if (err)
        goto error;

    err = create_sysclass();
    if (err)
        goto error;

    printk(KERN_NOTICE MODULE_TAG "loaded\n");
    return 0;

error:
    printk(KERN_ERR MODULE_TAG "failed to load\n");
    x_cleanup();
    cleanup_buffer();
    return err;
}

static void __exit mod_exit(void)
{
    x_cleanup();
    cleanup_buffer();
    printk(KERN_NOTICE MODULE_TAG "exited\n");
}

module_init( mod_init );
module_exit( mod_exit );

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Vitaliy Zhyrko <vitaliyzh@gmail.com>" );
MODULE_VERSION( "1.0" );
MODULE_DESCRIPTION("Example for sysfs get_cycles" );
