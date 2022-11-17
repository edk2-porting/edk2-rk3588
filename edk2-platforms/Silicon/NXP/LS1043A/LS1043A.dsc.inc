#  LS1043A.dsc
#  LS1043A Soc package.
#
#  Copyright 2017-2020 NXP
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#

!include Silicon/NXP/Chassis2/Chassis2.dsc.inc

[LibraryClasses.common]
  SocLib|Silicon/NXP/LS1043A/Library/SocLib/SocLib.inf
  SerialPortLib|Silicon/NXP/Library/DUartPortLib/DUartPortLib.inf
  SerDesHelperLib|Silicon/NXP/Library/SerDesHelperLib/SerDesHelperLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicDefault.common]

  #
  # ARM General Interrupt Controller
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x01401000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x01402000

[PcdsFixedAtBuild.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x021c0500

[PcdsFeatureFlag]
  gNxpQoriqLsTokenSpaceGuid.PcdDcfgBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdPciLutBigEndian|TRUE

[PcdsFixedAtBuild.common]
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseAddr|0x4000000000
  gNxpQoriqLsTokenSpaceGuid.PcdNumPciController|3
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutBase|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutDbg|0x7FC
  gNxpQoriqLsTokenSpaceGuid.PcdSerDesLanes|0x4
##
