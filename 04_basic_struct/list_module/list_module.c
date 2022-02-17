// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "linked_list.h"

static ssize_t list_count;
static linkedList *list;

static ssize_t list_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	if (list != NULL) {
		sprintf(buf, "%s", getNode(list_count, list)->data);
		int i;

		for (i = 0; i < list_count; i++)
			pr_info("String %d from the list: %s\n", i + 1, getNode(i, list)->data);
	} else {
		pr_info("Linked list is empty!\n");
	}
	return strlen(buf);
}

static ssize_t list_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	char *buf_string = kmalloc(strlen(buf) + 1, GFP_KERNEL);

	strcpy(buf_string, buf);
	list = add(buf_string, list_count, list);
	list_count++;
	return count;
}

static struct kobj_attribute list_attribute = __ATTR(list, 0664, list_show, list_store);

static struct kobject *list_kobj;

static int list_init(void)
{
	int res = 0;

	list_kobj = kobject_create_and_add("list", kernel_kobj);
	if (!list_kobj)
		return -ENOMEM;

	res = sysfs_create_file(list_kobj, &list_attribute.attr);
	if (res)
		kobject_put(list_kobj);
	pr_info("Module loaded\n");
	return res;
}

static void list_exit(void)
{
	kobject_put(list_kobj);
	deleteList(list);
	pr_info("Module exited\n");
}

module_init(list_init);
module_exit(list_exit);

MODULE_AUTHOR("Artem Shyshko");
MODULE_LICENSE("GPL");
