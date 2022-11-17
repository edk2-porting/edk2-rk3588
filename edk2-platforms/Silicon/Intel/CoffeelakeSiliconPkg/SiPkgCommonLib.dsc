## @file
#  Component description file for the Coffee Lake silicon package both PEI and DXE libraries DSC file.
#
# Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# Set PCH generation according PCD.
# The DEFINE will be used to select PCH library INF file corresponding to PCH generation
#
DEFINE  PCH = Cnl

[LibraryClasses.common]
#
# Cpu
#
 CpuPlatformLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf
 CpuMailboxLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/BaseCpuMailboxLibNull/BaseCpuMailboxLibNull.inf

#
# Me
#

#
# Pch
#
 PchCycleDecodingLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchCycleDecodingLib/PeiDxeSmmPchCycleDecodingLib.inf
 PchGbeLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchGbeLib/PeiDxeSmmPchGbeLib.inf
 PchInfoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchInfoLib/PeiDxeSmmPchInfoLib$(PCH).inf
 SataLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmSataLib/PeiDxeSmmSataLib$(PCH).inf
 PchPcieRpLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcieRpLib/PeiDxeSmmPchPcieRpLib.inf
 PchPcrLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
 PmcLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPmcLib/PeiDxeSmmPmcLib.inf
 GbeMdiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmGbeMdiLib/PeiDxeSmmGbeMdiLib.inf


 PchSbiAccessLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSbiAccessLib/PeiDxeSmmPchSbiAccessLib.inf
 GpioLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmGpioLib/PeiDxeSmmGpioLib.inf
!if gSiPkgTokenSpaceGuid.PcdSerialIoUartEnable == TRUE
 PchSerialIoUartLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSerialIoUartLib/PeiDxeSmmPchSerialIoUartLib.inf
!else
 PchSerialIoUartLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BasePchSerialIoUartLibNull/BasePchSerialIoUartLibNull.inf
!endif
 PchSerialIoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSerialIoLib/PeiDxeSmmPchSerialIoLibCnl.inf
 PchEspiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchEspiLib/PeiDxeSmmPchEspiLib.inf
 PchWdtCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchWdtCommonLib/PeiDxeSmmPchWdtCommonLib.inf
 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
 SmbusLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseSmbusLib/BaseSmbusLib.inf
 BiosLockLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmBiosLockLib/PeiDxeSmmBiosLockLib.inf
 #private
 PchPciExpressHelpersLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchPciExpressHelpersLib/PeiDxeSmmPchPciExpressHelpersLib.inf
 PchInitCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchInitCommonLib/PeiDxeSmmPchInitCommonLib.inf
 PchSpiCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/BasePchSpiCommonLib/BasePchSpiCommonLib.inf
 GpioPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmGpioPrivateLib/PeiDxeSmmGpioPrivateLibCnl.inf
 PchPsfPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchPsfPrivateLib/PeiDxeSmmPchPsfPrivateLib$(PCH).inf
 PmcPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLibCnl.inf
 PmcPrivateLibWithS3|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLibWithS3.inf
 PchDmiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchDmiLib/PeiDxeSmmPchDmiLib.inf
 PchDmiWithS3Lib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchDmiLib/PeiDxeSmmPchDmiWithS3Lib.inf
 SiScheduleResetLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/BaseSiScheduleResetLib/BaseSiScheduleResetLib.inf

#
# SA
#
 SaPlatformLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/PeiDxeSmmSaPlatformLib/PeiDxeSmmSaPlatformLib.inf

#
# Memory
#
