#!/bin/bash

devices_type=(ttyUSB i2c sd) 



function getDevicesList() {
	local -n array=$1
	for device in "${devices_type[@]}"; do
		gr=$(ls /dev | grep ${device})
	
		SAVEIFS=$IFS   # Save current IFS
		IFS=$'\n'      # Change IFS to new line
		array+=($gr) # split to array $names
		IFS=$SAVEIFS   # Restore IFS
	done
}

lastDevicesList=()
getDevicesList lastDevicesList

function isContains() {
	#local -n array=$1
	#local -n nameItem=$2
	for item in $1; do
		echo "process $item $2"
		if [[ $item == $2 ]]; then
			return 0
		fi
	done

	return 1
}

function compare() {
	#local -n current=$1
	#echo "aa ${#current[*]}"
	for item in ${current[@]}; do
		#echo "${lastDevicesList[$item]}"
		#echo "$item"
		#if [[ -z "${lastDevicesList[$item]}" ]]
#		echo "rtrtrt $(isContains $lastDevicesList $item)"


		#echo "find $item  $(isContains lastDevicesList $item)"
		
#		isContains $lastDevicesList $item

		isContains "${lastDevicesList[@]}" $item
		res=$?
#		echo "22 $item $res"
		if [[ $res != 0 ]]; then
			echo "fdd22 $item"
		fi
	done
}

while true; do
	sleep 1
	
	current=()
	getDevicesList current
	
	echo "--------- ${#current[*]}"	
#	for item in "${current[@]}"; do
#		echo "$item"
#	done
	
	compare current
	
	#echo "aa ${#current[*]}"
done


#for device in ${my_array[@]}; do
#	echo "ffffff"
#	echo "$device"
#done
