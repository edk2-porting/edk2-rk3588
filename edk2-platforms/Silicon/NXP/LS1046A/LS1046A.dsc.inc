#  LS1046A.dsc
#  LS1046A Soc package.
#
#  Copyright 2017-2020 NXP
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
#

!include Silicon/NXP/Chassis2/Chassis2.dsc.inc

[LibraryClasses.common]
  SocLib|Silicon/NXP/LS1046A/Library/SocLib/SocLib.inf
  SerialPortLib|Silicon/NXP/Library/DUartPortLib/DUartPortLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicDefault.common]

  #
  # ARM General Interrupt Controller
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x01410000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x01420000

[PcdsFixedAtBuild.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x021c0500
  gNxpQoriqLsTokenSpaceGuid.PcdNumGpioController|0x04
  gNxpQoriqLsTokenSpaceGuid.PcdGpioModuleBaseAddress|0x02300000
  gNxpQoriqLsTokenSpaceGuid.PcdGpioControllerOffset|0x10000

  gNxpQoriqLsTokenSpaceGuid.PcdUsbBaseAddr|0x2F00000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbSize|0x100000
  gNxpQoriqLsTokenSpaceGuid.PcdNumUsbController|3

[PcdsFeatureFlag]
  gNxpQoriqLsTokenSpaceGuid.PcdDcfgBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdScfgBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdGpioControllerBigEndian|TRUE

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

##
