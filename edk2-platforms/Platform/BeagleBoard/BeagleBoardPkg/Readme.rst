==============
BeagleBoardPkg
==============

This package provides support for the original OMAP3530-based BeagleBoard
(https://beagleboard.org/beagleboard).

Resources
---------

- BeagleBoard.org Support Page: https://elinux.org/Beagleboard:Main_Page#BeagleBoard
- BeagleBoard Main Page: https://elinux.org/Beagleboard:BeagleBoard
- BeagleBoard Community Page: https://elinux.org/BeagleBoard_Community
- System Reference Manual: https://github.com/beagleboard/BeagleBoard/blob/master/BeagleBoard_revC5_SRM.pdf
- OMAP35x Technical Reference Manual: https://www.ti.com/lit/ug/spruf98y/spruf98y.pdf
- Schematics, BOM etc.: https://github.com/beagleboard/beagleboard

Running
-------

Since the BeagleBoard only has 64KB SRAM and the ROM bootloader can load
a maximum 128KB file, we use the `MLO` loader to load EDK2.
BEAGLEBOARD_EFI.fd must be renamed to `u-boot.bin`.

The programs in the Tools directory are for adding the Configuration Header
that's needed for code to run via the ROM bootloader. They're kept for reference
and are unused.
