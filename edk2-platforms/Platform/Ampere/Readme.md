# Introduction

This document provides the guideline to build UEFI firmware for Ampere Computing's Arm64 reference platforms.

Platform code is located under Platform/Ampere/{Platform Name}Pkg.

Silicon code is located under Silicon/Ampere/Ampere{SoC Name}Pkg.

# Build machines

- x86 Linux host machines running latest Ubuntu or CentOS releases.
- Arm64 Linux host machines if native compiling. This has been tested on Ampere's eMAG and Altra hardware platforms with latest AArch64 CentOS or Ubuntu releases.

# How to build (Linux Environment)

Please follow top-level Readme.md for build instructions.

## Additional build tools

Ampere provides additional tools and documentation for automating the manual process described as described in the top-level README.md,
and for building a final Tianocore UEFI image that can be flashed onto the target system.

To use these tools, clone the following to the **WORKSPACE** location:

```bash
$ git clone https://github.com/AmpereComputing/edk2-ampere-tools.git
```

## Notes

If you run into any build issue with the Intel ASL+ Optimizing Compiler/Disassembler (IASL) that comes with your Linux distro,
download and install the IASL compiler from https://acpica.org/. At the time of this write-up, we have tested with version 20200110.

```bash
$ wget https://acpica.org/sites/acpica/files/acpica-unix2-20200110.tar.gz
$ tar xzf acpica-unix2-20200110.tar.gz
$ cd acpica-unix2-20200110
$ make HOST=_CYGWIN && sudo make install
```
