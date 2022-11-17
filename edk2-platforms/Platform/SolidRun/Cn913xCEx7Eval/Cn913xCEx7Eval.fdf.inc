#
#  Copyright (c) 2021 Semihalf
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

# Per-board additional content of the DXE phase firmware volume

  INF Silicon/Marvell/Drivers/Gpio/MvGpioDxe/MvGpioDxe.inf

  # DTB
  INF RuleOverride = DTB Silicon/Marvell/OcteonTx/DeviceTree/T91/$(PLATFORM_NAME).inf

  # ACPI support
!if $(ARCH) == AARCH64
  INF RuleOverride = ACPITABLE Silicon/Marvell/OcteonTx/AcpiTables/T91/$(PLATFORM_NAME).inf
!endif
