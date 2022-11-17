/** @file

  Header file for the Intel HD Audio Verb Table library.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _HDA_VERB_TABLE_LIB_H_
#define _HDA_VERB_TABLE_LIB_H_

#include <ConfigBlock/HdAudioConfig.h>
#include <Library/BaseLib.h>

enum HDAUDIO_CODEC_SELECT {
  PchHdaCodecPlatformOnboard = 0,
  PchHdaCodecExternalKit     = 1
};

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
  );

/**
  HDA VerbTable init function for PEI post memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
HdaVerbTableInit(
  IN UINT16 BoardId
  );

#endif