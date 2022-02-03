#!/bin/bash

echo -e "Mounted devices: "
ls /dev/sd*
echo ""

echo -e "USB devices: "
lsusb
echo ""

echo -e "USB to TTL adapters: "
ls /dev | grep ttyUSB
echo ""

echo -e "SD cards: "
lspci | grep -i SD
echo ""

echo -e "I2C devices: "
sudo i2cdetect -l
echo ""
