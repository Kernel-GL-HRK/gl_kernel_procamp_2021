// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>



typedef struct my_list {
    char *my_str;
    struct list_head list;
} my_list_t;


static LIST_HEAD(my_list);

static struct kobject *my_list_kobj;

// Helper functions

static void my_list_append(const char *user_str, ssize_t str_size)
{
    // create new node
    struct my_list* tmpNode = kmalloc(sizeof(struct my_list), GFP_KERNEL);

    if (!tmpNode)
        pr_err("Can't allocate memory for one node");

    tmpNode->my_str = (char*) kmalloc((str_size + 1), GFP_KERNEL);

    strncpy(tmpNode->my_str, user_str, (str_size + 1) );

    INIT_LIST_HEAD(&tmpNode->list);

    list_add_tail(&(tmpNode->list), &my_list);
}

static void list_print(void)
{
    struct my_list *cur_pos = NULL;

    list_for_each_entry(cur_pos, &my_list, list)
    {
        pr_info("%s\n", cur_pos->my_str);
    }
}


static ssize_t my_list_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    list_print();

    return strlen(buf);
}


static ssize_t my_list_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    my_list_append(buf, count);

    return count;
}

static struct kobj_attribute list_attribute =
    __ATTR(my_list, 0664, my_list_show, my_list_store);


static int __init my_list_start(void)
{
    int res = 0;

    my_list_kobj = kobject_create_and_add("my_list", kernel_kobj);
    if (!my_list_kobj)
        return -ENOMEM;

    res = sysfs_create_file(my_list_kobj, &list_attribute.attr);
    if (res)
        kobject_put(my_list_kobj);

    pr_info("Module initialized successfully \n");

    return res;
}

static void __exit my_list_start_end(void)
{
    struct my_list *cur_pos = NULL;

    list_for_each_entry(cur_pos, &my_list, list)
    {
       kfree(cur_pos->my_str);
       list_del(&(cur_pos->list));
       kfree(cur_pos);
    }

    kobject_put(my_list_kobj);

    pr_info("Module removed successfully \n");
}

module_init(my_list_start);
module_exit(my_list_start_end);

MODULE_AUTHOR("Vasyl Yosypenko");
MODULE_DESCRIPTION("Simle module with basic data structures");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");
