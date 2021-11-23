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

    for _dev in ${new_dev_list}
    do
        is_dev_in_list "${orig_dev_list}" "${_dev}" || {
            if [ -n "${print_event}" ] ; then
                echo "New device connected: ${_dev}"
            fi
            orig_dev_list=$(add_dev_to_list "${orig_dev_list}" "${_dev}")
            }
    done

    if [ -z "${print_event}" ] ; then
        echo "${orig_dev_list}"
    fi
}

detect_disconnect() {
    local orig_dev_list="${1}"
    local new_dev_list="${2}"
    local print_event="${3}"

    for _dev in ${orig_dev_list}
    do
        is_dev_in_list "${new_dev_list}" "${_dev}" || {
            if [ -n "${print_event}" ] ; then
                echo "Device disconnected: ${_dev}"
            fi

            orig_dev_list=$(remove_dev_from_list "${orig_dev_list}" "${_dev}")
            }
    done

    if [ -z "${print_event}" ] ; then
        echo "${orig_dev_list}"
    fi
}

show_flash_drives() {
    lsblk -S -o NAME,MODEL,SIZE,TRAN | grep usb
}

# Argument parsing
update_delay=${1}
[ -z "${update_delay}" ] && update_delay=1

# Main workflow
DEV_USB_TTL=

all_devices=$(get_devices)
DEV_USB_TTL=$(echo "${all_devices}" | grep "ttyUSB")

while true
do
    all_devices=$(get_devices)
    new_dev_usb_ttl=$(echo "${all_devices}" | grep "ttyUSB")

    detect_connect "${DEV_USB_TTL}" "${new_dev_usb_ttl}" print_event
    detect_disconnect "${DEV_USB_TTL}" "${new_dev_usb_ttl}" print_event
    DEV_USB_TTL=$(detect_connect "${DEV_USB_TTL}" "${new_dev_usb_ttl}")
    DEV_USB_TTL=$(detect_disconnect "${DEV_USB_TTL}" "${new_dev_usb_ttl}")

    echo "USB to TTL convertors:"
    echo "${DEV_USB_TTL}"
    echo "Flash drives and uSD cards:"
    show_flash_drives

    sleep ${update_delay}
done
