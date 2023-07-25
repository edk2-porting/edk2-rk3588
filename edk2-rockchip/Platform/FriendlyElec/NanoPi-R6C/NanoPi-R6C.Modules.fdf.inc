## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#  Copyright (c) 2023, Sergey Tyuryukanov <s199p.wa1k9r@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

  # ACPI Support
  INF RuleOverride = ACPITABLE $(PLATFORM_DIRECTORY)/AcpiTables/AcpiTables.inf

  # Splash screen logo
  INF $(VENDOR_DIRECTORY)/Drivers/LogoDxe/LogoDxe.inf
