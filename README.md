# EDK2 UEFI firmware for Rockchip RK35xx platforms

**WARNING: This repo is highly experimental**

## Description

This repository is based on the official open-source UEFI implementation from Rockchip, which is under active development.

Therefore, to keep up with the work from Rockchip, we should avoid modifying code from Rockchip in most cases.

Discussion thread: [Windows on Rock 5 (Mega thread)](https://forum.radxa.com/t/windows-on-rock-5-mega-thread)

## Building

Using Arch Linux as example

Install required packages:
```bash
sudo pacman -Syu
sudo pacman -S git base-devel gcc dtc aarch64-linux-gnu-binutils aarch64-linux-gnu-gcc aarch64-linux-gnu-glibc python python-pyelftools iasl --needed
```

Clone the repository:
```bash
git clone https://github.com/strongtz/edk2-rk35xx.git
cd edk2-rk35xx
```

Build UEFI (ROCK 5B for example):
```bash
./build.sh -d rock-5b
```

## TODO
 - Create gpt image in build process instead of using the prebuilt one
 - Fix hard reset
 - Fix resetting to maskrom (related to broken hard reset?)

## Suggestions for Rockchip
 - Use `Platform/Radxa/ROCK5B/Library/Dw8250SerialPortLib` as SerialPortLib, `Silicon/Rockchip/Library/SerialPortLib` looks very broken
