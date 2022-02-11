#!/bin/bash

ls /dev/ > device_list

while :
do
    sleep 1
	ls /dev/ > new_device_list

	device_num=$(wc -l < device_list)
	new_device_num=$(wc -l < new_device_list)
	cmp=$(($device_num - $new_device_num))

	if [ $(($cmp + 0)) -ne 0 ]; then
		if [ $(($device_num + 0)) -gt $(($new_device_num + 0)) ]; then
		echo Unplugged device:
		else
		echo Plugged device:
		fi
		diff=1
	else
	diff=0
	fi

	if [ $(($diff + 0)) -ne 0 ]; then
		diff device_list new_device_list | awk '{print $2}'
        echo ""
		cp new_device_list device_list
	fi
done
