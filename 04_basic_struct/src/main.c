#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/time.h>


#define MODULE_TAG      "example_module "
#define PROC_DIRECTORY  "example"
#define PROC_FILENAME   "buffer"
#define BUFFER_SIZE     32
#define LEN_MSG 160

struct msg_item
{
    struct list_head msg_list;
    char *message;
    size_t lenght;
};

LIST_HEAD( msg_list );

void show_msgs(void)
{
    struct list_head *iter;
    struct msg_item *item;
    list_for_each( iter, &msg_list )
    {
       item = list_entry( iter, struct msg_item, msg_list );
       printk( KERN_INFO "Show: [MSG] %s, [LEN] %ld\n", item->message, item->lenght );
    }
}

static void add_msg(const char *msg, const size_t len)
{
    struct msg_item *item;
    item = kmalloc( sizeof(*item), GFP_KERNEL );
    item->lenght = len;
    item->message = kmalloc(len, GFP_KERNEL);
    memcpy(item->message, (char *)msg, item->lenght);
    list_add( &(item->msg_list), &msg_list );
    printk( KERN_INFO "Add: [MSG] %s, [LEN] %ld\n", item->message, item->lenght );
}

void free_lists( void )
{
    struct list_head *iter, *iter_safe;
    struct msg_item *item;
    list_for_each_safe( iter, iter_safe, &msg_list )
    {
       item = list_entry( iter, struct msg_item, msg_list );
       list_del( iter );
       kfree( item->message );
       kfree( item );
    }
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)

#define IOFUNCS( name )                                                         \
static char buf_##name[ LEN_MSG + 1 ] = "0\n";                                  \
static ssize_t SHOW_##name( struct class *class, struct class_attribute *attr,  \
                            char *buf ) {                                       \
   strcpy( buf, buf_##name );                                                   \
   printk( "read %ld\n", (long)strlen( buf ) );                                 \
   show_msgs();                                                                 \
   return strlen( buf );                                                        \
}                                                                               \
static ssize_t STORE_##name( struct class *class, struct class_attribute *attr, \
                             const char *buf, size_t count ) {                  \
   printk( "write %ld\n", (long)count );                                        \
   strncpy( buf_##name, buf, count );                                           \
   buf_##name[ count ] = '\0';                                                  \
   add_msg(buf, count);                                                         \
   return count;                                                                \
}

#else

#define IOFUNCS( name )                                                         \
static char buf_##name[ LEN_MSG + 1 ] = "не инициализировано "#name"\n";        \
static ssize_t SHOW_##name( struct class *class, char *buf ) {                  \
   strcpy( buf, buf_##name );                                                   \
   printk( "read %ld\n", (long)strlen( buf ) );                                 \
   return strlen( buf );                                                        \
}                                                                               \
static ssize_t STORE_##name( struct class *class, const char *buf,              \
                             size_t count ) {                                   \
   printk( "write %ld\n", (long)count );                                        \
   strncpy( buf_##name, buf, count );                                           \
   buf_##name[ count ] = '\0';                                                  \
   return count;                                                                \
}
#endif

IOFUNCS( list );
#define OWN_CLASS_ATTR( name ) \
   struct class_attribute class_attr_##name = \
   __ATTR( name, ( S_IRWXG | S_IRWXU /*| S_IRWXO*/ ), &SHOW_##name, &STORE_##name )

static OWN_CLASS_ATTR( list );

static struct kobject *my_kobj;

static struct class *x_class=NULL;


static int create_sysclass(void)
{
    int res;
    x_class = class_create( THIS_MODULE, "example-class" );
    if( IS_ERR( x_class ) ) printk( "bad class create\n" );
    res = class_create_file( x_class, &class_attr_list );

    return res;
}

void x_cleanup(void)
{
    if(x_class)
    {
        class_remove_file( x_class, &class_attr_list );
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


    my_kobj = kobject_create_and_add("my_kobj", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;
    res = sysfs_create_file(my_kobj, &class_attr_list.attr);
    if (res)
        kobject_put(my_kobj);

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

    free_lists();
    kobject_put(my_kobj);
}

static int __init example_init(void)
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


static void __exit example_exit(void)
{
    x_cleanup();
    cleanup_buffer();
    printk(KERN_NOTICE MODULE_TAG "exited\n");
}


module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Vitaliy Zhyrko <vitaliyzh@gmail.com>" );
MODULE_VERSION( "1.0" );
MODULE_DESCRIPTION("Example for sysfs show/store" );
