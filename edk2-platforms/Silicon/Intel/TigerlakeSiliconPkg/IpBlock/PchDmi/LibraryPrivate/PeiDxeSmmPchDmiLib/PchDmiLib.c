/** @file
  PCH DMI library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchRegsLpc.h>

#include "PchDmi14.h"

/**
  This function checks if DMI Secured Register Lock (SRL) is set

  @retval SRL state
**/
BOOLEAN
IsPchDmiLocked (
  VOID
  )
{
  return IsPchDmi14Locked ();
}

/**
  Get PCH TCO base address.

  @retval Address                   Address of TCO base address.
**/
UINT16
PchDmiGetTcoBase (
  VOID
  )
{
  //
  // Read "TCO Base Address" PCR[DMI] + 2778h[15:5]
  //
  return (PchPcrRead16 (PID_DMI, R_PCH_DMI_PCR_TCOBASE) & B_PCH_DMI_PCR_TCOBASE_TCOBA);
}

/**
  Set PCH LPC/eSPI generic IO range decoding in DMI

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] RangeIndex                 Index of choosen range

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcGenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length,
  IN  UINT32                            RangeIndex
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set PCH eSPI eSPI CS1# generic IO range decoding in DMI

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetEspiCs1GenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set PCH LPC/eSPI memory range decoding in DMI

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcMemRange (
  IN  UINT32                            Address
  )
{
  if (IsPchDmiLocked ()) {
    DEBUG ((DEBUG_ERROR, "%a Error. DMI is locked.\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Program LPC Memory Range, PCR[DMI] + 2740h to the same value programmed in LPC/eSPI PCI Offset 98h.
  //
  PchPcrWrite32 (
    PID_DMI, R_PCH_DMI_PCR_LPCGMR,
    (Address | B_LPC_CFG_LGMR_LMRD_EN)
    );

  return EFI_SUCCESS;
}

/**
  Set PCH eSPI CS1# memory range decoding in DMI

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetEspiCs1MemRange (
  IN  UINT32                            Address
  )
{
  if (IsPchDmiLocked ()) {
    DEBUG ((DEBUG_ERROR, "%a Error. DMI is locked.\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Program LPC Memory Range, PCR[DMI] + 27C0h to the same value programmed in eSPI PCI Offset A8h.
  //
  PchPcrWrite32 (
    PID_DMI, R_PCH_DMI_PCR_SEGMR,
    (Address | B_LPC_CFG_LGMR_LMRD_EN)
    );

  return EFI_SUCCESS;
}

/**
  Check if Boot BIOS Strap is set for SPI.

  @retval TRUE                Boot BIOS Strap set for SPI
  @retval FALSE               Boot BIOS Strap set for LPC/eSPI
**/
BOOLEAN
PchDmiIsBootBiosStrapSetForSpi (
  VOID
  )
{
  //
  // Check General Control and Status (GCS) [10]
  // '0': SPI
  // '1': LPC/eSPI
  //
  return ((PchPcrRead32 (PID_DMI, R_PCH_DMI_PCR_GCS) & B_PCH_DMI_PCR_BBS) != B_PCH_DMI_PCR_BBS);
}

/**
  Set PCH BIOS range decoding in DMI
  Please check EDS for detail of BiosDecodeEnable bit definition.
    bit 15: F8-FF Enable
    bit 14: F0-F8 Enable
    bit 13: E8-EF Enable
    bit 12: E0-E8 Enable
    bit 11: D8-DF Enable
    bit 10: D0-D7 Enable
    bit  9: C8-CF Enable
    bit  8: C0-C7 Enable
    bit  7: Legacy F Segment Enable
    bit  6: Legacy E Segment Enable
    bit  5: Reserved
    bit  4: Reserved
    bit  3: 70-7F Enable
    bit  2: 60-6F Enable
    bit  1: 50-5F Enable
    bit  0: 40-4F Enable

  @param[in] BiosDecodeEnable           Bios decode enable setting.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetBiosDecodeEnable (
  IN  UINT16                            BiosDecodeEnable
  )
{
  if (IsPchDmiLocked ()) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program LPC BIOS Decode Enable, PCR[DMI] + 2744h to the same value programmed in LPC or SPI Offset D8h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_DMI_PCR_LPCBDE, BiosDecodeEnable);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO decode ranges in DMI
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          LPC/eSPI IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoDecodeRanges (
  IN  UINT16                            LpcIoDecodeRanges
  )
{
  //
  // This cycle decoding is only allowed to set when DMI is not locked.
  //
  if (IsPchDmiLocked ()) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program LPC I/O Decode Ranges, PCR[DMI] + 2770h[15:0] to the same value programmed in LPC/eSPI PCI offset 80h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_DMI_PCR_LPCIOD, LpcIoDecodeRanges);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO enable decoding in DMI

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchDmiSetLpcIoEnable (
  IN  UINT16                            LpcIoEnableDecoding
  )
{
  //
  // This cycle decoding is only allowed to set when DMI is not locked.
  //
  if (IsPchDmiLocked ()) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // program LPC I/O Decode Ranges, PCR[DMI] + 2774h[15:0] to the same value programmed in LPC/eSPI PCI offset 82h.
  //
  PchPcrWrite16 (PID_DMI, R_PCH_DMI_PCR_LPCIOE, LpcIoEnableDecoding);
  return EFI_SUCCESS;
}
