#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

unsigned long start = 0;
unsigned long total_time = 0; 

static ssize_t time_show(struct kobject *kobj, struct kobj_attribute *attr,
    char *buf)
{
    int min, sec, msec;

    total_time = jiffies - start;

    min = total_time / HZ / 60;
        total_time -= min * HZ * 60;
    sec = total_time / HZ;
        total_time -= sec * HZ;
    msec = total_time * 1000 / HZ;

    sprintf(buf, "%d min %d seconds %d  milliseconds\n", min, sec, msec);

    start = jiffies;
    return strlen(buf);
}

static struct kobj_attribute list_attribute =
    __ATTR(time-relative, 0664, time_show, NULL);

static struct kobject *hello_kobj;

static int hello_init(void)
{
    int res = 0;

    start = jiffies;

    hello_kobj = kobject_create_and_add("time-mod", kernel_kobj);
    if (!hello_kobj)
        return -ENOMEM;
    res = sysfs_create_file(hello_kobj, &list_attribute.attr);
    if (res)
        kobject_put(hello_kobj);
    return res;
}

static void hello_exit(void)
{
    kobject_put(hello_kobj);
    pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);



MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");


//get_jiffies_64()
//jiffies_to_clock_t()
//jiffies_64_to_clock_t()

// static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
//     const char *buf, size_t count)
// {
//     sscanf(buf, "%lu\n", &value);
//     return count;
// }