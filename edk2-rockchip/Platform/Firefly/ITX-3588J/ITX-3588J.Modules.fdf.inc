## @file
#
#  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
#  Copyright (c) 2023, Shimrra Shai <shimmyshai00@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  # ACPI Support
  INF RuleOverride = ACPITABLE Platform/Firefly/ITX-3588J/AcpiTables/AcpiTables.inf

  # Device Tree Support
  INF RuleOverride = DTB Platform/Firefly/ITX-3588J/DeviceTree/Vendor.inf

  # Splash screen logo
  INF $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
