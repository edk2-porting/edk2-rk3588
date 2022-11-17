#
#  Copyright (C) 2018 Marvell International Ltd. and its affiliates
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

# Per-board additional content of the DXE phase firmware volume

  INF Silicon/Marvell/Drivers/Gpio/MvPca95xxDxe/MvPca95xxDxe.inf

  # DTB
  INF RuleOverride = DTB Silicon/Marvell/Armada7k8k/DeviceTree/Armada80x0Db.inf

!if $(ARCH) == AARCH64
  # ACPI support
  INF RuleOverride = ACPITABLE Silicon/Marvell/Armada7k8k/AcpiTables/Armada80x0Db.inf
!endif
