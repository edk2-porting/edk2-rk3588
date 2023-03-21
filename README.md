# EDK2 UEFI firmware for Rockchip RK35xx platforms

**WARNING: This repo is highly experimental**

## Description

This repository is based on the official open-source UEFI implementation from Rockchip, which is under active development.

Therefore, to keep up with the work from Rockchip, we should avoid modifying code from Rockchip in most cases.

Discussion thread: [Windows / UEFI on Rock 5 (Mega thread)](https://forum.radxa.com/t/windows-uefi-on-rock-5-mega-thread/12924)

## Building

Using Arch Linux as example

Install required packages:
```bash
sudo pacman -Syu
sudo pacman -S git base-devel gcc dtc aarch64-linux-gnu-binutils aarch64-linux-gnu-gcc aarch64-linux-gnu-glibc python python-pyelftools iasl --needed
```

Required packages for Ubuntu/Debian:
```bash
sudo apt install git gcc g++ build-essential gcc-aarch64-linux-gnu iasl python3-pyelftools
```

Clone the repository:
```bash
git clone https://github.com/edk2-porting/edk2-rk35xx.git --recursive
cd edk2-rk35xx
```

Build UEFI (ROCK 5B for example):
```bash
./build.sh -d rock-5b
```

## TODO
 - Create gpt image in build process instead of using the prebuilt one
 - Fix resetting to maskrom

## Notes

### Flash layout

| Address    | Size       | Desc                  | File                   |
| ---------- | :--------- | --------------------- | ---------------------- |
| 0x00000000 | 0x00004400 | GPT Table             | rk3588_spi_nor_gpt.img |
| 0x00008000 |            | IDBlock               | idblock.bin            |
| 0x00088000 |            | IDBlock               | idblock.bin            |
| 0x00100000 | 0x00500000 | BL33_AP_UEFI FV       | ${DEVICE}_EFI.itb      |
| 0x007C0000 | 0x00010000 | NV_VARIABLE_STORE     |                        |
| 0x007D0000 | 0x00010000 | NV_FTW_WORKING header |                        |
| 0x007E0000 | 0x00010000 | NV_FTW_WORKING data   | NV_DATA.img            |

### Memory Map

| Address    | Size | Desc          | File                |
| ---------- | :--: | ------------- | ------------------- |
| 0x00040000 |      | ATF           | bl31_0x00040000.bin |
| 0x000f0000 |      | ATF           | bl31_0x000f0000.bin |
| 0x00200000 |      | UEFI FV       | BL33_AP_UEFI.Fv     |
| 0x08400000 |      | OP-TEE        | bl32.bin            |
| 0xff100000 |      | ATF (PMU_MEM) | bl31_0xff100000.bin |