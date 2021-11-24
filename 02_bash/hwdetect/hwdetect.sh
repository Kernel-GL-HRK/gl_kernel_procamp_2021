#!/bin/bash

devices_type=(tty i2c sd) 

function getDevicesList() {
	local -n array=$1
	for device in ${devices_type[@]}; do
		gr=$(ls /dev | grep ${device})
	
		SAVEIFS=$IFS   # Save current IFS
		IFS=$'\n'      # Change IFS to new line
		array+=($gr) # split to array $names
		IFS=$SAVEIFS   # Restore IFS
	done
}

local my_array
getDevicesList my_array
echo "aa ${#my_array[*]}"
#for device in ${my_array[@]}; do
#	echo "ffffff"
#	echo "$device"
#done
