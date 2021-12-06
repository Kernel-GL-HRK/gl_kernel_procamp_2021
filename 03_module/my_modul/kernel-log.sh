#!/bin/sh

# Simple script for generating logs for kernel module
# with different parameters

echo '[Hello it`s test for first Kernel module]'
echo ' '
echo '[With parameters]'
sleep 0.1s
insmod modul_add.ko znach_1=122  znach_2=133
rmmod modul_add.ko

echo ' '
echo '[With one parameters]'
sleep 0.1s 
insmod modul_add.ko znach_1=-10    
rmmod modul_add.ko

echo ' '
echo '[Without parameters]'
sleep 0.1s
insmod modul_add.ko
rmmod modul_add.ko
