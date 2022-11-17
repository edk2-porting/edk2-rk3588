## @file
#  Build description file for Purley Refresh silicon DXE libraries.
#
# Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.common.DXE_DRIVER]
  MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/DxeMmPciBaseLib/DxeMmPciBaseLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/DxeMmPciBaseLib/DxeMmPciBaseLib.inf
  ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/DxeRuntimeResetSystemLib/DxeRuntimeResetSystemLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/DxeMmPciBaseLib/SmmMmPciBaseLib.inf
  CsrToPcieLib|$(PLATFORM_SI_PACKAGE)/Library/CsrToPcieLib/CsrToPcieDxeLib.inf
  SpiFlashCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/SmmSpiFlashCommonLib/SmmSpiFlashCommonLib.inf

[LibraryClasses.X64.UEFI_APPLICATION]
