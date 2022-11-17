/** @file
  Chassis specific functions common to all SOCs based on a specific Chessis

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Chassis.h>
#include <Uefi.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>

/**
  Read Dcfg register

  @param  Address The MMIO register to read.

  @return The value read.
**/
UINT32
EFIAPI
DcfgRead32 (
  IN  UINTN     Address
  )
{
  MMIO_OPERATIONS *DcfgOps;

  DcfgOps = GetMmioOperations (FeaturePcdGet (PcdDcfgBigEndian));

  return DcfgOps->Read32 (Address);
}

/**
  Write Dcfg register

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

  @return Value.
**/
UINT32
EFIAPI
DcfgWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
  MMIO_OPERATIONS *DcfgOps;

  DcfgOps = GetMmioOperations (FeaturePcdGet (PcdDcfgBigEndian));

  return DcfgOps->Write32 (Address, Value);
}

/**
  Function to initialize Chassis Specific functions
 **/
VOID
ChassisInit (
  VOID
  )
{
  //
  // Early init serial Port to get board information.
  //
  SerialPortInitialize ();
}
