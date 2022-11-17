## @file
#  Component description file for the Coffee Lake silicon package DXE libraries.
#
# Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.common.DXE_CORE,LibraryClasses.common.DXE_SMM_DRIVER,LibraryClasses.common.SMM_CORE,LibraryClasses.common.DXE_DRIVER,LibraryClasses.common.DXE_RUNTIME_DRIVER,LibraryClasses.common.UEFI_DRIVER]
#
# Silicon Init Dxe Library
#

#
# Common
#
!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
 AslUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
!else
 AslUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/DxeAslUpdateLibNull/DxeAslUpdateLibNull.inf
!endif
 SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf

#
# Pch
#
 PchHdaLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/DxePchHdaLib/DxePchHdaLib.inf
 ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/DxeResetSystemLib/DxeResetSystemLib.inf
 DxePchPolicyLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/DxePchPolicyLib/DxePchPolicyLib.inf
 GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf
 GpioNameBufferLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/DxeGpioNameBufferLib/DxeGpioNameBufferLib.inf
 SmmPchPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/SmmPchPrivateLib/SmmPchPrivateLib.inf

#
# SystemAgent
#
 DxeSaPolicyLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/DxeSaPolicyLib/DxeSaPolicyLib.inf
