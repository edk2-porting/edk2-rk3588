Raspberry Pi 4 Platform
=======================

# Summary

This is a port of 64-bit Tiano Core UEFI firmware for the Raspberry Pi 4B platform.

This is intended to be useful 64-bit [TF-A](https://www.trustedfirmware.org/) +
UEFI implementation for the Raspberry Pi variants based on the BCM2711 SoC,
which should be good enough for most kind of UEFI development, as well running consumer
Operating Systems.

Raspberry Pi is a trademark of the [Raspberry Pi Foundation](https://www.raspberrypi.org).

# Hardware Supported

The RPi4 target supports Pi revisions based on the BCM2711 SoC:
- Raspberry Pi 4B

Please see the RPi3 target for the BCM2837-based variants, such as the Raspberry
Pi 3B.

# Status

This firmware is still in development stage, meaning that it comes with the
following __major__ limitations:

- xHCI USB may only work in pre-OS phase due to nonstandard DMA constraints.
  For 4 GB models running Linux, limiting the RAM to 3 GB should help.
  The 3 GB limitation is currently enabled by default in the user settings.
- Device Tree boot of OSes such as Linux may not work at all.
  For this reason, the provision of a Device Tree is disabled by default in
  the user settings, in order to enforce ACPI boot.
- Serial I/O from the OS may not work due to CPU throttling affecting the
  miniUART baudrate. This can be worked around by using the PL011 UART
  through the device tree overlays.

# Building

Build instructions from the top level edk2-platforms Readme.md apply.

# Booting the firmware

1. Format a uSD card as FAT16 or FAT32
2. Copy the generated `RPI_EFI.fd` firmware onto the partition
3. Download and copy the following files from https://github.com/raspberrypi/firmware/tree/master/boot
  - `bcm2711-rpi-4-b.dtb`
  - `fixup4.dat`
  - `start4.elf`
  - `overlays/miniuart-bt.dbto` or `overlays/disable-bt.dtbo` (Optional)
4. Create a `config.txt` with the following content:
    ```
    arm_64bit=1
    enable_uart=1
    enable_gic=1
    armstub=RPI_EFI.fd
    disable_commandline_tags=2
    device_tree_address=0x1f0000
    device_tree_end=0x200000
    ```
    Additionally, if you want to use PL011 instead of the miniUART, you can add the lines:
    ```
    dtoverlay=miniuart-bt
    ```
    Note: doing so requires `miniuart-bt.dbto` to have been copied into an `overlays/`
    directory on the uSD card. Alternatively, you may use `disable-bt` instead of
    `miniuart-bt` if you don't require Bluetooth.
5. Insert the uSD card and power up the Pi.

# Notes

## ARM Trusted Firmware (TF-A)

The TF-A binary was compiled from the latest TF-A release.
No aleration to the official source has been applied.

For more details on the TF-A compilation, see the relevant
[Readme](https://github.com/tianocore/edk2-non-osi/blob/master/Platform/RaspberryPi/RPi4/TrustedFirmware/Readme.md)
in the `TrustedFirmware/` directory from `edk2-non-osi`.

## Device Tree

By default, UEFI will use the device tree loaded by the VideoCore firmware. This
depends on the model/variant, and relies on the presence on specific files on your boot media.
E.g.:
 - `bcm2711-rpi-4-b.dtb` (for Pi 4B)

You can override the DTB and provide a custom one. Copy the relevant `.dtb` into the root of
the SD or USB, and then edit your `config.txt` so that it looks like:

```
(...)
disable_commandline_tags=2
device_tree_address=0x1f0000
device_tree_end=0x200000
device_tree=your_fdt_file.dtb
```

Note: the address range **must** be `[0x1f0000:0x200000]`. `dtoverlay` and `dtparam` parameters are also supported.

## Custom `bootargs`

This firmware will honor the command line passed by the GPU via `cmdline.txt`.

Note, that the ultimate contents of `/chosen/bootargs` are a combination of several pieces:
- GPU-passed hardware configuration.
- Additional boot options passed via `cmdline.txt`.

# Limitations

## NVRAM

The Raspberry Pi has no NVRAM.

NVRAM is emulated, with the non-volatile store backed by the UEFI image itself. This
means that any changes made in UEFI proper are persisted, but changes made from within
an Operating System aren't.

## RTC

The Rasberry Pi has no RTC.

An `RtcEpochSeconds` NVRAM variable is used to store the boot time.
This should allow you to set whatever date/time you want using the Shell date and
time commands. While in UEFI or HLOS, the time will tick forward.
`RtcEpochSeconds` is not updated on reboots.

## USB

This UEFI supports both the USB3 xHCI ports (front ports), and the Pi 3-style
DesignWare USB2 controller via the Type-C port (host only).

The following only apply to the Type-C port:
- USB1 BBB mass storage devices untested (USB2 and USB3 devices are fine).
- Some USB1 CBI (e.g. UFI floppy) mass storage devices may not work.

## ACPI

OS support for ACPI description of Pi-specific devices is still in development. Not
all functionality may be available.

## Missing Functionality

- Network booting via onboard NIC.
- SPCR hardcodes type to PL011, and thus will not expose correct
  (miniUART) UART if DT overlays to switch UART are used on Pi 4B.

# Configuration Settings
The Raspberry Pi UEFI configuration settings can be viewed and changed using both the UI configuration menu (under `Device Manager` -> `Raspberry Pi Configuration`), as well as the UEFI Shell. To configure using the UEFI Shell, use `setvar` command to read/write the UEFI variables with GUID = `CD7CC258-31DB-22E6-9F22-63B0B8EED6B5`.

The syntax to read a setting is:
```
setvar <NAME> -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5
```

The syntax to write a setting is:
```
setvar <NAME> -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5 -bs -rt -nv =<VALUE>
```

For string-type settings (e.g. Asset Tag), the syntax to write is:
```
setvar <NAME> -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5 -bs -rt -nv =L"<VALUE>" =0x0000
```

UEFI Setting                 |    NAME               |  VALUE
-----------------------------|-----------------------|-----------------------------
**CPU Configuration**        |
CPU Clock                    | `CpuClock` | Low = `0x00000000`<br> Default = `0x00000001` (default)<br> Max = `0x00000002`<br> Custom = `0x00000003`
CPU Clock Rate (MHz)         | `CustomCpuClock` | Hex numeric value, 4-bytes<br> (e.g. `0x000005DC` for 1500 MHz)
**Display Configuration**    |
Virtual 640x480              | `DisplayEnableScaledVModes` | Checked = Bit 1 set (i.e.  `<DisplayEnableScaledVModes> \| 0x02`)
Virtual 800x600              | `DisplayEnableScaledVModes` | Checked = Bit 0 set (i.e.  `<DisplayEnableScaledVModes> \| 0x01`)
Virtual 1024x768             | `DisplayEnableScaledVModes` | Checked = Bit 2 set (i.e.  `<DisplayEnableScaledVModes> \| 0x04`)
Virtual 720p                 | `DisplayEnableScaledVModes` | Checked = Bit 3 set (i.e.  `<DisplayEnableScaledVModes> \| 0x08`)
Virtual 1080p                | `DisplayEnableScaledVModes` | Checked = Bit 4 set (i.e.  `<DisplayEnableScaledVModes> \| 0x10`)
Native resolution            | `DisplayEnableScaledVModes` | Checked = Bit 5 set (i.e.  `<DisplayEnableScaledVModes> \| 0x20`) (default)
Screenshot support           | `DisplayEnableSShot` | Control-Alt-F12 = `0x00000001` (default)<br> Not Enabled = `0x00000000`
**Advanced Configuration**   |
Limit RAM to 3 GB            | `RamLimitTo3GB` | Disable = `0x00000000` <br> Enabled= `0x00000001` (default)
System Table Selection       | `SystemTableMode`| ACPI = `0x00000000` (default)<br> ACPI + Devicetree = `0x00000001` <br> Devicetree = `0x00000002`
Asset Tag                    | `AssetTag` | String, 32 characters or less (e.g. `L"ABCD123"`)<br> (default `L""`)
**SD/MMC Configuration**     |
uSD/eMMC Routing             | `SdIsArasan` | Arasan SDHC = `0x00000001` <br> eMMC2 SDHCI = `0x00000000` (default)
Multi-Block Support          | `MmcDisableMulti` | Multi-block transfers = `0x00000000` (default)<br> Single block transfers = `0x00000001`
uSD Max Bus Width            | `MmcForce1Bit` | 4-bit Mode = `0x00000000`  (default)<br> 1-bit Mode = `0x00000001`
uSD Force Default Speed      | `MmcForceDefaultSpeed` | Allow High Speed = `0x00000000` (default)<br> Force Default Speed = `0x00000001`
SD Default Speed (MHz)       | `MmcSdDefaultSpeedMHz` | Hex numeric value, 4-bytes (e.g. `0x00000019` for 25 MHz)<br>(default 25)
SD High Speed (MHz)          | `MmcSdHighSpeedMHz` | Hex numeric value, 4-bytes (e.g. `0x00000032` for 50 MHz)<br>(default 50)
**Debugging Configuration**  |
JTAG Routing                 | `DebugEnableJTAG` | Enable JTAG via GPIO = `0x00000001`<br> Disable JTAG= `0x00000000` (default)

**Examples:**

- To read the 'System Table Selection' setting :
```
setvar SystemTableMode -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5
```

- To change the 'System Table Selection' setting to 'Devicetree' :
```
setvar SystemTableMode -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5 -bs -rt -nv =0x00000002
```

- To read the 'Limit RAM to 3 GB' setting:
```
setvar RamLimitTo3GB -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5
```

- To change the 'Limit RAM to 3 GB' setting to 'Disabled':
```
setvar RamLimitTo3GB -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5 -bs -rt -nv =0x00000000
```

- To change the Asset Tag to the string "ASSET-TAG-123" :
```
setvar AssetTag -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5 -bs -rt -nv =L"ASSET-TAG-123" =0x0000
```
