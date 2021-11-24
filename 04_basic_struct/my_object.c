// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Artm Komyshan");
MODULE_DESCRIPTION("List of strings module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

struct string_list {
	struct list_head list;
	char *data;
};

struct string_list *strings;

static ssize_t list_show(struct kobject *kobj, struct kobj_attribute *attr,
			 char *buf)
{
	struct list_head *listptr;
	struct string_list *entry;
	size_t total_size;

	total_size = 0;

	list_for_each(listptr, &strings->list) {
		entry = list_entry(listptr, struct string_list, list);

		strcpy(buf + total_size, entry->data);
		total_size += strlen(entry->data);
	}

	return total_size;
}

static ssize_t list_store(struct kobject *kobj, struct kobj_attribute *attr,
			  const char *buf, size_t count)
{
	struct string_list *new_entry;

	new_entry = kmalloc(sizeof(struct string_list), GFP_KERNEL);
	if (!new_entry)
		return -ENOMEM;

	new_entry->data = kmalloc(count, GFP_KERNEL);
	if (!new_entry->data)
		return -ENOMEM;

	strcpy(new_entry->data, buf);

	list_add(&new_entry->list, &strings->list);

	return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(param, 0664, list_show, list_store);

static struct kobject *my_object_kobj;

static int my_object_init(void)
{
	int res = 0;

	my_object_kobj = kobject_create_and_add("list", kernel_kobj);
	if (!my_object_kobj)
		return -ENOMEM;

	res = sysfs_create_file(my_object_kobj, &list_attribute.attr);
	if (res)
		kobject_put(my_object_kobj);

	strings = kmalloc(sizeof(struct string_list), GFP_KERNEL);
	if (!strings)
		return -ENOMEM;

	INIT_LIST_HEAD(&strings->list);

	return res;
}

static void my_object_exit(void)
{
	struct string_list *entry;
	struct string_list *tmp;

	list_for_each_entry_safe(entry, tmp, &strings->list, list) {
		list_del(&entry->list);
		kfree(entry->data);
		kfree(entry);
	}

	BUG_ON(!list_empty(&strings->list));

	kobject_put(my_object_kobj);
}

module_init(my_object_init);
module_exit(my_object_exit);
