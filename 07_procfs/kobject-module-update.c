#include <linux/init.h>               // Macros used to mark up functions  __init __exit
#include <linux/module.h>            // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/moduleparam.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>


MODULE_LICENSE( "GPL" );
MODULE_DESCRIPTION("A simple Linux...");  
MODULE_VERSION("0.1");

static LIST_HEAD(linkedlist);

typedef struct my_arr_str_t {
    char str [ 20 ]; 
    struct list_head list;
}               my_arr_str_s;


static my_arr_str_s my_arr [ 20 ];
static unsigned int counter = 0;
static unsigned int store_counter = 0;
static unsigned int show_counter = 0;
char *author_name = "denys\n";
char msg [ 40 ] = { 0 };

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf)
{
    int res = 0;
    int cursor = 0;
    struct my_arr_str_t *cur_node = NULL;

    list_for_each_entry(cur_node, &linkedlist, list){
        res = sprintf(&buf[cursor], "%s\n", cur_node->str);

        if (res > 0)
			cursor += res;

    }
    show_counter++;
    //sprintf(buf, "%u\n", value);
    return strlen(buf);
}

static ssize_t my_store(struct kobject *kobj, struct kobj_attribute *attr,
    const char *buf, size_t count)
{
    sscanf(buf, "%s\n", my_arr[counter++].str);
    //pr_info("add: |%s|\n", my_arr[counter - 1].str);
    //list_add( &my_arr[counter - 1].list, &my_arr[0].list );
    list_add_tail(&my_arr[counter - 1].list, &linkedlist);
    store_counter++;
    return count;
}


static ssize_t myread(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    sprintf(msg, "%u\n", show_counter);

    int len = strlen( msg );
   //printk( KERN_INFO "=== read : %ld\n", (long)count );
   if( count < len ) return -EINVAL;
   if( *ppos != 0 ) {
      //printk( KERN_INFO "=== read return : 0\n" );  // EOF
      return 0;
   }
   if( copy_to_user( pbuf, msg, len ) ) return -EINVAL;
   *ppos = len;
   //printk( KERN_INFO "=== read return : %d\n", len );
   return len;
}

static ssize_t myread1(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    sprintf(msg, "%u\n", store_counter);

    int len = strlen( msg );
   //printk( KERN_INFO "=== read : %ld\n", (long)count );
   if( count < len ) return -EINVAL;
   if( *ppos != 0 ) {
      //printk( KERN_INFO "=== read return : 0\n" );  // EOF
      return 0;
   }
   if( copy_to_user( pbuf, msg, len ) ) return -EINVAL;
   *ppos = len;
   //printk( KERN_INFO "=== read return : %d\n", len );
   return len;
}

static ssize_t myread2(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    int len = strlen( author_name );
   //printk( KERN_INFO "=== read : %ld\n", (long)count );
   if( count < len ) return -EINVAL;
   if( *ppos != 0 ) {
      //printk( KERN_INFO "=== read return : 0\n" );  // EOF
      return 0;
   }
   if( copy_to_user( pbuf, author_name, len ) ) return -EINVAL;
   *ppos = len;
   //printk( KERN_INFO "=== read return : %d\n", len );
   return len;
}

static struct kobj_attribute list_attribute =
    __ATTR(param, 0664, my_show, my_store);

static struct kobject *my_kobj;

//procfs
static struct proc_ops myops =
{
    .proc_read = myread,
};
static struct proc_ops myops1 =
{
    .proc_read = myread1,
};
static struct proc_ops myops2 =
{
    .proc_read = myread2,
};

static struct proc_dir_entry *ent;
static struct proc_dir_entry *ent1;
static struct proc_dir_entry *ent2;
//

static int my_init(void)
{
    int res = 0;
//procfs
    ent = proc_create("my-show_counter", 0666, NULL, &myops);
    ent1 = proc_create("my-store_counter", 0666, NULL, &myops1);
    ent2 = proc_create("my-author_name", 0666, NULL, &myops2);

    INIT_LIST_HEAD(&my_arr[counter].list);

    my_kobj = kobject_create_and_add("my-kobj", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;
    res = sysfs_create_file(my_kobj, &list_attribute.attr);
    if (res)
        kobject_put(my_kobj);
    return res;
}



static void my_exit(void)
{
	struct my_arr_str_t *cur_node = NULL;
	struct my_arr_str_t *tmp_node = NULL;

    list_for_each_entry_safe(cur_node, tmp_node, &linkedlist, list) {
        list_del(&cur_node->list);
    }
    proc_remove(ent);
    proc_remove(ent1);
    proc_remove(ent2);
    kobject_put(my_kobj);
    pr_info("module exited\n");
}

module_init(my_init);
module_exit(my_exit);

