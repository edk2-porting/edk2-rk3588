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
- [FriendlyELEC NanoPC T6](https://wiki.friendlyelec.com/wiki/index.php/NanoPC-T6)

## Supported peripherals
Applicable to all platforms unless otherwise noted.

| Device | Status | Notes |
| --- | --- | --- |
| USB 3 / 2.0 / 1.1                  | 🟢 Working     | Host-mode only |
| PCIe 3.0 (RK3588)                  | 🟡 Partial     | |
| PCIe 2.1                           | 🔴 Not working | |
| SATA                               | 🟢 Working     | |
| SD/eMMC                            | 🟢 Working     | |
| HDMI output                        | 🟢 Working     | Single display with mode limited at 1080p 60 Hz |
| USB-C DP output                    | 🔴 Not working | |
| eDP output                         | 🔴 Not working | |
| DSI                                | 🔴 Not working | |
| GMAC Ethernet                      | 🔴 Not working | |
| UART                               | 🟢 Working     | |
| GPIO                               | 🟡 Partial     | No pin function support |
| I2C                                | 🟢 Working     | |
| SPI                                | 🟢 Working     | |
| SARADC                             | 🔴 Not working | |
| PWM                                | ⚪ Untested    | |
| SPI NOR Flash                      | 🟢 Working     | |
| Voltage regulators (RK806, RK860)  | 🟢 Working     | |
| FUSB302 USB-C PD negotiation       | 🔴 Not working | Only present on a few boards (ROCK 5B, Edge2) |

## Supported OSes
| OS | Version | Tested/supported hardware | Notes |
| --- | --- | --- | --- |
| Windows | 10 (1904x), 11 | [Status](https://github.com/worproject/Rockchip-Windows-Drivers#hardware-support-status) ||
| NetBSD | 10 | HDMI GOP, USB, SATA, UART ||
| VMware ESXi Arm Fling | 1.12 | HDMI GOP, USB |Need to hide PCIe 3 in ACPI|

## Installation & usage
Check out the latest release at: <https://github.com/edk2-porting/edk2-rk35xx/releases>, including install instructions.

## Configuration settings
The UEFI configuration settings can be viewed and changed using both the UI configuration menu (under `Device Manager` -> `Rockchip Platform Configuration`), as well as the UEFI Shell. To configure using the UEFI Shell, use `setvar` command to read/write the UEFI variables with GUID = `10f41c33-a468-42cd-85ee-7043213f73a3`.

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
| Variable    |    NAME         |  VALUE                       |
| ----------- |-----------------|----------------------------------- |
| PHY #0 Mode | `ComboPhy0Mode` | Unconnected = `0x00000000`<br> PCIe = `0x00000001`<br> SATA = `0x00000002` |
| PHY #1 Mode | `ComboPhy1Mode` | Unconnected = `0x00000000`<br> PCIe = `0x00000001`<br> SATA = `0x00000002` |
| PHY #2 Mode | `ComboPhy2Mode` | Unconnected = `0x00000000`<br> PCIe = `0x00000001`<br> SATA = `0x00000002`<br> USB3 = `0x00000003` |

Default values and supported options depend on the platform. Check documentation and schematics for more details on PHY wiring.

**Examples:**
- To read the 'CPUL Clock Preset' setting :
```
setvar CpuPerf_CPULClusterClockPreset -guid 10f41c33-a468-42cd-85ee-7043213f73a3
```

- To change the 'CPUL Clock Preset' setting to 'Maximum' :
```
setvar CpuPerf_CPULClusterClockPreset -guid 10f41c33-a468-42cd-85ee-7043213f73a3 -bs -rt -nv =0x00000002
```

## Building
The firmware can only be built on Linux currently. For Windows use WSL.

1. Install required packages:

   For Ubuntu/Debian:
   ```bash
   sudo apt install git gcc g++ build-essential gcc-aarch64-linux-gnu iasl python3-pyelftools
   ```
   For Arch Linux:
   ```bash
   sudo pacman -Syu
   sudo pacman -S git base-devel gcc dtc aarch64-linux-gnu-binutils aarch64-linux-gnu-gcc aarch64-linux-gnu-glibc python python-pyelftools iasl --needed
   ```

2. Clone the repository:
   ```bash
   git clone https://github.com/edk2-porting/edk2-rk35xx.git --recursive
   cd edk2-rk35xx
   ```

3. Build UEFI (ROCK 5B for example, check [list of platform configs](https://github.com/edk2-porting/edk2-rk35xx/tree/master/configs)):
   ```bash
   ./build.sh --device rock-5b --release Release # (or Debug)
   ```

## Notes
### ACPI
ACPI support is limited, with only boot critical devices being currently exposed. It has been tested with Windows, NetBSD and VMware ESXi.

### Flash layout
| Address    | Size       | Desc                  | File                   |
| ---------- | ---------- | --------------------- | ---------------------- |
| 0x00000000 | 0x00004400 | GPT Table             | rk3588_spi_nor_gpt.img |
| 0x00008000 |            | IDBlock               | idblock.bin            |
| 0x00088000 |            | IDBlock               | idblock.bin            |
| 0x00100000 | 0x00500000 | BL33_AP_UEFI FV       | ${DEVICE}_EFI.itb      |

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

## Licenses
The UEFI code and produced FD binary are licensed under the current EDK2 license, which is [BSD-2-Clause-Patent](https://github.com/tianocore/edk2/blob/master/License.txt).

The license of the blobs in the `misc/rkbin/` directory is to be determined. Most of them are built from open-source projects such as U-Boot, Arm Trusted Firmware and OP-TEE.

## Community
* Radxa forum: <https://forum.radxa.com/t/windows-uefi-on-rock-5-mega-thread/12924>
* Hack w/ Rockchip Telegram: <https://t.me/UEFIonRockchip>
* Windows on R Discord: <https://discord.gg/vjHwptUCa3>

## Credits & alternatives
This firmware is based on Rockchip's initial efforts at <https://gitlab.com/rk3588_linux/rk/uefi-monorepo>.

There's also minimal support for RK356X (likely broken in our repo with no plans of reviving), but there's a much better implementation made by @jaredmcneill at https://github.com/jaredmcneill/quartz64_uefi, from which we also reused some code.
