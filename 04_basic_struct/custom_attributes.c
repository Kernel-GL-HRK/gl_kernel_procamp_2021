// SPDX-License-Identifier: GPL-2.0-only
/* Custom kernel module */
/* Prints dmesg messages on load and exit */

#include <linux/init.h> // Macros used to mark up functions  __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/slab.h> // kmalloc

#include "custom_utils.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Aleksieiev <oleksandr.aleksieiev@globallogic.com>");
MODULE_DESCRIPTION("Attributes usage example.");
MODULE_VERSION("0.1");

struct string_attribute {
	char *data;
	struct list_head list;
};

struct attributes_list {
	struct list_head head;
	ssize_t size;
};

typedef void (*proc_attr_list_item)(struct string_attribute *);

static struct attributes_list strings_attr;

static void init_attr_list(struct attributes_list *head)
{
	head->size = 0;
	INIT_LIST_HEAD(&(head->head));
}

static struct string_attribute *
make_string_attributes_record_and_add(struct attributes_list *head,
				      const char *text, ssize_t size)
{
	struct string_attribute *res =
		kzalloc(sizeof(struct string_attribute), GFP_KERNEL);

	if (res != NULL) {
		if (size > 0) {
			res->data = kzalloc(size + 1, GFP_KERNEL);
			memcpy((void *)res->data, (void *)text, size);
		} else {
			res->data = NULL;
		}
		list_add_tail(&res->list, &head->head);

		head->size++;
	} else {
		pr_err("allocate_string_attributes_record: can't allocate string_attribute item");
	}
	return res;
}

static void foreach_string_attributes(struct attributes_list *head)
{
	struct string_attribute *item;
	ssize_t sz = head->size;
	struct list_head *ptr;

	for (ptr = head->head.next; sz > 0; ptr = ptr->next) {
		item = list_entry(ptr, struct string_attribute, list);
		if (item != NULL)
			pr_info("%s", item->data);

		sz--;
	}
}

static void foreach_r_string_attributes(struct attributes_list *head)
{
	struct string_attribute *item;
	ssize_t sz = head->size;
	struct list_head *ptr;

	for (ptr = head->head.prev; sz > 0; sz--) {
		item = list_entry(ptr, struct string_attribute, list);

		ptr = ptr->prev;
		list_del(&item->list);
		kfree(item->data);
		kfree(item);
	}
}

static void clear_attributes(struct attributes_list *head)
{
	foreach_r_string_attributes(head);
	head->size = 0;
}

static ssize_t string_list_read(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	pr_info("%s", buf);
	foreach_string_attributes(&strings_attr);
	return strlen(buf);
}

static ssize_t string_list_store(struct kobject *kobj,
				 struct kobj_attribute *attr, const char *buf,
				 size_t count)
{
	pr_info("New item: %s", buf);
	make_string_attributes_record_and_add(&strings_attr, buf, count);
	return count;
}

static struct kobj_attribute koa_string_list =
	__ATTR(string_list, 0644, string_list_read, string_list_store);

static struct kobject *custom_attr_kobj;

static int __init kpc_init(void)
{
	int result;

	custom_attr_kobj = kobject_create_and_add("custom_attr", kernel_kobj);

	if (!custom_attr_kobj) {
		pr_info("kobject_create_and_add fail.");
		return -ENOMEM;
	}

	result = sysfs_create_file(custom_attr_kobj, &koa_string_list.attr);

	if (result)
		kobject_put(custom_attr_kobj);

	init_attr_list(&strings_attr);

	pr_info("init %d", result);
	return result;
}

static void __exit kpc_exit(void)
{
	clear_attributes(&strings_attr);

	kobject_put(custom_attr_kobj);
	pr_info("exit.\n");
}

module_init(kpc_init);
module_exit(kpc_exit);
