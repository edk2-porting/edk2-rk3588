/** @file
  This file provide services for DXE phase policy default initialization

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <ConfigBlock/HdAudioConfig.h>
#include <ConfigBlock/GpioDevConfig.h>

/**
  Load DXE Config block default for HD Audio

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHdAudioDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HDAUDIO_DXE_CONFIG  *HdAudioDxeConfig;
  HdAudioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Name = %g\n", &HdAudioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdAudioDxeConfig->Header.GuidHob.Header.HobLength));

  HdAudioDxeConfig->DspEndpointDmic = PchHdaDmic4chArray;
  HdAudioDxeConfig->NhltDefaultFlow = TRUE;
}

/**
  Load DXE Config block default for GPIO

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadGpioDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GPIO_DXE_CONFIG  *GpioDxeConfig;
  GpioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Name = %g\n", &GpioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GpioDxeConfig->Header.GuidHob.Header.HobLength));

  GpioDxeConfig->HideGpioAcpiDevice = 0;
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchDxeIpBlocks [] = {
  {&gHdAudioDxeConfigGuid, sizeof (PCH_HDAUDIO_DXE_CONFIG), HDAUDIO_DXE_CONFIG_REVISION, LoadHdAudioDxeConfigDefault},
  {&gGpioDxeConfigGuid, sizeof (PCH_GPIO_DXE_CONFIG), GPIO_DXE_CONFIG_REVISION, LoadGpioDxeConfigDefault}
};

/**
  Print PCH_HDAUDIO_DXE_CONFIG.

  @param[in] HdaDxeConfig         Pointer to a PCH_HDAUDIO_DXE_CONFIG that provides the HD Audio settings
**/
VOID
PchPrintHdAudioDxeConfig (
  IN CONST PCH_HDAUDIO_DXE_CONFIG   *HdaDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH HD-Audio DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DSP Endpoint: DMIC : %d\n", HdaDxeConfig->DspEndpointDmic));
  DEBUG ((DEBUG_INFO, " DSP Endpoint: I2S  : %d\n", HdaDxeConfig->DspEndpointI2s));
  DEBUG ((DEBUG_INFO, " DSP Endpoint: BT   : %d\n", HdaDxeConfig->DspEndpointBluetooth));
  DEBUG ((DEBUG_INFO, " DSP Feature Mask   : 0x%x\n", HdaDxeConfig->DspFeatureMask));
  DEBUG ((DEBUG_INFO, " Nhlt Default Flow  : %d\n", HdaDxeConfig->NhltDefaultFlow));
}

/**
  Print PCH_GPIO_DXE_CONFIG.

  @param[in] GpioDxeConfig         Pointer to a PCH_GPIO_DXE_CONFIG that provides the GPIO settings
**/
VOID
PchPrintGpioDxeConfig (
  IN CONST PCH_GPIO_DXE_CONFIG   *GpioDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH GPIO DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " HideGpioAcpiDevice : %d\n", GpioDxeConfig->HideGpioAcpiDevice));
}

/**
  This function prints the PCH DXE phase policy.

  @param[in] PchPolicy - PCH DXE Policy protocol
**/
VOID
PchPrintPolicyProtocol (
  IN  PCH_POLICY_PROTOCOL      *PchPolicy
  )
{
  DEBUG_CODE_BEGIN();
  EFI_STATUS                 Status;
  PCH_HDAUDIO_DXE_CONFIG     *HdaDxeConfig;
  PCH_GPIO_DXE_CONFIG        *GpioDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) PchPolicy, &gHdAudioDxeConfigGuid, (VOID *)&HdaDxeConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) PchPolicy, &gGpioDxeConfigGuid, (VOID *)&GpioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %x\n", PchPolicy->TableHeader.Header.Revision));

  PchPrintHdAudioDxeConfig (HdaDxeConfig);
  PchPrintGpioDxeConfig (GpioDxeConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print End --------------------------\n"));
  DEBUG_CODE_END();
}

/**
  CreatePchDxeConfigBlocks generates the config blocksg of PCH DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPolicy                 The pointer to get PCH DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreatePchDxeConfigBlocks (
  IN OUT  PCH_POLICY_PROTOCOL      **DxePchPolicy
  )
{
  UINT16              TotalBlockSize;
  EFI_STATUS          Status;
  PCH_POLICY_PROTOCOL *PchPolicyInit;
  UINT16              RequiredSize;


  DEBUG ((DEBUG_INFO, "PCH Create Dxe Config Blocks\n"));

  PchPolicyInit = NULL;

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  PchPolicyInit->TableHeader.Header.Revision = PCH_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status =  AddComponentConfigBlocks ((VOID *) PchPolicyInit, &mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *DxePchPolicy = PchPolicyInit;
  return Status;
}

/**
  PchInstallPolicyProtocol installs PCH Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] SaPolicy                   The pointer to SA Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
PchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  )
{

  EFI_STATUS            Status;

  ///
  /// Print PCH DXE Policy
  ///
  PchPrintPolicyProtocol (PchPolicy);

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchPolicyProtocolGuid,
                  PchPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
