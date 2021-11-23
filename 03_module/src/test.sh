#/bin/bash
sudo dmesg -C
make remove
make clean
make
make test
dmesg -w
make remove