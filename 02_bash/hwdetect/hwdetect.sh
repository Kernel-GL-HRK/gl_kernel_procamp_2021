#!/usr/bin/env bash


get_devices () {
    ls /dev/
}

is_dev_in_list () {
    local _list="${1}"
    local _dev="${2}"

    for _i in ${_list}
    do
        if [[ "${_i}" == "${_dev}" ]] ; then
            return 0
        fi
    done

    return 1
}

add_dev_to_list () {
    local _list="${1}"
    local _dev="${2}"

    echo "${_list} ${_dev}"
}

remove_dev_from_list () {
    local _list="${1}"
    local _dev="${2}"
    local _ret=""

    for _i in ${_list}
    do
        if [[ "${_i}" != "${_dev}" ]] ; then
            _ret="${_ret} ${_i}"
        fi
    done

    echo ${_ret}
}

detect_connect() {
    local orig_dev_list="${1}"
    local new_dev_list="${2}"
    local print_event="${3}"
    local ret=0

    for _dev in ${new_dev_list}
    do
        is_dev_in_list "${orig_dev_list}" "${_dev}" || {
            if [ -n "${print_event}" ] ; then
                echo "New device connected: ${_dev}"
                ret=1
            fi
            orig_dev_list=$(add_dev_to_list "${orig_dev_list}" "${_dev}")
            }
    done

    if [ -z "${print_event}" ] ; then
        echo "${orig_dev_list}"
    fi
    return ${ret}
}

detect_disconnect() {
    local orig_dev_list="${1}"
    local new_dev_list="${2}"
    local print_event="${3}"
    local ret=0

    for _dev in ${orig_dev_list}
    do
        is_dev_in_list "${new_dev_list}" "${_dev}" || {
            if [ -n "${print_event}" ] ; then
                echo "Device disconnected: ${_dev}"
                ret=1
            fi

            orig_dev_list=$(remove_dev_from_list "${orig_dev_list}" "${_dev}")
            }
    done

    if [ -z "${print_event}" ] ; then
        echo "${orig_dev_list}"
    fi
    return ${ret}
}

get_flash_drives() {
    lsblk -S -o NAME,MODEL,SIZE,TRAN | grep usb
}

get_i2c_devices() {
    for i2c_bus in ${1}
    do
        echo -n "Devices on the bus ${i2c_bus}:"
        # bus name is i2c-0
        sudo i2cdetect -y "${i2c_bus:4:1}" | while read -r line
        do
            for element in ${line}
            do
                if [[ "${element}" == "--" ]] || [[ "${element:2:1}" == ":" ]] || [ ${#element} -eq 1 ]; then
                    continue
                else
                    echo -n " 0x${element}"
                fi
            done
        done
        echo ""
    done
}

# Argument parsing
update_delay=${1}
[ -z "${update_delay}" ] && update_delay=1

# Main workflow
DEV_USB_TTL=

all_devices=$(get_devices)
DEV_USB_TTL=$(echo "${all_devices}" | grep "ttyUSB")
DEV_I2C_DEV=$(echo "${all_devices}" | grep "i2c")
DEV_FLASH=""
DEV_I2C_DEVICES=""

echo "USB to TTL convertors:"
echo "${DEV_USB_TTL}"

while true
do
    _tmp=0

    all_devices=$(get_devices)
    new_dev_usb_ttl=$(echo "${all_devices}" | grep "ttyUSB")

    detect_connect "${DEV_USB_TTL}" "${new_dev_usb_ttl}" print_event
    if [ ${?} -eq 1 ]; then
        _tmp=1
    fi
    detect_disconnect "${DEV_USB_TTL}" "${new_dev_usb_ttl}" print_event
    if [ ${?} -eq 1 ]; then
        _tmp=1
    fi
    DEV_USB_TTL=$(detect_connect "${DEV_USB_TTL}" "${new_dev_usb_ttl}")
    DEV_USB_TTL=$(detect_disconnect "${DEV_USB_TTL}" "${new_dev_usb_ttl}")

    if [ ${_tmp} -eq 1 ] ; then
        echo "----------------------"
        echo "USB to TTL convertors:"
        echo "${DEV_USB_TTL}"
        echo "----------------------"
    fi

    _tmp=$(get_flash_drives)
    if [[ "${_tmp}" != "${DEV_FLASH}" ]]; then
        DEV_FLASH="${_tmp}"
        echo "---------------------------"
        echo "Flash drives and uSD cards:"
        echo "${DEV_FLASH}"
        echo "---------------------------"
    fi

    _tmp=$(get_i2c_devices "${DEV_I2C_DEV}")
    if [[ "${_tmp}" != "${DEV_I2C_DEVICES}" ]]; then
        DEV_I2C_DEVICES="${_tmp}"
        echo "------------"
        echo "I2C devices:"
        echo "${DEV_I2C_DEVICES}"
        echo "------------"
    fi

    sleep ${update_delay}
done
