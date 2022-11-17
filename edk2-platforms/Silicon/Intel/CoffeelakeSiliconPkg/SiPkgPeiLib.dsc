## @file
#  Component description file for the Coffee Lake silicon package PEI libraries.
#
# Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses]
#
# Silicon Init Pei Library
#
 SiPolicyLib|$(PLATFORM_SI_PACKAGE)/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf
 SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf
 StallPpiLib|$(PLATFORM_SI_PACKAGE)/Library/PeiInstallStallPpiLib/PeiStallPpiLib.inf

#
# Pch
#
 PchPolicyLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiPchPolicyLib/PeiPchPolicyLibCnl.inf
!if gSiPkgTokenSpaceGuid.PcdOcWdtEnable == TRUE
 OcWdtLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiOcWdtLib/PeiOcWdtLib.inf
!else
 OcWdtLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiOcWdtLibNull/PeiOcWdtLibNull.inf
!endif
 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiResetSystemLib/PeiResetSystemLib.inf
 PchResetLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiPchResetLib/PeiPchResetLib.inf
 SpiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiSpiLib/PeiSpiLib.inf
 GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiGpioHelpersLib/PeiGpioHelpersLib.inf
 GpioNameBufferLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiGpioNameBufferLib/PeiGpioNameBufferLib.inf

#
# Me
#
 PeiMePolicyLib|$(PLATFORM_SI_PACKAGE)/Me/Library/PeiMePolicyLib/PeiMePolicyLib.inf

#
# SA
#
  PeiSaPolicyLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/PeiSaPolicyLib/PeiSaPolicyLib.inf
#
# Cpu
#
 CpuPolicyLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiCpuPolicyLib/PeiCpuPolicyLib.inf
