# Introduction

Morello is an ARMv8-A platform that implements the capability architecture
extension. Capability architecture specific changes will be added [here](https://git.morello-project.org/morello).

The platform port in UEFI firmware provides ARMv8-A architecture enablement.

Platform code is located at Platform/ARM/Morello.

# Documentation

Further information on Morello Platform is available at this [page](https://developer.arm.com/architectures/cpu-architecture/a-profile/morello).

# Morello FVP

Morello FVP can be downloaded from this [location](https://developer.arm.com/tools-and-software/open-source-software/arm-platforms-software/arm-ecosystem-fvps).

# Supported Host and Toolchain

- Host PC should be running Ubuntu Linux 18.04 LTS.
- Please refer to the `edk2-platforms/Readme.md` for downloading the GCC toolchain.

# Build Instructions

Please refer to the `edk2-platforms/Readme.md` for build instructions.

# Dependencies

Once the FVP is running, the SCP will be the first to boot and will bring the AP
core out of reset. The AP core will start executing Trusted Firmware-A at BL31
and once it completes the execution, it will start executing UEFI.

The SCP and TF-A binaries are required to boot to the UEFI Shell.

## SCP Firmware

The SCP firmware source code can be downloaded from this [page](https://github.com/ARM-software/SCP-firmware).

Refer to the [SCP Readme](https://github.com/ARM-software/SCP-firmware/blob/master/user_guide.md)
for building SCP firmware.

## Trusted Firmware-A (TF-A)

The Trusted Firmware-A source code can be downloaded from this [page](https://trustedfirmware-a.readthedocs.io/en/latest/).

Refer to the [TF-A Readme](https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git/tree/docs/plat/arm/morello/index.rst?h=refs/heads/master) for building TF-A.
