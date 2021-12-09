#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/timex.h>
#include <linux/ktime.h>
#include <linux/vmalloc.h>

static void test_kmalloc(void)
{
	void *mem;
	int i;
	int max_bits = sizeof(size_t) * 8;
	size_t mem_size;
	ktime_t start, stop;
	ktime_t alloc_time, free_time;

	pr_info("|---------|------------------|----------------|\n");
	pr_info("|  Size   | kmalloc Time, ns | kfree Time, ns |\n");
	pr_info("|---------|------------------|----------------|\n");

	for (i = 0; i < max_bits; i++) {
		mem_size = (1 << i);

		if (mem_size > KMALLOC_MAX_SIZE)
			break;

		start = ktime_get();
		mem = kmalloc(mem_size, GFP_KERNEL);
		stop = ktime_get();
		alloc_time = ktime_sub(stop, start);

		if (mem) {
			*((char *)mem) = 1;

			start = ktime_get();
			kfree(mem);
			stop = ktime_get();
			free_time = ktime_sub(stop, start);
		} else {
			break;
		}

		pr_info("| %7lu | %16llu | %14llu |\n", mem_size, alloc_time,
			free_time);
	}
	pr_info("|---------|------------------|----------------|\n");
}

static void test_kzmalloc(void)
{
	void *mem;
	int i;
	int max_bits = sizeof(size_t) * 8;
	size_t mem_size;
	ktime_t start, stop;
	ktime_t alloc_time, free_time;

	pr_info("|---------|--------- ---------|----------------|\n");
	pr_info("|  Size   | kzmalloc Time, ns | kfree Time, ns |\n");
	pr_info("|---------|---------- --------|----------------|\n");

	for (i = 0; i < max_bits; i++) {
		mem_size = (1 << i);

		if (mem_size > KMALLOC_MAX_SIZE)
			break;

		start = ktime_get();
		mem = kzalloc(mem_size, GFP_KERNEL);
		stop = ktime_get();
		alloc_time = ktime_sub(stop, start);

		if (mem) {
			*((char *)mem) = 1;

			start = ktime_get();
			kfree(mem);
			stop = ktime_get();
			free_time = ktime_sub(stop, start);
		} else {
			break;
		}

		pr_info("| %7lu | %16llu  | %14llu |\n", mem_size, alloc_time,
			free_time);
	}
	pr_info("|---------|-------------------|----------------|\n");
}

static void test_vmalloc(void)
{
	void *mem;
	int i;
	int max_bits = sizeof(size_t) * 8;
	size_t mem_size;
	ktime_t start, stop;
	ktime_t alloc_time, free_time;

	pr_info("|----------|-------- ---------|----------------|\n");
	pr_info("|  Size    | vmalloc Time, ns | vfree Time, ns |\n");
	pr_info("|----------|--------- --------|----------------|\n");

	for (i = 0; i < max_bits; i++) {
		mem_size = (1 << i);

		start = ktime_get();
		mem = vmalloc(mem_size);
		stop = ktime_get();
		alloc_time = ktime_sub(stop, start);

		if (mem) {
			*((char *)mem) = 1;

			start = ktime_get();
			vfree(mem);
			stop = ktime_get();
			free_time = ktime_sub(stop, start);
		} else {
			break;
		}

		pr_info("| %8lu | %16llu | %14llu |\n", mem_size, alloc_time,
			free_time);
	}
	pr_info("|----------|------------------|----------------|\n");
}

static void test_get_free_pages(void)
{
	unsigned long mem;
	int i;
	ktime_t start, stop;
	ktime_t alloc_time, free_time;

	pr_info
	    ("|----------|-------- ----------------|---------------------|\n");
	pr_info
	    ("| order    | get_free_pages Time, ns | free_pages Time, ns |\n");
	pr_info
	    ("|----------|--------- ---------------|---------------------|\n");

	for (i = 0; i < 12; i++) {
		start = ktime_get();
		mem = __get_free_pages(GFP_KERNEL, i);
		stop = ktime_get();
		alloc_time = ktime_sub(stop, start);

		if (mem) {
			*((char *)mem) = 1;

			start = ktime_get();
			free_pages(mem, i);
			stop = ktime_get();
			free_time = ktime_sub(stop, start);
		} else {
			break;
		}

		pr_info("| %8u | %23llu | %19llu |\n", i, alloc_time,
			free_time);
	}
	pr_info
	    ("|----------|-------------------------|---------------------|\n");
}

static int my_module_init(void)
{
	test_kmalloc();
	test_kzmalloc();
	test_vmalloc();
	test_get_free_pages();

	return 0;
}

static void my_module_exit(void)
{
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR("Michael N. <michael.n@gmail.com>");
MODULE_DESCRIPTION("Memory allocation testing module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
