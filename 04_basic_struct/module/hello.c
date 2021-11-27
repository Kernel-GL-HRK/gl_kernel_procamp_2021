#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

struct node {
	char* value;
	struct list_head list_member;
};

static LIST_HEAD(nodeHead);

static void add_node(char* value, struct list_head *head)
{
    struct node *nodePtr = (struct node *)kmalloc(sizeof(struct node), GFP_USER);
    // assert(fooPtr != NULL); TODO use PANIC here
    
    nodePtr->value = value;
    INIT_LIST_HEAD(&nodePtr->list_member);
    list_add(&(nodePtr->list_member), head);
}

static void display(struct list_head *head)
{
    struct list_head *iter;
    struct node *nodePtr;

    list_for_each(iter, head) {
        nodePtr = list_entry(iter, struct node, list_member);
		printk(KERN_INFO "%s", nodePtr->value);
    }
}

static ssize_t list_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	// struct list_head *iter;
    // struct node *nodePtr;

	// char outStr[] = "";

    // list_for_each(iter, &nodeHead) {
        // nodePtr = list_entry(iter, struct node, list_member);
		// printk(KERN_INFO "Buf size: %ld", strlen(buf));
		// sprintf(buf, "%s\n", nodePtr->value);
		// strcat(outStr, nodePtr->value);
    // }
    // return sprintf(buf, "%s", outStr);

	sprintf(buf, "%s", "Hello");
    return strlen(buf);
}

static ssize_t list_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	char* value = (char*)kcalloc(count, sizeof(char), GFP_USER);
	memcpy(value, buf, count);
	add_node(value, &nodeHead);
    return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(list, 0664, list_show, list_store);

static struct kobject *my_kobj;

static int hello_init(void)
{
	int res = 0;

	my_kobj = kobject_create_and_add("MyObject", kernel_kobj);
	if (!my_kobj)
		return -ENOMEM;
	res = sysfs_create_file(my_kobj, &list_attribute.attr);
	if (res)
		kobject_put(my_kobj);
	return res;
}

static void hello_exit(void)
{
	kobject_put(my_kobj);
	// TODO clean memory in list
	pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Oleksandr Posukhov <alex.posukhov@gmail.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
