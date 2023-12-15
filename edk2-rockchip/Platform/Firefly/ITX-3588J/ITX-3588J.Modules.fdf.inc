## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#  Copyright (c) 2023, Shimrra Shai <shimmyshai00@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  # ACPI Support
  INF RuleOverride = ACPITABLE Platform/Firefly/ITX-3588J/AcpiTables/AcpiTables.inf

  # Device Tree Support
  FILE FREEFORM = gDtPlatformDefaultDtbFileGuid {
    SECTION RAW = Platform/Rockchip/DeviceTree/itx-3588j.dtb
  }

  # PCA9555 GPIO chipset
  INF $(VENDOR_DIRECTORY)/Drivers/Pca9555Dxe/Pca9555Dxe.inf
  
  # Splash screen logo
  INF $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
