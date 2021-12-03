#!/bin/sh

USB_UART="ttyUSB"
MMCBLK="mmcblk"
I2C="i2c"
SD="sd"

check_device()
{
    printf "\n$1 :{\n" >> devices.txt
    ls /dev/ | grep $1 >> devices.txt
    printf "\n}">> devices.txt
}


touch devices.txt
touch tmp_devices.txt

while [ 1 ]
do
    rm devices.txt
    touch devices.txt
    check_device $USB_UART
    check_device $MMCBLK
    check_device $I2C
    check_device $SD
    CHK_DVC=$(diff tmp_devices.txt devices.txt)

    if [ "$CHK_DVC" != "" ]
    then
        cp devices.txt tmp_devices.txt
	cat devices.txt
    fi

    sleep 1
done