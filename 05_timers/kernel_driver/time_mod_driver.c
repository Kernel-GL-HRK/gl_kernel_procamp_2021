#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/timekeeping.h>

bool has_read_request = false;

cycles_t cycles_stamp;
static ktime_t timestamp_last_read;

static void handle_first_read_request(void)
{
    cycles_stamp = get_cycles();
    timestamp_last_read = ktime_get_real();
    has_read_request = true;
}

static void update_use_request(void)
{
    cycles_stamp = get_cycles();
    timestamp_last_read = ktime_get_real();
}
static ssize_t relative_time_show(struct kobject *kobj,
                                  struct kobj_attribute *attr, char *buf)
{
    ssize_t bytes_show = 0;
    pr_debug("==TIMER_MODULE==:Called relative_time_show\n");

    if (unlikely(!has_read_request))
    {
        handle_first_read_request();
        bytes_show = sprintf(buf, "First read request\n");
    }
    else
    {
        cycles_t stamp_now = get_cycles();
        cycles_t ellapsed = stamp_now - cycles_stamp;

        bytes_show = sprintf(buf, "Relative cycles count:%lld \n", ellapsed);
        update_use_request();
    }
    return bytes_show;
}

static ssize_t absolute_time_show(struct kobject *kobj,
                                  struct kobj_attribute *attr, char *buf)
{
    ssize_t bytes_show = 0;
    struct tm current_time;
    struct timespec64 ts;

    pr_debug("==TIMER_MODULE==:Called absolute_time_show\n");
    if (unlikely(!has_read_request))
    {
        handle_first_read_request();
        bytes_show = sprintf(buf, "First read request\n");
    }
    else
    {

        ts = ns_to_timespec64(timestamp_last_read);
        time64_to_tm(ts.tv_sec, 0, &current_time);
        bytes_show = sprintf(buf, "Last time absolute read: %d:%d:%d %d/%d/%ld\n", current_time.tm_hour, current_time.tm_min, current_time.tm_sec, current_time.tm_mday, current_time.tm_mon + 1, current_time.tm_year + 1900);
            update_use_request();
    }
    return bytes_show;
}

static struct kobject *timer_mod_entry_root;
struct kobj_attribute relative_time_attr = __ATTR(rw_node, 0660, relative_time_show, NULL);
struct kobj_attribute absolute_time_attr = __ATTR(absolute_time_node, 0660, absolute_time_show, NULL);

static struct attribute *timer_attributes[] =
    {
        &relative_time_attr.attr,
        &absolute_time_attr.attr,
        NULL};

static struct attribute_group timer_group =
    {
        .attrs = timer_attributes};

static __init int time_module_init(void)
{
    int result = 0;
    timer_mod_entry_root = kobject_create_and_add("timer_module_dir", kernel_kobj);
    if (!timer_mod_entry_root)
    {
        pr_warn("%s:%d: kobject_create_and_add failed.\n", __func__,
                __LINE__);
        return -ENOMEM;
    }
    result = sysfs_create_group(timer_mod_entry_root, &timer_group);
    if (result)
    {
        pr_warn("%s:%d: sysfs_create_group failed.\n", __func__,
                __LINE__);
        kobject_put(timer_mod_entry_root);
        return -ENOMEM;
    }
    return 0;
}

static __exit void time_module_exit(void)
{
    kobject_put(timer_mod_entry_root);
}

module_init(time_module_init);
module_exit(time_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valentyn Korniienko");
MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION("A module for obtaining absolute time and last sysfs entry reading relative time");