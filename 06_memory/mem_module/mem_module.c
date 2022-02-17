// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME
#define KMALLOC 1
#define KZALLOC 2
#define VMALLOC 3
#define GET_FREE_PAGES 4

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/time.h>

static int arg = 1;
module_param(arg, int, 0660);

int power(int base, int power)
{
	if (power == 0) {
		return 0;
	int count = 1, tmp = base;

	while (count < power) {
		base *= tmp;
		count++;
	}
	return base;
}

int get_nsec(struct timespec64 ts)
{
	int time;

	ktime_get_real_ts64(&ts);
	time = ts.tv_nsec;
	return time;
}

struct timespec64 clock;

int init_module(void)
{
	int start, stop, diff_alloc, diff_free, pw, x;

	pr_info("Module loaded\n");
	for (x = 1; x <= 30; x++) {
		int *x_ptr;
		unsigned long addr; //for __get_free_pages

		x_ptr = NULL;
		pw = power(2, x);
		start = get_nsec(clock);  //for allocating
		switch (arg) {
		case KMALLOC:
			x_ptr = kmalloc(pw, GFP_KERNEL);
			break;
		case KZALLOC:
			x_ptr = kzalloc(pw, GFP_KERNEL);
			break;
		case VMALLOC:
			x_ptr = vmalloc(pw);
			break;
		case GET_FREE_PAGES:
			addr = __get_free_pages(GFP_KERNEL, x);
			break;
		default:
			pr_info("Uncompatible argument");
			return 1;
		}
		stop = get_nsec(clock);
		diff_alloc = stop - start;
		start = get_nsec(clock);  //for freeing
		if (arg != GET_FREE_PAGES) {
			if (arg != VMALLOC)
				kfree(x_ptr);
			else
				vfree(x_ptr);
			stop = get_nsec(clock);
			diff_free = stop - start;
			pr_info("%d ns for allocating and %d ns for freeing %d bytes\n", diff_alloc, diff_free, pw);
		} else {
			free_pages(addr, x);
			stop = get_nsec(clock);
			diff_free = stop - start;
			pr_info("%d ns for allocating and %d ns for freeing %d pages\n", diff_alloc, diff_free, x);
		}
	}
	return 0;
}

void cleanup_module(void)
{
	pr_info("Module exited\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Shyshko");
