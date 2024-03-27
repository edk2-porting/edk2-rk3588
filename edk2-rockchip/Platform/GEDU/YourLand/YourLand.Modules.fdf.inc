## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  # ACPI Support
  INF RuleOverride = ACPITABLE $(PLATFORM_DIRECTORY)/AcpiTables/AcpiTables.inf

  # Device Tree Support
  FILE FREEFORM = gDtPlatformDefaultDtbFileGuid {
    SECTION RAW = Platform/Rockchip/DeviceTree/yourland.dtb
  }

  # Splash screen logo
  INF $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf

  #
  # Multimedia Card Interface, added on 2023-11-6 when enable boot from emmc 
  #
  #EmbeddedPkg/Universal/MmcDxe/MmcDxe.inf
  #Silicon/Synopsys/DesignWare/Drivers/DwEmmcDxe/DwEmmcDxe.inf
  INF Silicon/Rockchip/Drivers/MmcDxe/MmcDxe.inf
  #Silicon/Rockchip/Drivers/DwEmmcDxe/DwEmmcDxe.inf
  INF Silicon/Rockchip/Drivers/SdhciHostDxe/SdhciHostDxe.inf

  #
  # SPI TEST
  #
  # Silicon/Rockchip/Library/SpiLib/SpiTest.inf
  INF MdeModulePkg/Application/CapsuleApp/CapsuleApp.inf
  # reset support on 2023-12-28
  INF EmbeddedPkg/ResetRuntimeDxe/ResetRuntimeDxe.inf