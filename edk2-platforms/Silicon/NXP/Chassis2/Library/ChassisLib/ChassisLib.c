/** @file
  Chassis specific functions common to all SOCs based on a specific Chessis

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Chassis.h>
#include <Uefi.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>

/**
  Or Scfg register

  @param  Address The MMIO register to read.

  @return The value read.
**/
UINT32
EFIAPI
ScfgOr32 (
  IN  UINTN     Address,
  IN  UINT32    Value
  )
{
  MMIO_OPERATIONS *ScfgOps;

  ScfgOps = GetMmioOperations (FeaturePcdGet (PcdScfgBigEndian));

  return ScfgOps->Or32 (Address, Value);
}

/**
  Read Scfg register

  @param  Address The MMIO register to read.

  @return The value read.
**/
UINT32
EFIAPI
ScfgRead32 (
  IN  UINTN     Address
  )
{
  MMIO_OPERATIONS *ScfgOps;

  ScfgOps = GetMmioOperations (FeaturePcdGet (PcdScfgBigEndian));

  return ScfgOps->Read32 (Address);
}

/**
  Write Scfg register

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

  @return Value.
**/
UINT32
EFIAPI
ScfgWrite32 (
  IN  UINTN     Address,
  IN  UINT32    Value
  )
{
  MMIO_OPERATIONS *ScfgOps;

  ScfgOps = GetMmioOperations (FeaturePcdGet (PcdScfgBigEndian));

  return ScfgOps->Write32 (Address, Value);
}

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

/*
 * Setup SMMU in bypass mode
 * and also set its pagesize
 */
STATIC
VOID
SmmuInit (
  VOID
  )
{
  UINT32 Value;

  /* set pagesize as 64K and ssmu-500 in bypass mode */
  Value = (MmioRead32 ((UINTN)SMMU_REG_SACR) | SACR_PAGESIZE_MASK);
  MmioWrite32 ((UINTN)SMMU_REG_SACR, Value);

  Value = (MmioRead32 ((UINTN)SMMU_REG_SCR0) | SCR0_CLIENTPD_MASK);
  Value &= ~SCR0_USFCFG_MASK;
  MmioWrite32 ((UINTN)SMMU_REG_SCR0, Value);

  Value = (MmioRead32 ((UINTN)SMMU_REG_NSCR0) | SCR0_CLIENTPD_MASK);
  Value &= ~SCR0_USFCFG_MASK;
  MmioWrite32 ((UINTN)SMMU_REG_NSCR0, Value);
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

  SmmuInit ();
}
