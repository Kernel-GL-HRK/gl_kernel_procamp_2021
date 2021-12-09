/*
 * Memory allocation/freeing time calculation module
 */
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>


MODULE_AUTHOR("Volodymyr Kniazkyi <kniazkyi@gmail.com>");
MODULE_DESCRIPTION("Memory allocation/freeing time calculation module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

#define MEM_PAGE_SIZE	4096
#define MAX_POWER_2		25
#define MAX_MEM_ORDER	10

#define FUNC_KMALLOC	0
#define FUNC_KZALLOC	1
#define FUNC_VMALLOC	2
#define FUNC_GFPAGES	3

#define FUNC_MAX_ID	4

#define FUNC_MAX_NAME	15

char func_name[FUNC_MAX_ID][FUNC_MAX_NAME] = {
		"kmalloc       ",
		"kzalloc       ",
		"vmalloc       ",
		"get_free_pages"};

unsigned int  get_page_order(int64_t size)
{
	unsigned int order;
	int64_t npage;

	npage = size / MEM_PAGE_SIZE;

	if ((size % MEM_PAGE_SIZE) != 0)
		npage++;

	for (order = 0; order < MAX_MEM_ORDER + 1; order++) {
		if (npage <= (1ull << order))
			break;
	}
	return order;
}
/*
 * Measure time for allocate memory
 */
static void *get_memory(int8_t func_id, int64_t size, int64_t *time)
{
	char *buf = NULL;

	int64_t start = 0;
	int64_t stop = 0;

	if (time != NULL)
		*time = 0;

	//start = ktime_get_ns();

	switch (func_id) {
	case FUNC_KMALLOC:
		start = ktime_get_ns();
		buf = kmalloc(size, GFP_KERNEL);
		stop = ktime_get_ns();
		break;
	case FUNC_KZALLOC:
		start = ktime_get_ns();
		buf = kzalloc(size, GFP_KERNEL);
		stop = ktime_get_ns();
		break;
	case FUNC_VMALLOC:
		start = ktime_get_ns();
		buf = vmalloc(size);
		stop = ktime_get_ns();
		break;
	case FUNC_GFPAGES:
		{
			unsigned int order;

			order = get_page_order(size);
			if (order > MAX_MEM_ORDER)
				break;

			start = ktime_get_ns();
			buf = (char *)__get_free_pages(GFP_KERNEL, order);
			stop = ktime_get_ns();
		}
		break;
	default:
		return NULL;
	}

	//stop = ktime_get_ns();

	if (time != NULL) {
		if (buf == NULL)
			*time = -1;// error happened
		else
			*time = stop - start;
	}

	return (void *)buf;
}

/*
 * Measure time for freeing memory
 */
static void free_memory(void *buf, int8_t func_id, int64_t size, int64_t *time)
{
	int64_t start = 0;
	int64_t stop = 0;

	if (time != NULL)
		*time = 0;

	if (buf == NULL)
		return;

	//start = ktime_get_ns();

	switch (func_id) {
	case FUNC_KMALLOC:
		start = ktime_get_ns();
		kfree(buf);
		stop = ktime_get_ns();
		break;
	case FUNC_KZALLOC:
		start = ktime_get_ns();
		kfree(buf);
		stop = ktime_get_ns();
		break;
	case FUNC_VMALLOC:
		start = ktime_get_ns();
		vfree(buf);
		stop = ktime_get_ns();
		break;
	case FUNC_GFPAGES:
		{
			unsigned int order;

			order = get_page_order(size);

			start = ktime_get_ns();
			free_pages((unsigned long)buf, order);
			stop = ktime_get_ns();
		}
		break;
	default:
		return;
	}

	//stop = ktime_get_ns();

	if (time != NULL) {
		if (buf == NULL)
			*time = -1;// error happened
		else
			*time = stop - start;
	}
}

/*
 * Show test results
 */
static ssize_t memtst_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	size_t wrlen = 0;
	int8_t func_id;
	int8_t s = 0;
	int64_t size = 0;
	int64_t tim_alloc;
	int64_t tim_dealloc;

	char *membuf = NULL;

	wrlen = sprintf(buf, "\n\tMemory allocation/deallocation test\n\n");

	wrlen += sprintf(buf+wrlen, "Size\t\tFunction\t\tAlloc(ns)\tDealloc(ns)\n");

	for (s = 0; s < MAX_POWER_2; s++) {

		size = 1ull << s;

		for (func_id = 0; func_id < FUNC_MAX_ID; func_id++) {
			membuf = get_memory(func_id, size, &tim_alloc);
			if (buf != NULL)
				free_memory(membuf, func_id, size, &tim_dealloc);
			if (tim_alloc == -1)
				wrlen += sprintf(buf+wrlen, "2^%i\t\t%s\t\terror\n",
					s, &func_name[func_id][0]);
			else
				wrlen += sprintf(buf+wrlen, "2^%i\t\t%s\t\t%llu\t\t%llu\n",
					s, &func_name[func_id][0], tim_alloc, tim_dealloc);
		}
	}
	return wrlen;
}

static struct kobj_attribute memtst_attribute =
	__ATTR(getmemtst, 0444, memtst_show, NULL);

static struct kobject *memtst_kobj;


static int memtst_module_init(void)
{
	int res;

	memtst_kobj = kobject_create_and_add("memory_tst_mod", kernel_kobj);
	if (!memtst_kobj)
		return -ENOMEM;

	res = sysfs_create_file(memtst_kobj, &memtst_attribute.attr);
	if (res) {
		kobject_put(memtst_kobj);
		return res;
	}

	//pr_info("module init\n");
	return res;
}

static void memtst_module_exit(void)
{
	kobject_put(memtst_kobj);
	//pr_info("module exit\n");
}

module_init(memtst_module_init);
module_exit(memtst_module_exit);
