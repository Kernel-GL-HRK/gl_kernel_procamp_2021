/*
 * Name of this module is English wiki random article choice.
 * To get away from "hello", "foo", "bar", "baz"... names.
 */
#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>


struct msg_item {
        struct list_head list;
        char *msg;
        size_t msg_len;
};
static LIST_HEAD(msg_list_head);

static ssize_t wimbledon_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t wimbledon_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);
static int wimbledon_init(void);
static void wimbledon_exit(void);

static struct kobj_attribute list_attribute =
        __ATTR(list, 0644, wimbledon_show, wimbledon_store);
static struct kobject *wimbledon_kobj;


static ssize_t wimbledon_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
    struct msg_item *pos = NULL;
    size_t item_number = 0;
    size_t count = 0;
    char *p_buff = buff;

    list_for_each_entry_reverse(pos, &msg_list_head, list) {
        pr_info("msg #%lu size: %lu '%.*s'\n",
                item_number, pos->msg_len, (int)pos->msg_len, (char *)pos->msg);

        memcpy(p_buff, (char *)pos->msg, pos->msg_len);
        p_buff += pos->msg_len;
        count += pos->msg_len;
        item_number += 1;
    }
    return count;
}


static ssize_t wimbledon_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
    char *p_msg = NULL;
    struct msg_item *p_item = NULL;

    p_msg = kmalloc(count, GFP_KERNEL);
    if (!p_msg) {
        pr_err("could not allocate msg cnt: %lu\n", count);
        return -ENOMEM;
    }
    p_item = kmalloc(sizeof(struct msg_item), GFP_KERNEL);
    if (!p_item) {
        pr_err("could not allocate msg_item\n");
        return -ENOMEM;
    }
    memcpy(p_msg, buff, count);
    p_item->msg = p_msg;
    p_item->msg_len = count;

    pr_info("adding msg size: %lu to the list: '%.*s'\n",
            p_item->msg_len, (int)p_item->msg_len, (char *)p_item->msg);
    list_add(&p_item->list, &msg_list_head);
    return count;
}


static int wimbledon_init(void)
{
    int res = 0;
    wimbledon_kobj = kobject_create_and_add("wimbledon", kernel_kobj);
    if (!wimbledon_kobj)
        return -ENOMEM;
    res = sysfs_create_file(wimbledon_kobj, &list_attribute.attr);
    if (res)
        kobject_put(wimbledon_kobj);

    return res;
}


static void wimbledon_exit(void)
{
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct msg_item *p_item = NULL;

    list_for_each_safe(pos, n, &msg_list_head) {
         p_item = list_entry(pos, struct msg_item, list);
         pr_info("freeing msg size: %lu from the list: '%.*s'\n",
                 p_item->msg_len, (int)p_item->msg_len, (char *)p_item->msg);
         list_del(pos);
         kfree(p_item->msg);
         kfree(p_item);
    }
    pr_info("list is empty: %d\n", list_empty_careful(&msg_list_head));
    kobject_put(wimbledon_kobj);
    pr_info("Module exited\n");
}

module_init(wimbledon_init);
module_exit(wimbledon_exit);

MODULE_AUTHOR("Ivan Stepanenko <istepanenko@gmail.com>");
MODULE_DESCRIPTION("Wimbledon module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
