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
#include <linux/proc_fs.h>


#define AUTHOR_NAME "Vasyl Yosypenko\n"
#define AUTHOR_NAME_SIZE (sizeof(AUTHOR_NAME)/sizeof(char))

#define PROCFS_FOLDER "my_module_iface"
#define PROCFS_ENTRY_AUTHOR "module_owner"
#define PROCFS_ENTRY_STORE_CNT "write_num"
#define PROCFS_ENTRY_SHOW_CNT "read_num"

#define INFO_MSG_BUFF_SIZE (64)
#define PROCFS_ENTRY_PERMISSON (0444)

typedef struct my_list {
    char *my_str;
    struct list_head list;
} my_list_t;


static LIST_HEAD(my_list);
static struct kobject *my_list_kobj;

static ssize_t show_cnt;
static ssize_t store_cnt;

static struct proc_dir_entry *procfs_dir_root_entry;

/* Helper functions */

static void my_list_append(const char *user_str, ssize_t str_size)
{
    // create new node
    struct my_list* tmpNode = (struct my_list*) kmalloc(sizeof(struct my_list), GFP_KERNEL);

    if (!tmpNode)
        pr_err("Can't allocate memory for one node");

    tmpNode->my_str = (char*) kmalloc((str_size + 1), GFP_KERNEL);
    strcpy(tmpNode->my_str, user_str);

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

/* End of Helper functions */


static ssize_t show_my_list(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    show_cnt++;

    list_print();

    return strlen(buf);
}


static ssize_t store_my_list(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    store_cnt++;

    my_list_append(buf, count);

    return count;
}

static struct kobj_attribute list_attribute =
    __ATTR(my_list, 0664, show_my_list, store_my_list);


static ssize_t procfs_read_author(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    static ssize_t not_copied;

    not_copied = copy_to_user(pbuf, AUTHOR_NAME, AUTHOR_NAME_SIZE);
    *ppos += AUTHOR_NAME_SIZE - not_copied;

    if (*ppos > AUTHOR_NAME_SIZE)
        return 0;
    else
        return AUTHOR_NAME_SIZE;
}

static ssize_t procfs_read_show_num(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    static ssize_t not_copied;
    static ssize_t buff_size;
    char msg_buff[INFO_MSG_BUFF_SIZE];

    buff_size = snprintf(msg_buff, INFO_MSG_BUFF_SIZE, "The show funct was called %zd times\n", show_cnt);

    not_copied = copy_to_user(pbuf, msg_buff, buff_size);
    *ppos += buff_size - not_copied;

    if (*ppos > buff_size)
        return 0;
    else
        return buff_size;
}

static ssize_t procfs_read_store_num(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    static ssize_t not_copied;
    static ssize_t buff_size;
    char msg_buff[INFO_MSG_BUFF_SIZE];

    buff_size = snprintf(msg_buff, INFO_MSG_BUFF_SIZE, "The store funct was called %zd times\n", store_cnt);

    not_copied = copy_to_user(pbuf, msg_buff, buff_size);
    *ppos += buff_size - not_copied;

    if (*ppos > buff_size)
        return 0;
    else
        return buff_size;
}

static struct proc_ops author_name_ops =
{
    .proc_read = procfs_read_author
};

static struct proc_ops read_show_num_ops =
{
    .proc_read = procfs_read_show_num
};

static struct proc_ops read_store_num_ops =
{
    .proc_read = procfs_read_store_num
};


static int __init my_list_start(void)
{
    int res = 0;
    procfs_dir_root_entry = NULL;

    show_cnt = 0;
    store_cnt = 0;

    my_list_kobj = kobject_create_and_add("my_list", kernel_kobj);
    if (!my_list_kobj)
        return -ENOMEM;

    res = sysfs_create_file(my_list_kobj, &list_attribute.attr);
    if (res)
        kobject_put(my_list_kobj);

    procfs_dir_root_entry = proc_mkdir(PROCFS_FOLDER, NULL);
    if(!procfs_dir_root_entry) {
        pr_err("Can't create procfs directory");
        return -ENOMEM;
    }

    if (!proc_create(PROCFS_ENTRY_AUTHOR, PROCFS_ENTRY_PERMISSON, procfs_dir_root_entry, &author_name_ops)) {
        pr_err("Can't create procfs author entry");
        return -ENOMEM;
    }

    if (!proc_create(PROCFS_ENTRY_STORE_CNT, PROCFS_ENTRY_PERMISSON, procfs_dir_root_entry, &read_store_num_ops)) {
        pr_err("Can't create procfs store counter entry");
        return -ENOMEM;
    }

    if (!proc_create(PROCFS_ENTRY_SHOW_CNT, PROCFS_ENTRY_PERMISSON, procfs_dir_root_entry, &read_show_num_ops)) {
        pr_err("Can't create procfs show counter entry");
        return -ENOMEM;
    }

    pr_info("Module initialized successfully \n");

    return res;
}

static void __exit my_list_start_end(void)
{
    struct my_list *cursor = NULL;
    struct my_list *temp = NULL;

    list_for_each_entry_safe(cursor, temp, &my_list, list)
    {
        kfree(&(cursor->my_str));
        list_del(&cursor->list);
        kfree(cursor);
    }

    kobject_put(my_list_kobj);

    remove_proc_entry(PROCFS_ENTRY_AUTHOR, procfs_dir_root_entry);
    remove_proc_entry(PROCFS_ENTRY_STORE_CNT, procfs_dir_root_entry);
    remove_proc_entry(PROCFS_ENTRY_SHOW_CNT, procfs_dir_root_entry);

    remove_proc_entry(PROCFS_FOLDER, NULL);

    pr_info("Module removed successfully \n");
}

module_init(my_list_start);
module_exit(my_list_start_end);

MODULE_AUTHOR(AUTHOR_NAME);
MODULE_DESCRIPTION("ProcFS task to extend SysFS simple module");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");
