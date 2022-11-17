/** @file
  This file is PeiMePolicy library.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiMePolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

/**
  Load default settings for ME config block in pre-mem phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiPreMemDefault (
  IN VOID                           *ConfigBlockPointer
  );

/**
  Load default settings for ME config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiDefault (
  IN VOID                           *ConfigBlockPointer
  );

STATIC COMPONENT_BLOCK_ENTRY  mMeCompontBlockPreMemBlocks [] = {
  {&gMePeiPreMemConfigGuid, sizeof (ME_PEI_PREMEM_CONFIG),  ME_PEI_PREMEM_CONFIG_REVISION,  LoadMePeiPreMemDefault}
};

STATIC COMPONENT_BLOCK_ENTRY  mMeCompontBlockBlocks [] = {
  {&gMePeiConfigGuid,       sizeof (ME_PEI_CONFIG),         ME_PEI_CONFIG_REVISION,         LoadMePeiDefault}
};

/**
  Load default settings for ME config block in pre-mem phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiPreMemDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_PEI_PREMEM_CONFIG *MePeiPreMemConfig;
  MePeiPreMemConfig = ConfigBlockPointer;

  MePeiPreMemConfig->HeciTimeouts                  = 1;

  MePeiPreMemConfig->Heci1BarAddress               = 0xFED1A000;
  MePeiPreMemConfig->Heci2BarAddress               = 0xFED1B000;
  MePeiPreMemConfig->Heci3BarAddress               = 0xFED1C000;

  //
  // Test policies
  //
  MePeiPreMemConfig->SendDidMsg                    = 1;

  MePeiPreMemConfig->KtDeviceEnable                = 1;
}

/**
  Load default settings for ME config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_PEI_CONFIG *MePeiConfig;
  MePeiConfig = ConfigBlockPointer;

  MePeiConfig->EndOfPostMessage     = EOP_SEND_IN_DXE;
  MePeiConfig->MeUnconfigOnRtcClear = 1;
}

/**
  Dump values of ME config block in pre-mem phase.

  @param[in] MePeiPreMemConfig                     The pointer to the config block
**/
VOID
EFIAPI
PrintMePeiPreMemConfig (
  IN ME_PEI_PREMEM_CONFIG               *MePeiPreMemConfig
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "------------------------ ME_PEI_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                  : 0x%x\n", MePeiPreMemConfig->Header.Revision));
  ASSERT (MePeiPreMemConfig->Header.Revision == ME_PEI_PREMEM_CONFIG_REVISION);

  DEBUG ((DEBUG_INFO, " HeciTimeouts              : 0x%x\n", MePeiPreMemConfig->HeciTimeouts));
  DEBUG ((DEBUG_INFO, " DidInitStat               : 0x%x\n", MePeiPreMemConfig->DidInitStat));
  DEBUG ((DEBUG_INFO, " DisableCpuReplacedPolling : 0x%x\n", MePeiPreMemConfig->DisableCpuReplacedPolling));
  DEBUG ((DEBUG_INFO, " SendDidMsg                : 0x%x\n", MePeiPreMemConfig->SendDidMsg));
  DEBUG ((DEBUG_INFO, " DisableHeciRetry          : 0x%x\n", MePeiPreMemConfig->DisableHeciRetry));
  DEBUG ((DEBUG_INFO, " DisableMessageCheck       : 0x%x\n", MePeiPreMemConfig->DisableMessageCheck));
  DEBUG ((DEBUG_INFO, " SkipMbpHob                : 0x%x\n", MePeiPreMemConfig->SkipMbpHob));
  DEBUG ((DEBUG_INFO, " HeciCommunication2        : 0x%x\n", MePeiPreMemConfig->HeciCommunication2));
  DEBUG ((DEBUG_INFO, " KtDeviceEnable            : 0x%x\n", MePeiPreMemConfig->KtDeviceEnable));
  DEBUG ((DEBUG_INFO, " Heci1BarAddress           : 0x%x\n", MePeiPreMemConfig->Heci1BarAddress));
  DEBUG ((DEBUG_INFO, " Heci2BarAddress           : 0x%x\n", MePeiPreMemConfig->Heci2BarAddress));
  DEBUG ((DEBUG_INFO, " Heci3BarAddress           : 0x%x\n", MePeiPreMemConfig->Heci3BarAddress));
  DEBUG_CODE_END ();
}

/**
  Dump values of ME config block in PEI phase.

  @param[in] MePeiConfig                    The pointer to the config block
**/
VOID
EFIAPI
PrintMePeiConfig (
  IN ME_PEI_CONFIG              *MePeiConfig
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "------------------------ ME_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                  : 0x%x\n", MePeiConfig->Header.Revision));
  ASSERT (MePeiConfig->Header.Revision == ME_PEI_CONFIG_REVISION);

  DEBUG ((DEBUG_INFO, " MctpBroadcastCycle        : 0x%x\n", MePeiConfig->MctpBroadcastCycle));
  DEBUG ((DEBUG_INFO, " EndOfPostMessage          : 0x%x\n", MePeiConfig->EndOfPostMessage));
  DEBUG ((DEBUG_INFO, " Heci3Enabled              : 0x%x\n", MePeiConfig->Heci3Enabled));
  DEBUG ((DEBUG_INFO, " DisableD0I3SettingForHeci : 0x%x\n", MePeiConfig->DisableD0I3SettingForHeci));
  DEBUG ((DEBUG_INFO, " MeUnconfigOnRtcClear      : 0x%x\n", MePeiConfig->MeUnconfigOnRtcClear));

  DEBUG_CODE_END ();
}

/**
  Print PEI ME config block

  @param[in] SiPolicyPpiPreMem The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPpiPreMem
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_PEI_PREMEM_CONFIG              *MePeiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpiPreMem, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Pre-Mem Print Begin -----------------\n"));
  PrintMePeiPreMemConfig (MePeiPreMemConfig);
  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Pre-Mem Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  Print PEI ME config block

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpi (
  IN  SI_POLICY_PPI *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_PEI_CONFIG                     *MePeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Print Begin -----------------\n"));
  PrintMePeiConfig (MePeiConfig);
  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  Get ME config block table total size.

  @retval        Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mMeCompontBlockPreMemBlocks[0], sizeof (mMeCompontBlockPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Get ME config block table total size.

  @retval        Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mMeCompontBlockBlocks[0], sizeof (mMeCompontBlockBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  MeAddConfigBlocksPreMem add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "Me AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mMeCompontBlockPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mMeCompontBlockPreMemBlocks[0], sizeof (mMeCompontBlockPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  MeAddConfigBlocks add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "ME AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mMeCompontBlockBlocks) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mMeCompontBlockBlocks[0], sizeof (mMeCompontBlockBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
