/** @file
  This file contains functions for PCH DMI SIP14

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Private/Library/PchDmiLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsDmi.h>
#include <Register/PchRegsDmi14.h>
#include <Register/PchRegsPcr.h>

/**
  This function checks if DMI SIP14 Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmi14Locked (
  VOID
  )
{
  return ((PchPcrRead32 (PID_DMI, R_PCH_DMI14_PCR_DMIC) & B_PCH_DMI14_PCR_DMIC_SRL) != 0);
}

/**
  Enable PCIe Relaxed Order for DMI SIP14
**/
VOID
PchDmi14EnablePcieRelaxedOrder (
  VOID
  )
{
  //
  // Enable Forced Relaxed Ordering to always allow downstream completions to pass posted writes.
  // Set Completion Relaxed Ordering Attribute Override Value
  // and Completion Relaxed Ordering Attribute Override Enable
  //
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI14_PCR_2314, ~0u, (BIT31 | BIT7));
}

/**
 Secure Register Lock data

 @param[out] SrlRegOffset        Register offset holding Secure Register Lock setting
 @param[out] SrlRegMask          Mask for Secure Register Lock setting
**/
VOID
PchDmi14SrlRegData (
  OUT UINT16  *SrlRegOffset,
  OUT UINT32  *SrlRegMask
  )
{
  *SrlRegMask = B_PCH_DMI14_PCR_DMIC_SRL;
  *SrlRegOffset = R_PCH_DMI14_PCR_DMIC;
}
