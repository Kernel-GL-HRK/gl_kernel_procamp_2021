#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME
#define get_millisec(jiff) ((jiff * 1000) / HZ)
#define EET_TIMEZONE 2

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/time.h>

bool is_first_read = true, is_abs = true;
static int get_jiff = 0;
struct timespec64 abs_clock;

static ssize_t time_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int diff;
    diff = get_millisec(jiffies) - get_jiff;
    get_jiff = get_millisec(jiffies);
    if (is_abs)
    {
        ktime_get_real_ts64(&abs_clock);
        sprintf(buf, "Absolute time: %.2llu:%.2llu:%.2llu:%.6lu\n",
                (abs_clock.tv_sec / 3600) % (24) + (EET_TIMEZONE),
                (abs_clock.tv_sec / 60) % (60),
                abs_clock.tv_sec % 60,
                abs_clock.tv_nsec / 1000);
        is_first_read = false;
    }
    else
    {
        if (is_first_read)
        {
            sprintf(buf, "First read!\n");
            is_first_read = false;
        }
        else
        {
            sprintf(buf, "%d milliseconds passed since previous read\n", diff);
        }
    }
    return strlen(buf);
}

static ssize_t time_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    if (buf[0] == '0')
    {
        is_abs = false;
    }
    else if (buf[0] == '1')
    {
        is_abs = true;
    }
    return count;
}

static struct kobj_attribute time_attribute = __ATTR(time, 0664, time_show, time_store);

static struct kobject *time_kobj;

static int time_init(void)
{
    int res = 0;

    time_kobj = kobject_create_and_add("time", kernel_kobj);
    if (!time_kobj)
        return -ENOMEM;

    res = sysfs_create_file(time_kobj, &time_attribute.attr);
    if (res)
        kobject_put(time_kobj);
    pr_info("Module loaded\n");
    return res;
}

static void time_exit(void)
{
    kobject_put(time_kobj);
    pr_info("Module exited\n");
}

module_init(time_init);
module_exit(time_exit);

MODULE_AUTHOR("Artem Shyshko");
MODULE_LICENSE("GPL");
