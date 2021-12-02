/*
 * Time intervals calculating module
 */
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/time64.h>

MODULE_AUTHOR("Volodymyr Kniazkyi <kniazkyi@gmail.com>");
MODULE_DESCRIPTION("Time intervals calculating module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");


/*
 * Show absolute time of previous reading
 * Show relation time passed since previous reading
 */
static ssize_t timing_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	static cycles_t prev_tick;
	static ktime_t  prev_time;

	cycles_t curr_tick;
	ktime_t  curr_time;

	size_t wr_len;
	struct timespec64 ts;
	struct tm dt;


	//current time stamp
	curr_tick = get_cycles();///CLOCK_TICK_RATE;
	curr_time = ktime_get_real();

	if (prev_tick == 0) {
		wr_len = sprintf(buf, "First reading\n");
	} else {
		ts = ns_to_timespec64(prev_time);
		//ktime_get_coarse_real_ts64(&ts);
		time64_to_tm(ts.tv_sec, 0, &dt);

		wr_len  = sprintf(buf, "Previous reading was at %02d-%02d-%02ld %02d:%02d:%02d.%09ld\n",
					dt.tm_mday,
					dt.tm_mon+1,
					dt.tm_year+1900,
					dt.tm_hour,
					dt.tm_min,
					dt.tm_sec,
					ts.tv_nsec);


		prev_time = curr_time - prev_time;
		ts = ns_to_timespec64(prev_time);

		wr_len  += sprintf(buf+wr_len, "Relative time                          %04lld.%09ld\n",
					ts.tv_sec,
					ts.tv_nsec);

		prev_tick = curr_tick - prev_tick;
		wr_len  += sprintf(buf+wr_len, "Relative cycles                            %09lld\n", prev_tick);
	}

	//save current time stamp
	prev_tick = curr_tick;
	prev_time = curr_time;

	//output of current time stamp
	ts = ns_to_timespec64(curr_time);
	time64_to_tm(ts.tv_sec, 0, &dt);

	wr_len  += sprintf(buf+wr_len, "Current time stamp      %02d-%02d-%02ld %02d:%02d:%02d.%09ld\n",
					dt.tm_mday,
					dt.tm_mon+1,
					dt.tm_year+1900,
					dt.tm_hour,
					dt.tm_min,
					dt.tm_sec,
					ts.tv_nsec);

	return wr_len;
}

//static struct kobj_attribute getclk_attribute = {
//	.attr = {.name = "getclk", 0444},
//	.show = absolute_time_show
//};

static struct kobj_attribute getclk_attribute =
	__ATTR(getclk, 0444, timing_show, NULL);

static struct kobject *calctime_kobj;


static int calctime_module_init(void)
{
	int res;

	calctime_kobj = kobject_create_and_add("clc_time", kernel_kobj);
	if (!calctime_kobj)
		return -ENOMEM;

	res = sysfs_create_file(calctime_kobj, &getclk_attribute.attr);
	if (res) {
		kobject_put(calctime_kobj);
		return res;
	}

	//pr_info("module init\n");
	return res;
}

static void calctime_module_exit(void)
{
	kobject_put(calctime_kobj);
	//pr_info("module exit\n");
}

module_init(calctime_module_init);
module_exit(calctime_module_exit);
