// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/types.h>

#define SYS_FOLDER_NAME					("Get_time_module")
#define ABSOLUTE_TIME_ATTRIBUTE_NAME	show_absolute_timestamp 
#define RELATIVE_TIME_ATTRIBUTE_NAME	show_relative_timestamp 
#define PERMISSIONS						(0444)

static ssize_t show_absolute_timestamp(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	static struct timespec64 previous_timestamp;

	sprintf(buf, "Time of previous reading: %llu.%lu sec\n", previous_timestamp.tv_sec,
			previous_timestamp.tv_nsec);
	ktime_get_real_ts64(&previous_timestamp);

	return strlen(buf);
}

static ssize_t show_relative_timestamp(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	static struct timespec64 previous_timestamp;
	struct timespec64 current_timestamp;
	struct timespec64 delta_time;

	ktime_get_ts64(&current_timestamp);
	delta_time = timespec64_sub(current_timestamp, previous_timestamp);
	previous_timestamp = current_timestamp;
	sprintf(buf, "Time passed since last reading: %llu.%lu sec\n", delta_time.tv_sec,
			delta_time.tv_nsec);
	return strlen(buf);
}

static struct kobj_attribute absolute_time_atr = __ATTR(ABSOLUTE_TIME_ATTRIBUTE_NAME,
														PERMISSIONS, show_absolute_timestamp, NULL);
static struct kobj_attribute relative_time_atr = __ATTR(RELATIVE_TIME_ATTRIBUTE_NAME,
														PERMISSIONS, show_relative_timestamp, NULL);
static struct kobject *time_module_kobject;

static int mod_init(void)
{
	int result = 0;

	time_module_kobject = kobject_create_and_add(SYS_FOLDER_NAME, kernel_kobj);
	if (!time_module_kobject) {
		result = -ENOMEM;
	} else {
		if (sysfs_create_file(time_module_kobject, &absolute_time_atr.attr))
			result = -ENOMEM;

		if (sysfs_create_file(time_module_kobject, &relative_time_atr.attr))
			result = -ENOMEM;

		pr_info("module loaded\n");
	}
	if (result) {
		pr_err("error no memory\n");
		kobject_put(time_module_kobject);
	}
	return result;
}

static void mod_exit(void)
{
	sysfs_remove_file(time_module_kobject, &absolute_time_atr.attr);
	sysfs_remove_file(time_module_kobject, &relative_time_atr.attr);
	kobject_put(time_module_kobject);
	pr_info("module exited\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_AUTHOR("Vadym Hutovskyi <vadym.hutovskyi@globallogic.com>");
MODULE_DESCRIPTION("Relative and absolute time kernel module");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");
