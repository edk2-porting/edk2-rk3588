## @file
#  Build description file for Kaby Lake silicon PEI and DXE libraries.
#
# Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.common]
#
# Silicon Init Common Library
#

#
# Common
#
 MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmMmPciLib/PeiDxeSmmMmPciLib.inf

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
 PchHsioLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchHsioLib/PeiDxeSmmPchHsioLib.inf
 PchInfoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchInfoLib/PeiDxeSmmPchInfoLib.inf
 PchP2sbLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchP2sbLib/PeiDxeSmmPchP2sbLib.inf
 PchPcieRpLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcieRpLib/PeiDxeSmmPchPcieRpLib.inf
 PchPcrLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
 PchPmcLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPmcLib/PeiDxeSmmPchPmcLib.inf
 PchSbiAccessLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSbiAccessLib/PeiDxeSmmPchSbiAccessLib.inf
 GpioLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmGpioLib/PeiDxeSmmGpioLib.inf
 PchSerialIoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSerialIoLib/PeiDxeSmmPchSerialIoLib.inf
 PchWdtCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchWdtCommonLib/PeiDxeSmmPchWdtCommonLib.inf
 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
 SmbusLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseSmbusLib/BaseSmbusLib.inf
 #private
 PchPciExpressHelpersLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeSmmPchPciExpressHelpersLib/PeiDxeSmmPchPciExpressHelpersLib.inf
 PchInitCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeSmmPchInitCommonLib/PeiDxeSmmPchInitCommonLib.inf
 PchSmbusCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeSmmPchSmbusCommonLib/PeiDxeSmmPchSmbusCommonLib.inf
 PchSpiCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/BasePchSpiCommonLib/BasePchSpiCommonLib.inf
 GpioPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeSmmGpioPrivateLib/PeiDxeSmmGpioPrivateLib.inf
 I2cMasterCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeI2cMasterCommonLib/PeiDxeI2cMasterCommonLib.inf
 PchPsfPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeSmmPchPsfPrivateLib/PeiDxeSmmPchPsfPrivateLib.inf
 PchPsfLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPsfLib/PeiDxeSmmPchPsfLib.inf
 RstPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/PeiDxeSmmRstPrivateLib/PeiDxeSmmRstPrivateLib.inf


#
# SA
#
 SaPlatformLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/PeiDxeSmmSaPlatformLib/PeiDxeSmmSaPlatformLib.inf

#
# Memory
#

