// SPDX-License-Identifier: GPL-2.0-only

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Nazarii Kurylko <kurylko.n@gmail.com>");
MODULE_DESCRIPTION("time in kernel module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static ssize_t time_prev_call_show(struct kobject *kobj,
				   struct kobj_attribute *attr, char *buf)
{
	static struct timespec64 ts_prev;
	ssize_t count;

	count = scnprintf(buf, PAGE_SIZE, "%llds.%ldns\n", ts_prev.tv_sec,
			  ts_prev.tv_nsec);

	ktime_get_real_ts64(&ts_prev);
	return count;
}

static ssize_t time_since_prev_call_show(struct kobject *kobj,
					 struct kobj_attribute *attr, char *buf)
{
	struct timespec64 ts;
	ktime_t now = 0;
	static int inited;
	static ktime_t prev;

	now = ktime_get();
	if (!inited) {
		prev = now;
		inited = 1;
	}

	now = ktime_get();
	ts = ktime_to_timespec64(ktime_sub(now, prev));
	prev = now;

	return scnprintf((char *)buf, PAGE_SIZE, "%llds.%ldns\n", ts.tv_sec,
			 ts.tv_nsec);
}

static struct kobj_attribute attribute1 =
	__ATTR(time_prev_call, 0444, time_prev_call_show, NULL);

static struct kobj_attribute attribute2 =
	__ATTR(time_since_prev_call, 0444, time_since_prev_call_show, NULL);

static struct kobject *myobject_kobj;

static struct attribute *attrs[] = {
	&attribute1.attr, &attribute2.attr, NULL,
	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static int myobject_init(void)
{
	int res = 0;

	myobject_kobj = kobject_create_and_add("MyTimeObject", kernel_kobj);
	if (!myobject_kobj)
		return -ENOMEM;
	res = sysfs_create_group(myobject_kobj, &attr_group);
	if (res)
		kobject_put(myobject_kobj);

	return res;
}

static void myobject_exit(void)
{
	kobject_put(myobject_kobj);
}

module_init(myobject_init);
module_exit(myobject_exit);
