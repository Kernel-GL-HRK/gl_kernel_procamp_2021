#!/bin/bash

#fucntion prints information about monitored device
function detect_devices()
{
  local dev_type=$1
  local dev_filepatern=$2
  local dev_filename=$3
  local created_deleted=$4

  if [[ "$dev_filename" == *"$dev_filepatern"* ]]
  then
    if [[ "$created_deleted" == *"DELETE"* ]]
    then
      echo "$dev_type deivce plugg off: file $dev_filename"
    elif [[ "$created_deleted" == *"CREATE"* ]]
    then
      echo "$dev_type device plugg in: file $dev_filename"
    fi
  fi
}

# script is using inotify toos. below is check if inotify tool is installed
if ! [ -x "$(command -v inotifywait)" ]; then
  echo "Inotify-tools is not installed. Please install using following commands:"
  echo "\$ sudo apt update"
  echo "\$ sudo apt install inotify-tools"
  exit 1
fi

echo "Script detects plugin/plug off of the USB to TTL, flash drive, sd card and i2c devices:"

# inotify tool is used to monitor the folder '/dev/''
inotifywait -m -r -q -e create -e delete --format '%w%f %e' "/dev/" | while read FILE_EVENT
do
  #sperate file name and event
  IN_ARRAY=($FILE_EVENT)

  #echo "original  $FILE_EVENT"
  #echo "elem1  ${IN_ARRAY[0]}"
  #echo "elem2  ${IN_ARRAY[1]}"

  detect_devices "USB to TTL"   "usbttl" "${IN_ARRAY[0]}" "${IN_ARRAY[1]}"
  detect_devices "flash drive"  "mmcblk" "${IN_ARRAY[0]}" "${IN_ARRAY[1]}"
  detect_devices "SD card"      "sd"     "${IN_ARRAY[0]}" "${IN_ARRAY[1]}"
  detect_devices "i2c device"   "i2c"    "${IN_ARRAY[0]}" "${IN_ARRAY[1]}"
done
