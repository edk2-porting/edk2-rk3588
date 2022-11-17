/** @file
  This file is SampleCode of the library for Intel HD Audio Verb Table configuration.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <ConfigBlock.h>
#include <PlatformBoardId.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HdaVerbTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

extern HDAUDIO_VERB_TABLE HdaVerbTableDisplayAudio;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc274;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc700;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc701;
extern HDAUDIO_VERB_TABLE WhlHdaVerbTableAlc700;

/**
  Add verb table helper function.
  This function calculates verbtable number and shows verb table information.

  @param[in,out] VerbTableEntryNum      Input current VerbTable number and output the number after adding new table
  @param[in,out] VerbTableArray         Pointer to array of VerbTable
  @param[in]     VerbTable              VerbTable which is going to add into array
**/
STATIC
VOID
InternalAddVerbTable (
  IN OUT  UINT8                   *VerbTableEntryNum,
  IN OUT  UINT32                  *VerbTableArray,
  IN      HDAUDIO_VERB_TABLE      *VerbTable
  )
{
  if (VerbTable == NULL) {
    DEBUG ((DEBUG_INFO, "InternalAddVerbTable wrong input: VerbTable == NULL\n"));
    return;
  }

  VerbTableArray[*VerbTableEntryNum] = (UINT32) VerbTable;
  *VerbTableEntryNum += 1;

  DEBUG ((DEBUG_INFO,
    "HDA: Add verb table for vendor = 0x%04X devId = 0x%04X (size = %d DWords)\n",
    VerbTable->Header.VendorId,
    VerbTable->Header.DeviceId,
    VerbTable->Header.DataDwords)
    );
}

/**
  Add verb table function.
  This function update the verb table number and verb table ptr of policy.

  @param[in]  HdAudioConfig            HD Audio config block
  @param[out] VerbTableEntryNum        Number of verb table entries
  @param[out] HdaVerbTablePtr          Pointer to the verb table
**/
VOID
AddPlatformVerbTables (
  IN   UINT8              CodecType,
  OUT  UINT8              *VerbTableEntryNum,
  OUT  UINT32             *HdaVerbTablePtr
  )
{
  UINT8                   VerbTableEntries;
  UINT32                  VerbTableArray[6];
  UINT32                  *VerbTablePtr;

  VerbTableEntries = 0;

  InternalAddVerbTable (&VerbTableEntries, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdDisplayAudioHdaVerbTable));

  if (CodecType == PchHdaCodecPlatformOnboard) {
    DEBUG ((DEBUG_INFO, "HDA Policy: Onboard codec selected\n"));
    InternalAddVerbTable (&VerbTableEntries, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable));
    InternalAddVerbTable (&VerbTableEntries, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable2));
  } else {
    DEBUG ((DEBUG_INFO, "HDA Policy: External codec kit selected\n"));
    InternalAddVerbTable (&VerbTableEntries, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdCommonHdaVerbTable1));
    InternalAddVerbTable (&VerbTableEntries, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdCommonHdaVerbTable2));
    InternalAddVerbTable (&VerbTableEntries, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdCommonHdaVerbTable3));
  }

  *VerbTableEntryNum = VerbTableEntries;

  VerbTablePtr = (UINT32 *) AllocateZeroPool (sizeof (UINT32) * VerbTableEntries);
  CopyMem (VerbTablePtr, VerbTableArray, sizeof (UINT32) * VerbTableEntries);
  *HdaVerbTablePtr = (UINT32) VerbTablePtr;
}

/**
  HDA VerbTable init function for PEI post memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
HdaVerbTableInit (
  IN UINT16 BoardId
  )
{
  HDAUDIO_VERB_TABLE *VerbTable;
  HDAUDIO_VERB_TABLE *VerbTable2;

  VerbTable = NULL;
  VerbTable2 = NULL;

  switch (BoardId) {

    case BoardIdWhiskeyLakeRvp:
      VerbTable = &WhlHdaVerbTableAlc700;
      break;

    default:
      DEBUG ((DEBUG_INFO, "HDA: Init default verb tables (Realtek ALC700 and ALC701)\n"));
      VerbTable = &HdaVerbTableAlc700;
      VerbTable2 = &HdaVerbTableAlc701;
      break;
  }

  PcdSet32S (PcdHdaVerbTable, (UINT32) VerbTable);
  PcdSet32S (PcdHdaVerbTable2, (UINT32) VerbTable2);
  PcdSet32S (PcdDisplayAudioHdaVerbTable, (UINT32) &HdaVerbTableDisplayAudio);

  // Codecs - Realtek ALC700, ALC701, ALC274 (external - connected via HDA header)
  PcdSet32S (PcdCommonHdaVerbTable1, (UINT32) &HdaVerbTableAlc700);
  PcdSet32S (PcdCommonHdaVerbTable2, (UINT32) &HdaVerbTableAlc701);
  PcdSet32S (PcdCommonHdaVerbTable3, (UINT32) &HdaVerbTableAlc274);

  return EFI_SUCCESS;
}
