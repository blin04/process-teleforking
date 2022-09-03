#!/bin/bash

sudo losetup -fP ../disk.img
sudo mount /dev/loop26 ~/Documents/Petnica/Project2022/Code/Shared/

sudo qemu-system-x86_64 \
	-kernel /home/ivan/Documents/Petnica/Project2022/KernelSourceFiles/obj/linux-86-basic/arch/x86_64/boot/bzImage \
    -initrd /home/ivan/Documents/Petnica/Project2022/KernelSourceFiles/initramfs3.cpio.gz \
	-hdb /dev/loop26 \
    -nographic -append "console=ttyS0"
