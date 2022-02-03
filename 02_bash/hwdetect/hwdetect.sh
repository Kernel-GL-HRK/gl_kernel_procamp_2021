#!/bin/bash

#colors
YEL='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YEL}This computer name: ${NC}"
hostname
echo ""

echo -e "${YEL}Current directory: ${NC}"
pwd
echo ""

echo -e "${YEL}Mounted devices: ${NC}"
mount | grep /dev/sd
echo ""

echo -e "${YEL}USB devices: ${NC}"
lsusb
echo ""

echo -e "${YEL}Block devices: ${NC}"
lsblk
echo ""
echo -e "${YEL}SCSI devices: ${NC}"
lsblk -S
echo ""

echo -e "${YEL}Bus devices:${NC}"
sudo lshw -businfo
echo ""

echo -e "${YEL}PCI devices: ${NC}"
lspci
echo ""

echo -e "${YEL}I2C devices: ${NC}"

#part1
sudo lshw | grep -w "\-pci\|pci"

#part2
i2c_num=$(sudo i2cdetect -l | wc -l)		# i2c_num = count number of lines in i2c busses list
((max_index= ${i2c_num} - 1))			# max_index = i2c_num - 1;

echo " - number of i2c busses: $i2c_num"
for i in $(seq 0 ${max_index})
do
	echo " - bus $i:"
	sudo i2cdetect -y ${i}
	echo ""
done
