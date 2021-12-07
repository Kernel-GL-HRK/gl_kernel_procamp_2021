#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/version.h>

#define MODULE_TAG      "test_module "
#define BUFFER_SIZE 100

static char *proc_buffer;
u64 start1, start2;
u64 end1, end2;
size_t size;

static const char * const mem_cr_names[] = {
	"kmalloc",
	"kzalloc",
	"vmalloc",
	"get_free_pages",
};

enum mem_cr_type {
	mem_cr_kmalloc = 0,
	mem_cr_kzalloc = 1,
	mem_cr_vmalloc = 2,
	mem_cr_get_free_pages = 3,
};

void ShowHeader(void)
{
	pr_info("al_name \t\t buf_size \t all_time \t fr_time");
}

void ShowReport(const char *name_timer)
{
	pr_info("%s: \t %ld, \t\t %lld ns, \t %lld ns\n",
	    name_timer, size, end1-start1, end2-start2);
}

void StartTimer1(void)
{
	start1 = ktime_get_ns();
}

void StopTimer1StartTimer2(void)
{
	end1 = ktime_get_ns();
	start2 = ktime_get_ns();
}

void StopTimer(void)
{
	end2 = ktime_get_ns();
}

static int create_buffer(int value)
{
	int ret = 0;

	switch (value) {
	case mem_cr_kmalloc:
		size = BUFFER_SIZE;
		proc_buffer = kmalloc(size, GFP_KERNEL);
		break;
	case mem_cr_kzalloc:
		size = BUFFER_SIZE;
		proc_buffer = kzalloc(size, GFP_KERNEL);
		break;
	case mem_cr_vmalloc:
		size = PAGE_SIZE;
		proc_buffer = vmalloc(size);
		break;
	case mem_cr_get_free_pages:
		size = 0;
		proc_buffer = (char *)__get_free_pages(GFP_KERNEL, 0);
		break;
	}
	if (proc_buffer == NULL)
		ret =  -ENOMEM;
	return ret;
}

static void cleanup_buffer(int value)
{
	if (proc_buffer) {
		switch (value) {
		case mem_cr_kmalloc:
			kfree(proc_buffer);
			break;
		case mem_cr_kzalloc:
			kfree(proc_buffer);
			break;
		case mem_cr_vmalloc:
			vfree(proc_buffer);
			break;
		case mem_cr_get_free_pages:
			free_pages((unsigned long)proc_buffer, 0);
			break;
	}
	proc_buffer = NULL;
	}
}


static int __init example_init(void)
{
	int err;

	pr_notice(MODULE_TAG "loaded\n");

	ShowHeader();

	StartTimer1();
	err = create_buffer(mem_cr_kmalloc);
	StopTimer1StartTimer2();
	cleanup_buffer(mem_cr_kmalloc);
	StopTimer();
	ShowReport(mem_cr_names[mem_cr_kmalloc]);

	StartTimer1();
	err = create_buffer(mem_cr_kzalloc);
	StopTimer1StartTimer2();
	cleanup_buffer(mem_cr_kzalloc);
	StopTimer();
	ShowReport(mem_cr_names[mem_cr_kzalloc]);

	StartTimer1();
	err = create_buffer(mem_cr_vmalloc);
	StopTimer1StartTimer2();
	StopTimer();
	ShowReport(mem_cr_names[mem_cr_vmalloc]);

	StartTimer1();
	err = create_buffer(mem_cr_get_free_pages);
	StopTimer1StartTimer2();
	cleanup_buffer(mem_cr_get_free_pages);
	StopTimer();
	ShowReport(mem_cr_names[mem_cr_get_free_pages]);

	return -1;

error:
	pr_err(MODULE_TAG "failed to load\n");
	return err;
}

static void __exit example_exit(void)
{
	pr_notice(MODULE_TAG "exited\n");
}

module_init(example_init);
module_exit(example_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vitaliy Zhyrko vitaliyzh@gmail.com");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");
