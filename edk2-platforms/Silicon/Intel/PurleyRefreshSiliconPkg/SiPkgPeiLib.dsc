## @file
#  Build description file for Purley Refresh silicon PEI libraries.
#
# Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.common.PEIM]
  CsrToPcieLib|$(PLATFORM_SI_PACKAGE)/Library/CsrToPcieLib/CsrToPciePeiLib.inf
  PcieAddrLib|$(PLATFORM_SI_PACKAGE)/Library/PcieAddressLib/PcieAddressLib.inf
