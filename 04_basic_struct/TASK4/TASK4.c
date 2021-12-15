// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

#define SYS_FOLDER		"MyObject"
#define PARAM_NAME		list

struct s_list {
	struct list_head node;
	const char *text;
	size_t size;
};

LIST_HEAD(top_head);

static ssize_t show_all_list_units(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int count = 0;
	struct s_list *element;

	if (list_empty(&top_head))
		count = sprintf(buf, "There is no records yet...\n");

	list_for_each_entry(element, &top_head, node) {
		count += sprintf(&buf[count], "%s", element->text);
	}
	return strlen(buf);
}

static ssize_t write_new_unit_to_list(struct kobject *kobj, struct kobj_attribute *attr,
										const char *buf, size_t count)
{
	struct s_list *element;

	element = kzalloc(sizeof(struct s_list), GFP_KERNEL);
	if (element) {
		element->text = kstrdup(buf, GFP_KERNEL);
		if (element->text)
			list_add(&element->node, &top_head);
	}
	return count;
}

static struct kobj_attribute list_attribute = __ATTR(PARAM_NAME, 0664, show_all_list_units,
														write_new_unit_to_list);

static struct kobject *MyObject;

static int module_initialization(void)
{
	int result = 0;

	INIT_LIST_HEAD(&top_head);

	MyObject = kobject_create_and_add(SYS_FOLDER, kernel_kobj);
	if (!MyObject) {
		pr_err("error: no memory\n");
		result = -ENOMEM;
	}
	result = sysfs_create_file(MyObject, &list_attribute.attr);
	if (result) {
		pr_err("error: no memory\n");
		kobject_put(MyObject);
		result = -ENOMEM;
	}
	pr_info("module loaded\n");

	return result;
}

static void module_deinitialization(void)
{
	struct s_list *element, *next;

	if (!list_empty(&top_head)) {
		list_for_each_entry_safe(element, next, &top_head, node) {
			list_del(&element->node);
			kfree(element->text);
			kfree(element);
		}
	}
	sysfs_remove_file(MyObject, &list_attribute.attr);
	kobject_put(MyObject);
	pr_info("module exited\n");
}

module_init(module_initialization);
module_exit(module_deinitialization);

MODULE_AUTHOR("Vadym Hutovskyi <vadym.hutovskyi@globallogic.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
