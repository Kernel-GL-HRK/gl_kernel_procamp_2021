#!/bin/sh
export ARCH=arm
export CROSS_COMPILE='arm-buildroot-linux-uclibcgnueabihf-'
export PATH=$PATH:~/Development/Embedded/exercise8/build_orange_image/host/bin
export BUILD_KERNEL=/home/valenti/Development/Embedded/exercise8/build_orange_image/build/linux-5.10.10
export CURDIR=$(pwd)