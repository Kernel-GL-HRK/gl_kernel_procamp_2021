// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

enum allocator {
	KMALLOC = 0,
	KZALLOC,
	VMALLOC,
	GET_FREE_PAGES,
};

void *call_alloc(enum allocator alloc, size_t i)
{
	void *p = NULL;

	switch (alloc) {
	case KMALLOC:
		p = kmalloc(i, GFP_KERNEL);
		break;
	case KZALLOC:
		p = kzalloc(i, GFP_KERNEL);
		break;
	case VMALLOC:
		p = vmalloc(i);
		break;
	case GET_FREE_PAGES:
		p = (void *)__get_free_pages(GFP_KERNEL, i);
		break;
	default:
		return NULL;
	}

	return p;
}

void call_free(enum allocator alloc, void *p, unsigned long i)
{
	switch (alloc) {
	case KMALLOC:
	case KZALLOC:
		kfree(p);
		break;
	case VMALLOC:
		vfree(p);
		break;
	case GET_FREE_PAGES:
		free_pages((unsigned long)p, i);
		break;
	default:
		break;
	}
}

void measure_one(enum allocator alloc, size_t i)
{
	ktime_t t[3];
	struct timespec64 t_a, t_f;
	void *p;
	size_t buffer_allocated = i;

	t[0] = ktime_get();

	p = call_alloc(alloc, i);
	if (p == NULL)
		return;

	t[1] = ktime_get();
	call_free(alloc, p, i);
	t[2] = ktime_get();

	t_a = ktime_to_timespec64(ktime_sub(t[1], t[0]));
	t_f = ktime_to_timespec64(ktime_sub(t[2], t[1]));

	if (alloc == GET_FREE_PAGES)
		buffer_allocated = (1 << i) * PAGE_SIZE;

	pr_info("%12ld    %lld.%09ld	  %lld.%09ld\n", buffer_allocated, t_a.tv_sec, t_a.tv_nsec, t_f.tv_sec, t_f.tv_nsec);
}

void measure_step_shift(enum allocator alloc, size_t size_max)
{
	size_t i;

	pr_info(" Buffer size  allocation time  freeing time\n");
	for (i = 2; i <= size_max; i *= 2)
		measure_one(alloc, i);
}

void measure_step_plus(enum allocator alloc, size_t size_max)
{
	size_t i;

	pr_info(" Buffer size  allocation time  freeing time\n");
	for (i = 0; i <= size_max; i++)
		measure_one(alloc, i);
}

static int module_malloc_init(void)
{
	pr_info("***** KMALLOC *****\n");
	measure_step_shift(KMALLOC, 4 * 1024 * 1024);

	pr_info("\n***** KZALLOC *****\n");
	measure_step_shift(KZALLOC, 4 * 1024 * 1024);

	pr_info("\n***** VMALLOC *****\n");
	measure_step_shift(VMALLOC, 64 * 1024 * 1024);

	pr_info("\n***** GET_FREE_PAGES *****\n");
	measure_step_plus(GET_FREE_PAGES, 11);

	pr_info("module_malloc: init");

	return 0;
}

static void module_malloc_exit(void)
{
	pr_info("module_malloc: exit");
}

module_init(module_malloc_init);
module_exit(module_malloc_exit);

MODULE_AUTHOR("Volodymyr Kharuk <jlabpih@ukr.net>");
MODULE_LICENSE("GPL v2");
