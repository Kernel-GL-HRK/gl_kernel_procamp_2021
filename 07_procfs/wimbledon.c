#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/minmax.h>
#include <asm/uaccess.h>

#define AUTHOR "Ivan Stepanenko <istepanenko@gmail.com>"

#define PROCFS_BUFF_SIZE     64
#define PROCFS_DIR           "wimbeldon"
#define PROCFS_FN_NSHOW      "nshow"
#define PROCFS_FN_NSTORE     "nstore"
#define PROCFS_FN_AUTH_NAME  "auth_name"

struct msg_item {
        struct list_head list;
        char *msg;
        size_t msg_len;
};
static LIST_HEAD(msg_list_head);

static unsigned int show_cb_counter = 0;
static unsigned int store_cb_counter = 0;


static ssize_t wimbledon_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
    struct msg_item *pos = NULL;
    size_t item_number = 0;
    size_t count = 0;
    char *p_buff = buff;

    show_cb_counter += 1;

    list_for_each_entry_reverse(pos, &msg_list_head, list) {
        pr_info("msg #%u size: %u '%.*s'\n", item_number, pos->msg_len, (int)pos->msg_len, (char *)pos->msg);

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

    store_cb_counter += 1;

    p_msg = kmalloc(count, GFP_KERNEL);
    if (!p_msg) {
        pr_err("could not allocate msg cnt: %u\n", count);
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

    pr_info("adding msg size: %u to the list: '%.*s'\n", p_item->msg_len, (int)p_item->msg_len, (char *)p_item->msg);
    list_add(&p_item->list, &msg_list_head);
    return count;
}

/* procfs operations */

static struct proc_dir_entry *procfs_root_entry, *procfs_nshow_entry, *procfs_nstore_entry, *procfs_auth_name_entry;


static ssize_t procfs_op_nshow_read(struct file *file, char __user *p_ubuf, size_t count, loff_t *ppos)
{
    char msg_buf[PROCFS_BUFF_SIZE];
    int len = 0;

    if(*ppos > 0)
        return 0;

    len = snprintf(NULL, 0, "%u\n", show_cb_counter);
    if (len < 0 || len >= sizeof(msg_buf)) {
        pr_err("msg len: %i doesn't fit buffer: %i\n", len, sizeof(msg_buf));
        return -ENOMEM;
    }

    snprintf(msg_buf, len+1, "%u\n", show_cb_counter);

    if (copy_to_user(p_ubuf, msg_buf, len)) {
        pr_err("can't copy msg size: %i to user space\n", len);
        return -EFAULT;
    }

    *ppos = len;

    return len;
}


static ssize_t procfs_op_nstore_read(struct file *file, char __user *p_ubuf, size_t count, loff_t *ppos)
{
    char msg_buf[PROCFS_BUFF_SIZE];
    int len = 0;

    if(*ppos > 0)
        return 0;

    len = snprintf(NULL, 0, "%u\n", store_cb_counter);
    if (len < 0 || len >= sizeof(msg_buf)) {
        pr_err("msg len: %i doesn't fit buffer: %i\n", len, sizeof(msg_buf));
        return -ENOMEM;
    }
    snprintf(msg_buf, len+1, "%u\n", store_cb_counter);

    if (copy_to_user(p_ubuf, msg_buf, len)) {
        pr_err("can't copy msg size: %i to user space\n", len);
        return -EFAULT;
    }

    *ppos = len;

    return len;
}


static ssize_t procfs_op_auth_name_read(struct file *file, char __user *p_ubuf, size_t count, loff_t *ppos)
{
    char msg_buf[sizeof(AUTHOR)+1];
    int len = 0;

    if(*ppos > 0)
        return 0;

    len = snprintf(NULL, 0, "%s\n", AUTHOR);
    if (len < 0 || len >= sizeof(msg_buf)) {
        pr_err("msg len: %i doesn't fit buffer: %i\n", len, sizeof(msg_buf));
        return -ENOMEM;
    }

    snprintf(msg_buf, len+1, "%s\n", AUTHOR);

    if (copy_to_user(p_ubuf, msg_buf, len)) {
        pr_err("can't copy msg size: %i to user space\n", len);
        return -EFAULT;
    }

    *ppos = len;

    return len;
}


static struct proc_ops procfs_nshow_ops =
{
    .proc_read = procfs_op_nshow_read
};

static struct proc_ops procfs_nstore_ops =
{
    .proc_read= procfs_op_nstore_read
};

static struct proc_ops procfs_auth_name_ops =
{
    .proc_read= procfs_op_auth_name_read
};

/* end procsfs operations */

static struct kobj_attribute list_attribute =
        __ATTR(list, 0644, wimbledon_show, wimbledon_store);

static struct kobject *wimbledon_kobj;

static int wimbledon_init(void)
{
    int res = 0;
    wimbledon_kobj = kobject_create_and_add("wimbledon", kernel_kobj);
    if (!wimbledon_kobj)
        return -ENOMEM;
    res = sysfs_create_file(wimbledon_kobj, &list_attribute.attr);
    if (res)
        kobject_put(wimbledon_kobj);

    procfs_root_entry = proc_mkdir(PROCFS_DIR, NULL);
    if (procfs_root_entry == NULL) {
        pr_err("error creating procfs: %s entry\n", PROCFS_DIR);
        return -ENOMEM;
    }
    procfs_nshow_entry = proc_create(PROCFS_FN_NSHOW, 0444, procfs_root_entry, &procfs_nshow_ops);
    if (procfs_nshow_entry == NULL) {
        pr_err("error creating procfs: %s entry\n", PROCFS_FN_NSHOW);
        return -ENOMEM;
    }
    procfs_nstore_entry = proc_create(PROCFS_FN_NSTORE, 0444, procfs_root_entry, &procfs_nstore_ops);
    if (procfs_nstore_entry == NULL) {
        pr_err("error creating procfs: %s entry\n", PROCFS_FN_NSTORE);
        return -ENOMEM;
    }
    procfs_auth_name_entry = proc_create(PROCFS_FN_AUTH_NAME, 0444, procfs_root_entry, &procfs_auth_name_ops);
    if (procfs_nstore_entry == NULL) {
        pr_err("error creating procfs: %s entry\n", PROCFS_FN_AUTH_NAME);
        return -ENOMEM;
    }

    return res;
}

static void wimbledon_exit(void)
{
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    struct msg_item *p_item = NULL;

    list_for_each_safe(pos, n, &msg_list_head) {
         p_item = list_entry(pos, struct msg_item, list);
         pr_info("freeing msg size: %u from the list: '%.*s'\n", p_item->msg_len, (int)p_item->msg_len, (char *)p_item->msg);
         list_del(pos);
         kfree(p_item->msg);
         kfree(p_item);
    }
    pr_info("list is empty: %d\n", list_empty_careful(&msg_list_head));
    kobject_put(wimbledon_kobj);

    pr_info("removing procfs entries\n");
    proc_remove(procfs_auth_name_entry);
    proc_remove(procfs_nstore_entry);
    proc_remove(procfs_nshow_entry);
    proc_remove(procfs_root_entry);

    pr_info("Module exited\n");
}

module_init(wimbledon_init);
module_exit(wimbledon_exit);

MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION("Shows num of store/show callbacks, stores data in the list items.");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
