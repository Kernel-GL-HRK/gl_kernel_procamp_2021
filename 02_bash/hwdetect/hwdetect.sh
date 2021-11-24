#!/bin/bash

find /dev  2>/dev/null > dev.dump
lsusb  2>/dev/null > usb.dump

function cleanup {
  rm  usb.dump
  rm dev.dump
}

trap cleanup EXIT

while :
do
    find /dev  2>/dev/null > dev2.dump
    diff --old-line-format='' --unchanged-line-format='' dev.dump dev2.dump 
    rm  dev.dump
    mv dev2.dump dev.dump

    lsusb  2>/dev/null > usb2.dump
    diff --old-line-format='' --unchanged-line-format='' usb.dump usb2.dump 
    rm  usb.dump
    mv usb2.dump usb.dump 
sleep 1
done
