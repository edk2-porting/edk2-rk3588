This branch holds all platforms actively maintained against the
[edk2](https://github.com/tianocore/edk2) master branch.

For generic information about the edk2-platforms repository, and the process
under which _stable_ and _devel_ branches can be added for individual platforms,
please see
[the introduction on the about branch](https://github.com/tianocore/edk2-platforms/blob/about/Readme.md).

The majority of the content in the EDK II open source project uses a
[BSD-2-Clause Plus Patent License](License.txt).  Additional details on EDK II
open source project code contributions can be found in the edk2 repository
[Readme.md](https://github.com/tianocore/edk2/blob/master/Readme.md).
The EDK II Platforms open source project contains the following components that
are covered by additional licenses:

- [`Silicon/RISC-V/ProcessorPkg/Library/RiscVOpensbiLib/opensbi`](https://github.com/riscv/opensbi/blob/master/COPYING.BSD)

# INDEX
* [Overview](#overview)
* [How To Build (Linux Environment)](#how-to-build-linux-environment)
   * [Manual building](#manual-building)
   * [Using uefi-tools helper scripts](#using-uefi-tools-helper-scripts)
* [How To Build (Windows Environment)](#how-to-build-windows-environment)
* [Supported Platforms](#supported-platforms)
* [Maintainers](#maintainers)

# Overview

Platform description files can be found under `Platform/{Vendor}/{Platform}`.

Many platforms require additional image processing beyond the EDK2 build.
Any such steps should be documented (as a Readme.md), and any necessary helper
scripts be contained, under said platform directory.

Any contributions to this branch should be submitted via email to the
edk2-devel mailing list with a subject prefix of `[platforms]`. See
[Laszlo's excellent guide](https://github.com/tianocore/tianocore.github.io/wiki/Laszlo's-unkempt-git-guide-for-edk2-contributors-and-maintainers) for details
on how to do this successfully.

# How to build (Linux Environment)

## Prerequisites
The build tools themselves depend on Python (2) and libuuid. Most Linux systems
will come with a Python environment installed by default, but you usually need
to install uuid-dev (or uuid-devel, depending on distribution) manually.

## If cross compiling
If building EDK2 for a different archtecture than the build machine, you need to
obtain an appropriate cross-compiler. X64 (x86_64) compilers also support IA32,
but the reverse may not always be true.

Target architecture | Cross compilation prefix
--------------------|-------------------------
AARCH64             | aarch64-linux-gnu-
ARM                 | arm-linux-gnueabihf-
IA32                | i?86-linux-gnu-* _or_ x86_64-linux-gnu-
IPF                 | ia64-linux-gnu
X64                 | x86_64-linux-gnu-
RISCV64             | riscv64-unknown-elf-

\* i386, i486, i586 or i686

### GCC
Arm provides GCC toolchains for aarch64-linux-gnu and arm-linux-gnueabihf at
[GNU Toolchain for the A-profile Architecture](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads)
compiled to run on x86_64/i686 Linux and i686 Windows. Some Linux distributions
provide their own packaged cross-toolchains.

### GCC for RISC-V
RISC-V open source community provides GCC toolchains for
[riscv64-unknown-elf](https://github.com/riscv/riscv-gnu-toolchain)
compiled to run on x86 Linux.

### clang
Clang does not require separate cross compilers, but it does need a
target-specific binutils. These are included with any prepackaged GCC toolchain
(see above), or can be installed or built separately.

## Obtaining source code
1. Create a new folder (directory) on your local development machine
   for use as your workspace. This example uses `/work/git/tianocore`, modify as
   appropriate for your needs.
   ```
   $ export WORKSPACE=/work/git/tianocore
   $ mkdir -p $WORKSPACE
   $ cd $WORKSPACE
   ```

1. Into that folder, clone:
   1. [edk2](https://github.com/tianocore/edk2)
   1. [edk2-platforms](https://github.com/tianocore/edk2-platforms)
   1. [edk2-non-osi](https://github.com/tianocore/edk2-non-osi) (if building
      platforms that need it)
   ```
   $ git clone https://github.com/tianocore/edk2.git
   $ git submodule update --init
   ...
   $ git clone https://github.com/tianocore/edk2-platforms.git
   $ git submodule update --init
   ...
   $ git clone https://github.com/tianocore/edk2-non-osi.git
   ```

1. Set up a **PACKAGES_PATH** to point to the locations of these three
   repositories:

   `$ export PACKAGES_PATH=$PWD/edk2:$PWD/edk2-platforms:$PWD/edk2-non-osi`

## Manual building

1. Set up the build environment (this will modify your environment variables)

   `$ . edk2/edksetup.sh`

   (This step _depends_ on **WORKSPACE** being set as per above.)
1. Build BaseTools

   `make -C edk2/BaseTools`

   (BaseTools can currently not be built in parallel, so do not specify any `-j`
   option, either on the command line or in a **MAKEFLAGS** environment
   variable.)

### Build options
There are a number of options that can (or must) be specified at the point of
building. Their default values are set in `edk2/Conf/target.txt`. If we are
working only on a single platform, it makes sense to just update this file.

target.txt option | command line | Description
------------------|--------------|------------
ACTIVE_PLATFORM   | `-p`         | Description file (.dsc) of platform.
TARGET            | `-b`         | One of DEBUG, RELEASE or NOOPT.
TARGET_ARCH       | `-a`         | Architecture to build for.
TOOL_CHAIN_TAG    | `-t`         | Toolchain profile to use for building.

There is also MAX_CONCURRENT_THREAD_NUMBER (`-n`), roughly equivalent to
`make -j`.

When specified on command line, `-b` can be repeated multiple times in order to
build multiple targets sequentially.

After a successful build, the resulting images can be found in
`Build/{Platform Name}/{TARGET}_{TOOL_CHAIN_TAG}/FV`.

### Build a platform
The main build process _can_ run in parallel - so figure out how many threads we
have available.

```
$ getconf _NPROCESSORS_ONLN
8
```
OK, so we have 8 CPUs - let's tell the build to use a little more than that:
```
$ NUM_CPUS=$((`getconf _NPROCESSORS_ONLN` + 2))
```
For the toolchain tag, use GCC5 for gcc version 5 or later, GCC4x for
earlier versions, or CLANG35/CLANG38 as appropriate when building with clang.
```
$ build -n $NUM_CPUS -a AARCH64 -t GCC5 -p Platform/ARM/JunoPkg/ArmJuno.dsc
```
(Note that the description file gets resolved by the build command through
searching in all locations specified in **PACKAGES_PATH**.)

#### If cross-compiling
When cross-compiling, or building with a different version of the compiler than
the default `gcc` or `clang`(/binutils), we additionally need to inform the
build command which toolchain to use. We do this by setting the environment
variable `{TOOL_CHAIN_TAG}_{TARGET_ARCH}_PREFIX` - in the case above,
**GCC5_AARCH64_PREFIX**.

So, referring to the cross compiler toolchain table above, we should prepend the `build` command line with `GCC5_AARCH64_PREFIX=aarch64-linux-gnu-`.

## Using uefi-tools helper scripts
uefi-tools is a completely unofficial set of helper-scripts developed by Linaro.
They automate figuring out all of the manual options above, and store the paths
to platform description files in a separate configuration file. Additionally,
they simplify bulk-building large numbers of platforms.

The (best effort) intent is to keep this configuration up to date with all
platforms that exist in the edk2-platforms master branch.

The equivalent of the manual example above would be
```
$ git clone https://git.linaro.org/uefi/uefi-tools.git
...
$ ./uefi-tools/edk2-build.sh juno
...
------------------------------------------------------------
                         aarch64 Juno (AARCH64) RELEASE pass
------------------------------------------------------------
pass   1
fail   0
```
The build finishes with a summary of which platforms/targets were built, which
succeeded and which failed (and the total number of either).

Like the `build` command itself, `edk2-build.sh` it supports specifying multiple
targets on a single command line, but it also lets you specify multiple
platforms (or `all` for building all known platforms). So in order to build all
platforms described by the configuration file, for both DEBUG and RELEASE
targets:
```
$ ./uefi-tools/edk2-build.sh -b DEBUG -b RELEASE
```

# How To Build (Windows Environment)

(I genuinely have no idea. Please help!)


# Supported Platforms

These are the platforms currently supported by this tree - grouped by
Processor/SoC vendor, rather than platform vendor.

If there are any additional build steps beyond the generic ones listed above,
they will be documented with the platform.

## AMD
* [Cello](Platform/LeMaker/CelloBoard)
* [Overdrive](Platform/AMD/OverdriveBoard)
* [Overdrive 1000](Platform/SoftIron/Overdrive1000Board)

## [Ampere](Platform/Ampere/Readme.md)
* [Mt. Jade](Platform/Ampere/JadePkg)

## [ARM](Platform/ARM/Readme.md)
* [Juno](Platform/ARM/JunoPkg)
* [SGI family](Platform/ARM/SgiPkg)

## BeagleBoard
* [BeagleBoard](Platform/BeagleBoard/BeagleBoardPkg)

## Hisilicon
* [D03](Platform/Hisilicon/D03)
* [D05](Platform/Hisilicon/D05)
* [D06](Platform/Hisilicon/D06)
* [HiKey](Platform/Hisilicon/HiKey)
* [HiKey960](Platform/Hisilicon/HiKey960)

## Intel
### [Minimum Platforms](Platform/Intel/Readme.md)
* [Kaby Lake](Platform/Intel/KabylakeOpenBoardPkg)
* [Purley](Platform/Intel/PurleyOpenBoardPkg)
* [Simics](Platform/Intel/SimicsOpenBoardPkg)
* [Whiskey Lake](Platform/Intel/WhiskeylakeOpenBoardPkg)
* [Comet Lake](Platform/Intel/CometlakeOpenBoardPkg)
* [Tiger Lake](Platform/Intel/TigerlakeOpenBoardPkg)
* [Whitley/Cedar Island](Platform/Intel/WhitleyOpenBoardPkg)

For more information, see the
[EDK II Minimum Platform Specification](https://edk2-docs.gitbooks.io/edk-ii-minimum-platform-specification).
### Other Platforms
##### Intel&reg; Quark SoC X1000 based platforms
* [Galileo](Platform/Intel/QuarkPlatformPkg)
##### Minnowboard Max/Turbot based on Intel Valleyview2 SoC
* [Minnowboard Max](Platform/Intel/Vlv2TbltDevicePkg)

## Marvell
* [Armada 70x0](Platform/Marvell/Armada70x0Db)
* [Armada 80x0](Platform/Marvell/Armada80x0Db)
* [CN913x](Platform/Marvell/Cn913xDb)
* [SolidRun Armada MacchiatoBin](Platform/SolidRun/Armada80x0McBin)

## Raspberry Pi
* [Pi 3](Platform/RaspberryPi/RPi3)
* [Pi 4](Platform/RaspberryPi/RPi4)

## RISC-V
### SiFive
* [Sifive U5 Series](Platform/SiFive/U5SeriesPkg) Refer to Platform/SiFive/U5Series/Readme.md on edk2-platform repository.
* [Freedom U500 VC707 FPGA](Platform/SiFive/U5SeriesPkg/FreedomU500VC707Board)
* [Freedom U540 HiFive Unleashed](Platform/SiFive/U5SeriesPkg/FreedomU540HiFiveUnleashedBoard)

## Socionext
* [SynQuacer](Platform/Socionext/DeveloperBox)

## NXP
* [LS1043aRdb](Platform/NXP/LS1043aRdbPkg)

## Qemu
* [SBSA](Platform/Qemu/SbsaQemu)

# Maintainers

See [Maintainers.txt](Maintainers.txt).

# Submodules

Submodule in EDK II Platforms is allowed but submodule chain should be avoided
as possible as we can. Currently EDK II Platforms contains the following
submodules

- Silicon/RISC-V/ProcessorPkg/Library/RiscVOpensbiLib/opensbi

To get a full, buildable EDK II repository, use following steps of git command

```bash
  git clone https://github.com/tianocore/edk2-platforms.git
  cd edk2-platforms
  git submodule update --init
  cd ..
```

If there's update for submodules, use following git commands to get the latest
submodules code.

```bash
  cd edk2-platforms
  git pull
  git submodule update
```

Note: When cloning submodule repos, '--recursive' option is not recommended.
EDK II Platforms itself will not use any code/feature from submodules in above
submodules. So using '--recursive' adds a dependency on being able to reach
servers we do not actually want any code from, as well as needlessly
downloading code we will not use.
