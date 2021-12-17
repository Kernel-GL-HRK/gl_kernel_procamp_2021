// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2021 Vasyl Yosypenko
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>

#define GET_ENTIRE_SEC(TIME_NS) (TIME_NS / (1000000000UL))
#define GET_REMAIN_NS(TIME_NS) (TIME_NS % (1000000000UL))

static struct kobject *time_kobj;

static ssize_t show_abs_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t show_rel_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf);


static struct kobj_attribute abs_time_attribute =
    __ATTR(abs_time, 0664, show_abs_time, NULL);

static struct kobj_attribute rel_time_attribute =
    __ATTR(rel_time, 0664, show_rel_time, NULL);

static ssize_t show_abs_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    static u64 last_abs_time;

    sprintf(buf, "%llu.%llu sec\n", GET_ENTIRE_SEC(last_abs_time), GET_REMAIN_NS(last_abs_time));

    last_abs_time = ktime_get();

    return strlen(buf);
}

static ssize_t show_rel_time(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    static u64 last_rel_time;
    static u64 curr_rel_time;
    u64 diff_time;

    curr_rel_time = ktime_get();

    diff_time = curr_rel_time - last_rel_time;

    sprintf(buf, "%llu.%llu sec\n", GET_ENTIRE_SEC(diff_time), GET_REMAIN_NS(diff_time));

    last_rel_time = curr_rel_time;

    return strlen(buf);
}

static int __init time_module_start(void)
{
    int res = 0;

    time_kobj = kobject_create_and_add("time_status", kernel_kobj);
    if (!time_kobj) {
        return -ENOMEM;
    }

    res = sysfs_create_file(time_kobj, &abs_time_attribute.attr);
    if (res) {
        kobject_put(time_kobj);
    }

    res = sysfs_create_file(time_kobj, &rel_time_attribute.attr);
    if (res) {
        kobject_put(time_kobj);
    }

    pr_info("Module initialized successfully \n");

    return res;
}

static void __exit time_module_end(void)
{
    kobject_put(time_kobj);

    pr_info("Module removed successfully \n");
}

module_init(time_module_start);
module_exit(time_module_end);

MODULE_AUTHOR("Vasyl Yosypenko");
MODULE_DESCRIPTION("Simle module that gets relative and absolute time of sysfs file read");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");
