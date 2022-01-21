// SPDX-License-Identifier: GPL

/*
 * Task: Create kernel module and test allocation/freeing time for functions:
 * **kmalloc, kzmalloc, vmalloc, get_free_pages,
 *  (optional and only for drivers integrated to kernel)alloc_bootmem**.
 * Measure the time of each allocation/freeing except alloc_bootmem.
 * The results should be presented in text file table with followed columns:
 * Buffer size, allocation time, freeing time.
 * Size unit is 1 byte, time unit is 1 ns.
 *
 * Pull request should contains source code of developed driver, Makefile
 * and program output from system log in text format.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/slab_def.h>

MODULE_AUTHOR("Nazarii Kurylko <kurylko.n@gmail.com>");
MODULE_DESCRIPTION("GL Linux Kernel ProCamp 06_memory");
MODULE_LICENSE("GPL");

// paramter to select alloc type:
// 1 - kmalloc; 2 - kzmalloc; 3 - vmalloc; 4 - get_free_pages
static int alloc_type;
enum ALLOC_TYPE { KMALLOC, KZALLOC, VMALLOK, GET_FREE_PAGES, ALLOC_TYPE_CNT };

static unsigned int pow2(unsigned int y)
{
	if (y == 0)
		return 1;
	else if (y % 2 == 0)
		return pow2(y / 2) * pow2(y / 2);
	else
		return 2 * pow2(y / 2) * pow2(y / 2);
}

static void *my_alloc(size_t size)
{
	switch (alloc_type) {
	case KMALLOC:
		return kmalloc(size, GFP_KERNEL);
	case KZALLOC:
		return kzalloc(size, GFP_KERNEL);
	case VMALLOK:
		return vmalloc(size);
	case GET_FREE_PAGES:
		return alloc_pages_exact(size, GFP_KERNEL);
	default:
		return NULL;
	}
}

static void my_free(void *ptr, size_t size)
{
	switch (alloc_type) {
	case KMALLOC:
		return kfree(ptr);
	case KZALLOC:
		return kfree(ptr);
	case VMALLOK:
		return vfree(ptr);
	case GET_FREE_PAGES:
		return free_pages_exact(ptr, size);
	default:
		return;
	}
}

static char *get_alloc_string(void)
{
	switch (alloc_type) {
	case KMALLOC:
		return "kmalloc";
	case KZALLOC:
		return "kzalloc";
	case VMALLOK:
		return "vmalloc";
	case GET_FREE_PAGES:
		return "alloc_pages_exact";
	default:
		return "unknown";
	}
}

static u64 timediff_in_ns(ktime_t time_stamp1, ktime_t time_stamp2)
{
	return ktime_to_ns(ktime_sub(time_stamp2, time_stamp1));
}

/*main algorithm*/
static void memory_allocation_test(void)
{
	void *ptr = NULL;
	ktime_t time_stamp1 = 0, time_stamp2 = 0;
	u64 alloc_time = 0, free_time = 0;
	int i = 0;
	size_t size = 0;

	for (alloc_type = 0; alloc_type < ALLOC_TYPE_CNT; ++alloc_type) {
		pr_info("\ntest allocation/deallocation time for %s\n",
			get_alloc_string());
		pr_info("bytes       | alloc time, ns | free time, ns()\n");
		pr_info("----------------------------------------------\n");
		for (i = 0; i < 32; ++i) {
			size = pow2(i);

			//alloc time measurements
			time_stamp1 = ktime_get();
			ptr = my_alloc(size);
			time_stamp2 = ktime_get();

			alloc_time = timediff_in_ns(time_stamp1, time_stamp2);

			//free time measurements
			if (ptr) {
				time_stamp1 = ktime_get();
				my_free(ptr, size);
				time_stamp2 = ktime_get();
				free_time = timediff_in_ns(time_stamp1,
							   time_stamp2);
			} else {
				pr_info("error when try to %s %lu bytes",
					get_alloc_string(), size);
				break;
			}

			pr_info("  %10lu|   %10llu   |%10llu   |\n", size,
				alloc_time, free_time);
		}
	}
}

static int kmodule_init(void)
{
	memory_allocation_test();
	pr_info("init... kmodule\n");
	return 0;
}

static void kmodule_cleanup(void)
{
	pr_info("bye... kmodule\n");
}

module_init(kmodule_init);
module_exit(kmodule_cleanup);
