#!/bin/bash 
#-x

D_USB=""
D_SD=""
D_I2C=""

list_devs ()
{
    DEVS=`ls /dev/*`
}


detect_usb ()
{
    DUSB=""
    echo =-usb-=
    for val in $DEVS
    do
      if [[ $val == *USB* ]]; then
          echo - $val
          DUSB="$DUSB $val"
      fi
    done
    if [ ! -z $DUSB ]; then
	echo Finded usb devices: $DUSB
    fi
    echo ====
}


detect_sd ()
{
    #/dev/sd* or /dev/mmcblk

    DSD=""
    echo =-sd-=
    for val in $DSD
    do
      if [[ $val == *sd* ]]; then
          echo - $val
          DSD="$DSD $val"
      fi
    done
    if [ ! -z $DSD ]; then
	echo Finded sd devices: $DSD
    fi
    echo ====
}

detect_i2c ()
{
    echo i2c
}


list_devs

detect_usb
detect_sd
detect_i2c

