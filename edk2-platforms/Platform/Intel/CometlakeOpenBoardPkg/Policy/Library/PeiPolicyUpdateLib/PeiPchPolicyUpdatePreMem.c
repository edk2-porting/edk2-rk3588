/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiPchPolicyUpdate.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/ConfigBlockLib.h>

//
// Sawtooth Peak
// Single SPD EEPROM at 0xA2 serves both C0D0 and C1D0 (LPDDR is 1DPC only)
//
#define DIMM_SMB_SPD_P0C0D0_STP 0xA2
#define DIMM_SMB_SPD_P0C0D1_STP 0xA0
#define DIMM_SMB_SPD_P0C1D0_STP 0xA2
#define DIMM_SMB_SPD_P0C1D1_STP 0xA0

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusSTPRsvdAddresses[] = {
  DIMM_SMB_SPD_P0C0D0_STP,
  DIMM_SMB_SPD_P0C0D1_STP,
  DIMM_SMB_SPD_P0C1D0_STP,
  DIMM_SMB_SPD_P0C1D1_STP
};


/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPreMemPolicy  The SI PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyPreMem (
  IN OUT   SI_PREMEM_POLICY_PPI  *SiPreMemPolicy
  )
{
  EFI_STATUS                      Status;
  UINT8                           *SmBusReservedTable;
  UINT8                           SmBusReservedNum;

  PCH_GENERAL_PREMEM_CONFIG       *PchGeneralPreMemConfig;
  PCH_TRACE_HUB_PREMEM_CONFIG     *PchTraceHubPreMemConfig;
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;
  PCH_LPC_PREMEM_CONFIG           *LpcPreMemConfig;
  PCH_WDT_PREMEM_CONFIG           *WatchDogPreMemConfig;
  PCH_DCI_PREMEM_CONFIG           *DciPreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;
  PCH_HDAUDIO_PREMEM_CONFIG       *HdaPreMemConfig;
  PCH_ISH_PREMEM_CONFIG           *IshPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchTraceHubPreMemConfigGuid, (VOID *) &PchTraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gWatchDogPreMemConfigGuid, (VOID *) &WatchDogPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gIshPreMemConfigGuid, (VOID *) &IshPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DciPreMemConfig->DciUsb3TypecUfpDbg = 2;
  PchTraceHubPreMemConfig->MemReg0Size = 3;
  PchTraceHubPreMemConfig->MemReg1Size = 3;
  //
  // SMBUS
  //
  SmbusPreMemConfig->Enable = TRUE;
  SmbusPreMemConfig->SmbAlertEnable = PcdGetBool (PcdSmbusAlertEnable);
  //
  // SMBUS reserved addresses
  //
  SmBusReservedTable = NULL;
  SmBusReservedNum   = 0;
  SmbusPreMemConfig->SmbusIoBase = PcdGet16 (PcdSmbusBaseAddress);
  SmBusReservedTable = mSmbusSTPRsvdAddresses;
  SmBusReservedNum   = sizeof (mSmbusSTPRsvdAddresses);

  if (SmBusReservedTable != NULL) {
    SmbusPreMemConfig->NumRsvdSmbusAddresses = SmBusReservedNum;
    CopyMem (
      SmbusPreMemConfig->RsvdSmbusAddressTable,
      SmBusReservedTable,
      SmBusReservedNum
      );
  }

  return Status;
}

