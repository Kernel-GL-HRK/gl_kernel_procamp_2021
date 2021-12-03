#!/bin/sh

# Simple script for generating logs for kernel module
# with different parameters

echo '[Hello it`s test for first Kernel module]'
echo ' '
echo '[With parameters]'
sleep 0.1s
insmod main.ko user=Valentin
#insmod main.ko user=Valentin task=3
rmmod main.ko

echo ' '
echo '[With bad parameters]'
sleep 0.1s
insmod main.ko user=Valentin task=1
rmmod main.ko

echo ' '
echo '[Without parameters]'
sleep 0.1s
insmod main.ko
rmmod main.ko
