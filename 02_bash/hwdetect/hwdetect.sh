#!/bin/bash

get_devs() {
    echo $( ls /dev )
}

find_changed() {
    for el in $1
    do
        if [[ "$2" != *"$el"* ]] ;
        then
            echo $el
        fi
    done
}

filter_devices() {
    # name of devices in /dev can hold some numbers and prefixes
    # only SD cards, USB to TTL, i2c, USB
    local devices="mmc ttyACM ttyUSB i2c sd"

    for dev in $devices
    do
        for el in $1
        do
            if [[ "$el" == *"$dev"* ]] ;
            then
                echo $el
            fi
        done
    done
}

find_added() {
    # look in new list for changed devices
    local res=$(find_changed "$1" "$2")
    res=$(filter_devices "$res")

    if [[ "$res" != "" ]] ;
    then
        echo "============="
        echo "Added: "
        echo "$res"
        echo "============="
    fi
}

find_removed() {
    # look in prev list for changed devices
    local res=$(find_changed "$2" "$1")
    res=$(filter_devices "$res")

    if [[ "$res" != "" ]] ;
    then
        echo "============="
        echo "Removed: "
        echo "$res"
        echo "============="
    fi
}

prev=$(get_devs)
while true
do
    new=$(get_devs)
    if [[ $prev != $new ]] ;
    then
        find_added "$new" "$prev"
        find_removed "$new" "$prev"
        prev=$new
    fi
    sleep 1
done
