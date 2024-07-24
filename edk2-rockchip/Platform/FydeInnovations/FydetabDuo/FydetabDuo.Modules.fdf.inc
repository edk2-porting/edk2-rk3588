## @file
#
#  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  # ACPI Support
  INF RuleOverride = ACPITABLE $(PLATFORM_DIRECTORY)/AcpiTables/AcpiTables.inf

  # Device Tree Support
  FILE FREEFORM = gDtPlatformDefaultDtbFileGuid {
    SECTION RAW = Platform/Rockchip/DeviceTree/rk3588s-tablet-12c-linux.dtb
  }

  # Splash screen logo
  INF $(PLATFORM_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
