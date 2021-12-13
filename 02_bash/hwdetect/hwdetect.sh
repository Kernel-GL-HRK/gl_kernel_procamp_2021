#!/bin/sh

devices=`lsusb`
while :
do
    sleep 1

    new_devices=`lsusb`

    # discover disconnected devices
    for device in $devices
    do
        is_present=0
        for new_device in $new_devices; 
        do 
            if [ $new_device = $device ]
            then
                is_present=1
                break
            fi
        done
        
        if [ $is_present -eq 0 ]
        then
            echo "Disconnected: $device"
        fi
    done

    # discover connected devices
    for new_device in $new_devices
    do
        is_present=0
        for device in $devices; 
        do 
            if [ $new_device = $device ]
            then
                is_present=1
                break
            fi
        done
        
        if [ $is_present -eq 0 ]
        then
            echo "Connected: $new_device"
        fi
    done

    devices=$new_devices
done