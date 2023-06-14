## @file
#
#  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

!include Silicon/Rockchip/RK3588/RK3588Base.dsc.inc

################################################################################
#
# Pcd Section - list of all common EDK II PCD Entries used by RK3588S platforms.
#
################################################################################

[PcdsFixedAtBuild.common]
  # SMBIOS platform config
  gRockchipTokenSpaceGuid.PcdProcessorName|"Rockchip RK3588S"

  #
  # DWC3 controller
  #
  gRockchipTokenSpaceGuid.PcdDwc3BaseAddresses|{ UINT32(0xfc000000), UINT32(0xfcd00000) }

  #
  # PCI Express 3.0 support flags and default values
  #
  gRK3588TokenSpaceGuid.PcdPcie30Supported|FALSE
