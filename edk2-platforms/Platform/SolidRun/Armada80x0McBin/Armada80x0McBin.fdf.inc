#
#  Copyright (C) 2018 Marvell International Ltd. and its affiliates
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

# Per-board additional content of the DXE phase firmware volume

  INF Silicon/Marvell/Drivers/Gpio/MvGpioDxe/MvGpioDxe.inf

  # DTB
  INF RuleOverride = DTB Silicon/Marvell/Armada7k8k/DeviceTree/Armada80x0McBin.inf

!if $(ARCH) == AARCH64
  INF RuleOverride = ACPITABLE Silicon/Marvell/Armada7k8k/AcpiTables/Armada80x0McBin.inf
!endif
