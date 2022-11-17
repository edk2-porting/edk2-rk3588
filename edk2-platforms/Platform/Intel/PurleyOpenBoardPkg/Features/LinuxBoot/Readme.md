
linux.efi/initrd.cpio.xz are dummy files. 
These dummy files needs to be replaced by building the Linux Kernel.

1.	Follow directions on http://osresearch.net/Building/ to compile the heads kernel and initrd for qemu-system_x86_64 
2.	Copy the following built files 
(1) initrd.cpio.xz  to LinuxBoot/LinuxBinaries/initrd.cpio.xz 
(2) bzimage to LinuxBoot/LinuxBinaries/linux.efi

