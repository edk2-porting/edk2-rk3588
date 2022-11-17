# Overview

This directory holds the UEFI implementation for Sbsa-ref machine which is
a fully SW emulated SBSA machine (ARM64). It aims to emulate the real HW as
close as possible. It's purpose is to enable new feature development when
HW for those features are not yet present on the market. It allows poking
the HW to do all kinds of things (including errors) which are beyond control
on real HW. It also allows easy simulations and debugging of FW-to-HW
interactions.

Keep in mind that all of the above is possible as this machine is fully
emulated in SW. Sbsa-ref machine does not use any HW acceleration of your
platform, even if you run it on ARM64 platform. The EL3 (and ARM TrustZone)
is also emulated in SW.

# How to build (Linux Environment)

## Prerequisites

Build process for Sbsa-ref uses FDF file for flash image composition. This is
different to what some might expect as you need to first build the TF-A before
building EDK2.
Flash0 (secure) is used by BL1 and FIP (BL2 + BL31).
Flash1 contains EFI code and EFI variables.

## Obtaining source code

Create a directory $WORKSPACE that would hold source code of the components.

  1. [qemu](https://github.com/qemu/qemu.git)
  2. [edk2](https://github.com/tianocore/edk2)
  3. [edk2-platforms](https://github.com/tianocore/edk2-platforms)
  4. [edk2-non-osi](https://github.com/tianocore/edk2-non-osi)
  5. [tf-a](https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git)

## Manual building

1. Compile QEMU

  Sbsa-ref machine support was added to QEMU in version v4.1.0
  If your distribution package provides an earlier version then you need to
  compile QEMU from the source. Below is a short set of instructions on how
  to compile without referring to QEMU docs.

  Set $INSTALL_PATH to /usr/local, ~/local or any of your preferred location.

  ```
  cd $WORKSPACE/qemu
  mkdir -p build-native
  cd build-native
  ../configure --target-list=aarch64-softmmu --prefix=$INSTALL_PATH
  make install
  ```

  QEMU should be installed now in $INSTALL_PATH

2. Compile TF-A

  This step is only needed if users want to compile a custom ATF binary.
  Else, the edk2-non-osi directory contains prebuilt bl1.bin and fip.bin
  binaries which will be automatically used in the build process.

  As noted before, for Sbsa-ref machine we use FDF to compose two flash images.
  Those flash images need BL1, BL2 and BL31 from TF-A in form of two files
  `bl1.bin` and `fip.bin`. Follow the instructions below to get those artifacts.

  ```
  cd $WORKSPACE/atf
  make PLAT=sbsa all fip
  ```
  Then copy `bl1.bin` and `fip.bin` to the the edk2-non-osi directory:

  ```
  cp build/sbsa/release/bl1.bin $WORKSPACE/edk2-non-osi/Platform/Qemu/Sbsa/
  cp build/sbsa/release/fip.bin $WORKSPACE/edk2-non-osi/Platform/Qemu/Sbsa/
  ```

3. Compile UEFI for QEMU Sbsa platform

  Detailed build instructions can be found on the following link:
  https://github.com/tianocore/edk2-platforms

  Following is a short description to build for the Sbsa platform.

  Compilation of BaseTools and preparation:

  ```
  cd $WORKSPACE
  export PACKAGES_PATH=$WORKSPACE/edk2:$WORKSPACE/edk2-platforms:$WORKSPACE/edk2-non-osi
  make -C edk2/BaseTools
  . edk2/edksetup.sh
  ```

  Now compile UEFI for Sbsa QEMU:

  ```
  cd $WORKSPACE
  build -b RELEASE -a AARCH64 -t GCC5 -p edk2-platforms/Platform/Qemu/SbsaQemu/SbsaQemu.dsc
  ```
  Copy SBSA_FLASH0.fd and SBSA_FLASH0.fd to top $WORKSPACE directory.
  Then extend the file size to match the machine flash size.
  ```
  cp Build/SbsaQemu/RELEASE_GCC5/FV/SBSA_FLASH[01].fd .
  truncate -s 256M SBSA_FLASH[01].fd
  ```

# Running

  The resulting SBSA_FLASH0.fd file will contain Secure flash0 image (TF-A code).
  The SBSA_FLASH1.fd will contain Non-secure UEFI code and UEFI variables.

  You will boot to the UEFI console with following QEMU command line:
  ```
  $INSTALL_PATH/qemu-system-aarch64 -m 1024 -M sbsa-ref -pflash SBSA_FLASH0.fd -pflash SBSA_FLASH1.fd -serial stdio
  ```
  You can add XHCI controller with keyboard and mouse by:
  ```
  -device qemu-xhci -device usb-mouse -device usb-kbd
  ```
  You can add the hard drive to platform AHCI controller by `hda` parameter:
  ```
  -hda disk1.img
  ```
  For TEE and other secure development you might get use of secure serial which would require following commands. First create `secure_serial` fifo and read it from separate terminal (open new terminal emulator window for it):
  ```
  mkfifo secure_serial
  tail -f secure_serial
  ```
  Then on first console:
  ```
  qemu-system-aarch64 -m 1024 -M sbsa-ref -pflash SBSA_FLASH0.fd -pflash SBSA_FLASH1.fd -serial stdio -hda disk1.img -serial file:secure_serial
  ```
