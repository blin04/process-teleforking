#!/bin/bash

sudo qemu-system-x86_64 \
	-kernel /home/ivan/Documents/Petnica/Project2022/KernelSourceFiles/obj/linux-x86-custom/arch/x86_64/boot/bzImage \
    -initrd /home/ivan/Documents/Petnica/Project2022/KernelSourceFiles/minimal-initramfs.cpio.gz \
	-hdb /dev/loop26 \
    -nographic -append "console=ttyS0"
