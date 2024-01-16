# EDK2 UEFI firmware for Rockchip RK3588 platforms
This repository contains an UEFI firmware implementation based on EDK2 for various RK3588 boards.

## Supported platforms
- [Radxa ROCK 5B](https://wiki.radxa.com/Rock5/hardware/5b)
- [Radxa ROCK 5A](https://wiki.radxa.com/Rock5/hardware/5a)
- [Orange Pi 5](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-5.html)
- [Orange Pi 5 Plus](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-5-plus.html)
- [ameriDroid Indiedroid Nova](https://indiedroid.us)
- [Firefly ROC-RK3588S-PC](https://en.t-firefly.com/product/industry/rocrk3588spc)
- [StationPC Station M3](https://www.stationpc.com/product/stationm3)
- [Mekotronics R58X](https://www.mekotronics.com/h-pd-75.html)
- [Mekotronics R58 Mini](https://www.mekotronics.com/h-pd-76.html)
- [Khadas Edge2](https://www.khadas.com/edge2)
- [Mixtile Blade 3](https://www.mixtile.com/blade-3)
- [FriendlyELEC NanoPC T6](https://wiki.friendlyelec.com/wiki/index.php/NanoPC-T6)
- [FriendlyELEC NanoPi R6C](https://wiki.friendlyelec.com/wiki/index.php/NanoPi_R6C)
- [FriendlyELEC NanoPi R6S](https://wiki.friendlyelec.com/wiki/index.php/NanoPi_R6S)
- [Hinlink H88K](http://www.hinlink.com)
- [YourLand Notebook](https://nanocode.cn/#/yl/index)

## Supported peripherals
Applicable to all platforms unless otherwise noted.

| Device | Status | Notes |
| --- | --- | --- |
| USB 3 / 2.0 / 1.1                  | 🟢 Working     | Host-mode only, USB 3 devices connected to a Type-C port only work in one orientation. |
| PCIe 3.0 (RK3588)                  | 🟢 Working     | No bifurcation support |
| PCIe 2.1                           | 🟢 Working     | |
| SATA                               | 🟢 Working     | |
| SD/eMMC                            | 🟢 Working     | |
| HDMI output                        | 🟡 Partial     | Single display with mode limited at 1080p 60 Hz |
| DisplayPort output (USB-C)         | 🟡 Partial     | Mode fixed at 1080p 60 Hz, only works in one orientation of the Type-C port. |
| eDP output                         | 🟡 Partial     | Disabled, requires manual configuration depending on the platform and panel. |
| DSI output                         | 🔴 Not working | |
| GMAC Ethernet                      | 🔴 Not working | Only brought-up for OS usage |
| Realtek PCIe Ethernet              | 🟢 Working     | Some platforms don't have MAC addresses set, networking may not work in that case. |
| UART                               | 🟢 Working     | UART2 console available at 1500000 baud rate |
| GPIO                               | 🟡 Partial     | Only read, write and alt function supported |
| I2C                                | 🟢 Working     | |
| SPI                                | 🟢 Working     | |
| PWM                                | 🟢 Working     | |
| SPI NOR Flash                      | 🟢 Working     | |
| HYM8563 real-time clock            | 🟢 Working     | |
| Cooling fan                        | 🟢 Working     | Supported on most platforms. Fan connector where present, otherwise available at the GPIO header for 3-pin PWM fans (do *not* connect 2-pin fans there!):<br>* Orange Pi 5: `GPIO4_B2`<br>* Indiedroid Nova: `GPIO4_B4` |
| Status LED                         | 🟢 Working     | |
| Voltage regulators (RK806, RK860)  | 🟢 Working     | |
| FUSB302 USB Type-C Controller      | 🔴 Not working | Required for PD negotiation and connector orientation switching |

## Supported OSes
### In ACPI mode
| OS | Version | Tested/supported hardware | Notes |
| --- | --- | --- | --- |
| Windows | 10 (1904x), 11 | [Status](https://github.com/worproject/Rockchip-Windows-Drivers#hardware-support-status) ||
| NetBSD | 10 | Display, UART, USB, PCIe (incl. NVME), SATA, eMMC, GMAC Ethernet ||
| VMware ESXi Arm Fling | >= 1.12 | Display, USB | * PCIe devices will hang at boot, need to disable in settings or leave the ports empty.<br>* GMAC Ethernet gets detected but does not work. |
| Linux | tested Ubuntu 22.04, kernel 5.15.0-75-generic | Display, UART, USB, PCIe (incl. NVME & Ethernet), SATA | For full hardware functionality, use a kernel with RK3588 support and switch to Device Tree mode. |

#### Additional limitations when using ACPI
* Devices behind PCIe switches do not work (e.g. the two NICs on Mixtile Blade 3).
* GMAC is limited to Gigabit speed (i.e. no 10/100).

### In Device Tree mode
| OS | Version | Tested/supported hardware | Notes |
| --- | --- | --- | --- |
| Rockchip SDK Linux | 5.10 legacy, tested with [Armbian rk3588-live-iso](https://github.com/amazingfate/rk3588-live-iso) | Platform-dependent, most peripherals work. | If using a different kernel, see [Device Tree configuration](#device-tree-configuration). |

## Getting started
### 1. Requirements
* One of the [supported devices](#supported-platforms).
* Either SPI NOR flash (included with some devices), SD card or eMMC to store the firmware on.
* Quality power supply that can provide at least 15 W. Depending on the peripherals you use, more may be needed.

  Note: on Mixtile Blade 3, a fixed voltage *higher than* 5V must be supplied. The board cannot power any external peripherals if the input voltage is just 5V. USB-PD negotiation is not supported by firmware.
* HDMI or DisplayPort (USB-C) screen capable of at least 1080p 60Hz.
* Optionally, if display is not available or for debugging purposes, an UART adapter capable of 1500000 baud rate (e.g. USB CH340, CP2104).

### 2. Download the firmware image
The latest version can be obtained from <https://github.com/edk2-porting/edk2-rk3588/releases>.

If your platform is not yet supported, using an image meant for another device is **not** recommended. Although they are generally similar, voltage setup can happen to be different and you may risk damaging the board. External peripherals are unlikely to work either.

### 3. Flash the firmware
UEFI can be flashed to either a SPI NOR flash, SD card or eMMC module:
* For removable SD or eMMC (easiest), you can simply use balenaEtcher, RPi Imager or dd.
* For SPI NOR or soldered eMMC, instructions can be found at: <https://wiki.radxa.com/Rock5/install/spi>.

  In short, you can flash the image from Linux booted on the device or by using RKDevTool on another computer. The latter requires entering Maskrom mode on the device. The way to do this slightly varies across platforms, refer to your vendor documentation.

**Warning:** these operations will erase data on the storage device. Make a backup first!

If you wish to have both UEFI and an OS on the same SD or eMMC device: flash UEFI first, then create any additional partitions without touching the first, reserved one. Steps for updating the firmware in this case can be found [here](#updating-the-firmware).

Note: Using SPI NOR (if present) is recommeded, as it leaves the other storage options free for other purposes. Additionally, SD/eMMC will limit the firmware's ability to access its own storage (variable store) when an OS is running. This feature is mostly used by OS installers to create the boot menu options, it is not mandatory.

### 4. Connect peripherals and power on the device
If the flashing process has been done correctly, you should see the status LED blinking (if present), and shortly after, the platform's boot logo with a progress bar at the bottom on the connected display.

At this stage, you can press <kbd>Esc</kbd> to enter the firmware setup, <kbd>F1</kbd> to launch the UEFI Shell, or, provided you also have an UEFI bootloader/app on a storage device, you can let the system automatically run that, which is the default behavior if no action is taken.

Check the [Supported peripherals](#supported-peripherals) and [Supported OSes](#supported-oses-with-acpi) sections to see what's currently possible with this firmware.

Also check the configuration options described below, some of which may need to be changed depending on the OS used.

If you experience any issues, please see the [Troubleshooting](#troubleshooting) section.

## Configuration settings
The UEFI provides a few configuration options, like CPU frequency, PCIe/SATA selection for an M.2 port, fan control, etc. These can be viewed and changed using both the UI configuration menu (under `Device Manager` -> `Rockchip Platform Configuration`), as well as the UEFI Shell.

Configuration through the user interface is fairly straightforward and help information is provided on the right side of the menu.

Configuration through the UEFI shell is more advanced and mostly useful for scripts. See [Setting configuration options via the shell](#setting-configuration-options-via-the-shell).

### Tips
* CPU clocks are set to 816 MHz (boot default) on platforms without a cooling fan included. If you have adequate cooling, go to the configuration menu -> `CPU Performance` and set all Cluster Presets to `Maximum`.

* If you only wish to boot non-Windows OSes in ACPI mode, go to the configuration menu -> `ACPI / Device Tree` and set `USB 2.0 Support` to `Enabled`, in order to get maximum speed from USB 2.0 ports.

  Booting Windows with this option enabled will cause it to crash.

### Device Tree configuration
For rich Linux support, it is recommended to enable Device Tree mode. You can do so by going to the configuration menu -> `ACPI / Device Tree` and setting `Config Table Mode` to `Device Tree`.

By default, the firmware installs a [DTB compatible with (most) Rockchip SDK Linux 5.10 legacy kernel variants](https://github.com/edk2-porting/edk2-rk3588/tree/master/edk2-rockchip-non-osi/Platform/Rockchip/DeviceTree).

#### Custom Device Tree Blob (DTB) override and overlays
It is also possible to provide a custom DTB and overlays. To enable this, go to the configuration menu -> `ACPI / Device Tree` and set `Support DTB override & overlays` to `Enabled`.

The firmware will now look for overrides in the partition of a selected boot option / OS loader. In most cases this will be the first FAT32 EFI System Partition.

* The base DTB must be located at `\dtb\base\<PLATFORM-DT-NAME>.dtb`.

* The overlays can be placed in:
  * `\dtb\overlays` - will be applied first, regardless of the platform.
  * `\dtb\overlays\<PLATFORM-DT-NAME>` - will be applied only to the specified platform.

  and must have the `.dtbo` extension.

The paths above are relative to the root of the file system. That is, the `dtb` directory must not be placed in a sub-directory.

`<PLATFORM-DT-NAME>` can be:
| Name                                    | Platform                      |
| --------------------------------------- | ----------------------------- |
| `rk3588s-9tripod-linux`                 | Indiedroid Nova               |
| `roc-rk3588s-pc`                        | ROC-RK3588S-PC / Station M3   |
| `rk3588-nanopc-t6`                      | NanoPC T6                     |
| `rk3588s-nanopi-r6c`                    | NanoPi R6C                    |
| `rk3588s-nanopi-r6s`                    | NanoPi R6S                    |
| `rk3588-hinlink-h88k`                   | H88K                          |
| `rk3588s-khadas-edge2`                  | Edge2                         |
| `rk3588-blueberry-minipc-linux`         | R58 Mini                      |
| `rk3588-blueberry-edge-v12-linux`       | R58X (v1.2)                   |
| `rk3588-blade3-v101-linux`              | Blade 3                       |
| `rk3588s-orangepi-5`                    | Orange Pi 5                   |
| `rk3588-orangepi-5-plus`                | Orange Pi 5 Plus              |
| `rk3588s-rock-5a`                       | ROCK 5A                       |
| `rk3588-rock-5b`                        | ROCK 5B                       |
| `yourland`                              | GEDU YourLand Notebook        |

In the absence of a custom base DTB override, the overlays are applied on top of the firmware-provided DTB.

The firmware applies some fix-ups to its own DTB depending on the user settings (e.g. PCIe/SATA/USB selection, making SATA overlays redundant). These fix-ups are not applied to a custom base DTB - overlays must be used instead.

If the application of an overlay fails (e.g. due to it being invalid in regard to the base DTB), all overlays are discarded, including those that got applied up to that point.

If the custom base DTB is invalid, the firmware-provided one will be passed to the OS instead.

This entire process is logged to the [serial console](#advanced-troubleshooting). There's currently no other way to see potential errors.

## Updating the firmware
If the storage is only used for UEFI and nothing else, simply download the latest image and flash it as described in the [Getting started](#getting-started) section.

If it is also used by an OS and has additional partitions, only part of the image needs to be applied. This can be done with the `dd` tool:
```bash
dd if=FIRMWARE.img of=DESTINATION bs=512 skip=64 seek=64 conv=notrunc
```

`FIRMWARE.img` is the firmware image for your platform. E.g. `edge2_UEFI_Release_v0.8.img`.

`DESTINATION` is the destination storage that you wish to update the firmware on. E.g. `/dev/sdb`.

Here we skip the GPT and copy the firmware starting at offset 0x8000 (`64` blocks * `512` bytes block size) until its end. See [Flash layout](#flash-layout) for more details.

## Troubleshooting
First of all, make sure your device can only possibly load the UEFI firmware and nothing else. U-Boot must not present on either SPI NOR, SD or eMMC, otherwise it could take precedence.

Below you can find some basic debugging information. If none of this helps, please see the [Advanced troubleshooting](#advanced-troubleshooting) section.

### Meaning of the Status LED
If your device has an activity LED, the firmware will blink it in different patterns to indicate the current system status.

1. Immediately after power on, the LED should start pulsing quickly. This indicates that the firmware is initializing.

2. After initialization (usually takes less than 5 seconds), the LED will switch to a short pulsing every 2 seconds or so. This indicates that the firmware is ready and waiting for user action or the countdown to boot automatically. The display output should also be enabled at this point.

3. When the firmware boots an OS and is about to exit, the LED will stop blinking.

If the LED:
* does not light up after power on, this means the firmware has not managed to load up at all.
* gets stuck in either on or off state after blinking a few times and never recovers, something went wrong and the firmware has crashed or frozen.

  Note that it is only expected to stop as described at point 3) above.

### Common issues
#### Nothing shows up on the screen
Make sure you've flashed the firmware correctly and that it is the version designed for your device. In most cases this is the culprit.

Assuming the firmware loads fine:
* The display must support a resolution of at least 1080p at 60 Hz.
* If you're using HDMI and the system has two ports, only one will work. Try both.
* If you're using USB-C to DisplayPort, only one orientation of the USB-C connector will work. Check both.

If you are not able to get any display output, the only way to interact with UEFI is via the [serial console](#advanced-troubleshooting).

#### USB 3 devices do not work
* Try a different port.
* If you're using USB-C, 3.0 devices will only work in one orientation of the connector. Check both.
* Make sure the power supply and cable are good

#### Networking does not work
* Only Realtek PCIe and USB controllers are supported. Native Gigabit provided by RK3588 isn't.

* Some boards with Realtek NICs do not have a MAC address set at factory and will show-up as being all zeros in UEFI, possibly preventing the adapter from obtaining an IP address.

  You can easily fix this by writing the MAC address manually:

  1. Boot into Linux and open up a terminal. The commands below apply to Armbian with legacy kernel.

  2. Install the headers for your kernel version:
      ```bash
      sudo apt install -y linux-headers-legacy-rk35xx
      ```

  3. Clone Realtek PGTool and build the driver:
      ```bash
      git clone https://github.com/redchenjs/rtnicpg
      cd rtnicpg
      make
      ```

  4. Unload all Realtek modules and load the driver built above:
      ```bash
      sudo rmmod pgdrv
      sudo ./pgload.sh
      ```
      Note: make sure there aren't any remaining Realtek modules loaded after this, except for the new `pgdrv`.<br> If you have `r8125` built-in, you might have to reboot with `initcall_blacklist=rtl8125_init_module` as a kernel parameter (in Grub).

   5. Burn a MAC address into the eFuses:

      For only one NIC:
      ```bash
      sudo ./rtnicpg-aarch64-linux-gnu /efuse /nodeid 00E04C001234
      ```
      For two or more:
      ```bash
      sudo ./rtnicpg-aarch64-linux-gnu /efuse /# 1 /nodeid 00E04C001234
      sudo ./rtnicpg-aarch64-linux-gnu /efuse /# 2 /nodeid 00E04C001235
      ```
      `00E04C001234` is an example address. You can generate random and unique ones using: <https://www.macvendorlookup.com/mac-address-generator>

  **Note:** the number of eFuses is limited, thus MAC addresses can only be changed a few times.

### Advanced troubleshooting
The firmware will log detailed information to the serial console when using a debug version. See the [release notes](https://github.com/edk2-porting/edk2-rk3588/releases) for details on how to obtain this version.

1. The debug image needs to be flashed in place of the existing one.

2. Connect the **UART2** RX, TX and GND pins on your device (check vendor documentation) to the UART adapter on your other computer.

3. Open up a serial terminal (`PuTTY` on Windows, `stty` on Linux) set to 1500000 baud rate and 8n1 (default).

4. Power on the device.

You should be able to see a lot of debug messages being printed. If that's not the case, double check the connections (swap RX/TX), make sure the adapter is functional and configured correctly.

The logs should give an insight of what's going on. If you need help analyzing them, feel free to open an issue ticket.

## Reporting issues
You can open issues related to UEFI at <https://github.com/edk2-porting/edk2-rk3588/issues>.

Please include as many details as possible: expected behavior, what actually happens, steps to reproduce, [serial logs](#advanced-troubleshooting), etc.

Also check the existing issues in case yours might be already reported.

## Building
The firmware can only be built on Linux currently. For Windows use WSL.

1. Install required packages:

   For Ubuntu/Debian:
   ```bash
   sudo apt install git gcc g++ build-essential gcc-aarch64-linux-gnu iasl python3-pyelftools uuid-dev
   ```
   For Arch Linux:
   ```bash
   sudo pacman -Syu
   sudo pacman -S git base-devel gcc dtc aarch64-linux-gnu-binutils aarch64-linux-gnu-gcc aarch64-linux-gnu-glibc python python-pyelftools iasl --needed
   ```

2. Clone the repository:
   ```bash
   git clone https://github.com/edk2-porting/edk2-rk3588.git --recursive
   cd edk2-rk3588
   ```

3. Build UEFI (ROCK 5B for example, check [list of platform configs](https://github.com/edk2-porting/edk2-rk3588/tree/master/configs)):
   ```bash
   ./build.sh --device rock-5b --release Release # (or Debug)
   ```

If you get build errors, it is very likely that you're still missing some dependencies. The list of packages above is not complete and depending on the distro you may need to install additional ones. In most cases, looking up the error messages on the internet will point you at the right packages.

## Notes

### Flash layout
| Address    | Size       | Desc                  | File                   |
| ---------- | ---------- | --------------------- | ---------------------- |
| 0x00000000 | 0x00004400 | GPT Table             | rk3588_spi_nor_gpt.img |
| 0x00008000 |            | IDBlock               | idblock.bin            |
| 0x00100000 | 0x00500000 | BL33_AP_UEFI FV       | ${DEVICE}_EFI.itb      |
| 0x007C0000 | 0x00010000 | NV_VARIABLE_STORE     |                        |
| 0x007D0000 | 0x00010000 | NV_FTW_WORKING        |                        |
| 0x007E0000 | 0x00010000 | NV_FTW_SPARE          |                        |

The variable store is not included in the flash image, in order to prevent overwriting it and to maintain the user settings across updates.

The firmware expects these exact offsets, do not change them.

### Memory Map
| Address    | Size       | Desc                  | File                |
| ---------- | ---------  | --------------------- | ------------------- |
| 0x00040000 |            | ATF                   | bl31_0x00040000.bin |
| 0x000f0000 |            | ATF                   | bl31_0x000f0000.bin |
| 0x00200000 | 0x00500000 | UEFI FV               | BL33_AP_UEFI.Fv     |
| 0x007C0000 | 0x00010000 | NV_VARIABLE_STORE     |                     |
| 0x007D0000 | 0x00010000 | NV_FTW_WORKING        |                     |
| 0x007E0000 | 0x00010000 | NV_FTW_SPARE          |                     |
| 0x08400000 |            | OP-TEE                | bl32.bin            |
| 0xff100000 |            | ATF (PMU_MEM)         | bl31_0xff100000.bin |

### Setting configuration options via the shell
To configure using the UEFI Shell, use `setvar` command to read/write the UEFI variables with GUID = `10f41c33-a468-42cd-85ee-7043213f73a3`.

The syntax to read a setting is:
```
setvar <NAME> -guid 10f41c33-a468-42cd-85ee-7043213f73a3
```

The syntax to write a setting is:
```
setvar <NAME> -guid 10f41c33-a468-42cd-85ee-7043213f73a3 -bs -rt -nv =<VALUE>
```
`VALUE` must be in hexadecimal.

For string-type settings, the syntax to write is:
```
setvar <NAME> -guid 10f41c33-a468-42cd-85ee-7043213f73a3 -bs -rt -nv =L"<VALUE>" =0x0000
```

### CPU Performance
#### Cluster clocks / voltages
| Variable                          |    NAME                            |  VALUE                       |
| --------------------------------- |----------------------------------- |----------------------------- |
| CPU`x` Clock Preset               | `CpuPerf_CPUxClusterClockPreset`   | Boot default = `0x00000000`<br> Min = `0x00000001`<br> Max = `0x00000002`<br> Custom = `0x00000003` |
| CPU`x` Custom Clock Preset (MHz)  | `CpuPerf_CPUxClusterClockCustom`   | Hex numeric option, 4-bytes<br> See below. |
| CPU`x` Voltage Mode               | `CpuPerf_CPUxClusterVoltageMode`   | Auto = `0x00000000` (default)<br> Custom = `0x00000001`|
| CPU`x` Custom Voltage (uV)        | `CpuPerf_CPUxClusterVoltageCustom` | Hex numeric value, 4-bytes<br> See below. |

`x` can be :
* `L` for LITTLE cluster
* `B01` for big cluster #0
* `B23` for big cluster #1

`CpuPerf_CPUxClusterClockCustom` can have one of the following values:
* All clusters: `408000000`, `600000000`, `816000000`, `1008000000`, `1200000000`, `1416000000`, `1608000000`, `1800000000`
* Big cluster additional clocks: `2016000000`, `2208000000`, `2256000000`, `2304000000`, `2352000000`, `2400000000`

`CpuPerf_CPUxClusterVoltageCustom` is the cluster voltage in microvolts. Min: `500000`, Max: `1500000`.
Default value depends on cluster type.

### PCIe/SATA/USB Combo PIPE PHY
| Variable    |    NAME         |  VALUE                             |
| ----------- | --------------- | ---------------------------------- |
| PHY #0 Mode | `ComboPhy0Mode` | Unconnected = `0x00000000`<br> PCIe = `0x00000001`<br> SATA = `0x00000002` |
| PHY #1 Mode | `ComboPhy1Mode` | Unconnected = `0x00000000`<br> PCIe = `0x00000001`<br> SATA = `0x00000002` |
| PHY #2 Mode | `ComboPhy2Mode` | Unconnected = `0x00000000`<br> PCIe = `0x00000001`<br> SATA = `0x00000002`<br> USB3 = `0x00000003` |

Default values and supported options depend on the platform. Check documentation and schematics for more details on PHY wiring.

### USB/DP Combo PHY
| Variable                      |    NAME              |  VALUE                            |
| ----------------------------- | -------------------- | --------------------------------- |
| PHY #0 USB 3 SuperSpeed State | `UsbDpPhy0Usb3State` | Enabled = `0x00000001`<br> Disabled = `0x00000000` |
| PHY #1 USB 3 SuperSpeed State | `UsbDpPhy1Usb3State` | Enabled = `0x00000001`<br> Disabled = `0x00000000` |

### PCI Express 3.0
| Variable      |    NAME       |  VALUE                            |
| ------------- | ------------- | --------------------------------- |
| Support State | `Pcie30State` | Enabled = `0x00000001`<br> Disabled = `0x00000000` |

### ACPI / Device Tree
| Variable          |    NAME           |  VALUE                            |
| ----------------- | ----------------- | --------------------------------- |
| Config Table Mode | `ConfigTableMode` | ACPI = `0x00000001`<br> DeviceTree = `0x00000002`<br> Both = `0x00000003` |

#### ACPI Configuration
| Variable        |    NAME         |  VALUE                            |
| --------------- | --------------- | --------------------------------- |
| USB 2.0 Support | `AcpiUsb2State` | Enabled = `0x00000001`<br> Disabled = `0x00000000` |

#### Device Tree Configuration
| Variable                        |    NAME               |  VALUE                            |
| ------------------------------- | --------------------- | --------------------------------- |
| Support DTB override & overlays | `FdtSupportOverrides` | Enabled = `0x01`<br> Disabled = `0x00` |

### Cooling fan
| Variable        |    NAME           |  VALUE                            |
| --------------- | ----------------- | --------------------------------- |
| On-board Fan    | `CoolingFanState` | Enabled = `0x00000001`<br> Disabled = `0x00000000` |
| Fan Speed (%)   | `CoolingFanSpeed` | Hex numeric value, 4-bytes<br> Percentage: 0-100 |

**Examples:**
- To read the 'CPUL Clock Preset' setting :
```
setvar CpuPerf_CPULClusterClockPreset -guid 10f41c33-a468-42cd-85ee-7043213f73a3
```

- To change the 'CPUL Clock Preset' setting to 'Maximum' :
```
setvar CpuPerf_CPULClusterClockPreset -guid 10f41c33-a468-42cd-85ee-7043213f73a3 -bs -rt -nv =0x00000002
```

## Licenses
Most of the UEFI code is licensed under the default EDK2 license, which is [BSD-2-Clause-Patent](https://github.com/tianocore/edk2/blob/master/License.txt).

Some non-critical components have been ported from Rockchip's U-Boot fork and are licensed as **GPL-2.0-or-later**:
* UsbDpPhy
* DwDpLib

The files in `edk2-rockchip-non-osi` are licensed as **GPL-2.0-only**.

The license for some of the blobs in the `misc/rkbin/` directory can be found at: <https://github.com/rockchip-linux/rkbin/blob/master/LICENSE>. Note that it also contains binaries built from open-source projects such as U-Boot (SPL), Arm Trusted Firmware and OP-TEE, having a different license.

## Community
* Radxa forum: <https://forum.radxa.com/t/windows-uefi-on-rock-5-mega-thread/12924>
* Hack w/ Rockchip Telegram: <https://t.me/UEFIonRockchip>
* Windows on R Discord: <https://discord.gg/vjHwptUCa3>

## Credits & alternatives
This firmware is based on Rockchip's initial efforts at <https://gitlab.com/rk3588_linux/rk/uefi-monorepo>.

For RK356x, check out the Quartz64-UEFI project at https://github.com/jaredmcneill/quartz64_uefi, from which we also reused some code.
