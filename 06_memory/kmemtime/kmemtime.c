// SPDX-License-Identifier: GPL-2.0

/**
 * @brief Kernel module to measure time spent for different type of memory allocators
 */

#include <linux/math.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/string.h>
#include <asm/bug.h>
#include <asm/page.h>

/* Max n for 2^n to allocate the buffer */
#define KMEMTIME_TESTS_MAX_N   10

enum kmem_type {
	e_malloc,
	e_calloc,
	e_get_free_pages,
	e_alloca,
};

struct kmemtime {
	enum kmem_type type;
	const char *alloc_name;
	const char *free_name;
	int buff_size[KMEMTIME_TESTS_MAX_N];
	u64 time_alloc[KMEMTIME_TESTS_MAX_N];
	u64 time_free[KMEMTIME_TESTS_MAX_N];
};

static struct kmemtime kmemtime_test[] = {
	{ .type = e_malloc,         .alloc_name = "kmalloc       ", .free_name = "kfree     " },
	{ .type = e_calloc,         .alloc_name = "vmalloc       ", .free_name = "vfree     " },
	{ .type = e_get_free_pages, .alloc_name = "get_free_pages", .free_name = "free_pages" },
	{ .type = e_alloca,         .alloc_name = "kzalloc       ", .free_name = "kfree     " },
};

static char *memtime_print_result(const struct kmemtime *result)
{
	const char *size_str = "Buffer size, bytes |";
	int buff_pos = 0;
	char *table;

	/* Estimate buffer size */
	const int header_len = snprintf(NULL, 0, "%s %s, ns | %s, ns |\n",
		size_str, result->alloc_name, result->free_name);

	/* Table columns width for data printing. -2 for '|' and space characters */
	const int buff_size_width  = strlen(size_str) - 2;
	const int alloc_time_width = strlen(result->alloc_name) + strlen(", ns | ") - 2;
	const int free_time_width  = strlen(result->free_name) + strlen(", ns | ") - 2;
	const int row_len    = snprintf(NULL, 0, "%*d |%*llu |%*llu |\n",
		buff_size_width, result->buff_size[0],
		alloc_time_width, result->time_alloc[0],
		free_time_width, result->time_free[0]);

	/* +1 for '\0' */
	const size_t table_len = header_len + row_len * KMEMTIME_TESTS_MAX_N + 1;

	table = kmalloc(table_len, GFP_KERNEL);
	if (table == NULL)
		return NULL;

	table[0] = '\0';

	buff_pos = snprintf(table, table_len, "%s %s, ns | %s, ns |\n",
		size_str, result->alloc_name, result->free_name);

	for (int i = 0; i < KMEMTIME_TESTS_MAX_N; ++i) {
		buff_pos += snprintf(&table[buff_pos], table_len - buff_pos, "%*d |%*llu |%*llu |\n",
			buff_size_width, result->buff_size[i],
			alloc_time_width, result->time_alloc[i],
			free_time_width, result->time_free[i]);
	}

	return table;
}

static void memtime_get_result(struct kmemtime *result)
{
	int *test_ptr;
	int buff_size;
	unsigned long page_addr;
	u64 alloc_start;
	u64 alloc_end;
	u64 free_end;

	for (int i = 0; i < KMEMTIME_TESTS_MAX_N; ++i) {
		buff_size            = int_pow(2, i);
		result->buff_size[i] = buff_size;

		switch (result->type) {
		case e_malloc:
			alloc_start = ktime_get_ns();
			test_ptr    = kmalloc(buff_size, GFP_KERNEL);
			BUG_ON(test_ptr == NULL);
			alloc_end   = ktime_get_ns();
			kfree(test_ptr);
			free_end    = ktime_get_ns();
			break;

		case e_alloca:
			alloc_start = ktime_get_ns();
			test_ptr    = kzalloc(buff_size, GFP_KERNEL);
			BUG_ON(test_ptr == NULL);
			alloc_end   = ktime_get_ns();
			kfree(test_ptr);
			free_end    = ktime_get_ns();
			break;

		case e_calloc:
			alloc_start = ktime_get_ns();
			test_ptr    = vmalloc(buff_size);
			BUG_ON(test_ptr == NULL);
			alloc_end   = ktime_get_ns();
			vfree(test_ptr);
			free_end    = ktime_get_ns();
			break;

		case e_get_free_pages:
			/* Overwrite buff size for __get_free_pages */
			result->buff_size[i] = int_pow(2, i) * PAGE_SIZE;
			alloc_start = ktime_get_ns();
			page_addr   = __get_free_pages(GFP_KERNEL, i);
			alloc_end   = ktime_get_ns();
			free_pages(page_addr, i);
			free_end    = ktime_get_ns();
			break;

		default:
			BUG();
			break;
		}

		result->time_alloc[i] = alloc_end - alloc_start;
		result->time_free[i]  = free_end - alloc_end;
		pr_debug("time %lld nanoseconds for %s\n", result->time_alloc[i], result->alloc_name);
		pr_debug("Free time %lld nanoseconds for %s\n", result->time_free[i], result->free_name);
	}
}

int kmemtime_init(void)
{
	char *res;

	for (int i = 0; i < ARRAY_SIZE(kmemtime_test); ++i) {
		memtime_get_result(&kmemtime_test[i]);
		res = memtime_print_result(&kmemtime_test[i]);
		if (res == NULL) {
			BUG();
			return -ENOMEM;
		}

		pr_info("%s\n", res);
		kfree(res);
	}

	return 0;
}

void kmemtime_exit(void)
{

}

module_init(kmemtime_init);
module_exit(kmemtime_exit);

MODULE_DESCRIPTION("kmemtime kernel module");
MODULE_AUTHOR("Sergey D.");
MODULE_LICENSE("GPL");
