/** @file
  This file provides services for Hda policy function

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <HdAudioConfig.h>

/**
  Print HDAUDIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
HdaDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS            Status;
  HDAUDIO_DXE_CONFIG    *HdaDxeConfig;
  UINT32                Index;

  Status = GetConfigBlock ((VOID *) PchPolicy, &gHdAudioDxeConfigGuid, (VOID *) &HdaDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ HD-Audio DXE Config ------------------\n"));

  for (Index = 0; Index < PCH_MAX_HDA_SNDW_LINK_NUM; Index++) {
    DEBUG ((DEBUG_INFO, " SNDW%d AutonomousClockStop  : %d\n", Index, HdaDxeConfig->SndwConfig[Index].AutonomousClockStop));
    DEBUG ((DEBUG_INFO, " SNDW%d DODS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnDelaySelect));
    DEBUG ((DEBUG_INFO, " SNDW%d DOAS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnActiveIntervalSelect));
  }
  DEBUG ((DEBUG_INFO, " DSP Feature Mask              : 0x%x\n", HdaDxeConfig->DspFeatureMask));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
HdaDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  HDAUDIO_DXE_CONFIG  *HdAudioDxeConfig;
  HdAudioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Name = %g\n", &HdAudioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdAudioDxeConfig->Header.GuidHob.Header.HobLength));
}

STATIC COMPONENT_BLOCK_ENTRY  mHdaBlocks = {
  &gHdAudioDxeConfigGuid,
  sizeof (HDAUDIO_DXE_CONFIG),
  HDAUDIO_DXE_CONFIG_REVISION,
  HdaDxeLoadConfigDefault
};

/**
  Get Hda config block table size.

  @retval      Size of config block
**/
UINT16
HdaDxeGetConfigBlockTotalSize (
  VOID
  )
{
  return mHdaBlocks.Size;
}

/**
  Add Hda ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
HdaDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mHdaBlocks, 1);
}
