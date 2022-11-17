/** @file
  PCH SMM private lib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Register/CommonMsr.h>

/**
  Set InSmm.Sts bit
**/
VOID
PchSetInSmmSts (
  VOID
  )
{
  UINT32      Data32;


  ///
  /// Read memory location FED30880h OR with 00000001h, place the result in EAX,
  /// and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///
  Data32 = MmioRead32 (0xFED30880);
  AsmWriteMsr32 (MSR_SPCL_CHIPSET_USAGE, Data32 | BIT0);
  ///
  /// Read FED30880h back to ensure the setting went through.
  ///
  Data32 = MmioRead32 (0xFED30880);
}

/**
  Clear InSmm.Sts bit
**/
VOID
PchClearInSmmSts (
  VOID
  )
{
  UINT32      Data32;

  ///
  /// Read memory location FED30880h AND with FFFFFFFEh, place the result in EAX,
  /// and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///
  Data32 = MmioRead32 (0xFED30880);
  AsmWriteMsr32 (MSR_SPCL_CHIPSET_USAGE, Data32 & (UINT32) (~BIT0));
  ///
  /// Read FED30880h back to ensure the setting went through.
  ///
  Data32 = MmioRead32 (0xFED30880);
}
