#!/bin/bash

# PLEASE NOTE: the script works with bash only!
# Other shells, like zsh are not supported.

# Add other subsystems which can be tested with hotplugging to monitor.
# Currently we listen to addition/removal of:
# 1) USB pendrives/SD/MMC cards: mmc and block
# 2) USB-HID devices (and generic HID devices, like PS/2? I tested with yubikey & mice only): hid
# 3) USB-to-UART converters: usb-serial (also tty may be added)
monitored_subsys=(block hid usb-serial mmc)

temp_buf=`mktemp`

for subsys in "${monitored_subsys[@]}"
do
    query+="--subsystem-match=$subsys "
done

udevadm monitor -k $query > $temp_buf &
udevadm_pid=$!

cleanup () {
    kill -SIGTERM $udevadm_pid
    rm -rf $temp_buf
}

filter() {
    # tail + grep combo returns a single block of strings separated with
    # spaces instead of '\n's. udevadm does not have an ability to filter
    # events by action types by itself so, for example inserting/removing a
    # USB device generates lots of output which contain also 'bind/unbind' actions
    # which we consider just a noise for this task. These must be excluded
    # from the output, so we iterate through the list of words in search
    # for the given tag (like 'add' or 'remove'); in case one is found
    # the next token is the device name (according to udevadm format). Sub-classes
    # are filtered by --subsystem-match so we don't need to filter that lines.
    # This also eats up [KERNEL... headers.
    # The same could be probably achieved by awk?

    local tag=$1
    local tag_found=0
    local input=("$@")

    # Iterate from the first arg (not zero-th!)
    # this is a crutch to suppress garbage in output
    for line in "${input[@]:1}";
    do
        if [ $tag_found -eq 0 ];
        then
            if [ "$tag" == "$line" ];
            then
                tag_found=1
            fi
        else
            echo $line
            tag_found=0
        fi
    done
}

trap cleanup EXIT SIGTERM

read_pos=0

while : ;
do
    diff_list=`tail -c +$read_pos $temp_buf`
    diff_len=${#diff_list}
    ((read_pos+=$diff_len))

    # Preprocess add/remove lists in order not to
    # filter empty input or an input which does not
    # contain data which we're interested in.
    added_list=`echo $diff_list | grep add`
    removed_list=`echo $diff_list | grep remove`

    if [ ! -z "${added_list}" ]; then
        echo
        echo Added devices:
        filter "add" $added_list
    fi

    if [ ! -z "${removed_list}" ]; then
        echo
        echo Removed devices:
        filter "remove" $removed_list
    fi

    sleep 1
done
