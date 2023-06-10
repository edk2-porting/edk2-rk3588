## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  #
  # The current drivers only support PCIe 3 x4, which is not present on RK3588S.
  #
  DEFINE RK_PCIE_ENABLE = FALSE

!include Silicon/Rockchip/RK3588/RK3588Base.dsc.inc

################################################################################
#
# Pcd Section - list of all common EDK II PCD Entries used by RK3588S platforms.
#
################################################################################

[PcdsFixedAtBuild.common]
  #
  # DWC3 controller
  #
  gRockchipTokenSpaceGuid.PcdDwc3BaseAddresses|{ UINT32(0xfc000000), UINT32(0xfcd00000) }
