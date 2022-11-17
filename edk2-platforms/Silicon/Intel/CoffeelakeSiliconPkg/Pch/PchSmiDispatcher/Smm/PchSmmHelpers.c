/** @file
  Helper functions for PCH SMM dispatcher.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchSmmHelpers.h"
#include <Register/PchRegsPmc.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsItss.h>
#include <Private/Library/PmcPrivateLib.h>

///
/// #define BIT_ZERO 0x00000001
///
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32  BIT_ZERO = 0x00000001;

///
/// SUPPORT / HELPER FUNCTIONS (PCH version-independent)
///

/**
  Compare 2 SMM source descriptors' enable settings.

  @param[in] Src1                 Pointer to the PCH SMI source description table 1
  @param[in] Src2                 Pointer to the PCH SMI source description table 2

  @retval TRUE                    The enable settings of the 2 SMM source descriptors are identical.
  @retval FALSE                   The enable settings of the 2 SMM source descriptors are not identical.
**/
BOOLEAN
CompareEnables (
  CONST IN PCH_SMM_SOURCE_DESC *Src1,
  CONST IN PCH_SMM_SOURCE_DESC *Src2
  )
{
  BOOLEAN IsEqual;
  UINTN   DescIndex;

  IsEqual = TRUE;
  for (DescIndex = 0; DescIndex < NUM_EN_BITS; DescIndex++) {
    ///
    /// It's okay to compare a NULL bit description to a non-NULL bit description.
    /// They are unequal and these tests will generate the correct result.
    ///
    if (Src1->En[DescIndex].Bit != Src2->En[DescIndex].Bit ||
        Src1->En[DescIndex].Reg.Type != Src2->En[DescIndex].Reg.Type ||
        Src1->En[DescIndex].Reg.Data.raw != Src2->En[DescIndex].Reg.Data.raw
        ) {
      IsEqual = FALSE;
      break;
      ///
      /// out of for loop
      ///
    }
  }

  return IsEqual;
}

/**
  Compare a bit descriptor to the enables of source descriptor. Includes null address type.

  @param[in] BitDesc              Pointer to the PCH SMI bit descriptor
  @param[in] Src                  Pointer to the PCH SMI source description table 2

  @retval TRUE                    The bit desc is equal to any of the enables in source descriptor
  @retval FALSE                   The bid desc is not equal to all of the enables in source descriptor
**/
BOOLEAN
IsBitEqualToAnySourceEn (
  CONST IN PCH_SMM_BIT_DESC    *BitDesc,
  CONST IN PCH_SMM_SOURCE_DESC *Src
  )
{
  BOOLEAN IsEqual;
  UINTN   DescIndex;

  IsEqual = FALSE;

  for (DescIndex = 0; DescIndex < NUM_EN_BITS; ++DescIndex) {
    if ((BitDesc->Reg.Type == Src->En[DescIndex].Reg.Type) &&
        (BitDesc->Reg.Data.raw == Src->En[DescIndex].Reg.Data.raw) &&
        (BitDesc->Bit == Src->En[DescIndex].Bit)) {
      IsEqual = TRUE;
      break;
    }
  }
  return IsEqual;
}

/**
  Compare 2 SMM source descriptors' statuses.

  @param[in] Src1                 Pointer to the PCH SMI source description table 1
  @param[in] Src2                 Pointer to the PCH SMI source description table 2

  @retval TRUE                    The statuses of the 2 SMM source descriptors are identical.
  @retval FALSE                   The statuses of the 2 SMM source descriptors are not identical.
**/
BOOLEAN
CompareStatuses (
  CONST IN PCH_SMM_SOURCE_DESC *Src1,
  CONST IN PCH_SMM_SOURCE_DESC *Src2
  )
{
  BOOLEAN IsEqual;
  UINTN   DescIndex;

  IsEqual = TRUE;

  for (DescIndex = 0; DescIndex < NUM_STS_BITS; DescIndex++) {
    ///
    /// It's okay to compare a NULL bit description to a non-NULL bit description.
    /// They are unequal and these tests will generate the correct result.
    ///
    if (Src1->Sts[DescIndex].Bit != Src2->Sts[DescIndex].Bit ||
        Src1->Sts[DescIndex].Reg.Type != Src2->Sts[DescIndex].Reg.Type ||
        Src1->Sts[DescIndex].Reg.Data.raw != Src2->Sts[DescIndex].Reg.Data.raw
        ) {
      IsEqual = FALSE;
      break;
      ///
      /// out of for loop
      ///
    }
  }

  return IsEqual;
}

