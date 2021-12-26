#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/time64.h>
#include <linux/timer.h>


static ssize_t time_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
    static cycles_t prev_tick;
	static ktime_t  prev_time;

	cycles_t curr_tick;
	ktime_t  curr_time;

	size_t wr_len;

	struct timespec64 ts;
	struct tm dt;

    curr_tick = get_cycles();
    curr_time = ktime_get_real();

    if(prev_tick == 0)
    {
        wr_len = sprintf(buf, "\nFirst Reading\n");
    }
    else
    {
        ts = ns_to_timespec64(prev_time);
        time64_to_tm(ts.tv_sec, 0, &dt);

	    wr_len = sprintf(buf+wr_len, "\nPrev Reading was at     %02d-%02d-%02ld %02d:%02d:%02d.%09ld\n", 
                            dt.tm_mday,
                            dt.tm_mon+1,
                            dt.tm_year+1990,
                            dt.tm_hour,
                            dt.tm_min,
                            dt.tm_sec,
                            ts.tv_nsec);
    }

    prev_tick = curr_tick;
    prev_time = curr_time;

    ts = ns_to_timespec64(curr_time);
    time64_to_tm(ts.tv_sec, 0, &dt);

    wr_len += sprintf(buf+wr_len, "Current time stamp      %02d-%02d-%02ld %02d:%02d:%02d.%09ld\n",
					dt.tm_mday,
					dt.tm_mon+1,
					dt.tm_year+1900,
					dt.tm_hour,
					dt.tm_min,
					dt.tm_sec,
					ts.tv_nsec);

    return wr_len;
}

static struct kobj_attribute list_attribute =
	__ATTR(GetTime, 0664, time_show, NULL);

static struct kobject *time_kobj;

static int hello_init(void){
    int res = 0;
    printk(KERN_WARNING "\nWe are loaded\n");
    time_kobj = kobject_create_and_add("Time", kernel_kobj);
    if (!time_kobj)
		return -ENOMEM;

    res = sysfs_create_file(time_kobj, &list_attribute.attr);
    if(res)
        kobject_put(time_kobj);

	return res;
}

static void hello_exit(void){
    kobject_put(time_kobj);
	pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Ostap Pokornyi <ostappokornyi@gmail.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");