Qemu SBSA TF-A binaries
-----------------------

These binaries have been created from the mainline TF-A
code checked out at the following commit ID:

commit 99c447f4406c8ae66b6cab2341ef0c8e8d9751d2
Merge: 331723509 8e570b71d
Author: André Przywara <andre.przywara@arm.com>
Date:   Tue Jul 7 22:06:31 2020 +0000

    Merge "drivers: arm: gicv3: auto-detect presence of GIC600-AE" into integration


This ensures that the following fix for qemu_sbsa platform is
merged upstream and is included in the build:

commit d7f5be8ec2fc5254fc581af52156b5fde5deb822
Author: Masahisa Kojima <masahisa.kojima@linaro.org>
Date:   Tue May 19 19:49:36 2020 +0900

    qemu/qemu_sbsa: increase size to handle fdt


NOTE: No modifications to the source code have been done.
      The binaries have been generated for the AARCH64 platform
      with the following command:

	make PLAT=qemu_sbsa all fip
