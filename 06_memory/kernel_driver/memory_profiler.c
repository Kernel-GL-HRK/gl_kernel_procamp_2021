#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/msr.h>
#include <linux/gfp.h>

#include <linux/sched.h>

// https://elixir.bootlin.com/linux/latest/source/kernel/kcsan/debugfs.c#L79
// https://elixir.bootlin.com/linux/latest/source/arch/sparc/kernel/traps_64.c#L905
// https://www.kernel.org/doc/gorman/html/understand/understand009.html

typedef enum
{
    k_kmalloc,
    k_vmalloc,
    k_get_free_pages,
    k_kzalloc
} alloc_profile_mode_t;

const char *alloc_function_name[] = {"kmalloc", "kvmalloc", "get_free_pages", "kzalloc"};

static void profile_alloc(alloc_profile_mode_t profile_mode)
{

    cycles_t alloc_start_time;
    cycles_t alloc_end_time;
    cycles_t freeing_start;
    cycles_t freeing_end;

    char *kernel_mem = NULL;

    size_t alloc_request = 1;
    do
    {
        unsigned long order = 1;
        unsigned long psize;
        for (psize = PAGE_SIZE; psize < alloc_request; psize *= 2)
        {
            ++order;
        }

        alloc_start_time = get_cycles();
        switch (profile_mode)
        {
        case k_kmalloc:
            kernel_mem = (char *)kmalloc(alloc_request, GFP_KERNEL);
            break;
        case k_vmalloc:
            kernel_mem = (char *)vmalloc(alloc_request);
            break;
        case k_get_free_pages:
            kernel_mem = (char *)__get_free_pages(GFP_KERNEL, order);
            break;
        case k_kzalloc:
            kernel_mem = (char *)kzalloc(alloc_request, GFP_KERNEL);
            break;
        default:
            break;
        }
        alloc_end_time = get_cycles();
        if (!kernel_mem)
            break;

        pr_debug("Allocation:%s, Bytes:%ld, order: %ld, cycles for request: %lld\n", alloc_function_name[profile_mode], alloc_request, order, (alloc_end_time - alloc_start_time));

        freeing_start = get_cycles();
        switch (profile_mode)
        {
        case k_kmalloc:
            kfree(kernel_mem);
            break;
        case k_vmalloc:
            vfree(kernel_mem);
            break;
        case k_get_free_pages:
            free_pages((unsigned long)kernel_mem, order);
            break;
        case k_kzalloc:
            kfree(kernel_mem);
            break;
        default:
            break;
        }
        freeing_end = get_cycles();
        pr_debug("Freeing:%s, Bytes:%ld, order:%ld, cycles for request: %lld\n", alloc_function_name[profile_mode], alloc_request, order, (freeing_end - freeing_start));

        alloc_request *= 2;
    } while (true);
}
static int __init init(void)
{
    profile_alloc(k_kmalloc);
    profile_alloc(k_vmalloc);
    profile_alloc(k_get_free_pages);
    profile_alloc(k_kzalloc);

    return -1;
}
module_init(init);
MODULE_AUTHOR("Valentyn Korniienko");
MODULE_DESCRIPTION("Speedtest of the the different allocation functions");
MODULE_LICENSE("GPL");