#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/ktime.h>
#include <linux/time64.h>

static ktime_t prev_time = 0;
static ktime_t prev_abs_time = 0;

static ssize_t relative_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	pr_debug("---> %s", __func__);

	if (prev_time == 0) {
		prev_time = ktime_get();
		pr_debug("Previous time initialization: %llu", prev_time);
	} else {
		ktime_t cur_time = ktime_get();
		ktime_t delta = ktime_sub(cur_time, prev_time);
		printk(KERN_INFO "Delta: %llu", delta);
		prev_time = cur_time;
		pr_debug("Previous time updated: %llu", prev_time);
	}

	pr_debug("---< %s", __func__);
	return 0;
}

static ssize_t relative_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	pr_debug("---> %s size_t: %ld", __func__, count);
	
	if (sysfs_streq(buf, "reset")) {
		pr_debug("Previous time reset: %llu", prev_time);
		prev_time = 0;
	}

	pr_debug("---< %s", __func__);
	return count;
}

static ssize_t absolute_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct timespec64 ts;
	pr_debug("---> %s", __func__);
	ts = ktime_to_timespec64(prev_abs_time);
	printk(KERN_INFO "Absolute: %lld.%09ld", ts.tv_sec, ts.tv_nsec);
	prev_abs_time = ktime_get_real();
	pr_debug("---< %s", __func__);
	return 0;
}

static ssize_t absolute_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	pr_debug("---> %s size_t: %ld", __func__, count);

	pr_debug("---< %s", __func__);
	return count;
}

static struct kobj_attribute relative_attribute =
	__ATTR(relative, 0664, relative_show, relative_store);

static struct kobj_attribute absolute_attribute =
	__ATTR(absolute, 0444, absolute_show, absolute_store);

static struct kobject *time_kobj;

static int time_init(void)
{
	int res = 0;

	pr_debug("---> %s", __func__);

	time_kobj = kobject_create_and_add("time", kernel_kobj);
	if (!time_kobj) {
		pr_err("Object was not created");
		return -ENOMEM;
	}
	res = sysfs_create_file(time_kobj, &relative_attribute.attr);
	if (res) {
		pr_err("Attribute for Object was not created");
		kobject_put(time_kobj);
	}
	res = sysfs_create_file(time_kobj, &absolute_attribute.attr);
	if (res) {
		pr_err("Attribute for Object was not created");
		kobject_put(time_kobj);
	}
	pr_debug("---< %s", __func__);
	return res;
}

static void time_exit(void)
{
	pr_debug("---> %s", __func__);

	kobject_put(time_kobj);

	pr_debug("---< %s", __func__);
}

module_init(time_init);
module_exit(time_exit);

MODULE_AUTHOR("Anatolii Berchanov <tberchanov@gmail.com>");
MODULE_DESCRIPTION("Time module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
