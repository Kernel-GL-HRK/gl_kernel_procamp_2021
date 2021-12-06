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
MODULE_AUTHOR("Serhii Mykhailov <vitaliyzh@gmail.com>");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Basic data struct modul");

static ssize_t value;

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr,
                          char *buf)
{
    sprintf(buf, "%lu\n", value);
    return strlen(buf);
}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count)
{
    sscanf(buf, "%lu\n", &value);
    return count;
}

static struct kobj_attribute list_attribute =
    __ATTR(param, 0664, hello_show, hello_store);
static struct kobject *hello_kobj;

static int hello_init(void)
{
    hello_kobj = kobject_create_and_add("hello", kernel_kobj);
    if (!hello_kobj) {
        return -ENOMEM;
    }

    int result = sysfs_create_file(hello_kobj, &list_attribute.attr)
    if (result) {
        kobject_put(hello_kobj);
    }

    return result;
}

static void hello_exit(void)
{
    kobject_put(hello_kobj);
    pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);
