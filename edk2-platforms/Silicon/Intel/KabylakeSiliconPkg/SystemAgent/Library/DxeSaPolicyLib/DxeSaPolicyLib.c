/** @file
  This file provide services for DXE phase policy default initialization

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DxeSaPolicyLibrary.h"
#include <Library/ConfigBlockLib.h>

extern EFI_GUID gGraphicsDxeConfigGuid;
extern EFI_GUID gMiscDxeConfigGuid;
/**
  This function prints the SA DXE phase policy.

  @param[in] SaPolicy - SA DXE Policy protocol
**/
VOID
SaPrintPolicyProtocol (
  IN  SA_POLICY_PROTOCOL      *SaPolicy
  )
{
  EFI_STATUS                  Status;
  GRAPHICS_DXE_CONFIG         *GraphicsDxeConfig;
  MISC_DXE_CONFIG             *MiscDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SaPolicy, &gGraphicsDxeConfigGuid, (VOID *)&GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SaPolicy, &gMiscDxeConfigGuid, (VOID *)&MiscDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG_CODE_BEGIN ();

  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : %x\n", SaPolicy->TableHeader.Header.Revision));
  ASSERT (SaPolicy->TableHeader.Header.Revision >= SA_POLICY_PROTOCOL_REVISION);
  DEBUG ((DEBUG_INFO, "------------------------ SA_MISC_CONFIGURATION -----------------\n"));
  DEBUG ((DEBUG_INFO, " EnableAbove4GBMmio : %x\n", MiscDxeConfig->EnableAbove4GBMmio));
  DEBUG ((DEBUG_INFO, "\n------------------------ SA Policy (DXE) print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

EFI_STATUS
EFIAPI
LoadIgdDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  GRAPHICS_DXE_CONFIG        *GraphicsDxeConfig;

  GraphicsDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "GraphicsDxeConfig->Header.GuidHob.Name = %g\n", &GraphicsDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GraphicsDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GraphicsDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the Graphics configuration
  ///
  GraphicsDxeConfig->VbtAddress = 0x00000000;
  GraphicsDxeConfig->Size = 0;
  GraphicsDxeConfig->PlatformConfig = 1;
  GraphicsDxeConfig->AlsEnable = 2;
  GraphicsDxeConfig->BacklightControlSupport = 2;
  GraphicsDxeConfig->IgdBootType = 0;
  GraphicsDxeConfig->IgdPanelType = 0;
  GraphicsDxeConfig->IgdPanelScaling = 0;
  GraphicsDxeConfig->IgdBlcConfig = 2;
  GraphicsDxeConfig->LowPowerMode = 1;
  GraphicsDxeConfig->IgdDvmtMemSize = 1;
  GraphicsDxeConfig->GfxTurboIMON = 31;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LoadMiscDxeDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  MISC_DXE_CONFIG        *MiscDxeConfig;

  MiscDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "MiscDxeConfig->Header.GuidHob.Name = %g\n", &MiscDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "MiscDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", MiscDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// RMRR Base and Limit Address for USB
  ///
  MiscDxeConfig->RmrrUsbBaseAddress = AllocateZeroPool (sizeof (EFI_PHYSICAL_ADDRESS) * 2);
  ASSERT (MiscDxeConfig->RmrrUsbBaseAddress != NULL);
  if (MiscDxeConfig->RmrrUsbBaseAddress != NULL) {
    ///
    /// BIOS must update USB RMRR base address
    ///
    MiscDxeConfig->RmrrUsbBaseAddress[0] = 0;
    MiscDxeConfig->RmrrUsbBaseAddress[1] = 0;
  }

  return EFI_SUCCESS;
}


/**
  LoadSaDxeConfigBlockDefault - Initialize default settings for each SA Config block

  @param[in] ConfigBlockPointer         The buffer pointer that will be initialized as specific config block
  @param[in] BlockId                    Request to initialize defaults of specified config block by given Block ID

  @retval EFI_SUCCESS                   The given buffer has contained the defaults of requested config block
  @retval EFI_NOT_FOUND                 Block ID is not defined so no default Config block will be initialized
**/
EFI_STATUS
EFIAPI
LoadSaDxeConfigBlockDefault (
  IN   VOID          *ConfigBlockPointer,
  IN   EFI_GUID      BlockGuid
  )
{
  if (CompareGuid (&BlockGuid, &gGraphicsDxeConfigGuid)) {
    LoadIgdDxeDefault (ConfigBlockPointer);
  } else if (CompareGuid (&BlockGuid, &gMiscDxeConfigGuid)) {
    LoadMiscDxeDefault (ConfigBlockPointer);
  } else {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}


/**
  CreateSaDxeConfigBlocks generates the config blocksg of SA DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SaPolicy               The pointer to get SA  DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateSaDxeConfigBlocks (
  IN OUT  SA_POLICY_PROTOCOL      **SaPolicy
  )
{
  UINT16                            TotalBlockSize;
  UINT16                            TotalBlockCount;
  UINT16                            BlockCount;
  VOID                              *ConfigBlockPointer;
  EFI_STATUS                        Status;
  SA_POLICY_PROTOCOL                *SaInitPolicy;
  UINT16                            RequiredSize;
  STATIC CONFIG_BLOCK_HEADER        SklSaDxeIpBlocks [] = {
       {{{0, sizeof (GRAPHICS_DXE_CONFIG),    0},  {0}},     GRAPHICS_DXE_CONFIG_REVISION,           0, {0, 0}},
       {{{0, sizeof (MISC_DXE_CONFIG),   0},  {0}},     MISC_DXE_CONFIG_REVISION,          0, {0, 0}}
};

  SaInitPolicy = NULL;
  TotalBlockCount = sizeof (SklSaDxeIpBlocks) / sizeof (CONFIG_BLOCK_HEADER);
  DEBUG ((DEBUG_INFO, "TotalBlockCount = 0x%x\n", TotalBlockCount));

  TotalBlockSize = 0;
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    TotalBlockSize += (UINT32) SklSaDxeIpBlocks[BlockCount].GuidHob.Header.HobLength;
    DEBUG ((DEBUG_INFO, "TotalBlockSize after adding  Block[0x%x]= 0x%x\n", BlockCount, TotalBlockSize));
  }

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&SaInitPolicy);
  ASSERT_EFI_ERROR (Status);
  //
  // Initalize SklSaIpBlocks table GUID
  //
  CopyMem (&SklSaDxeIpBlocks[0].GuidHob.Name,  &gGraphicsDxeConfigGuid,    sizeof (EFI_GUID));
  CopyMem (&SklSaDxeIpBlocks[1].GuidHob.Name,  &gMiscDxeConfigGuid,   sizeof (EFI_GUID));

  //
  // Initialize Policy Revision
  //
  SaInitPolicy->TableHeader.Header.Revision = SA_POLICY_PROTOCOL_REVISION;
  //
  // Initialize ConfigBlockPointer to NULL
  //
  ConfigBlockPointer = NULL;
  //
  // Loop to identify each config block from SklSaDxeIpBlocks[] Table and add each of them
  //
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    ConfigBlockPointer = (VOID *)&SklSaDxeIpBlocks[BlockCount];
    Status = AddConfigBlock ((VOID *) SaInitPolicy, (VOID *)&ConfigBlockPointer);
    ASSERT_EFI_ERROR (Status);
    LoadSaDxeConfigBlockDefault ((VOID *) ConfigBlockPointer, SklSaDxeIpBlocks[BlockCount].GuidHob.Name);
  }
  //
  // Assignment for returning SaPolicy config block base address
  //
  *SaPolicy = SaInitPolicy;
  return EFI_SUCCESS;
}


/**
  SaInstallPolicyProtocol installs SA Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] SaPolicy                   The pointer to SA Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
SaInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  SA_POLICY_PROTOCOL         *SaPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Print SA DXE Policy
  ///
  SaPrintPolicyProtocol (SaPolicy);

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gSaPolicyProtocolGuid,
                  SaPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}