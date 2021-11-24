#!/usr/bin/bash

function detect_connected_devices {
    echo 'USB to TTL converters:'
    echo $(ls -l /dev/ttyUSB* 2>/dev/null | awk '{print $10}')

    echo 'USB Mass Storage Devices:'
    usb_device_ids=$(lsusb | awk '{ printf "%d:%d\n", $2, $4 }')
    declare -r USB_MASS_STORAGE_CLASS='8'
    while IFS= read -r line; do
        curr_interface_class="$(lsusb -s $line -v 2>/dev/null | grep bInterfaceClass | awk '{print $2}')"
        if [ "$curr_interface_class" == $USB_MASS_STORAGE_CLASS ]; then
            echo "$(lsusb -s $line 2>/dev/null)"
        fi
    done <<< "$usb_device_ids"
}

export -f detect_connected_devices
watch -d -n 0.2 -t -x bash -c detect_connected_devices
