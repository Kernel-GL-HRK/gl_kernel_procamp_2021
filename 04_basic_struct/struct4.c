#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>


struct m_item {
        struct list_head list;
        char *m;
        size_t m_len;
};
static LIST_HEAD(m_list_head);

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    struct m_item *pos = NULL;
    size_t item_number = 0;
    size_t count = 0;
    char *p_buff = buf;

    list_for_each_entry_reverse(pos, &m_list_head, list) 
    {
        pr_info("report #%lu size: %lu '%.*s'\n",
            item_number, pos->m_len, (int)pos->m_len, (char *)pos->m);
        
        memcpy(p_buff, (char *)pos->m, pos->m_len);
        p_buff += pos->m_len;
        count += pos->m_len;
        item_number += 1;
    }
    return count;
}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
    char *p_msg = NULL;
    struct m_item *p_item = NULL;

    p_msg = kmalloc(count, GFP_KERNEL);
    if (!p_msg) 
    {
        pr_err("could not allocate report: %lu\n", count);
        return -ENOMEM;
    }
    p_item = kmalloc(sizeof(struct m_item), GFP_KERNEL);
    if (!p_item) 
    {
        pr_err("could not allocate m_item\n");
        return -ENOMEM;
    }
    memcpy(p_msg, buf, count);
    p_item->m = p_msg;
    p_item->m_len = count;

    pr_info("adding report size: %lu to the list: '%.*s'\n",
            p_item->m_len, (int)p_item->m_len, (char *)p_item->m);
    list_add(&p_item->list, &m_list_head);  
    return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(list, 0664, hello_show, hello_store);

static struct kobject *hello_kobj;


static int hello_init(void)
{
	int res = 0;

    pr_info("module load\n");

	hello_kobj = kobject_create_and_add("hello", kernel_kobj);
    
	if (!hello_kobj)
		return -ENOMEM;
	res = sysfs_create_file(hello_kobj, &list_attribute.attr);
    
	if (res)
		kobject_put(hello_kobj);
	return res;
}

static void hello_exit(void)
{
	struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct m_item *p_item = NULL;

    list_for_each_safe(pos, n, &m_list_head) 
    {
        p_item = list_entry(pos, struct m_item, list);
        pr_info("freeing log size: %lu from the list: '%.*s'\n",
                 p_item->m_len, (int)p_item->m_len, (char *)p_item->m);
        list_del(pos);
        kfree(p_item->m);
        kfree(p_item);
    }
    pr_info("list is empty: %d\n", list_empty_careful(&m_list_head));
    kobject_put(hello_kobj);
	pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Serhii Siedin <serhii.siedin@globallogic.com>");
MODULE_DESCRIPTION("Basic data structures module Homework");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");