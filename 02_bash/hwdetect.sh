#!/bin/bash

echo Hello. This script for detecting to connected devices in PC

cd /home/user

touch detect.txt
touch detect2.txt

chmod +x detect.txt
chmod +x detect2.txt

ls /dev > detect.txt
ls /dev > detect2.txt

while [ a != 2 ]

do

ls /dev > detect2.txt

sleep 2

diff detect.txt detect2.txt

if cmp detect.txt detect2.txt ;
 then

echo "Device disconnection check"

else 
break

fi

done

echo "An external device has been disconnected from the PC";




