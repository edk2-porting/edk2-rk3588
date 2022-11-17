# **EDK II Minimum Platform Firmware for Intel&reg; Platforms**

The Minimum Platform is a software architecture that guides uniform delivery of Intel platforms enabling firmware
solutions for basic boot functionality with extensibility built-in. Please see the
[EDK II Minimum Platform Draft Specification](https://edk2-docs.gitbooks.io/edk-ii-minimum-platform-specification/)
for more details.

Package maintainers for the Minimum Platform projects are listed in Maintainers.txt.

## Overview
The key elements of the architecture are organized into a staged boot approach where each stage has requirements and
functionality for specific use cases. The generic control flow through the boot process is implemented in the
[`MinPlatformPkg`](https://github.com/tianocore/edk2-platforms/tree/master/Platform/Intel/MinPlatformPkg).
The generic nature of the tasks performed in MinPlatformPkg lends to reuse across all Intel platforms with no
source modification. Details for any particular board are made accessible to the MinPlatformPkg through a well-defined
statically linked board API. A complete platform solution then consists of the MinPlatformPkg and a compatible board
package.

## Board Naming Convention
The board packages supported by Intel follow the naming convention \<xxx\>OpenBoardPkg where xxx refers to the
encompassing platform name for a particular platform generation. For example, the [`KabylakeOpenBoardPkg`](https://github.com/tianocore/edk2-platforms/tree/master/Platform/Intel/KabylakeOpenBoardPkg) contains the
board code for Intel KabyLake reference systems. Intel uses the moniker "OpenBoardPkg" to indicate that this package
is the open source board code. A closed source counterpart may exist which simply uses "BoardPkg". Both directly use
the MinPlatformPkg from edk2-platforms.

## Stage Selection
Stage selection is controlled via the PCD `gMinPlatformPkgTokenSpaceGuid.PcdBootStage` in [`MinPlatformPkg.dec`](https://github.com/tianocore/edk2-platforms/tree/master/Platform/Intel/MinPlatformPkg/MinPlatformPkg.dec).
The stage should be configured in the board package DSC file to the appropriate value. For example, a board may disable
all advanced features by setting this value to 4 instead of 6. This may be used to improve boot time for a particular
use case. Decrementing the stage can also be used for debug since only the actions required for that stage objective
should be executed. As an example, ACPI initialization is not required for a Stage 3 boot.

The stages are defined as follows:

| Stage  | Functional Objective         | Example Capabilities                                                                               |
| -------|------------------------------|----------------------------------------------------------------------------------------------------|
| I      | Minimal Debug                | Serial port output, source debug enabled, hardware debugger enabled                                |
| II     | Memory Functional            | Basic hardware initialization necessary to reach memory initialization, permanent memory available |
| III    | Boot to UI                   | Simple console input and output to a UI, UEFI shell                                                |
| IV     | Boot to OS                   | Boot an operating system with the minimally required features                                      |
| V      | Security Enable              | UEFI Secure Boot, TCG measured boot, DMA protections                                               |
| VI     | Advanced Feature Enable      | Firmware update, power management, non-essential I/O                                               |

## Minimum Platform Firmware Solution Stack
A UEFI firmware implementation using MinPlatformPkg is constructed using the following pieces.

|                                    |
|------------------------------------|
| [EDK II](https://github.com/tianocore/edk2)                                                                              |
| [Intel(r) FSP](https://github.com/IntelFsp/FSP)                                                                            |
| [Minimum Platform (`MinPlatformPkg`)](https://github.com/tianocore/edk2-platforms/tree/master/Platform/Intel/MinPlatformPkg)                        |
| [Board Support (\<xxx\>OpenBoardPkg)](https://github.com/tianocore/edk2-platforms/tree/master/Platform/Intel)  |


## Board Support
* The `KabylakeOpenBoardPkg` contains board implementations for KabyLake systems.
* The `PurleyOpenBoardPkg` contains board implementations for Purley systems.
* The `SimicsOpenBoardPkg` contains board implementations for the Simics hardware simulator.
* The `WhiskeylakeOpenBoardPkg` contains board implementations for WhiskeyLake systems.
* The `CometlakeOpenBoardPkg` contains board implementations for CometLake systems.
* The `TigerlakeOpenBoardPkg` contains board implementations for TigerLake systems.
* The `WhitleyOpenBoardPkg` contains board implementations for Ice Lake-SP and Cooper Lake systems.

### **Supported Hardware**

#### AAEON

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| UP Xtreme                             | Whiskey Lake                               | WhiskeylakeOpenBoardPkg      | UpXtreme           |

#### Acer

***Aspire VN7-572G Laptop***

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| Aspire VN7-572G                       | SkyLake                                    | KabylakeOpenBoardPkg         | AspireVn7Dash572G  |

#### Intel

***Intel Reference and Validation Platform***

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| RVP 3                                 | SkyLake, KabyLake, KabyLake Refresh        | KabylakeOpenBoardPkg         | KabylakeRvp3       |
| WHL-U DDR4 RVP                        | WhiskeyLake                                | WhiskeylakeOpenBoardPkg      | WhiskeylakeURvp    |
| CML-U LPDDR3 RVP                      | CometLake V1                               | CometlakeOpenBoardPkg        | CometlakeURvp      |
| TGL-U DDR4 RVP                        | TigerLake                                  | TigerlakeOpenBoardPkg        | TigerlakeURvp      |
| Wilson City RVP                       | IceLake-SP (Xeon Scalable)                 | WhitleyOpenBoardPkg          | WilsonCityRvp      |
| Cooper City RVP                       | Copper Lake                                | WhitleyOpenBoardPkg          | CooperCityRvp      |

*Note: RVP = Reference and Validation Platform*

#### Microsoft

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| Mt. Olympus                           | Purley                                     | PurleyOpenBoardPkg           | BoardMtOlympus     |

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| TiogaPass                             | Purley                                     | PurleyOpenBoardPkg           | BoardTiogaPass     |


#### Simics

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| Simics Quick Start Package            | Nehalem                                    | SimicsOpenBoardPkg           | BoardX58Ich10      |

#### System 76

***Galago Pro Laptop***

| Machine Name                          | Supported Chipsets                         | BoardPkg                     | Board Name         |
----------------------------------------|--------------------------------------------|------------------------------|--------------------|
| galp2                                 | KabyLake                                   | KabylakeOpenBoardPkg         | GalagoPro3         |
| galp3 & galp3-b                       | KabyLake Refresh                           | KabylakeOpenBoardPkg         | GalagoPro3         |

## Board Package Organization
The board package follows the standard EDK II package structure with the following additional elements and guidelines:
* Only code usable across more than one board at the root level.
* Board-specific code in a directory. The directory name should match that of the board supported.
* Features not essential to achieve stage 5 or earlier boots are maintained in edk2-platforms/Features/Intel.

Shared resources in the package root directory can include interfaces described in header files, library instances,
firmware modules, binaries, etc. The UEFI firmware implementation is built using the process described below from the
board-specific directory.

A board package must implement the board APIs defined in the MinPlatformPkg even if a "NULL" implementation is used to
return back to the minimum platform caller.

## **Windows Build Instructions**

### Pre-requisites

* GIT client: Available from https://git-scm.com/downloads
* Microsoft Visual Studio.
  - Visual Studio 2015 recommended and is used in the examples below.
* ASL compiler: Available from http://www.acpica.org
  - Install into ```C:\ASL``` to match default tools_def.txt configuration.
* NASM assembler:  Available from: http://www.nasm.us/
  - Install into ```C:\NASM``` to match default tools_def.txt configuration.
* Python 3.7.3:  Available from: https://www.python.org/downloads/release/python-373/
  - Other versions of Python 3.x should be compatible.
  - It is recommended to use the Python launcher to ensure the Python build script is launched using Python 3.
    - E.g. "py -3.7 build_bios.py -l"

## **Linux Build Instructions**

### Pre-requisites

 * Set up a EDK II build environment for Linux following the instructions in
   [Using EDK II with Native GCC](https://github.com/tianocore/tianocore.github.io/wiki/Using-EDK-II-with-Native-GCC).
 * Proceed to the [Common EDK II build instructions for Linux](https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions)
   to verify your basic EDK II build environment is set up properly.

### Download the required components

1. Create a new directory for the EDK II WORKSPACE.

2. Download below repository to this WORKSPACE:

* edk2 repository
  * ``git clone https://github.com/tianocore/edk2.git``

* edk2-platforms repository
  * ``git clone https://github.com/tianocore/edk2-platforms.git``

* edk2-non-osi repository
  * ``git clone https://github.com/tianocore/edk2-non-osi.git``

* FSP repository
  * ``git clone https://github.com/IntelFsp/FSP.git``

### Board Builds

**Building with the python script**

1. Open command window, go to the workspace directory, e.g. c:\Edk2Workspace or ~/Edk2Workspace in the case of a linux OS
2. If using a linux OS
   * Type "cd edk2"
   * Type "source edksetup.sh"
   * Type "cd ../" to go back to the workspace directory
3. Type "cd edk2-platforms/Platform/Intel
4. Type "python build_bios.py -p REPLACE_WITH_BOARD_NAME"

* build_bios.py arguments:

  | Argument              | Function                            |
  | ----------------------|-------------------------------------|
  | -h, --help            | show this help message and exit     |
  | --platform, -p        | the platform to build               |
  | --toolchain, -t       | tool Chain to use in build process  |
  | --DEBUG, -d           | debug flag                          |
  | --RELEASE, -r         | release flag                        |
  | --TEST_RELEASE, -tr   | test Release flag                   |
  | --RELEASE_PDB, -rp    | release flag                        |
  | --list, -l            | lists available platforms           |
  | --cleanall            | cleans all                          |
  | --clean               | cleans specified platform           |
  | --capsule             | capsule build enabled               |
  | --silent              | silent build enabled                |
  | --performance         | performance build enabled           |
  | --fsp                 | fsp wrapper build enabled           |
  | --fspapi              | API mode fsp wrapper build enabled  |
  | --hash                | Enable hash-based caching           |
  | --binary-destination  | create cache in specified directory |
  | --binary-source       | Consume cache from directory        |
  |                                                             |

* For more information on build options
  * Type "python build_bios.py -h"

* Note
  * The Python build scripts were compatible with Python 2.7.16. But Python 2.x support is no longer maintained or recommended.

  * This python build script has been tested on Windows 10 and Ubuntu 18.04.1 LTS.

  * Unless otherwise noted, all boards build with the following components and versions:
    * Linux build: Ubuntu 18.04.1 LTS with GCC version 5.4.0
    * Windows build: Windows 10 with the Microsoft Visual Studio 2015 compiler
    * iASL version: 20190816
    * NASM version: 2.12.02

  * Unless otherwise noted all boards have been tested for boot to Windows 10 x64 RS3.

  * See [known limitations](#Known-limitations)

* Configuration Files
  * The edk2-platforms\Platform\Intel\build.cfg file contains the default settings used by build_bios.py
  * The default settings are under the DEFAULT_CONFIG section
  * Each board can have a settings file that will override the edk2-platforms\Platform\Intel\build.cfg settings
  * An example of a board specific settings:
    * edk2-platforms\Platform\Intel\KabylakeOpenBoardPkg\KabylakeRvp3\build_config.cfg

* Workspace view of the build scripts
  * <pre>
    WORKSPACE
          |------edk2
          |------edk2-non-osi
          |------edk2-platforms
          |       |---Platform
          |       |    |--Intel
          |       |        |------build.cfg: Default build settings. These are overridden by
          |       |        |                 platform specific settings (build_config.cfg) and
          |       |        |                 then command-line settings.
          |       |        |
          |       |        |------build_bios.py: Main build script. Generic pre-build, build,
          |       |        |                     post-build, and clean functions.
          |       |        |
          |       |        |------KabylakeOpenBoardPkg
          |       |        |       |------GalagoPro3
          |       |        |       |       |---build_config.cfg: System 76 Galago Pro 3 specific build
          |       |        |       |                             settings environment variables.
          |       |        |       |------KabylakeRvp3
          |       |        |               |---build_config.cfg: KabylakeRvp3 specific
          |       |        |               |                     build settings, environment variables.
          |       |        |               |---build_board.py: Optional board-specific pre-build, build
          |       |        |                                   and clean post-build functions.
          |       |        |
          |       |        |------PurleyOpenBoardPkg
          |       |        |       |------BoardMtOlympus
          |       |        |               |---build_config.cfg: BoardMtOlympus specific
          |       |        |               |                     build settings, environment variables.
          |       |        |               |---build_board.py: Optional board-specific pre-build,
          |       |        |                                   build, post-build and clean functions.
          |       |        |------PurleyOpenBoardPkg
          |       |        |       |------BoardTiogaPass
          |       |        |               |---build_config.cfg: BoardTiogaPass specific
          |       |        |               |                     build settings, environment variables.
          |       |        |               |---build_board.py: Optional board-specific pre-build,
          |       |        |                                   build, post-build and clean functions.          
          |       |        |
          |       |        |------SimicsOpenBoardPkg
          |       |        |       |------BoardX58Ich10
          |       |        |               |---build_config.cfg: BoardX58Ich10 specific
          |       |        |                                     build settings, environment variables.
          |       |        |
          |       |        |------WhiskeylakeOpenBoardPkg
          |       |        |       |------CooperCityRvp
          |       |        |       |       |---build_config.cfg: CooperCityRvp specific build
          |       |        |       |                             settings environment variables.
          |       |        |       |------WilsonCityRvp
          |       |        |               |---build_config.cfg: WilsonCityRvp specific build
          |       |        |                                     settings environment variables.
          |       |        |
          |       |        |------WhitleyOpenBoardPkg
          |       |        |       |------UpXtreme
          |       |        |               |---build_config.cfg: UpXtreme specific build
          |       |        |                                     settings environment variables.
          |       |        |       |------WhiskeylakeURvp
          |       |        |               |---build_config.cfg: WhiskeylakeURvp specific build
          |       |        |                                     settings environment variables.
          |       |        |
          |       |        |------CometlakeOpenBoardPkg
          |       |        |       |------CometlakeURvp
          |       |        |               |---build_config.cfg: CometlakeURvp specific build
          |       |        |                                     settings environment variables.
          |       |        |
          |       |        |------TigerlakeOpenBoardPkg
          |       |        |       |------TigerlakeURvp
          |       |        |               |---build_config.cfg: TigerlakeURvp specific build
          |       |        |                                     settings environment variables.
          |       |        |
          |------FSP
  </pre>

**Building with the batch scripts**

Only PurleyOpenBoardPkg still supports batch script build (in addition to Python build). Batch scripts are deprecated
and will be removed from PurleyOpenBoardPkg in the future. All other board packages must only use the Python build
infrastructure.

For PurleyOpenBoardPkg
1. Open command window, go to the workspace directory, e.g. c:\Edk2Workspace.
2. Type "cd edk2-platforms\Platform\Intel\PurleyOpenBoardPkg\BoardMtOlympus".
3. Type "GitEdk2MinMtOlympus.bat" to setup GIT environment.
4. Type "bld" to build Purley Mt Olympus board UEFI firmware image, "bld release" for release build, "bld clean" to
   remove intermediate files."bld cache-produce" Generate a cache of binary files in the specified directory,
   "bld cache-consume" Consume a cache of binary files from the specified directory, BINARY_CACHE_PATH is empty,
   used "BinCache" as default path.

For PurleyOpenBoardPkg (TiogaPass)
1. Open command window, go to the workspace directory, e.g. c:\Edk2Workspace.
2. Type "cd edk2-platforms\Platform\Intel\PurleyOpenBoardPkg\BoardTiogaPass".
3. Type "GitEdk2MinBoardTiogaPass.bat" to setup GIT environment.
4. Type "bld" to build Purley BoardTiogaPass board UEFI firmware image, "bld release" for release build, "bld clean" to
   remove intermediate files."bld cache-produce" Generate a cache of binary files in the specified directory,
   "bld cache-consume" Consume a cache of binary files from the specified directory, BINARY_CACHE_PATH is empty,
   used "BinCache" as default path.  
5. Final BIOS image will be Build\PurleyOpenBoardPkg\BoardTiagoPass\DEBUG_VS2015x86\FV\PLATFORM.fd or    
   Build\PurleyOpenBoardPkg\BoardTiagoPass\RELEASE_VS2015x86\FV\PLATFORM.fd, depending on bld batch script input.
6. This BIOS image needs to be merged with SPS FW    

### **Known limitations**

**KabylakeOpenBoardPkg**
*GalagoPro3*
1. The firmware project has not been tested on the Galago Pro 3B.

*KabylakeRvp3*
1. This firmware project has only been tested for Microsoft Windows 10 x64 boot with AHCI mode and Integrated Graphic
   Device.

**PurleyOpenBoardPkg**
1. This firmware project has only been tested booting to Microsoft Windows Server 2016 with NVME on M.2 slot.
2. This firmware project does not build with the GCC compiler.
3. The validated version of iASL compiler that can build MinPurley is 20180629. Older versions may generate ACPI build errors.

**PurleyOpenBoardPkg Tioga Pass **
1. This firmware project has only been tested on the Tioga Pass hardware.
2. This firmware project build has only been tested using the Microsoft Visual Studio 2015 build tools.
3. This firmware project does not build with the GCC compiler.
4. The validated version of iASL compiler that can build MinPurley is 20180629. Older versions may generate ACPI build errors.
5. Installed and booted to UEFI Windows 2016 on M.2 NVME slot
6. Installed and booted to UEFI Windows 2019 on M.2 NVME slot and with SATA HDD.
7. Installed and booted to UEFI RHEL 7.3 on SATA HDD
8. Installed and booted to Ubuntu 18.04 on M.2 NVME slot.
9. Verified Mellanox card detection during POST and OS
10. LINUX Boot Support (PcdLinuxBootEnable needs to be enabled)

1.	Follow directions on http://osresearch.net/Building/ to compile the heads kernel and initrd for qemu-system_x86_64 
2.	Copy the following built files 
(1) initrd.cpio.xz  to LinuxBootPkg/LinuxBinaries/initrd.cpio.xz 
(2) bzimage to LinuxBootPkg/LinuxBinaries/linux.efi



**SimicsOpenBoardPkg**
1. This firmware project has only been tested booting to Microsoft Windows 10 x64 and Ubuntu 17.10 with AHCI mode.

**WhiskeylakeOpenBoardPkg**
1. This firmware project has mainly been tested booting to Microsoft Windows 10 x64 with AHCI mode and Integrated Graphic
   Device.
2. UP Xtreme boards might hang during Windows 10 boot.
3. The UP Xtreme boards below boot to x64 windows 10 home edition and Ubuntu 18.04
      * UP Xtreme Intel(R) Core(TM) i3-8145UE CPU @ 2.20GHz with 8GB RAM
      * UP Xtreme Intel(R) Core(TM) i7-8565U CPU @ 1.80GHz with 16GB RAM
      * UP Xtreme Intel(R) Core(TM) i7-8665UE CPU @ 1.70GHz with 16GB RAM
      * UP Xtreme Intel(R) Celeron(R) CPU 4305UE @ 2.00GHz with 4GB RAM

**CometlakeOpenBoardPkg**
1. This firmware project has been tested booting to Microsoft Windows 10 x64 with AHCI mode and External Graphic Device.
2. This firmware project has been also tested booting to Ubuntu 17.10 with AHCI mode and Integrated Graphic Device.

**TigerlakeOpenBoardPkg**
1. This firmware project has been tested booting to Microsoft Windows 10 x64 with AHCI mode and Integrated Graphic Device.
2. This firmware project has been also tested booting to Puppy Linux BionicPup64 8.0 with AHCI mode and Integrated Graphic Device.

**WhitleyOpenBoardPkg**
1. This firmware project has been tested booting to UEFI shell with headless serial console

### **Package Builds**

In some cases, such as BoardModulePkg, a package may provide a set of functionality that is included in other
packages. To test the build of the whole package, the "build" command should be used following the instructions below.

1. Execute edksetup.bat (Windows) or edksetup.sh (Linux).
2. Verify the "WORKSPACE" environment variable is set to the edk2 directory in your workspace.
3. Set the "PACKAGES_PATH" environment variable to include the edk2-platforms/Platform/Intel, edk2-platforms/Silicon/Intel,
   and edk2-platforms/Features/Intel directories.
   * Windows example: set PACKAGES_PATH=c:\Edk2Workspace\edk2-platforms\Platform\Intel;
     c:\Edk2Workspace\edk2-platforms\Silicon\Intel;c:\Edk2Workspace\edk2-platforms\Features\Intel
4. Build the package by specifying the package DSC as the platform build target from the Platform/Intel or Silicon/Intel directory:
   "build -p BoardModulePkg/BoardModulePkg.dsc -a IA32 -a X64"


### **Firmware Image Flashing**

The full Intel firmware image on a flash device is called the Integrated Firmware Image (IFWI). Users with access to the Intel
proprietary FITC tool and ME ingredients can build full IFWI images that may be flashed (Descriptor, UEFI FW, ME FW, etc.).

Users without such access can directly flash a custom built UEFI FW image over the highest area of the flash region directly.
It is always recommended to have a hardware flash programmer accessible to recover the firmware image. The original full flash
image should always be backed up so it may be flashed again for recovery. Please be aware that if a system supports a technology
that authenticates the initial firmware boot image such as Boot Guard, it will fail to boot with a custom firmware image
that is not signed properly.

### **Planned Activities**
* Expand Intel's open source platform code presence through new platform and board support.
* Expand advanced feature code and quality.
* Support open source community continuous integration for Minimum Platform compliant boards.

### **Ideas**
If you would like to help but are not sure where to start some areas currently identified for improvement include:
 * Adding board ports for more motherboards and systems
 * Adding Clang support

Please feel free to contact Isaac Oram (isaac.w.oram at intel.com)
if you would like to discuss contribution ideas.
