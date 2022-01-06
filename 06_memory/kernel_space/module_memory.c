// SPDX-License-Identifier: GPL
/*
 * Copyright (c) 2022 Vasyl Yosypenko
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/unistd.h>


typedef enum alloc_modes{
    KMALLOC_MODE = 0,
    KZALLOC_MODE,
    VMALLOC_MODE,
    GET_FREE_PAGES_MODE
} alloc_modes_t;


// Implemented 2^N
static uint64_t pow2(uint8_t n)
{
    return 1<<n;
}

static void check_memory_allocation(alloc_modes_t alloc_mode)
{
    char* test_ptr = NULL;

    u64 start_time;
    u64 end_time;
    u64 alloc_time;
    u64 freeing_time;

    uint8_t order = 0;
    uint64_t bytes_numbers = 0;



    pr_info("\n=======================================================================\n");
    pr_info("|%15s|%15s|%18s|%18s|\n","Order","Bytes","Alloc time(ns)","Freeing time(ns)");
    pr_info("=======================================================================\n");

    do
    {
        bytes_numbers = pow2(order);

        switch(alloc_mode)
        {
        case KMALLOC_MODE:
            start_time = ktime_get_ns();
            test_ptr = (char*)kmalloc(bytes_numbers * sizeof(char), GFP_KERNEL);
            end_time = ktime_get_ns();
            break;

        case KZALLOC_MODE:
            start_time = ktime_get_ns();
            test_ptr = (char*)kzalloc(bytes_numbers * sizeof(char), GFP_KERNEL);
            end_time = ktime_get_ns();
            break;

        case VMALLOC_MODE:
            start_time = ktime_get_ns();
            test_ptr = (char*)vmalloc(bytes_numbers * sizeof(char));
            end_time = ktime_get_ns();
            break;

		case GET_FREE_PAGES_MODE:
			start_time = ktime_get_ns();
            test_ptr = (char*)__get_free_pages(GFP_KERNEL, order);
            end_time = ktime_get_ns();
			break;
        }

        alloc_time = end_time - start_time;

		if (!test_ptr)
            break;

		switch(alloc_mode)
		{
		case KMALLOC_MODE:
		case KZALLOC_MODE:
			start_time = ktime_get_ns();
			kfree(test_ptr);
			end_time = ktime_get_ns();
			break;

		case VMALLOC_MODE:
		    start_time = ktime_get_ns();
			vfree(test_ptr);
			end_time = ktime_get_ns();
			break;

		case GET_FREE_PAGES_MODE:
		    start_time = ktime_get_ns();
			free_pages((unsigned long)test_ptr, order);
			end_time = ktime_get_ns();
			break;
		}

		freeing_time = end_time - start_time;

        // For __get_free_pages, number of allocated byte
        // is 2 ^ order * PAGE_SIZE
        if (alloc_mode == GET_FREE_PAGES_MODE)
            bytes_numbers = PAGE_SIZE * bytes_numbers;

		pr_info("|%15d|%15llu|%18llu|%18llu|\n", order, bytes_numbers, alloc_time, freeing_time);

        order++;

    } while (test_ptr);

    pr_info("=======================================================================\n");

}


static int __init memory_module_start(void)
{

    pr_info("Memory module initialized successfully \n");

    pr_info("\nCheck memory allocation with kmalloc:");
    check_memory_allocation(KMALLOC_MODE);

    pr_info("\nCheck memory allocation with kzalloc:");
    check_memory_allocation(KZALLOC_MODE);

    pr_info("\nCheck memory allocation with vmalloc:");
    check_memory_allocation(VMALLOC_MODE);

    pr_info("\nCheck memory allocation with get_free_pages:");
    check_memory_allocation(GET_FREE_PAGES_MODE);

    return 0;
}

static void __exit memory_module_end(void)
{
    pr_info("Memory module removed successfully \n");
}

module_init(memory_module_start);
module_exit(memory_module_end);

MODULE_AUTHOR("Vasyl Yosypenko");
MODULE_DESCRIPTION("The module to check allocation time in the kernel space");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");
