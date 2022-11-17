#  LS1043aRdbPkg.dsc
#
#  LS1043ARDB Board package.
#
#  Copyright 2017-2020 NXP
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  #
  # Defines for default states.  These can be changed on the command line.
  # -D FLAG=VALUE
  #
  PLATFORM_NAME                  = LS1043aRdbPkg
  PLATFORM_GUID                  = 60169ec4-d2b4-44f8-825e-f8684fd42e4f
  OUTPUT_DIRECTORY               = Build/LS1043aRdbPkg
  FLASH_DEFINITION               = Platform/NXP/LS1043aRdbPkg/LS1043aRdbPkg.fdf

  #
  # Network definition
  #
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE

!include Silicon/NXP/NxpQoriqLs.dsc.inc
!include MdePkg/MdeLibs.dsc.inc
!include Silicon/NXP/LS1043A/LS1043A.dsc.inc

[LibraryClasses.common]
  ArmPlatformLib|Platform/NXP/LS1043aRdbPkg/Library/ArmPlatformLib/ArmPlatformLib.inf
  RealTimeClockLib|Silicon/Maxim/Library/Ds1307RtcLib/Ds1307RtcLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf

[PcdsFixedAtBuild.common]
  #
  # RTC Pcds
  #
  gDs1307RtcLibTokenSpaceGuid.PcdI2cSlaveAddress|0x68
  gDs1307RtcLibTokenSpaceGuid.PcdI2cBusFrequency|100000

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # Architectural Protocols
  #
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <PcdsFixedAtBuild>
    gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE
  }

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  Silicon/NXP/Drivers/I2cDxe/I2cDxe.inf
  Platform/NXP/LS1043aRdbPkg/Drivers/PlatformDxe/PlatformDxe.inf

  #
  # PCI
  #
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  #
  # Networking stack
  #
!include NetworkPkg/Network.dsc.inc
 ##
