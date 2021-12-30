#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/ktime.h>

static struct kobject *my_kobj;

static ssize_t show_relative_time(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	static ktime_t last_read_kt = 0;
	ktime_t ktime = ktime_get();
	ktime_t delta;
	struct timespec64 ts;
	int ret;

	pr_debug("last_read_kt: %llu\n", last_read_kt);

	delta = ktime_sub(ktime, last_read_kt);
	ts = ktime_to_timespec64(delta);

	ret = sprintf(buf,
		      "relative time since last read: %llu, tv_sec: %llu tv_nsec: %lu\n",
		      delta, ts.tv_sec, ts.tv_nsec);

	last_read_kt = ktime;

	return ret;
}

static struct kobj_attribute relative_time_attr =
__ATTR(relative_time, 0664, show_relative_time, NULL);

static ssize_t show_absolute_time(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	static struct timespec64 last_read_tv = { 0, };
	struct timespec64 tv;
	int ret;

	ktime_get_real_ts64(&tv);

	pr_debug("last_read_tv: %llu.%lu\n",
		 last_read_tv.tv_sec, last_read_tv.tv_nsec);

	ret = sprintf(buf, "absolute time of previous reading: %llu.%lu\n",
		      last_read_tv.tv_sec, last_read_tv.tv_nsec);

	last_read_tv = tv;

	return ret;
}

static struct kobj_attribute absolute_time_attr =
__ATTR(absolute_time, 0664, show_absolute_time, NULL);

static int my_module_init(void)
{
	int ret;

	my_kobj = kobject_create_and_add("my_module", kernel_kobj);
	if (!my_kobj)
		return -ENOMEM;

	ret = sysfs_create_file(my_kobj, &relative_time_attr.attr);
	if (ret)
		kobject_put(my_kobj);

	ret = sysfs_create_file(my_kobj, &absolute_time_attr.attr);
	if (ret)
		kobject_put(my_kobj);

	return ret;
}

static void my_module_exit(void)
{
	kobject_put(my_kobj);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR("Michael N. <michael.n@gmail.com>");
MODULE_DESCRIPTION("Basic module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
