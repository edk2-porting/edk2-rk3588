## @file
#  Flash Description include file for RD-N2-Cfg1 platform.
#
#  Copyright (c) 2021, Arm Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

# Per-platform additional content of the DXE phase firmware volume

  # ACPI support
  INF RuleOverride=ACPITABLE Platform/ARM/SgiPkg/AcpiTables/RdN2Cfg1AcpiTables.inf
