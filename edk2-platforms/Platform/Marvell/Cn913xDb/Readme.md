Marvell CN913x Development Board
================================

# Summary

This is a port of 64-bit TianoCore EDK II firmware for the Marvell CN913x Development Board.

# Building the firmware

## Prepare toolchain (for cross-compilation only):

1. Download the toolchain:

   ```
   wget https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
   ```

1. After extracting, setup the path and compiler prefix to GCC5\_AARCH64\_PREFIX variable:

   ```
   export GCC5_AARCH64_PREFIX=<toolchain_path>/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   ```

## Prepare prerequisites

1. Create a new folder (directory) on your local development machine
   for use as your workspace. This example uses `/work/git/tianocore`, modify as
   appropriate for your needs.

   ```
   $ export WORKSPACE=/work/git/tianocore
   $ mkdir -p $WORKSPACE
   $ cd $WORKSPACE
   ```

1. Clone the Trusted Firmware repository:

   ```
   $ cd ${WORKSPACE}
   $ git clone https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git
   ```
1. Clone repository for auxiliary firmware on the SoC co-processors and checkout to binaries-marvell-armada-SDK10.0.1.0:

   ```
   $ cd ${WORKSPACE}
   $ git clone https://github.com/MarvellEmbeddedProcessors/binaries-marvell.git
   $ cd binaries-marvell/
   $ git checkout -b binaries-marvell-armada-SDK10.0.1.0 origin/binaries-marvell-armada-SDK10.0.1.0
   ```
1. Clone the DDR training code from:

   ```
   $ cd ${WORKSPACE}
   $ git clone https://github.com/MarvellEmbeddedProcessors/mv-ddr-marvell.git
   ```
## Prepare EDKII environment:

Please follow instructions from "Obtaining source code" and "Manual building" from the top level edk2-platforms [Readme.md](https://github.com/tianocore/edk2-platforms#readme).

## Build EDKII:

1. Use below build command:

   ```
   $ cd ${WORKSPACE}
   $ build -a AARCH64 -t GCC5 -b RELEASE -D CN9132 -D INCLUDE_TFTP_COMMAND -D CAPSULE_ENABLE -p Platform/Marvell/Cn913xDb/Cn913xDbA.dsc
   ```

## Build the final firmware image:

1. Set BL33 variable to path to EDK II output binary:

   ```
   $ export BL33=${WORKSPACE}/Build/Cn9132DbA-AARCH64/RELEASE_GCC5/FV/ARMADA_EFI.fd
   ```
1. Export SCP_BL2 variable:

   ```
   $ export SCP_BL2=${WORKSKPACE}/binaries-marvell/mrvl_scp_bl2.img
   ```
1. Export compiler variables (for cross-compilation only):

   ```
   $ export ARCH=arm64
   $ export CROSS_COMPILE=<toolchain_path>/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   ```
1. Build the image:

   ```
   $ cd ${WORKSPACE}/trusted-firmware-a/
   $ make LOG_LEVEL=20 MV_DDR_PATH=${WORKSPACE}/mv-ddr-marvell CP_NUM=3 PLAT=t9130 all fip mrvl_flash

   ```
The firmware image `flash-image.bin` can be found in `build/t9130/release/` directory.

# ARM System Ready certification.

CN913x Development Board is [System Ready ES](https://developer.arm.com/architectures/system-architectures/arm-systemready/es) certified. Release binary and the firmware components' baselines list are available in a dedicated [wiki page](https://github.com/semihalf/edk2-platforms/wiki).
