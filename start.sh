#!/bin/bash
sudo qemu-system-x86_64 \
	-kernel /home/ivan/Documents/Petnica/Project2022/KernelSourceFiles/obj/linux-86-basic/arch/x86_64/boot/bzImage \
    -initrd /home/ivan/Documents/Petnica/Project2022/KernelSourceFiles/initramfs.cpio.gz \
    -nographic -append "console=ttyS0"
