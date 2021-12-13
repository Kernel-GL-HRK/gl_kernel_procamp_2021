// SPDX-License-Identifier: GPL-2.0
/*
 * Create kernel module and test allocation/freeing time for functions:
 * kmalloc, kzmalloc, vmalloc, get_free_pages,
 * (optional and only for drivers integrated to kernel)alloc_bootmem.
 *
 * Measure the time of each allocation/freeing except alloc_bootmem.
 * The results should be presented in text file table with followed columns:
 * Buffer size, allocation time, freeing time. Size unit is 1 byte, time unit is 1 ns.
 */


#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,	 KBUILD_MODNAME
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

#define MAX_TABLE_LENGTH 28

struct table_row_s {
	int power;
	size_t size;
	u64 allocate_took;
	u64 free_took;
};

static ssize_t mem_alloc_kmalloc_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t mem_alloc_vmalloc_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t mem_alloc_kzalloc_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t mem_alloc_free_page_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff);
static ssize_t mem_alloc_table_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count);
static size_t measure_alloc_free_pow2_bytes(struct table_row_s table[], struct table_row_s *p_total_row, void *(fp_allocate)(size_t size), void (fp_free)(char *ptr));
static size_t measure_alloc_free_pages(struct table_row_s table[], struct table_row_s *p_total_row, unsigned long (fp_allocate)(unsigned long order), void (fp_free)(unsigned long addr, unsigned long order));
static size_t print_stat_table_to_buff(char *buff, struct table_row_s table[], size_t table_length, struct table_row_s *p_total_row, char const *table_name);

static struct kobject *table_kobj;
static struct kobj_attribute param_kmalloc_table_attribute = __ATTR(kmalloc_table, 0644, mem_alloc_kmalloc_table_show, mem_alloc_table_store);
static struct kobj_attribute param_vmalloc_table_attribute = __ATTR(vmalloc_table, 0644, mem_alloc_vmalloc_table_show, mem_alloc_table_store);
static struct kobj_attribute param_kzalloc_table_attribute = __ATTR(kzalloc_table, 0644, mem_alloc_kzalloc_table_show, mem_alloc_table_store);
static struct kobj_attribute param_free_pages_table_attribute = __ATTR(free_page_table, 0644, mem_alloc_free_page_table_show, mem_alloc_table_store);


static void *allocate_with_kmalloc(size_t size)
{
	return kmalloc(size, GFP_KERNEL);
}

static void free_with_kfree(char *ptr)
{
	kfree(ptr);
}

static void *allocate_with_vmalloc(size_t size)
{
	return vmalloc(size);
}

static void free_with_vfree(char *ptr)
{
	vfree(ptr);
}

static void *allocate_with_kzalloc(size_t size)
{
	return kzalloc(size, GFP_KERNEL);
}

static unsigned long allocate_with__get_free_pages(unsigned long order)
{
	return __get_free_pages(GFP_KERNEL, order);
}

static void free_with_free_pages(unsigned long addr, unsigned long order)
{
	free_pages(addr, order);
}


static size_t measure_alloc_free_pow2_bytes(struct table_row_s table[], struct table_row_s *p_total_row, void *(fp_allocate)(size_t size), void (fp_free)(char *ptr))
{
	u64 allocate_start, free_start;
	size_t allocate_size = 1, table_length = 0;
	int i, power;
	struct table_row_s *p_row = NULL;
	char *ptr = NULL;

	p_total_row->power = 0;
	p_total_row->size = 0;
	p_total_row->allocate_took = 0;
	p_total_row->free_took = 0;

	for (i = 0, power = 1, allocate_size = 1; i < MAX_TABLE_LENGTH; allocate_size *= 2, power++, i++) {
		p_row = &table[i];
		p_row->power = power;
		p_row->size = allocate_size;
		p_total_row->size += allocate_size;

		allocate_start = ktime_get_mono_fast_ns();
		ptr = (char *)fp_allocate(allocate_size);
		if (ptr == NULL)
			break;

		p_row->allocate_took = ktime_get_mono_fast_ns() - allocate_start;
		p_total_row->allocate_took += p_row->allocate_took;

		free_start = ktime_get_mono_fast_ns();
		fp_free(ptr);
		p_row->free_took = ktime_get_mono_fast_ns() - free_start;
		p_total_row->free_took += p_row->free_took;
		table_length++;
	}

	return table_length;
}