/**
  Compare 2 SMM source descriptors, based on Enable settings and Status settings of them.

  @param[in] Src1                 Pointer to the PCH SMI source description table 1
  @param[in] Src2                 Pointer to the PCH SMI source description table 2

  @retval TRUE                    The 2 SMM source descriptors are identical.
  @retval FALSE                   The 2 SMM source descriptors are not identical.
**/
BOOLEAN
CompareSources (
  CONST IN PCH_SMM_SOURCE_DESC *Src1,
  CONST IN PCH_SMM_SOURCE_DESC *Src2
  )
{
  return (BOOLEAN) (CompareEnables (Src1, Src2) && CompareStatuses (Src1, Src2));
}

/**
  Check if an SMM source is active.

  @param[in] Src                  Pointer to the PCH SMI source description table
  @param[in] SciEn                Indicate if SCI is enabled or not
  @param[in] SmiEnValue           Value from R_ACPI_IO_SMI_EN
  @param[in] SmiStsValue          Value from R_ACPI_IO_SMI_STS

  @retval TRUE                    It is active.
  @retval FALSE                   It is inactive.
**/
BOOLEAN
SourceIsActive (
  CONST IN PCH_SMM_SOURCE_DESC  *Src,
  CONST IN BOOLEAN              SciEn,
  CONST IN UINT32               SmiEnValue,
  CONST IN UINT32               SmiStsValue
  )
{
  UINTN   DescIndex;

  ///
  /// This source is dependent on SciEn, and SciEn == 1.  An ACPI OS is present,
  /// so we shouldn't do anything w/ this source until SciEn == 0.
  ///
  if ((Src->Flags == PCH_SMM_SCI_EN_DEPENDENT) && (SciEn)) {
    return FALSE;
  }

  ///
  /// Checking top level SMI status. If the status is not active, return false immediately
  ///
  if (!IS_BIT_DESC_NULL (Src->PmcSmiSts)) {
    if ((Src->PmcSmiSts.Reg.Type == ACPI_ADDR_TYPE) &&
        (Src->PmcSmiSts.Reg.Data.acpi == R_ACPI_IO_SMI_STS) &&
        ((SmiStsValue & (1u << Src->PmcSmiSts.Bit)) == 0)) {
      return FALSE;
    }
  }

  //
  // Special handling for NMI bit since it requires PMC IPC command.
  // Do w/a here instead of in ReadBitDesc to reduce the PMC IPC command usage.
  //
  // The PCR[ITSS].NMI register can only be accessed with BOOT_SAI and SMM_SAI.
  // Since in CFL there is no SMM_SAI it needs PMC assistance to access this register.
  //
  if ((Src->En[0].Reg.Data.Pcr.Fields.Pid == PID_ITSS) &&
      (Src->En[0].Reg.Data.Pcr.Fields.Offset == R_ITSS_PCR_NMI))
  {
    UINT32  ItssNmi;
    ItssNmi = PmcGetNmiControl ();
    if ((ItssNmi & (BIT0 << Src->En[0].Bit)) &&
        (ItssNmi & (BIT0 << Src->Sts[0].Bit)))
    {
      return TRUE;
    } else {
      return FALSE;
    }
  }

  ///
  /// Read each bit desc from hardware and make sure it's a one
  ///
  for (DescIndex = 0; DescIndex < NUM_EN_BITS; DescIndex++) {
    if (!IS_BIT_DESC_NULL (Src->En[DescIndex])) {
      if ((Src->En[DescIndex].Reg.Type == ACPI_ADDR_TYPE) &&
          (Src->En[DescIndex].Reg.Data.acpi == R_ACPI_IO_SMI_EN) &&
          ((SmiEnValue & (1u << Src->En[DescIndex].Bit)) == 0)) {
        return FALSE;
      } else if (ReadBitDesc (&Src->En[DescIndex]) == 0) {
        return FALSE;
      }
    }
  }

  ///
  /// Read each bit desc from hardware and make sure it's a one
  ///
  for (DescIndex = 0; DescIndex < NUM_STS_BITS; DescIndex++) {
    if (!IS_BIT_DESC_NULL (Src->Sts[DescIndex])) {
      if ((Src->Sts[DescIndex].Reg.Type == ACPI_ADDR_TYPE) &&
          (Src->Sts[DescIndex].Reg.Data.acpi == R_ACPI_IO_SMI_STS) &&
          ((SmiStsValue & (1u << Src->Sts[DescIndex].Bit)) == 0)) {
        return FALSE;
      } else if (ReadBitDesc (&Src->Sts[DescIndex]) == 0) {
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
  Enable the SMI source event by set the SMI enable bit, this function would also clear SMI
  status bit to make initial state is correct

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmEnableSource (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  )
{
  UINTN DescIndex;

  ///
  /// Set enables to 1 by writing a 1
  ///
  for (DescIndex = 0; DescIndex < NUM_EN_BITS; DescIndex++) {
    if (!IS_BIT_DESC_NULL (SrcDesc->En[DescIndex])) {
      WriteBitDesc (&SrcDesc->En[DescIndex], 1, FALSE);
    }
  }
  ///
  /// Clear statuses to 0 by writing a 1
  ///
  for (DescIndex = 0; DescIndex < NUM_STS_BITS; DescIndex++) {
    if (!IS_BIT_DESC_NULL (SrcDesc->Sts[DescIndex])) {
      WriteBitDesc (&SrcDesc->Sts[DescIndex], 1, TRUE);
    }
  }
}

/**
  Disable the SMI source event by clear the SMI enable bit

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmDisableSource (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  )
{
  UINTN DescIndex;

  for (DescIndex = 0; DescIndex < NUM_EN_BITS; DescIndex++) {
    if (!IS_BIT_DESC_NULL (SrcDesc->En[DescIndex])) {
      WriteBitDesc (&SrcDesc->En[DescIndex], 0, FALSE);
    }
  }
}

/**
  Clear the SMI status bit by set the source bit of SMI status register

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmClearSource (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  )
{
  UINTN DescIndex;

  for (DescIndex = 0; DescIndex < NUM_STS_BITS; DescIndex++) {
    if (!IS_BIT_DESC_NULL (SrcDesc->Sts[DescIndex])) {
      WriteBitDesc (&SrcDesc->Sts[DescIndex], 1, TRUE);
    }
  }
}

/**
  Sets the source to a 1 and then waits for it to clear.
  Be very careful when calling this function -- it will not
  ASSERT.  An acceptable case to call the function is when
  waiting for the NEWCENTURY_STS bit to clear (which takes
  3 RTCCLKs).

  @param[in] SrcDesc              Pointer to the PCH SMI source description table

**/
VOID
PchSmmClearSourceAndBlock (
  CONST PCH_SMM_SOURCE_DESC *SrcDesc
  )
{
  UINTN   DescIndex;
  BOOLEAN IsSet;

  for (DescIndex = 0; DescIndex < NUM_STS_BITS; DescIndex++) {

    if (!IS_BIT_DESC_NULL (SrcDesc->Sts[DescIndex])) {
      ///
      /// Write the bit
      ///
      WriteBitDesc (&SrcDesc->Sts[DescIndex], 1, TRUE);

      ///
      /// Don't return until the bit actually clears.
      ///
      IsSet = TRUE;
      while (IsSet) {
        IsSet = ReadBitDesc (&SrcDesc->Sts[DescIndex]);
        ///
        /// IsSet will eventually clear -- or else we'll have
        /// an infinite loop.
        ///
      }
    }
  }
}

