// SPDX-License-Identifier: GPL-2.0-only
/* Custom kernel module */
/* Print delta time between attribute read */

#include <linux/init.h> // Macros used to mark up functions  __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/ktime.h> // Time operations purpose

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Aleksieiev <oleksandr.aleksieiev@globallogic.com>");
MODULE_DESCRIPTION("Print delta time between reading attribute.");
MODULE_VERSION("0.1");

static const char *const error_codes[] = { "OK",
					   "Operation not permitted",
					   "No such file or directory",
					   "No such process",
					   "Interrupted system call",
					   "I/O error",
					   "No such device or address",
					   "Argument list too long",
					   "Exec format error",
					   "Bad file number",
					   "No child processes",
					   "Try again",
					   "Out of memory",
					   "Permission denied",
					   "Bad address",
					   "Block device required",
					   "Device or resource busy",
					   "File exists",
					   "Cross-device link",
					   "No such device",
					   "Not a directory",
					   "Is a directory",
					   "Invalid argument",
					   "File table overflow",
					   "Too many open files",
					   "Not a typewriter",
					   "Text file busy",
					   "File too large",
					   "No space left on device",
					   "Illegal seek",
					   "Read-only file system",
					   "Too many links",
					   "Broken pipe",
					   "Math argument out of domain of func",
					   "Math result not representable",
					   "Undefined error code" };

static const int ERROR_CODE_MAX = 35;
static const int SOURCE_NAME_MAX = 2;

static const char *const get_error_code(int e)
{
	e = (e < 0) ? -e : e;
	e = (e >= ERROR_CODE_MAX) ? ERROR_CODE_MAX : e;

	return error_codes[e];
}

static void print_time_delta(int t);

static struct kobject *sysfs_time_marker;

static ssize_t trigger_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	print_time_delta(CLOCK_REALTIME);
	print_time_delta(CLOCK_MONOTONIC);

	return 0;
}

static struct kobj_attribute koa_trigger = __ATTR_RO(trigger);

static int __init kpc_init(void)
{
	int result = 0;

	sysfs_time_marker = kobject_create_and_add("time_marker", kernel_kobj);
	if (!sysfs_time_marker) {
		pr_info("kobject_create_and_add fail.\n");
		result = -ENOMEM;
	}

	if (result == 0) {
		result =
			sysfs_create_file(sysfs_time_marker, &koa_trigger.attr);
		if (result)
			kobject_put(sysfs_time_marker);
	}

	pr_info("init %s.\n", get_error_code(result));
	return result;
}

static void __exit kpc_exit(void)
{
	kobject_put(sysfs_time_marker);
	pr_info("exit.\n");
}

static void print_time_delta(int s)
{
	static struct timespec64 ts_now;
	static struct timespec64 ts_absolute = { 0l, 0l };
	static struct timespec64 ts_relative = { 0l, 0l };
	static struct timespec64 *p_last_stored_ts;
	static const char *const source_name[] = { "Realtime", "Monotonic",
						   "" };
	static const char *p_source_name;

	if (s == CLOCK_REALTIME) {
		ktime_get_real_ts64(&ts_now);
		p_last_stored_ts = &ts_absolute;
		p_source_name = source_name[s];
	} else if (s == CLOCK_MONOTONIC) {
		ktime_get_ts64(&ts_now);
		p_last_stored_ts = &ts_relative;
		p_source_name = source_name[s];
	} else {
		p_source_name = source_name[SOURCE_NAME_MAX];
	}

	if (p_last_stored_ts->tv_nsec != 0) {
		static struct tm t;

		*p_last_stored_ts = timespec64_sub(ts_now, *p_last_stored_ts);
		time64_to_tm(p_last_stored_ts->tv_sec, 0, &t);

		pr_info("%s delta: %2.2d:%2.2d:%2.2d.%ld\n", p_source_name,
			t.tm_hour, t.tm_min, t.tm_sec,
			p_last_stored_ts->tv_nsec);
	} else {
		pr_info("Nothing to show yet.\n");
	}

	*p_last_stored_ts = ts_now;
}

module_init(kpc_init);
module_exit(kpc_exit);