static size_t measure_alloc_free_pages(struct table_row_s table[], struct table_row_s *p_total_row, unsigned long (fp_allocate)(unsigned long order), void (fp_free)(unsigned long addr, unsigned long order))
{
	u64 allocate_start, free_start;
	unsigned long order = 0, addr = 0;
	size_t size, table_length = 0;
	int i;
	struct table_row_s *p_row = NULL;

	p_total_row->power = 0;
	p_total_row->size = 0;
	p_total_row->allocate_took = 0;
	p_total_row->free_took = 0;

	for (i = 0, order = 0, size = 0; i < MAX_TABLE_LENGTH; order++, i++) {
		p_row = &table[i];
		p_row->power = order;
		p_row->size = (1 << order) * PAGE_SIZE;
		p_total_row->size += p_row->size;

		allocate_start = ktime_get_mono_fast_ns();
		addr = fp_allocate(order);
		if (addr == 0)
			break;

		p_row->allocate_took = ktime_get_mono_fast_ns() - allocate_start;
		p_total_row->allocate_took += p_row->allocate_took;

		free_start = ktime_get_mono_fast_ns();
		fp_free(addr, order);
		p_row->free_took = ktime_get_mono_fast_ns() - free_start;
		p_total_row->free_took += p_row->free_took;
		table_length++;
	}

	return table_length;
}


static size_t print_stat_table_to_buff(char *buff, struct table_row_s table[], size_t table_length, struct table_row_s *p_total_row, char const *table_name)
{
	size_t i = 0;
	char *p_buff = buff;
	struct table_row_s *p_row = NULL;

	sprintf(p_buff, "------------------------------- %20s\n", table_name);
	p_buff += strlen(p_buff);

	sprintf(p_buff, "|%-5s|%-20s|%-20s|%-20s|\n", "pow", "size b", "allocate took ns", "free took ns");
	p_buff += strlen(p_buff);
	sprintf(p_buff, "----------------------------------------------------------------------\n");
	p_buff += strlen(p_buff);

	for (i = 0; i < table_length; i++) {
		p_row = &table[i];
		sprintf(p_buff, "|%-5i|%-20lu|%-20llu|%-20llu|\n", p_row->power, p_row->size, p_row->allocate_took, p_row->free_took);
		p_buff += strlen(p_buff);
	}

	sprintf(p_buff, "|%-5s|%-20lu|%-20llu|%-20llu|\n", "total", p_total_row->size, p_total_row->allocate_took, p_total_row->free_took);
	p_buff += strlen(p_buff);
	sprintf(p_buff, "----------------------------------------------------------------------\n");
	p_buff += strlen(p_buff);
	return p_buff - buff;
}


static ssize_t mem_alloc_kmalloc_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	size_t table_length;
	struct table_row_s table[MAX_TABLE_LENGTH];
	struct table_row_s total_row;

	table_length = measure_alloc_free_pow2_bytes(table, &total_row, allocate_with_kmalloc, free_with_kfree);
	return print_stat_table_to_buff(buff, table, table_length, &total_row, "kmalloc");
}


static ssize_t mem_alloc_vmalloc_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	size_t table_length = 0;
	struct table_row_s table[MAX_TABLE_LENGTH];
	struct table_row_s total_row;

	table_length = measure_alloc_free_pow2_bytes(table, &total_row, allocate_with_vmalloc, free_with_vfree);
	return print_stat_table_to_buff(buff, table, table_length, &total_row, "vmalloc");
}


static ssize_t mem_alloc_kzalloc_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	size_t table_length = 0;
	struct table_row_s table[MAX_TABLE_LENGTH];
	struct table_row_s total_row;

	table_length = measure_alloc_free_pow2_bytes(table, &total_row, allocate_with_kzalloc, free_with_kfree);
	return print_stat_table_to_buff(buff, table, table_length, &total_row, "kzalloc");
}


static ssize_t mem_alloc_free_page_table_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	size_t table_length = 0;
	struct table_row_s table[MAX_TABLE_LENGTH];
	struct table_row_s total_row;

	table_length = measure_alloc_free_pages(table, &total_row, allocate_with__get_free_pages, free_with_free_pages);
	return print_stat_table_to_buff(buff, table, table_length, &total_row, "free_pages");
}


static ssize_t mem_alloc_table_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
		return count;
}


static int mem_alloc_init(void)
{
	int res = 0;

	table_kobj = kobject_create_and_add("mem_alloc_mod", kernel_kobj);
	if (!table_kobj)
		return -ENOMEM;

	res = sysfs_create_file(table_kobj, &param_kmalloc_table_attribute.attr);
	if (res)
		kobject_put(table_kobj);

	res = sysfs_create_file(table_kobj, &param_vmalloc_table_attribute.attr);
	if (res)
		kobject_put(table_kobj);

	res = sysfs_create_file(table_kobj, &param_kzalloc_table_attribute.attr);
	if (res)
		kobject_put(table_kobj);

	res = sysfs_create_file(table_kobj, &param_free_pages_table_attribute.attr);
	if (res)
		kobject_put(table_kobj);

	return res;

}


static void mem_alloc_exit(void)
{
	kobject_put(table_kobj);
	pr_info("Module exited\n");
}

module_init(mem_alloc_init);
module_exit(mem_alloc_exit);

MODULE_AUTHOR("Ivan Stepanenko <istepanenko@gmail.com>");
MODULE_DESCRIPTION("Wimbledon module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
