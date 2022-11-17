/** @file
  Default PCI Segment Information Library that returns one segment whose
  segment base address equals to PcdPciExpressBaseAddress.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/PciSegmentInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

#define PCI_SEGMENT_INFO_HOB_GUID { \
  0x6eab7169, 0x9b21, 0x450e, { 0x99, 0xe8, 0xe, 0xb1, 0x1b, 0x7d, 0xfe, 0xcb } \
  }

EFI_GUID  mPciSegmentInfoHobGuid = PCI_SEGMENT_INFO_HOB_GUID;

volatile PCI_SEGMENT_INFO  mPciSegmentInfo;

/**
  Return an array of PCI_SEGMENT_INFO holding the segment information.

  Note: The returned array/buffer is owned by callee.

  @param  Count  Return the count of segments.

  @retval A callee owned array holding the segment information.
**/
PCI_SEGMENT_INFO *
EFIAPI
GetPciSegmentInfo (
  OUT UINTN  *Count
  )
{
  VOID              *Hob;
  PCI_SEGMENT_INFO  *PciSegmentInfo;

  if (Count == NULL) {
    return NULL;
  }

  *Count = 1;
  Hob = GetFirstGuidHob (&mPciSegmentInfoHobGuid);
  if (Hob != NULL) {
    PciSegmentInfo = GET_GUID_HOB_DATA(Hob);
  } else {
    mPciSegmentInfo.SegmentNumber  = 0;
    mPciSegmentInfo.BaseAddress    = PcdGet64(PcdPciExpressBaseAddress);
    mPciSegmentInfo.StartBusNumber = 0;
    mPciSegmentInfo.EndBusNumber   = (UINT8)((PcdGet32 (PcdPciExpressRegionLength) / 0x100000) - 1);

    DEBUG ((DEBUG_INFO, "mPciSegmentInfo.BaseAddress - 0x%x\n", mPciSegmentInfo.BaseAddress));

    if (mPciSegmentInfo.BaseAddress == 0) {
      // Premem phase
      PciSegmentInfo = BuildGuidHob (&mPciSegmentInfoHobGuid, sizeof(PCI_SEGMENT_INFO));
      ASSERT(PciSegmentInfo != NULL);
      if (PciSegmentInfo == NULL) {
        return NULL;
      }
      PciSegmentInfo->SegmentNumber  = 0;
      PciSegmentInfo->BaseAddress    = PcdGet64(PcdPciExpressBaseAddress);
      PciSegmentInfo->StartBusNumber = 0;
      PciSegmentInfo->EndBusNumber   = (UINT8)((PcdGet32 (PcdPciExpressRegionLength) / 0x100000) - 1);
    } else {
      PciSegmentInfo = (VOID *)&mPciSegmentInfo;
    }
  }
  return PciSegmentInfo;
}
