/**
 * @file timediff.c
 * @brief Timediff kernel module.
 *   This module uses ktime CLOCK_MONOTONIC to calculate relative and absolute time.
 *   It gives you relative time since last time_relative attribute read and
 *   gives you absolute time on each time_absolute reading.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/jiffies.h>
#include <linux/sysfs.h>
#include <asm-generic/bug.h>

static ssize_t timediff_relative_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf);

static ssize_t timediff_absolute_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf);

static struct kobject *kobj;
static struct kobj_attribute  kattr_rel      = __ATTR(time_relative, 0440, timediff_relative_show, NULL);
static struct kobj_attribute  kattr_absolute = __ATTR(time_absolute, 0440, timediff_absolute_show, NULL);

static ktime_t last_rel_ktime;
static struct timespec64 last_abs_ktime;

static ssize_t timediff_relative_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf)
{
    ktime_t ktime_rel;
    ktime_t ktime_diff;
    struct timespec64 ktime_rel_show;
    int ret;

    // CLOCK_MONOTONIC
    ktime_rel  = ktime_get();

    if (last_rel_ktime == 0)
    {
        ret = sprintf(buf, "You have not read time_relative yet\n");    
    }
    else
    {
        ktime_diff     = ktime_sub(ktime_rel, last_rel_ktime);
        ktime_rel_show = ktime_to_timespec64(ktime_diff);

        ret = sprintf(buf, "Seconds past since last read: %lld.%ld\n",
            ktime_rel_show.tv_sec, ktime_rel_show.tv_nsec / 1000000);
    }

    last_rel_ktime = ktime_rel;

    pr_debug("[TIMEDIFF] %s", buf);

    return ret;
}

static ssize_t timediff_absolute_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf)
{
    int ret;

    if (last_abs_ktime.tv_sec == 0)
    {
        ret = sprintf(buf, "You have not read time_absolute yet\n");
    }
    else
    {
        ret = sprintf(buf, "Last read was at %lld.%ld seconds after system start\n",
            last_abs_ktime.tv_sec, last_abs_ktime.tv_nsec / 100);
    }
    
    pr_debug("[TIMEDIFF] %s", buf);

    // CLOCK_MONOTONIC
    ktime_get_ts64(&last_abs_ktime);

    return ret;
}

static int timediff_init(void)
{
    int res;

    kobj = kobject_create_and_add("timediff", kernel_kobj);
    if (kobj == NULL)
        return -ENOMEM;

    res = sysfs_create_file(kobj, &kattr_rel.attr);
    if (res)
    {
        BUG();
        pr_err("[TIMEDIFF] Failed to create sysfs file");
        kobject_put(kobj);
        return res;
    }

    res = sysfs_create_file(kobj, &kattr_absolute.attr);
    if (res)
    {
        BUG();
        pr_err("[TIMEDIFF] Failed to create sysfs file");
        kobject_put(kobj);
        return res;
    }

    return res;
}

static void timediff_exit(void)
{
    kobject_put(kobj);
}

module_init(timediff_init);
module_exit(timediff_exit);

MODULE_DESCRIPTION("Timediff kernel module");
MODULE_AUTHOR("Sergey D.");
MODULE_LICENSE("GPL");
