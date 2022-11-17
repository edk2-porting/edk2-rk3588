Raspberry Pi 3 Platform
=======================

# Summary

This is a port of 64-bit Tiano Core UEFI firmware for the Raspberry Pi 3B (and related)
platforms, based on [Ard Biesheuvel's 64-bit](http://www.workofard.com/2017/02/uefi-on-the-pi/)
and [Microsoft's 32-bit](https://github.com/ms-iot/RPi-UEFI/tree/ms-iot/Pi3BoardPkg)
implementations, as maintained by [Andrei Warkentin](https://github.com/andreiw/RaspberryPiPkg).

This is meant as a generally useful 64-bit ATF + UEFI implementation for Raspberry Pi
variants based on the BCM2837 SoC, which should be good enough for most kind of UEFI development,
as well as for running consummer Operating Systems in such as Linux, Windows or the BSDs.

Raspberry Pi is a trademark of the [Raspberry Pi Foundation](http://www.raspberrypi.org).

# Hardware Supported

The RPi3 target supports Pi revisions based on the BCM2837 SoC:
- Raspberry Pi 2B v1.2 (older versions are *not* compatible)
- Raspberry Pi 3A+
- Raspberry Pi 3B
- Raspberry Pi 3B+
- Raspberry Pi CM3

Note: a CM3L, lacking eMMC and thus similar to the 3B, will probably work as well, but just
has not been tested.

Please see the RPi4 target for BCM2711-based variants, such as the Raspberry Pi 4B.

# Status

This firmware, that has been validated to compile against the current
[edk2](https://github.com/tianocore/edk2)/[edk2-platforms](https://github.com/tianocore/edk2-platforms),
should be able to boot Linux (Debian, Ubuntu, SUSE), NetBSD, FreeBSD as well as Windows
10 ARM64 (full GUI version).

It also provides support for ATF ([Arm Trusted Platform](https://github.com/ARM-software/arm-trusted-firmware)).

HDMI and the mini-UART serial port can be used for output devices, with mirrored output.
USB keyboards and the mini-UART serial port can be used as input.

The default for the firmware is to first attempt boot from SD then USB. The UEFI
Shell can also be accessed by pressing F1. To change the boot order you can edit
the preferences in the _Boot Maintenance Manager_ menu.

For additional information about the tested systems and how to set them up,
please see [Systems.md](./Systems.md).

# Building

Build instructions from the top level edk2-platforms Readme.md apply.

# Booting the firmware

1. Format a uSD card as FAT32
2. Copy the generated `RPI_EFI.fd` firmware onto the partition
3. Download and copy the following files from https://github.com/raspberrypi/firmware/tree/master/boot
  - `bcm2710-rpi-3-b.dtb` (for Pi 3B)
  - `bcm2710-rpi-3-b-plus.dtb` (for Pi 3B+)
  - `bcm2710-rpi-2-b.dtb` (for Pi 2B v1.2)
  - `bcm2710-rpi-cm3.dtb` (for Pi CM3)
  - `bootcode.bin`
  - `fixup.dat`
  - `start.elf`
  - `overlays/miniuart-bt.dtbo` or `overlays/disable-bt.dtbo` (Optional)
4. Create a `config.txt` with the following content:
  ```
  arm_control=0x200
  enable_uart=1
  armstub=RPI_EFI.fd
  disable_commandline_tags=2
  device_tree_address=0x1f0000
  device_tree_end=0x200000
  ```
  If you want to use PL011 instead of the miniUART on your Pi 3B/3B+, you can add the lines:
  ```
  dtoverlay=miniuart-bt
  ```
  Note: doing so requires `miniuart-bt.dbto` to have been copied into an `overlays/`
  directory on the uSD card. Alternatively, you may use `disable-bt` instead of
 `miniuart-bt` if you don't require Bluetooth.
5. Insert the uSD card and power up the Pi.

Note that if you have a model 3+ or a model 3 where you enabled USB boot through OTP
(see [here](https://www.raspberrypi.org/documentation/hardware/raspberrypi/bootmodes/msd.md))
you may also be able to boot from a FAT32 USB driver rather than uSD.

# Notes

## ARM Trusted Firmware (TF-A)

The TF-A binary files were compiled from the latest TF-A release.
No aleration to the official source has been applied.

For more details on the TF-A compilation, see the relevant
[Readme](https://github.com/tianocore/edk2-non-osi/blob/master/Platform/RaspberryPi/RPi3/TrustedFirmware/Readme.md)
in the `TrustedFirmware/` directory from `edk2-non-osi`.

## Custom Device Tree

By default, UEFI will use the device tree loaded by the VideoCore firmware. This
depends on the model/variant, and relies on the presence on specific files on your boot media.
E.g.:
 - `bcm2710-rpi-3-b.dtb` (for Pi 3B)
 - `bcm2710-rpi-3-b-plus.dtb` (for Pi 3B+)
 - `bcm2710-rpi-2-b.dtb` (for Pi 2B v1.2)
 - `bcm2710-rpi-cm3.dtb` (for Pi CM3)

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

## HDMI

The UEFI HDMI video support relies on the VC (that's the GPU)
firmware to correctly detect and configure the attached screen.
Some screens are slow, and this detection may not occur fast
enough. Finally, you may wish to be able to boot your Pi
headless, yet be able to attach a display to it later for
debugging.

To accommodate these issues, the following extra lines
are recommended for your `config.txt`:
- `hdmi_force_hotplug=1` to allow plugging in video after system is booted.
- `hdmi_group=1` and `hdmi_mode=4` to force a specific mode, both to accommodate
   late-plugged screens or buggy/slow screens. See [official documentation](https://www.raspberrypi.org/documentation/configuration/config-txt/video.md)
   to make sense of these parameters (example above sets up 720p 60Hz).

## NVRAM

The Raspberry Pi has no NVRAM.

NVRAM is emulated, with the non-volatile store backed by the UEFI image itself. This means
that any changes made in UEFI proper are persisted, but changes made from a High Level
Operating System (HLOS) aren't.

## RTC

The Rasberry Pi has no RTC.

An `RtcEpochSeconds` NVRAM variable is used to store the boot time.
This should allow you to set whatever date/time you want using the Shell date and
time commands. While in UEFI or HLOS, the time will tick forward.
`RtcEpochSeconds` is not updated on reboots.

## uSD (and eMMC for CM3)

UEFI supports both the Arasan SDHCI and the Broadcom SDHost controllers to access the flash
media (eMMC on CM3, and uSD slot on everything else). You can use either. The other controller
gets routed to the SDIO WiFi card (N/A to models without WiFi). The choice made will
impact ACPI OSes booted (e.g. Windows 10). Arasan, being an SDIO controller, is usually used
with the WiFi adapter (where available). SDHost cannot be used with SDIO. In UEFI setup screen:
- go to `Device Manager`
- go to `Raspberry Pi Configuration`
- go to `SD/MMC Configuration`
- configure `uSD/eMMC Routing`

Known issues:
- Arasan HS/4bit support is missing.
- No 8 bit mode support for (e)MMC (slow UEFI file I/O on CM3).
- Hacky (e)MMC support (no HS).
- No card removal/replacement detection, tons of timeouts and slow down during boot without an uSD card present.

## USB

- USB1 BBB mass storage devices untested (USB2 and USB3 devices are fine).
- Some USB1 CBI (e.g. UFI floppy) mass storage devices may not work.

## ACPI

Both Arasan and SDHost SD controllers are exposed.

Note that the ACPI tables were derived or copied from the MS-IoT one. This means that they
are not truly ACPI compliant, especially when it comes to their descriptors, and therefore
not suitable for Linux environments. If you want to use a Linux HLOS, you are encouraged to
install a kernel that relies on Device Tree rather than ACPI.

## Missing Functionality

- Network booting via onboard NIC.
- SPCR hardcodes UART type to miniUART, and thus will not expose correct
  (PL011) UART on CM3 and Pi2B or if DT overlays to switch UART are used
  on Pi 3B/3B+/3A+.

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
System Table Selection       | `SystemTableMode`| ACPI = `0x00000000` <br> ACPI + Devicetree = `0x00000001` (default)<br> Devicetree = `0x00000002`
Asset Tag                    | `AssetTag` | String, 32 characters or less (e.g. `L"ABCD123"`)<br> (default `L""`)
**SD/MMC Configuration**     |
uSD/eMMC Routing             | `SdIsArasan` | Arasan SDHC = `0x00000001` <br> Broadcom SDHOST = `0x00000000` (default)
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

- To change the Asset Tag to the string "ASSET-TAG-123" :
```
setvar AssetTag -guid CD7CC258-31DB-22E6-9F22-63B0B8EED6B5 -bs -rt -nv =L"ASSET-TAG-123" =0x0000
```
