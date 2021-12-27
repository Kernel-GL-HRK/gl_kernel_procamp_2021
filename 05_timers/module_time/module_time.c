// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>

static ssize_t my_time_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "hello");
}

static struct kobj_attribute my_attribute = __ATTR_RO_MODE(my_time, 0664);

static struct kobject *my_object;

static int module_time_init(void)
{
	int ret;

	my_object = kobject_create_and_add("my_object", kernel_kobj);
	if (!my_object)
		return -ENOMEM;

	ret = sysfs_create_file(my_object, &my_attribute.attr);
	if (ret)
		kobject_put(my_object);

	pr_info("module_time: init");

	return ret;
}

static void module_time_exit(void)
{
	kobject_put(my_object);
	pr_info("module_time: exit");
}

module_init(module_time_init);
module_exit(module_time_exit);

MODULE_AUTHOR("Volodymyr Kharuk <jlabpih@ukr.net>");
MODULE_LICENSE("GPL v2");
