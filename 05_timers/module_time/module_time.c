// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>

static int abs_first = 1;
static struct timespec64 prev_abs_time;

static ssize_t abs_time_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	if (abs_first) {
		abs_first = 0;
		return scnprintf(buf, PAGE_SIZE, "This one is first read of absolute time\n");
	}

	prev_abs_time = ktime_to_timespec64(ktime_get_real());
	return scnprintf(buf, PAGE_SIZE, "Previous reading was at %lld.%09lds\n", prev_abs_time.tv_sec, prev_abs_time.tv_nsec);
}

static int rel_first = 1;
static ktime_t prev_rel_time;

static ssize_t rel_time_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	ktime_t rt_nxt;
	struct timespec64 rt;

	if (rel_first) {
		rel_first = 0;
		return scnprintf(buf, PAGE_SIZE, "This one is first read of relative time\n");
	}

	rt_nxt = ktime_get();
	rt = ktime_to_timespec64(ktime_sub(rt_nxt, prev_rel_time));
	prev_rel_time = rt_nxt;

	return scnprintf(buf, PAGE_SIZE, "Time since previous reading %lld.%09lds\n", rt.tv_sec, rt.tv_nsec);
}

static struct kobj_attribute abs_time_attribute = __ATTR_RO_MODE(abs_time, 0664);
static struct kobj_attribute rel_time_attribute = __ATTR_RO_MODE(rel_time, 0664);

static struct kobject *my_object;

static int module_time_init(void)
{
	int ret;

	my_object = kobject_create_and_add("my_object", kernel_kobj);
	if (!my_object)
		return -ENOMEM;

	ret = sysfs_create_file(my_object, &abs_time_attribute.attr);
	if (ret)
		kobject_put(my_object);

	ret = sysfs_create_file(my_object, &rel_time_attribute.attr);
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
