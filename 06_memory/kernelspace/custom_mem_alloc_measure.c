// SPDX-License-Identifier: GPL-2.0-only
/* Custom kernel module */
/* Memory allocation time measurement unit */

#include <linux/init.h> // Macros used to mark up functions  __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/ktime.h> // Time operations purpose
#include <linux/slab.h> // kzalloc
#include <linux/vmalloc.h> // kzalloc

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Aleksieiev <oleksandr.aleksieiev@globallogic.com>");
MODULE_DESCRIPTION("Measure memory allocation/de-allocation time.");
MODULE_VERSION("0.1");

#define MM_SIZE (262144ul)
static const int SOURCE_NAME_MAX = 2;

static void make_time_mark_and_print(bool print, int src);

static int __init kpc_init(void)
{
	int result = 0;
	void *p = 0;
	unsigned long pages = 0;

	pr_info("kmalloc of %lu\n", MM_SIZE);
	make_time_mark_and_print(false, CLOCK_REALTIME);
	p = kmalloc(MM_SIZE, GFP_KERNEL);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("kfree\n\n");
	make_time_mark_and_print(false, CLOCK_REALTIME);
	kfree(p);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("kzalloc of %lu\n", MM_SIZE);
	make_time_mark_and_print(false, CLOCK_REALTIME);
	p = kzalloc(MM_SIZE, GFP_KERNEL);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("kfree\n\n");
	make_time_mark_and_print(false, CLOCK_REALTIME);
	kfree(p);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("vmalloc of %lu\n", MM_SIZE);
	make_time_mark_and_print(false, CLOCK_REALTIME);
	p = vmalloc(MM_SIZE);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("vfree\n\n");
	make_time_mark_and_print(false, CLOCK_REALTIME);
	vfree(p);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("__get_free_pages of %lu\n", MM_SIZE);
	make_time_mark_and_print(false, CLOCK_REALTIME);
	pages = (unsigned long)__get_free_pages(GFP_KERNEL, MM_SIZE);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	pr_info("free_pages\n\n");
	make_time_mark_and_print(false, CLOCK_REALTIME);
	free_pages(pages, MM_SIZE);
	make_time_mark_and_print(true, CLOCK_REALTIME);

	return result;
}

static void __exit kpc_exit(void)
{
}

static void make_time_mark_and_print(bool print, int src)
{
	static struct timespec64 ts_now;
	static struct timespec64 ts_absolute = { 0l, 0l };
	static struct timespec64 ts_relative = { 0l, 0l };
	static struct timespec64 *p_last_stored_ts;
	static const char *const source_name[] = { "Realtime", "Monotonic",
						   "" };
	static const char *p_source_name;

	if (src == CLOCK_REALTIME) {
		ktime_get_real_ts64(&ts_now);
		p_last_stored_ts = &ts_absolute;
		p_source_name = source_name[src];
	} else if (src == CLOCK_MONOTONIC) {
		ktime_get_ts64(&ts_now);
		p_last_stored_ts = &ts_relative;
		p_source_name = source_name[src];
	} else {
		p_source_name = source_name[SOURCE_NAME_MAX];
	}

	if (p_last_stored_ts->tv_nsec != 0) {
		static struct tm t;

		*p_last_stored_ts = timespec64_sub(ts_now, *p_last_stored_ts);
		time64_to_tm(p_last_stored_ts->tv_sec, 0, &t);

		if (print) {
			pr_info("%s delta: %2.2d:%2.2d:%2.2d.%ld\n",
				p_source_name, t.tm_hour, t.tm_min, t.tm_sec,
				p_last_stored_ts->tv_nsec);
		}
	}

	*p_last_stored_ts = ts_now;
}

module_init(kpc_init);
module_exit(kpc_exit);
