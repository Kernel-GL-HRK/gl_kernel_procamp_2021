#!/bin/bash

arr1=( $(ls /dev) )
arr2=


while [ 0 ]
do

    arr2=( $(ls /dev) )

    for t in ${arr2[@]}; do
        if [[ "${arr1[@]}" =~ "$t" ]]; then
            continue
        else
            echo "New dev = $t"
        fi
    done
    arr1=${arr2[@]}
    sleep 1
done

