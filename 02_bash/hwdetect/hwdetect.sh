#!/bin/bash

function echoDetected() {
    if [ -s $2 ]
    then
        echo $1:
        cat $2
    else
        echo No $1 detected.
    fi
}

function echoDiff {
    ls /dev | grep $2 > temp_diff.txt
    local tmp_diff=$(diff temp_diff.txt $1)
    if [ -n "$tmp_diff" ]
    then
        echo $tmp_diff | cut -f 2- -d " "
        cp temp_diff.txt $1
        #ls /dev | grep $2 > $1
    else 
        sleep 0.25
    fi
}

ls /dev | grep sd > storage_devs.txt
echoDetected "Storage devices" storage_devs.txt 

ls /dev | grep tty[UA] > tty_devs.txt
echoDetected "USB->TTL convertors" tty_devs.txt 

ls /dev | grep i2c > i2c_devs.txt
echoDetected "I2C devices" i2c_devs.txt 

ls /dev | grep mmcblk > sd_cards.txt
echoDetected "SD cards" sd_cards.txt 

echo "Scaning for changes:"

while [ 1 ]
do

    echoDiff storage_devs.txt sd
    echoDiff tty_devs.txt tty[UA]
    echoDiff i2c_devs.txt i2c
    echoDiff sd_cards.txt mmcblk

done
