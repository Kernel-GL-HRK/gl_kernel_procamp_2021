/**
 * @file string_storage.c
 * @brief String Storage kernel module.
 *   This module allows you to store and retrieve stored strings through sysfs
 *   file in /sys/kernel/string_storage/list.
 *   Type echo "some string" > /sys/kernel/string_storage/list to save the string,
 *   type cat /sys/kernel/string_storage/list to print all the saved strings.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <asm-generic/bug.h>

struct node
{
    struct list_head list;
    const char *string;
};

struct my_object
{
    struct kobject  *kobj;
    struct list_head strings_list;
    struct kobj_attribute kattr;
};

static ssize_t string_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf);

static ssize_t string_store(struct kobject *kobj, struct kobj_attribute *attr,
    const char *buf, size_t count);

static struct my_object string_storage =
{
    .kattr = __ATTR(list, 0660, string_show, string_store),
};

static ssize_t string_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf)
{
    struct node *entry;
    struct list_head *entity;
    struct list_head *tmp;
    ssize_t len = 0;

    (void)kobj;
    (void)attr;

    list_for_each_safe(entity, tmp, &string_storage.strings_list)
    {
        entry = list_entry(entity, struct node, list);

        BUG_ON(entry == NULL);
        BUG_ON(entry->string == NULL);

        const size_t entry_len = strlen(entry->string);
        const size_t tmp       = strlcpy(&buf[len], entry->string, PAGE_SIZE - len);
        if (tmp != entry_len)
        {
            BUG();
            len = -ENOMEM;
            pr_debug("[STR_STORAGE] String copy failed or PAGE_SIZE was reached");
            break;
        }

        len += tmp;
    }

    return len;
}

static ssize_t string_store(struct kobject *kobj, struct kobj_attribute *attr,
    const char *buf, size_t count)
{
    struct node *str;

    (void)kobj;
    (void)attr;

    str = kmalloc(sizeof(struct node), GFP_KERNEL);
    if (str == NULL)
        return -ENOMEM;

    str->string = kstrdup_const(buf, GFP_KERNEL);
    if (str->string == NULL)
        return -ENOMEM;

    pr_debug("[STR_STORAGE] String %s stored", str->string);

    list_add_tail(&str->list, &string_storage.strings_list);

    return count;
}

static int string_store_init(void)
{
    string_storage.kobj = kobject_create_and_add("string_storage", kernel_kobj);
    if (string_storage.kobj == NULL)
        return -ENOMEM;

    const int res = sysfs_create_file(string_storage.kobj, &string_storage.kattr.attr);
    if (res)
    {
        pr_err("[STR_STORAGE] Failed to create sysfs file");
        kobject_put(string_storage.kobj);
        return res;
    }

    INIT_LIST_HEAD(&string_storage.strings_list);

    return res;
}

static void string_store_exit(void)
{
    struct node *entry;
    struct list_head *entity;
    struct list_head *tmp;

    list_for_each_safe(entity, tmp, &string_storage.strings_list)
    {
        entry = list_entry(entity, struct node, list);

        list_del(entity);

        BUG_ON(entry == NULL);
        BUG_ON(entry->string == NULL);

        kfree(entry->string);
        kfree(entry);
    }

    kobject_put(string_storage.kobj);
    string_storage.kobj = NULL;
}

module_init(string_store_init);
module_exit(string_store_exit);

MODULE_DESCRIPTION("String Storage kernel module");
MODULE_AUTHOR("Sergey D.");
MODULE_LICENSE("GPL");
