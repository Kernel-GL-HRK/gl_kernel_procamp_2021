## Homework: Linux Kernel Time Management

1. Implement program which return absolute time in user space.
Use clock_gettime() from time.h. Try different clock id.
Find the difference. Show possible clock resolution provided by clock_getres().

2. Implement kernel module with API in sysfs, which returns relative
time in maximum possible resolution passed since previous read of it.
Implement kernel module with API in sysfs which returns absolute time
of previous reading with maximum resolution like ‘400.123567’ seconds.
