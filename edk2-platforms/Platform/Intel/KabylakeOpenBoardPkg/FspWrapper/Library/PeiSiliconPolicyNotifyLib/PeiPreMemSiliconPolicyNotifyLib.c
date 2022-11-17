/** @file
  This library implements constructor function to register notify call back
  when policy PPI installed.

Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/ConfigBlockLib.h>

/**
  Callback function to update policy when policy PPI installed.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             Succeeds.
  @retval Others                  Error code returned by sub-functions.
**/
EFI_STATUS
EFIAPI
SiPreMemPolicyPpiNotify (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  EFI_STATUS                  Status;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;
  SA_MISC_PEI_PREMEM_CONFIG   *MiscPeiPreMemConfig;

  DEBUG ((DEBUG_INFO, "SiPreMemPolicyPpiNotify() Start\n"));

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (SiPreMemPolicyPpi != NULL) {
    //
    // Get requisite IP Config Blocks which needs to be used here
    //
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);

    //
    // Update SpdAddressTable policy when it is installed.
    //
    if (MiscPeiPreMemConfig != NULL) {
      MiscPeiPreMemConfig->SpdAddressTable[0] = PcdGet8 (PcdMrcSpdAddressTable0);
      DEBUG ((DEBUG_INFO, "MiscPeiPreMemConfig->SpdAddressTable[0] 0x%x\n", MiscPeiPreMemConfig->SpdAddressTable[0]));
      MiscPeiPreMemConfig->SpdAddressTable[1] = PcdGet8 (PcdMrcSpdAddressTable1);
      DEBUG ((DEBUG_INFO, "MiscPeiPreMemConfig->SpdAddressTable[1] 0x%x\n", MiscPeiPreMemConfig->SpdAddressTable[1]));
      MiscPeiPreMemConfig->SpdAddressTable[2] = PcdGet8 (PcdMrcSpdAddressTable2);
      DEBUG ((DEBUG_INFO, "MiscPeiPreMemConfig->SpdAddressTable[2] 0x%x\n", MiscPeiPreMemConfig->SpdAddressTable[2]));
      MiscPeiPreMemConfig->SpdAddressTable[3] = PcdGet8 (PcdMrcSpdAddressTable3);
      DEBUG ((DEBUG_INFO, "MiscPeiPreMemConfig->SpdAddressTable[3] 0x%x\n", MiscPeiPreMemConfig->SpdAddressTable[3]));
    }
  }
  return Status;
}

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiPreMemPolicyPpiNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSiPreMemPolicyPpiGuid,
    SiPreMemPolicyPpiNotify
  }
};

/**
  The library constructuor.
  The function register a policy install notify callback.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS for now.
                                It will ASSERT on error for debug version.
**/
EFI_STATUS
EFIAPI
PeiPreMemSiliconPolicyNotifyLibConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  //
  // Register call back after PPI produced
  //
  Status = PeiServicesNotifyPpi (mSiPreMemPolicyPpiNotifyList);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
