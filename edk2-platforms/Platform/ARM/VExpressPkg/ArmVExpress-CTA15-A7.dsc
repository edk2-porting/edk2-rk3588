#
#  Copyright (c) 2012-2018, ARM Limited. All rights reserved.
#  Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = ArmVExpressPkg-CTA15-A7
  PLATFORM_GUID                  = 0b511920-978d-4b34-acc0-3d9f8e6f9d81
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
!ifdef $(EDK2_OUT_DIR)
  OUTPUT_DIRECTORY               = $(EDK2_OUT_DIR)
!else
  OUTPUT_DIRECTORY               = Build/ArmVExpress-CTA15-A7
!endif
  SUPPORTED_ARCHITECTURES        = ARM
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/ARM/VExpressPkg/ArmVExpress-CTA15-A7.fdf

  DEFINE EDK2_SKIP_PEICORE = 1
  DEFINE ARM_BIGLITTLE_TC2 = 1 # We build for the TC2 hardware by default

!include Platform/ARM/VExpressPkg/ArmVExpress.dsc.inc
!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses.common]
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmMmuLib|ArmPkg/Library/ArmMmuLib/ArmMmuBaseLib.inf
  ArmPlatformLib|Platform/ARM/VExpressPkg/Library/ArmVExpressLibCTA15-A7/ArmVExpressLib.inf

  ArmPlatformSysConfigLib|Platform/ARM/VExpressPkg/Library/ArmVExpressSysConfigLib/ArmVExpressSysConfigLib.inf

  #DebugAgentTimerLib|Platform/ARM/VExpressPkg/Library/DebugAgentTimerLib/DebugAgentTimerLib.inf

  # ARM General Interrupt Driver in Secure and Non-secure
  ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicLib.inf

  LcdHwLib|ArmPlatformPkg/Library/HdLcd/HdLcd.inf
  LcdPlatformLib|Platform/ARM/VExpressPkg/Library/HdLcdArmVExpressLib/HdLcdArmVExpressLib.inf

  ArmSmcLib|ArmPkg/Library/ArmSmcLib/ArmSmcLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  ArmPlatformSysConfigLib|Platform/ARM/VExpressPkg/Library/ArmVExpressSysConfigRuntimeLib/ArmVExpressSysConfigRuntimeLib.inf

[LibraryClasses.ARM]
  ArmSoftFloatLib|ArmPkg/Library/ArmSoftFloatLib/ArmSoftFloatLib.inf
  #
  # PSCI support in EL3 may not be available if we are not running under a PSCI
  # compliant secure firmware. Assume PSCI on AARCH64, and fall back to the
  # syscfg MMIO register implementation on ARM.
  # This will not work at actual runtime.
  #
  ResetSystemLib|Platform/ARM/VExpressPkg/Library/ResetSystemLib/ResetSystemLib.inf

[BuildOptions]
!ifdef ARM_BIGLITTLE_TC2
  *_*_ARM_ARCHCC_FLAGS  = -DARM_BIGLITTLE_TC2=1
  *_*_ARM_PP_FLAGS  = -DARM_BIGLITTLE_TC2=1
!endif

  RVCT:*_*_ARM_PLATFORM_FLAGS == --cpu Cortex-A15 -I$(WORKSPACE)/Platform/ARM/VExpressPkg/Include/Platform/CTA15-A7

  GCC:*_*_ARM_PLATFORM_FLAGS == -mcpu=cortex-a15 -I$(WORKSPACE)/Platform/ARM/VExpressPkg/Include/Platform/CTA15-A7

  XCODE:*_*_ARM_PLATFORM_FLAGS = -I$(WORKSPACE)/Platform/ARM/VExpressPkg/Include/Platform/CTA15-A7

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]
  gArmTokenSpaceGuid.PcdNormalMemoryNonshareableOverride|TRUE
  gArmPlatformTokenSpaceGuid.PcdSendSgiToBringUpSecondaryCores|TRUE

  ## If TRUE, Graphics Output Protocol will be installed on virtual handle created by ConsplitterDxe.
  #  It could be set FALSE to save size.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE

[PcdsFixedAtBuild.common]
  gArmVExpressTokenSpaceGuid.PcdAndroidFastbootProductName|"ARM Versatile Express"

  gArmPlatformTokenSpaceGuid.PcdCoreCount|5

  #
  # NV Storage PCDs. Use base of 0x0C000000 for NOR1
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase|0x0FFC0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize|0x00010000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase|0x0FFD0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize|0x00010000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase|0x0FFE0000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize|0x00010000

  gArmTokenSpaceGuid.PcdVFPEnabled|1

  # System Memory (1GB) - An additional 1GB will be added if UEFI is running on a 2GB Test Chip
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x40000000

