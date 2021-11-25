#!/bin/bash
# Hardware detection script
# Using inotify-tools
# needs root to scan i2c bus devices
# usage hwdetect [i2c]
# i2c - print i2c ports stat for each device

function make_devices_list() {
    DEVICES_IN_ONE_ROW=$(ls /dev | egrep 'sd[a-z]+[0-9]?|ttyUSB[0-9]+|i2c-[0-9]+' | tr '\n' ' ')
    IFS=' '
    read -ra $1 <<< "$DEVICES_IN_ONE_ROW"
}

function make_lsusb_dev_lsit() {
    ONE_ROW=$(lsusb | tr '\n' '*')
    IFS='*'
    read -ra $1 <<< "$ONE_ROW"
}


function show_device_inform() {
    printf ":$1\n"
}

function usb_device_changed() {
    if [[ $1 =~ "CREATE" ]]; then
        echo "Device: $1"
    fi

    sleep 2
    make_devices_list USB_DEVICES

    for ud in ${!USB_DEVICES[@]}; do
        is_old_device='NO'
        new_device=''

        for iud in ${!INITIAL_USB_DEVICES[@]}; do
            if [ ${INITIAL_USB_DEVICES[$iud]} == ${USB_DEVICES[$ud]} ]; then
                is_old_device='YES'
                break
            fi
        done

        if [[ ${is_old_device} == "NO" ]]; then
            show_device_inform ${USB_DEVICES[$ud]}
        fi
    done
    INITIAL_USB_DEVICES=$USB_DEVICES

    make_devices_list INITIAL_USB_DEVICES

    lsusb_device_detected
}

function lsusb_device_detected() {
    make_lsusb_dev_lsit LSUSB
    for i in ${!LSUSB[@]}; do
        is_old_device='NO'
        new_device=''
        for j in ${!LSUSB_STASH[@]}; do
            if [ ${LSUSB_STASH[$j]} == ${LSUSB[$i]} ]; then
                is_old_device='YES'
                break
            fi
        done

        if [[ ${is_old_device} == "NO" ]]; then
            show_device_inform ${LSUSB[$i]}
        fi
    done

    make_lsusb_dev_lsit LSUSB_STASH
}

function get_i2c_gates_list() {
    NAMES_ONE_ROW=$(i2cdetect -l | awk -F'\t' 'BEGIN {OFS = FS} {gsub(/[ ]+$/,"", $3);print $3}' | tr '\n' '*')
    IFS='*'
    read -ra $1 <<< "$NAMES_ONE_ROW"
    ID_ONE_ROW=$(i2cdetect -l | awk -F'\t' 'BEGIN {OFS = FS} {gsub(/^i2c-/,"", $1);print $1}' | tr '\n' ' ')
    IFS=' '
    read -ra $2 <<< "$ID_ONE_ROW"
}

function show_i2c_gates() {
    get_i2c_gates_list gates ids

    for i in ${!ids[@]}; do
        printf "${gates[$i]}\n"
        sudo i2cdetect -y -r "${ids[$i]}"
    done
}

function print_help_and_exit() {
    printf "Hardware detection script\n"
    printf "Using inotify-tools\n"
    printf "needs root to scan i2c bus devices\n"
    printf "usage hwdetect [i2c]\n"
    printf "i2c - print i2c ports stat for each device\n"
    exit 0
}

#
# main
#

if [[ $1 == "i2c" ]]; then
    show_i2c_gates
elif [[ $1 == "--help" ]]; then
    print_help_and_exit
fi

make_devices_list INITIAL_USB_DEVICES
make_lsusb_dev_lsit LSUSB_STASH

while true; do
    usb_device_changed $(inotifywait -e create,delete -r /dev -q --exclude '/dev/shm')
done
