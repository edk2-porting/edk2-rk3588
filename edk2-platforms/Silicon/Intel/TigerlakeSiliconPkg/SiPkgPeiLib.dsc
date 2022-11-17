## @file
#  Component description file for the TigerLake silicon package PEI libraries.
#
#  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

#
# Silicon Init Pei Library
#

#
# FRUs
#
!include $(PLATFORM_SI_PACKAGE)/Fru/TglCpu/PeiLib.dsc

!include $(PLATFORM_SI_PACKAGE)/Fru/TglPch/PeiLib.dsc

 SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf
