# EDK2 UEFI firmware for Rockchip RK35xx platforms

**WARNING: This repo is highly experimental**

## Description

This repository is based on the official open-source UEFI implementation from Rockchip, which is under active development.

Therefore, to keep up with the work from Rockchip, we should avoid modifying code from Rockchip in most cases.

## Building

Using Arch Linux as example

Install required packages:
```bash
sudo pacman -Syu
sudo pacman -S git base-devel gcc aarch64-linux-gnu-binutils aarch64-linux-gnu-gcc aarch64-linux-gnu-glibc python --needed
```

Clone the repository:
```bash
mkdir git && cd git
git clone https://github.com/radxa/u-boot.git -b stable-5.10-rock5
cd ..
git clone https://github.com/strongtz/edk2-rk35xx-TESTING.git
cd edk2-rk35xx-TESTING
```

Build UEFI (ROCK 5B for example):
```bash
./build.sh -d rock-5b
```

## TODO
 - Remove dependency on u-boot repository
 - Create gpt image in build process instead of using the prebuilt one
 - Fix GpioLib (why not working?) to enable USB Vbus
 - Fix hard reset
 - Fix resetting to maskrom (related to broken hard reset?)

## Suggestions for Rockchip
 - Use `Platform/Radxa/ROCK5B/Library/Dw8250SerialPortLib` as SerialPortLib, `Silicon/Rockchip/Library/SerialPortLib` looks very broken