#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Serhii Mykhailov <milossomnium@gmail.com>");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Basic data struct module");

struct value {
    struct list_head list;
    char *data;
};

LIST_HEAD(values_head);

static ssize_t values_show(struct kobject *kobj, struct kobj_attribute *attr,
                          char *buf)
{
    struct value *current_value;
    struct list_head *current_list;
    ssize_t len = 0;

    pr_info("List of values");

    ssize_t index = 1;
    list_for_each(current_list, &values_head) {
        current_value = list_entry(current_list, struct value, list);
        pr_info("%lu. %s", index, current_value->data);
        len += strlen(current_value->data);
        ++index;
    }

    return len;
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count)
{
    struct value *val = kmalloc(sizeof(struct value), GFP_KERNEL);
    if (!val) {
        return -ENOMEM;
    }

    val->data = kmalloc(count, GFP_KERNEL);
    if (!val->data) {
        return -ENOMEM;
    }

    strcpy(val->data, buf);
    list_add_tail(&val->list, &values_head);

    pr_info("New value received: %s(count=%lu)", val->data, count);
    return count;
}

static struct kobj_attribute list_attribute =
    __ATTR(param, 0664, values_show, value_store);
static struct kobject *values_kobj;

static int values_init(void)
{
    values_kobj = kobject_create_and_add("values", kernel_kobj);
    if (!values_kobj) {
        return -ENOMEM;
    }

    int result = sysfs_create_file(values_kobj, &list_attribute.attr);
    if (result) {
        kobject_put(values_kobj);
    }

    return result;
}

static void values_exit(void)
{
    struct value *current_value;
    struct value *tmp;

    list_for_each_entry_safe(current_value, tmp, &values_head, list) {
        pr_info("Clear  value: %s", current_value->data);
        kfree(current_value->data);
        list_del(&current_value->list);
        kfree(current_value);
    }

    pr_info("List is empty: %d", list_empty_careful(&values_head));
    kobject_put(values_kobj);
    pr_info("module exited");
}

module_init(values_init);
module_exit(values_exit);
