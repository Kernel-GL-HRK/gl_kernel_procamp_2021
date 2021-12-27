#!/bin/bash
set et ts=4 sts=4 sw=4

CURRENT=$(uname -r)
#BUILD_KERNEL=/lib/modules/$CURRENT/build
BUILD_KERNEL=~/projects/LinuxKernelCoursePro/kernel/buildroot-2021.02.7/output/build/linux-5.10.10/
$BUILD_KERNEL/scripts/checkpatch.pl -f --no-tree ./test_mem.c #--fix
