## @file UbaCommon.dsc
# UBA DSC include file containing common build items.
#
# @copyright
# Copyright 2012 - 2021 Intel Corporation. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

[LibraryClasses.IA32]
  UbaPlatLib|$(RP_PKG)/Library/PeiUbaPlatLib/PeiUbaPlatLib.inf
  UbaGpioInitLib|$(RP_PKG)/Library/UbaGpioInitLib/UbaGpioInitLib.inf

[Components.IA32]
  $(RP_PKG)/Uba/CfgDb/Pei/CfgDbPei.inf
  $(RP_PKG)/Uba/UbaUpdatePcds/Pei/UpdatePcdsPei.inf

[Components.X64]
  #
  # Requires a board specific library port
  #
  $(RP_PKG)/Uba/BoardInit/Dxe/BoardInitDxe.inf
  $(RP_PKG)/Uba/CfgDb/Dxe/CfgDbDxe.inf

  #
  # Common
  #
  $(RP_PKG)/Uba/UbaMain/Common/Dxe/SystemBoardInfoDxe/SystemBoardInfoDxe.inf
  $(RP_PKG)/Uba/UbaMain/Common/Dxe/SystemConfigUpdateDxe/SystemConfigUpdateDxe.inf
