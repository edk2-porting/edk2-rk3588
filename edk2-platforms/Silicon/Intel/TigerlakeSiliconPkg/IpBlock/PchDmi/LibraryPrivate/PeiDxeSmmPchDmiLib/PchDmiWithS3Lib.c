/** @file
  PCH DMI library with S3 boot script support.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchDmiRegs.h>

#include "PchDmi14.h"

/**
  Configure DMI Lock
**/
VOID
PchDmiSetLockWithS3BootScript (
  VOID
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;
  UINT16  Address;

  Data32And = 0xFFFFFFFF;

  PchDmi14SrlRegData (&Address, &Data32Or);

  PchPcrAndThenOr32 (
    PID_DMI, Address,
    Data32And,
    Data32Or
    );
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_DMI, Address,
    &Data32Or,
    &Data32And
    );
}

/**
  Set BIOS interface Lock-Down
**/
VOID
PchDmiSetBiosLockDownWithS3BootScript (
  VOID
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  //
  // Set BIOS Lock-Down (BILD)
  // When set, prevents GCS.BBS from being changed
  //
  Data32And = 0xFFFFFFFF;
  Data32Or = B_PCH_DMI_PCR_BILD;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI_PCR_GCS, Data32And, Data32Or);
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_DMI, R_PCH_DMI_PCR_GCS,
    &Data32Or,
    &Data32And
    );
}
