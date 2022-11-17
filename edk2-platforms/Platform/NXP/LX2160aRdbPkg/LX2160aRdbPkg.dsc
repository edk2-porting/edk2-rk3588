#  LX2160aRdbPkg.dsc
#
#  LX2160ARDB Board package.
#
#  Copyright 2018-2020 NXP
#  Copyright 2020 Puresoftware Ltd
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
  PLATFORM_NAME                  = LX2160aRdbPkg
  PLATFORM_GUID                  = be06d8bc-05eb-44d6-b39f-191e93617ebd
  OUTPUT_DIRECTORY               = Build/LX2160aRdbPkg
  FLASH_DEFINITION               = Platform/NXP/LX2160aRdbPkg/LX2160aRdbPkg.fdf

  #
  # Network definition
  #
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE

  # This flag controls the dynamic acpi generation
  #
  DEFINE DYNAMIC_ACPI_ENABLE            = TRUE

!include Silicon/NXP/NxpQoriqLs.dsc.inc
!include MdePkg/MdeLibs.dsc.inc
!include Silicon/NXP/LX2160A/LX2160A.dsc.inc

!if $(DYNAMIC_ACPI_ENABLE) == TRUE
  !include DynamicTablesPkg/DynamicTables.dsc.inc
!endif

[LibraryClasses.common]
  ArmPlatformLib|Platform/NXP/LX2160aRdbPkg/Library/ArmPlatformLib/ArmPlatformLib.inf
  RealTimeClockLib|EmbeddedPkg/Library/VirtualRealTimeClockLib/VirtualRealTimeClockLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf

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

  Silicon/NXP/Drivers/UsbHcdInitDxe/UsbHcd.inf
  Silicon/NXP/Drivers/SataInitDxe/SataInitDxe.inf
  Platform/NXP/LX2160aRdbPkg/Drivers/PlatformDxe/PlatformDxe.inf

  #
  # PCI
  #
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  #
  # Dynamic Table Factory
  !if $(DYNAMIC_ACPI_ENABLE) == TRUE
    DynamicTablesPkg/Drivers/DynamicTableFactoryDxe/DynamicTableFactoryDxe.inf {
      <LibraryClasses>
        NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiFadtLibArm/AcpiFadtLibArm.inf
        NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiGtdtLibArm/AcpiGtdtLibArm.inf
        NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiMadtLibArm/AcpiMadtLibArm.inf
        NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiMcfgLibArm/AcpiMcfgLibArm.inf
        NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiSpcrLibArm/AcpiSpcrLibArm.inf
        NULL|Platform/NXP/LX2160aRdbPkg/AcpiTablesInclude/PlatformAcpiDsdtLib.inf
    }
  !endif

  #
  # Acpi Support
  #
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf

  #
  # Networking stack
  #
!include NetworkPkg/Network.dsc.inc
 ##
