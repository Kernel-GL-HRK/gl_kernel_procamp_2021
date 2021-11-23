#!/bin/bash


# Color constants
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

serial_port_name=""

function detect_usb_devices()
{
    local old_usb_state=$1
    local new_usb_state=$2

    if [ "$old_usb_state" != "$new_usb_state" ]; then
        test=$(diff <(echo "$old_usb_state") <(echo "$new_usb_state"))
        case "$test" in
            *'>'* ) 
            echo -en "${GREEN}New USB device connected:${NC} " 
            echo "$test"
            echo
            ;;
            *'<'* )
            echo -en "${RED}New USB device disconnected:${NC} "
            echo "$test"
            echo
            ;;
        esac
    fi
}

function get_usb_uart_connection_status()
{
    local dev_conn_flag
    
    if [ -n "$(ls /dev | grep ttyACM*)" ]; then
        dev_conn_flag=true
    else
        dev_conn_flag=false
    fi

    echo "$dev_conn_flag"
}


function detect_usb_uart_devices()
{
    local prev_status=$1
    local curr_status=$2

    if [ "$curr_status" != "$prev_status" ]; then

        echo "<<<<<  usb-serial device >>>>>"

        if [ "$curr_status" == true ] ; then
            serial_port_name="$(ls /dev | grep ttyACM*)"
            echo -en "${GREEN}Connected${NC}: "
            echo "$serial_port_name"
        else
            echo -en "${RED}Disconnected${NC}: "
            echo "$serial_port_name"
        fi

        echo "<<<<<  usb-serial device >>>>>"
        echo
    fi
}


echo "scanning ..."

previous_usb_state=$(lsusb)
previous_usb_uart_state=$(get_usb_uart_connection_status)


while true
do

    sleep 0.25

    # check usb state

    current_usb_state=$(lsusb)  
    detect_usb_devices "$previous_usb_state" "$current_usb_state"
    previous_usb_state=${current_usb_state}

    current_usb_uart_state=$(get_usb_uart_connection_status)
    detect_usb_uart_devices "$previous_usb_uart_state" "$current_usb_uart_state"
    previous_usb_uart_state=${current_usb_uart_state}
    
done
