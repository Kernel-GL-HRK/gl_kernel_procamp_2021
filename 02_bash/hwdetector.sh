#!/bin/bash

listI2cdevices(){
    echo "###############I2C devices###############"
    i2cdetect -l
    echo "###############I2C devices###############"
}
listUsbDevices(){
    echo "###############USB devices###############"
    lsusb
    echo "###############USB devices###############"
}


listI2cdevices
listUsbDevices

# detect defvs connections
inotifywait -m  --event CREATE --event DELETE /dev/