!ifdef ARM_BIGLITTLE_TC2
  # TC2 Dual-Cluster profile
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  # Core Ids and Gic values
  # A15_0 = 0x000, GicCoreId = 0
  # A15_1 = 0x001, GicCoreId = 1
  #  A7_0 = 0x100, GicCoreId = 2
  #  A7_1 = 0x101, GicCoreId = 3
  #  A7_2 = 0x102, GicCoreId = 4
  gArmTokenSpaceGuid.PcdArmPrimaryCore|0x100
!endif

  #
  # ARM PrimeCell
  #

  ## SP805 Watchdog - Motherboard Watchdog
  gArmPlatformTokenSpaceGuid.PcdSP805WatchdogBase|0x1C0F0000

  ## PL011 - Serial Terminal
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x1C090000
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|38400
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultReceiveFifoDepth|0

  ## PL031 RealTimeClock
  gArmPlatformTokenSpaceGuid.PcdPL031RtcBase|0x1C170000

!ifdef ARM_BIGLITTLE_TC2
  ## PL111 Lcd & HdLcd
  gArmPlatformTokenSpaceGuid.PcdPL111LcdBase|0x1C1F0000
  gArmPlatformTokenSpaceGuid.PcdArmHdLcdBase|0x2B000000
  gArmVExpressTokenSpaceGuid.PcdHdLcdVideoModeOscId|5
!endif

  #
  # PL180 MMC/SD card controller
  #
  gArmVExpressTokenSpaceGuid.PcdPL180SysMciRegAddress|0x1C010048
  gArmVExpressTokenSpaceGuid.PcdPL180MciBaseAddress|0x1C050000


  #
  # ARM Generic Interrupt Controller
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x2C001000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x2C002000

  # ISP1761 USB OTG Controller
  gArmVExpressTokenSpaceGuid.PcdIsp1761BaseAddress|0x1B000000

  # Ethernet (SMSC LAN9118)
  gArmVExpressTokenSpaceGuid.PcdLan9118DxeBaseAddress|0x1A000000
  gArmVExpressTokenSpaceGuid.PcdLan9118DefaultNegotiationTimeout|400000

  #
  # Define the device path to the FDT for the platform
  #
  gFdtPlatformDxeTokenSpaceGuid.PcdFdtDevicePaths|L"VenHw(93E34C7E-B50E-11DF-9223-2443DFD72085,00)/ca15a7"

  #
  # ARM Architectural Timer Frequency
  #
!ifdef ARM_BIGLITTLE_TC2
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|24000000
!else
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|10000000
!endif

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # PEI Phase modules
  #
  ArmPlatformPkg/PrePi/PeiMPCore.inf {
    <LibraryClasses>
      ArmPlatformLib|Platform/ARM/VExpressPkg/Library/ArmVExpressLibCTA15-A7/ArmVExpressLib.inf
  }

  #
  # DXE
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
  }

  #
  # Architectural Protocols
  #
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      NULL|EmbeddedPkg/Library/NvVarStoreFormattedLib/NvVarStoreFormattedLib.inf
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  }
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf

  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  ArmPlatformPkg/Drivers/NorFlashDxe/NorFlashDxe.inf
  ArmPlatformPkg/Drivers/LcdGraphicsOutputDxe/LcdGraphicsOutputDxe.inf
  ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  ArmPlatformPkg/Drivers/SP805WatchdogDxe/SP805WatchdogDxe.inf

  #
  # Platform
  #
  Platform/ARM/VExpressPkg/Drivers/ArmVExpressDxe/ArmHwDxe.inf

  #
  # Filesystems
  #
!ifndef ARM_BIGLITTLE_TC2
  ArmPkg/Filesystem/SemihostFs/SemihostFs.inf
!endif

  #
  # Multimedia Card Interface
  #
  EmbeddedPkg/Universal/MmcDxe/MmcDxe.inf
  Platform/ARM/VExpressPkg/Drivers/PL180MciDxe/PL180MciDxe.inf

  # SMSC LAN 9118
  Platform/ARM/VExpressPkg/Drivers/Lan9118Dxe/Lan9118Dxe.inf

  #
  # FAT filesystem + GPT/MBR partitioning
  #
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf

  #
  # Bds
  #
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
  }

  #
  # FDT installation
  #
  Platform/ARM/Drivers/FdtPlatformDxe/FdtPlatformDxe.inf {
    <LibraryClasses>
      BdsLib|Platform/ARM/Library/BdsLib/BdsLib.inf
  }
