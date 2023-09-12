## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

!if $(RK_STATUS_LED_ENABLE) == TRUE
  INF Silicon/Rockchip/Drivers/StatusLedDxe/StatusLedDxe.inf
!endif

  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
