## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  # ACPI Support
  INF RuleOverride = ACPITABLE Platform/Firefly/ROC-RK3588S-PC/AcpiTables/AcpiTables.inf

  # Splash screen logo
  INF $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
