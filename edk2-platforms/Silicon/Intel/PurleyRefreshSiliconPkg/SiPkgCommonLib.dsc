## @file
#  Build description file for Purley Refresh silicon PEI and DXE libraries.
#
# Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.common]

  #
  # Uncore
  #
  PcieAddrLib|$(PLATFORM_SI_PACKAGE)/Library/PcieAddressLib/PcieAddressLib.inf
  SiliconAccessLib|$(PLATFORM_SI_PACKAGE)/Library/UsraAccessLib/UsraAccessLib.inf
  CsrToPcieLib|$(PLATFORM_SI_PACKAGE)/Library/CsrToPcieLibNull/BaseCsrToPcieLibNull.inf
  PcieAddrLib|$(PLATFORM_SI_PACKAGE)/Library/PcieAddressLib/PcieAddressLib.inf
  MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/MmPciBaseLib/MmPciBaseLib.inf

  #
  # Pch
  #
  GpioLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmGpioLib/PeiDxeSmmGpioLib.inf
  PchPolicyLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiPchPolicyLib/PeiPchPolicyLib.inf
  PchCycleDecodingLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchCycleDecodingLib/PeiDxeSmmPchCycleDecodingLib.inf
  PchGbeLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchGbeLib/PeiDxeSmmPchGbeLib.inf
  PchInfoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchInfoLib/PeiDxeSmmPchInfoLib.inf
  PchP2sbLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchP2sbLib/PeiDxeSmmPchP2sbLib.inf
  PchPcrLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
  PchSbiAccessLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSbiAccessLib/PeiDxeSmmPchSbiAccessLib.inf
  PchResetCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/LibraryPrivate/BasePchResetCommonLib/BasePchResetCommonLib.inf
  PchPmcLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPmcLib/PeiDxeSmmPchPmcLib.inf
