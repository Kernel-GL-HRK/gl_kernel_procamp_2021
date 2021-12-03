#! /bin/bash

ls /dev | grep ttyUSB > ttyusb.txt
ls /dev | grep sd | grep disk > storage.txt
ls /dev | grep mmc | grep disk > sdcard.txt
ls /dev | grep i2c > i2c.txt

 output () {
	  s1=$(stat -c %s $1)
	  s2=$(stat -c %s $2)

  	  comm -3 $2 $1 > diff.txt


  	  if [ -s diff.txt ]
	then
		if (( $s1 < $s2 ))
		then
			echo ' '
			echo Connected:
			echo '------------------'
			cat diff.txt
		elif (( $s1 > $s2 )); then
			echo ' '
			echo Disconnected:
			echo '------------------'
			cat diff.txt
		fi
	fi
	cp $2 $1
 }


while :
do
	ls /dev | grep ttyUSB > _ttyusb.txt
	ls /dev | grep sd > _storage.txt
	ls /dev | grep mmc > _sdcard.txt
	ls /dev | grep i2c > _i2c.txt

	output ttyusb.txt _ttyusb.txt
	output storage.txt _storage.txt
	output sdcard.txt _sdcard.txt
	output i2c.txt _i2c.txt
	
	sleep 1
done

