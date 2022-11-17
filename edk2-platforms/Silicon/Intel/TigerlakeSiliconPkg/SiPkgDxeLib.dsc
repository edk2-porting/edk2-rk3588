## @file
#  Component description file for the TigerLake silicon package DXE libraries.
#
#  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# Silicon Init Dxe Library
#

#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/TglCpu/DxeLib.dsc
!include $(PLATFORM_SI_PACKAGE)/Fru/TglPch/DxeLib.dsc

#
# Common
#
SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf

#
# Pch
#
DxePchPolicyLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/DxePchPolicyLib/DxePchPolicyLib.inf
SmmPchPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/SmmPchPrivateLib/SmmPchPrivateLib.inf

#
# SystemAgent
#
DxeSaPolicyLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/DxeSaPolicyLib/DxeSaPolicyLib.inf
DxeVtdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vtd/LibraryPrivate/DxeVtdPolicyLib/DxeVtdPolicyLib.inf

#
#  CPU PCIe IpBlock
#

DxeCpuPcieRpLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuPcieRp/LibraryPrivate/DxeCpuPcieRpLib/DxeCpuPcieRpLib.inf
