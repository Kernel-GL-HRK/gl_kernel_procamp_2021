#!/bin/bash

# delete temporary files
cleanup() {
  rm udev.txt
  rm udev_new.txt
  rm i2cdev.txt
  rm i2cdev_new.txt
  echo "cleaning up..."
  exit
}

echo "Device conecting monitor"
echo "For exit press Ctrl-C"

# detect usb devices
lsusb > udev.txt

# detect i2c devices
i2cdetect -l >i2cdev.txt

while true
do
# create new list of usb devices
  lsusb > udev_new.txt
# create new list of i2c devices
  i2cdetect -l >i2cdev_new.txt

# look for differences in usb devices list
  diffresult=$(diff udev.txt udev_new.txt)
  size=$(wc -c < udev.txt)
  size_new=$(wc -c < udev_new.txt)
# show differences
  if [ $size -ne $size_new ]
    then
      if [ $size -gt $size_new ]
      then
        echo "Disconnected USB devices:"
      else
        echo "Connected USB devices:"
      fi
      echo $diffresult
      mv -f udev_new.txt udev.txt
    fi

# look for differences in i2c devices list
    diffresult=$(diff i2cdev.txt i2cdev_new.txt)
    size=$(wc -c < i2cdev.txt)
    size_new=$(wc -c < i2cdev_new.txt)
# show differences
    if [ $size -ne $size_new ]
      then
        if [ $size -gt $size_new ]
        then
          echo "Disconnected i2c devices:"
        else
          echo "Connected i2c devices:"
        fi
        echo $diffresult
        mv -f i2cdev_new.txt i2cdev.txt
      fi

  sleep  1
# check if Ctrl-C pressed and exit
  trap  'cleanup' INT
done
