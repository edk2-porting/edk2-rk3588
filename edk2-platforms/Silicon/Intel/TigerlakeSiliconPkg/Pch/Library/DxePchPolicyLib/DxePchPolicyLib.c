/** @file
  This file provide services for DXE phase policy default initialization

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <ScsConfig.h>
#include <Library/DxeGpioPolicyLib.h>
#include <Library/DxeHdaPolicyLib.h>
#include <Library/DxePchPcieRpPolicyLib.h>

/**
  Load DXE Config block default for eMMC

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadEmmcDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig;
  EmmcDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "EmmcDxeConfig->Header.GuidHob.Name = %g\n", &EmmcDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "EmmcDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", EmmcDxeConfig->Header.GuidHob.Header.HobLength));

  EmmcDxeConfig->DriverStrength = DriverStrength40Ohm;
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchDxeIpBlocks [] = {
  {&gEmmcDxeConfigGuid,    sizeof (SCS_EMMC_DXE_CONFIG), SCS_EMMC_DXE_CONFIG_REVISION, LoadEmmcDxeConfigDefault}
};

/**
  Print SCS_EMMC_DXE_CONFIG.

  @param[in] EmmcDxeConfig         Pointer to a SCS_EMMC_DXE_CONFIG that provides the eMMC settings
**/
VOID
PchPrintEmmcDxeConfig (
  IN CONST SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH eMMC DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DriverStrength : %d\n", EmmcDxeConfig->DriverStrength));
  DEBUG ((DEBUG_INFO, " EnableSoftwareHs400Tuning: %d\n", EmmcDxeConfig->EnableSoftwareHs400Tuning));
  DEBUG ((DEBUG_INFO, " TuningLba : %X\n", EmmcDxeConfig->TuningLba));
  DEBUG ((DEBUG_INFO, " Previous tuning success : %d\n", EmmcDxeConfig->PreviousTuningResults.TuningSuccessful));
  if (EmmcDxeConfig->PreviousTuningResults.TuningSuccessful) {
    DEBUG ((DEBUG_INFO, "  Hs400 Rx DLL value : %X\n", EmmcDxeConfig->PreviousTuningResults.Hs400RxValue));
    DEBUG ((DEBUG_INFO, "  Hs400 Tx DLL value : %X\n", EmmcDxeConfig->PreviousTuningResults.Hs400TxValue));
  }
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
  SCS_EMMC_DXE_CONFIG        *EmmcDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) PchPolicy, &gEmmcDxeConfigGuid, (VOID *)&EmmcDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %x\n", PchPolicy->TableHeader.Header.Revision));

  PchPrintEmmcDxeConfig (EmmcDxeConfig);
  GpioDxePrintConfig (PchPolicy);
  HdaDxePrintConfig (PchPolicy);
  PchPcieRpDxePrintConfig (PchPolicy);

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
  TotalBlockSize += GpioDxeGetConfigBlockTotalSize();
  TotalBlockSize += HdaDxeGetConfigBlockTotalSize();
  TotalBlockSize += PchPcieRpDxeGetConfigBlockTotalSize();

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

  Status =  GpioDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  Status =  HdaDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  Status = PchPcieRpDxeAddConfigBlock ((VOID *) PchPolicyInit);
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
