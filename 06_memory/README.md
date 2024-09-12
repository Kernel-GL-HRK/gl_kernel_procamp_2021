# Memory management

## Homework
1. Create user-space C or C++ program which tries to allocate buffers
  with sizes 2^x for x in range from 0 to maximium possible value 
  using functions:
  **malloc, calloc, alloca, (optional for C++) new **.
  Measure time of each allocation/freeing.
  2^x means x power of 2 in this task.
Pull request should contains program source code and program output
in text format.

2. Create kernel module and test allocation/freeing time for functions:
  **kmalloc, kzmalloc, vmalloc, get_free_pages,
   (optional and only for drivers integrated to kernel)alloc_bootmem**.
  Measure the time of each allocation/freeing except alloc_bootmem.
  The results should be presented in text file table with followed columns:
  Buffer size, allocation time, freeing time.
  Size unit is 1 byte, time unit is 1 ns.

Pull request should contains source code of developed driver, Makefile
and program output from system log in text format.
