# Introduction of SiFive U5 Series Platforms
U5SeriesPkg provides the common EDK2 libraries and drivers for SiFive U5 series
platforms. Currently the supported platforms are Freedom U500 VC707 platform and
Freedom U540 HiFive Unleashed platform.

Both platforms are built with below common edk2 packages under edk2-platforms
repository,
- [**U5SeriesPkg**](https://github.com/tianocore/edk2-platforms/tree/master/Platform/SiFive/U5SeriesPkg)
- [**RiscVPlatformPkg**](https://github.com/tianocore/edk2-platforms/tree/master/Platform/RISC-V/PlatformPkg)
- [**RiscVProcessorPkg**](https://github.com/tianocore/edk2-platforms/tree/master/Silicon/RISC-V/ProcessorPkg)

## U500 Platform
This is a sample platform package used against to SiFive Freedom U500
VC707 FPGA Dev Kit, please refer to "SiFive Freedom U500 VC707 FPGA Getting
Started Guide" on https://www.sifive.com/documentation.
The binary built from Platform/SiFive/U5SeriesPkg/FreedomU500VC707Board can run
on U500 VC707 FPGA board.
```
build -a RISCV64 -t GCC5 -p Platform/SiFive/U5SeriesPkg/FreedomU500VC707Board/U500.dsc
```

## U540 Platform
This is a sample platform package used for the SiFive Freedom U540 HiFive Unleashed
development board, please refer to "SiFive Freedom U540-C000 Manual" on
https://www.sifive.com. The binary built from
Platform/SiFive/U5SeriesPkg/FreedomU540HiFiveUnleashedBoard/ can run on both
hardware and [QEMU](https://git.qemu.org/?p=qemu.git;a=summary).
It is confirmed that version 5.0 of QEMU can boot with this  firmware to EFI shell
and Linux user space.
```
build -a RISCV64 -t GCC5 -p Platform/SiFive/U5SeriesPkg/FreedomU540HiFiveUnleashedBoard/U540.dsc
```
For running U540 edk2 binary on QEMU, use qemu-system-riscv64 under riscv64-softmmu
to launch the binary with below parameters,

```
qemu-system-riscv64 -cpu sifive-u54 -machine sifive_u -bios U540.fd -m 2048 -nographic -smp cpus=5,maxcpus=5
```

## Download the Source Code
```
git clone https://github.com/tianocore/edk2.git
git clone https://github.com/tianocore/edk2-platforms.git
# Below to clone opensbi submodule.
git submodule update --init
```
Refer to [Readme.md](https://github.com/tianocore/edk2-platforms/blob/master/Platform/RISC-V/PlatformPkg/Readme.md) for building RISC-V platforms.

## Platform Status
**FreedomU500VC707Board**
Currently the binary built from U500 edk2 package can boot SiFive Freedom U500 VC707
FPGA to EFI shell with console in/out enabled.

**FreedomU540HiFiveUnleashedBoard**
Currently the binary built from U540 edk2 package can boot SiFive Freedom U540 HiFive
Unleashed to EFI shell with console in/out enabled and Linux kernel. Please refer to
https://github.com/riscv/riscv-uefi-edk2-docs for booting to Linux kernel.

## Supported Operating Systems
Please refer to https://github.com/riscv/riscv-uefi-edk2-docs.

## Known Issues and Limitations
Only RISC-V RV64 is verified on this platform.

## Related Materials
- [RISC-V UEFI Documents](https://github.com/riscv/riscv-uefi-edk2-docs)
- [RISC-V OpenSbi](https://github.com/riscv/opensbi)
- [SiFive U500 VC707 FPGA Getting Started Guide](https://sifive.cdn.prismic.io/sifive%2Fc248fabc-5e44-4412-b1c3-6bb6aac73a2c_sifive-u500-vc707-gettingstarted-v0.2.pdf)
- [SiFive Freedom U540-C000 Manual](https://sifive.cdn.prismic.io/sifive%2F834354f0-08e6-423c-bf1f-0cb58ef14061_fu540-c000-v1.0.pdf)
- [SiFive RISC-V Core Document](https://www.sifive.com/documentation)

## U5SeriesPkg Libraries and Drivers
### PeiCoreInfoHobLib
This is the library to create RISC-V core characteristics for building up RISC-V
related SMBIOS records to support a single boot loader  or OS image on all RISC-V
platforms by discovering RISC-V hart configurations dynamically. This library leverage
the silicon libraries provided in Silicon/SiFive.

### RiscVPlatformTimerLib
This is common U5 series platform timer library which has the platform-specific
timer implementation.

### SerialLib
This is common U5 series platform serial port library.

### TimerDxe
This is common U5 series platform timer DXE driver which has the platform-specific
timer implementation.

## U500 Platform Libraries and Drivers
### RiscVOpensbiPlatformLib
In order to reduce the dependencies with RISC-V OpenSBI project
(https://github.com/riscv/opensbi) and avoid duplicating code we use it, the
implementation of RISC-V EDK2 platform is leveraging platform source code from OpenSBI
code tree. The "platform.c" under OpenSbiPlatformLib is cloned from
[RISC-V OpenSBI code tree](Silicon/RISC-V/ProcessorPkg/Library/RiscVOpensbiLib/opensbi)
and built based on edk2 build environment.

### PlatformPei
This is the platform-implementation specific library which is executed in early PEI
phase for U500 platform initialization.

## U540 Platform Libraries and Drivers
### RiscVOpensbiPlatformLib
In order to reduce the dependencies with RISC-V OpenSBI project
(https://github.com/riscv/opensbi) and fewer burdens to EDK2 build process, the
implementation of RISC-V EDK2 platform is leveraging platform source code from
OpenSBI code tree. The "platform.c" under OpenSbiPlatformLib is cloned from
[RISC-V OpenSBI code tree](Silicon/RISC-V/ProcessorPkg/Library/RiscVOpensbiLib/opensbi)
and built based on edk2build environment.

### PlatformPei
This is the platform-implementation specific library which is executed in early PEI
phase for U540 platform initialization.

## U5SeriesPkg Platform PCD settings

| **PCD name** |**Usage**|
|----------------|----------|
|PcdU5PlatformSystemClock| U5 series platform system clock|
|PcdNumberofU5Cores| Number of U5 core enabled on U5 series platform|
|PcdE5MCSupported| Indicates whether the Monitor Core (E5) is supported on U5 series platform|
|PcdU5UartBase|Platform serial port base address|


## Platform Owners
Chang, Abner <abner.chang@hpe.com>
Chen, Gilbert <gilbert.chen@hpe.com>
Schaefer, Daniel <daniel.schaefer@hpe.com>
