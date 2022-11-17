/** @file
  This file contains internal PSF routines for PCH PSF VER2 lib usage

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegsPsf.h>
#include "PsfLibInternal.h"
#include <PchPcieRpInfo.h>


/**
  Get EOI register data for given PSF ID

  @param[in]  PsfId           PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[out] EoiTargetBase   EOI Target register
  @param[out] EoiControlBase  EOI Control register

  @retval MaxTargets          Number of supported targets

**/
UINT8
PsfEoiRegData (
  UINT32        PsfId,
  UINT16        *EoiTargetBase,
  UINT16        *EoiControlBase
  )
{
  UINT8  MaxTargets;

  MaxTargets = 0;
  *EoiTargetBase = 0;
  *EoiControlBase = 0;

  switch (PsfId) {
    case 1:
      break;

    case 3:
      break;

    case 7:
      break;

    case 8:
      break;

    case 9:
      break;

    default:
      break;

  }
  return MaxTargets;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT_DEST_ID PchLpRpDestId[] =
{
  {0x18000}, {0x18001}, {0x18002}, {0x18003}, // SPA: PSF1, PortID = 0
  {0x18100}, {0x18101}, {0x18102}, {0x18103}, // SPB: PSF1, PortID = 1
  {0x18200}, {0x18201}, {0x18202}, {0x18203}, // SPC: PSF1, PortID = 2
};

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PsfPcieDestinationId (
  IN UINT32  RpIndex
  )
{
  if (RpIndex < ARRAY_SIZE (PchLpRpDestId)) {
    return PchLpRpDestId[RpIndex];
  }
  ASSERT (FALSE);
  return (PSF_PORT_DEST_ID){0};
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_SEGMENT mPchLpPsfTable[] =
{
  {1, PID_PSF1},
  {2, PID_PSF2},
  {3, PID_PSF3},
  {4, PID_PSF4},
  {5, PID_CSME_PSF},
  {6, PID_PSF6}
};

/**
  Get list of supported PSF segments.

  @param[out] PsfTable        Array of supported PSF segments
  @param[out] PsfTableLength  Length of PsfTable
**/
VOID
PsfSegments (
  OUT PSF_SEGMENT  **PsfTable,
  OUT UINT32       *PsfTableLength
  )
{
*PsfTable = mPchLpPsfTable;
    *PsfTableLength = ARRAY_SIZE (mPchLpPsfTable);
}
