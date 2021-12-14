// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct message_list {
	struct list_head entry;
	char *buffer;
};

static struct list_head my_list;

static ssize_t list_show(struct kobject *kobj, struct kobj_attribute * attr,
	char *buf)
{
	struct message_list *pos;
	char *p = buf, *prehead = "Constent of list";
	ssize_t ret, size, size_left = PAGE_SIZE; // sysfs allocated for us whole page
	int i = 0;

	list_for_each_entry(pos, &my_list, entry) {
		// size of one list element with head message
		size = strlen(pos->buffer) + strlen(prehead) + 4;
		// copy into buffer formatted string
		ret = scnprintf(p, size_left, "%s %d: %s", prehead, i++, pos->buffer);
		if (ret != size)
			return -ENOMEM;

		size_left -= size;
		p += size;
	}

	return PAGE_SIZE - size_left; 
}

static ssize_t list_store(struct kobject *kobj, struct kobj_attribute * attr,
	const char *buf, size_t count)
{
	char *new_msg;
	struct message_list *new_entry;

	new_msg = kzalloc(count, GFP_KERNEL);
	if (!new_msg)
		return -ENOMEM;

	memcpy(new_msg, buf, count);

	new_entry = kzalloc(sizeof(*new_entry), GFP_KERNEL);
	if (!new_entry)
		return -ENOMEM;

	new_entry->buffer = new_msg;
	list_add_tail(&new_entry->entry, &my_list);

	return count;
}

static struct kobj_attribute my_list_attribute = __ATTR_RW_MODE(list, 0664);

static struct kobject *my_object;

static int basic_struct_init(void)
{
	int ret;

	my_object = kobject_create_and_add("my_object", kernel_kobj);
	if (!my_object)
		return -ENOMEM;

	ret = sysfs_create_file(my_object, &my_list_attribute.attr);
	if (ret)
		kobject_put(my_object);

	INIT_LIST_HEAD(&my_list);

	pr_info("basic_struct: init");

	return ret;
}

static void basic_struct_exit(void)
{
	struct message_list *pos, *tmp;

	list_for_each_entry_safe(pos, tmp, &my_list, entry) {
		list_del(&pos->entry);
		kfree(pos->buffer);
		kfree(pos);
	}
	kobject_put(my_object);
	pr_info("basic_struct: exit");
}

module_init(basic_struct_init);
module_exit(basic_struct_exit);

MODULE_AUTHOR("Volodymyr Kharuk <jlabpih@ukr.net>");
MODULE_LICENSE("GPL v2");
