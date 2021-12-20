#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

static struct msg_obj {
    struct list_head list;
    char *data;
};

struct list_head head;

static ssize_t kobj_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
    struct msg_obj *curr_msg;
    struct list_head *listptr;
    size_t len = 0;
    list_for_each(listptr, &head) {
        curr_msg = list_entry(listptr, struct msg_obj, list);
        strcpy(buf+len, curr_msg->data);
        len += strlen(curr_msg->data);
    }

    return len;
}

static ssize_t kobj_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
    struct msg_obj *new_data = kmalloc(sizeof(struct msg_obj), GFP_KERNEL);
    new_data->data = kmalloc(sizeof(char)*count, GFP_KERNEL);
    strcpy(new_data->data, buf);
    list_add_tail(&new_data->list, &head);
    return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(param, 0664, kobj_show, kobj_store);

static struct kobject *hello_kobj;

static int hello_init(void)
{
	int res = 0;
    INIT_LIST_HEAD(&head);
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
	kobject_put(hello_kobj);
	pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Ostap Pokornyi <ostappokornyi@gmail.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
