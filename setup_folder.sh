#! /bin/bash

sudo losetup -fP ../disk.img
sudo mount /dev/loop26 ~/Documents/Petnica/Project2022/Code/Shared/
