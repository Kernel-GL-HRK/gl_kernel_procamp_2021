// SPDX-License-Identifier: GPL-2.0
/*
 * 1 Implement kernel module with API in sysfs,
 *   which returns relative time in maximum possible
 *   resolution passed since previous read of it.
 * 2 Implement kernel module with API in sysfs
 *   which returns absolute time of previous reading
 *   with maximum resolution like ‘400.123567’ seconds.
 */


#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/jiffies.h>


static ssize_t time_attr_presenter_last_abs_time_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t time_attr_presenter_last_relative_time_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t time_attr_presenter_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);
static int time_attr_presenter_init(void);
static void time_attr_presenter_exit(void);


static struct kobject *time_attr_presenter_kobj;
static u64 last_read_jiffies;
static struct timespec64 last_read_time;

static struct kobj_attribute param_abs_attribute =
		__ATTR(last_abs_time, 0644, time_attr_presenter_last_abs_time_show, time_attr_presenter_store);
static struct kobj_attribute param_relative_attribute =
		__ATTR(last_relative_time, 0644, time_attr_presenter_last_relative_time_show, time_attr_presenter_store);


static ssize_t time_attr_presenter_last_abs_time_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	if (last_read_time.tv_sec == 0 && last_read_time.tv_nsec == 0)
		sprintf(buff, "-1\n");
	else
		sprintf(buff, "%lli.%li\n", last_read_time.tv_sec, last_read_time.tv_nsec);

	ktime_get_real_ts64(&last_read_time);

	return strlen(buff);
}

static ssize_t time_attr_presenter_last_relative_time_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	u64 delta = 0;
	struct timespec64 ts;

	if (last_read_jiffies == 0) {
		sprintf(buff, "-1\n");
	} else {
		delta = get_jiffies_64() - last_read_jiffies;
		jiffies_to_timespec64(delta, &ts);
		sprintf(buff, "%lli.%li\n", ts.tv_sec, ts.tv_nsec);
	}
	last_read_jiffies = get_jiffies_64();

	return strlen(buff);
}


static ssize_t time_attr_presenter_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
	return count;
}


static int time_attr_presenter_init(void)
{
	int res = 0;

	time_attr_presenter_kobj = kobject_create_and_add("time_attr_presenter", kernel_kobj);
	if (!time_attr_presenter_kobj)
		return -ENOMEM;
	res = sysfs_create_file(time_attr_presenter_kobj, &param_relative_attribute.attr);
	if (res)
		kobject_put(time_attr_presenter_kobj);
	res = sysfs_create_file(time_attr_presenter_kobj, &param_abs_attribute.attr);
	if (res)
		kobject_put(time_attr_presenter_kobj);

	last_read_jiffies = 0;
	last_read_time.tv_sec = 0;
	last_read_time.tv_nsec = 0;

	return res;
}


static void time_attr_presenter_exit(void)
{
	kobject_put(time_attr_presenter_kobj);
	pr_info("Module exited\n");
}

module_init(time_attr_presenter_init);
module_exit(time_attr_presenter_exit);

MODULE_AUTHOR("Ivan Stepanenko <istepanenko@gmail.com>");
MODULE_DESCRIPTION("Time attribute presenter module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
