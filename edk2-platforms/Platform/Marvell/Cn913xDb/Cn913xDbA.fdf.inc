#
#  Copyright (C) 2019 Marvell International Ltd. and its affiliates
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

# Per-board additional content of the DXE phase firmware volume

  INF Silicon/Marvell/Drivers/Gpio/MvPca95xxDxe/MvPca95xxDxe.inf
  INF Silicon/Marvell/Drivers/Gpio/MvGpioDxe/MvGpioDxe.inf

  # DTB
  INF RuleOverride = DTB Silicon/Marvell/OcteonTx/DeviceTree/T91/$(PLATFORM_NAME).inf

  # ACPI support
!if $(ARCH) == AARCH64
!ifndef $(CN9132)
  INF RuleOverride = ACPITABLE Silicon/Marvell/OcteonTx/AcpiTables/T91/$(PLATFORM_NAME).inf
!else
  INF RuleOverride = ACPITABLE Silicon/Marvell/OcteonTx/AcpiTables/T91/Cn9131DbA.inf
!endif
!endif
