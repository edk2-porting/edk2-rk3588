/** @file
  Intel PCH PEI Policy initialization.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyUpdate.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/PchGbeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchHsioLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPcieRpLib.h>
#include <GpioConfig.h>
#include <GpioPinsSklH.h>
#include <Library/DebugLib.h>
#include <Library/PchGbeLib.h>

extern PCH_PCIE_DEVICE_OVERRIDE mPcieDeviceTable[];

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
    DEBUG ((DEBUG_ERROR, "InternalAddVerbTable wrong input: VerbTable == NULL\n"));
    return;
  }

  VerbTableArray[*VerbTableEntryNum] = (UINT32) VerbTable;
  *VerbTableEntryNum += 1;

  DEBUG ((DEBUG_INFO,
    "Add verb table for vendor = 0x%04X devId = 0x%04X (size = %d DWords)\n",
    VerbTable->Header.VendorId,
    VerbTable->Header.DeviceId,
    VerbTable->Header.DataDwords)
    );
}

enum HDAUDIO_CODEC_SELECT {
  PchHdaCodecPlatformOnboard = 0,
  PchHdaCodecExternalKit     = 1
};

/**
  Add verb table function.
  This function update the verb table number and verb table ptr of policy.

  @param[in] HdAudioConfig            HDAudie config block
  @param[in] CodecType                Platform codec type indicator
  @param[in] AudioConnectorType       Platform audio connector type
**/
STATIC
VOID
InternalAddPlatformVerbTables (
  IN OUT FSPS_UPD                     *FspsUpd,
  IN  UINT8                           CodecType,
  IN  UINT8                           AudioConnectorType
  )
{
  UINT8                           VerbTableEntryNum;
  UINT32                          VerbTableArray[32];
  UINT32                          *VerbTablePtr;

  VerbTableEntryNum = 0;

  InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdDisplayAudioHdaVerbTable));

  if (CodecType == PchHdaCodecPlatformOnboard) {
    DEBUG ((DEBUG_INFO, "HDA Policy: Onboard codec selected\n"));
    if ((VOID *) (UINTN) PcdGet32 (PcdExtHdaVerbTable) != NULL) {
      if (AudioConnectorType == 0) { //Type-C Audio connector selected in Bios Setup menu
        InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdExtHdaVerbTable));
        InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, NULL);
        InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, NULL);
        DEBUG ((DEBUG_INFO, "HDA: Type-C Audio connector selected!\n"));
      } else { //Stacked Jack Audio connector selected in Bios Setup menu
        InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable));
        InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable2));
        InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, NULL);
        DEBUG ((DEBUG_INFO, "HDA: Stacked-Jack Audio connector selected!\n"));
      }
    } else {
      InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable));
      InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable2));
      InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, NULL);
    }
  } else {
    DEBUG ((DEBUG_INFO, "HDA Policy: External codec kit selected\n"));
    InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdCommonHdaVerbTable1));
    InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdCommonHdaVerbTable2));
    InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdCommonHdaVerbTable3));
  }

  FspsUpd->FspsConfig.PchHdaVerbTableEntryNum = VerbTableEntryNum;

  VerbTablePtr = (UINT32 *) AllocateZeroPool (sizeof (UINT32) * VerbTableEntryNum);
  CopyMem (VerbTablePtr, VerbTableArray, sizeof (UINT32) * VerbTableEntryNum);
  FspsUpd->FspsConfig.PchHdaVerbTablePtr = (UINT32) VerbTablePtr;
}

/**
  Performs FSP PCH PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  )
{

  FspsUpd->FspsConfig.PchSubSystemVendorId = V_PCH_INTEL_VENDOR_ID;
  FspsUpd->FspsConfig.PchSubSystemId       = V_PCH_DEFAULT_SID;

  FspsUpd->FspsConfig.PchPcieDeviceOverrideTablePtr = (UINT32) mPcieDeviceTable;

  InternalAddPlatformVerbTables (FspsUpd, PchHdaCodecPlatformOnboard, PcdGet8 (PcdAudioConnector));

DEBUG_CODE_BEGIN();
if ((PcdGet8 (PcdSerialIoUartDebugEnable) == 1) &&
      FspsUpd->FspsConfig.SerialIoDevMode[PchSerialIoIndexUart0 + PcdGet8 (PcdSerialIoUartNumber)] == PchSerialIoDisabled ) {
    FspsUpd->FspsConfig.SerialIoDevMode[PchSerialIoIndexUart0 + PcdGet8 (PcdSerialIoUartNumber)] = PchSerialIoLegacyUart;
  }
DEBUG_CODE_END();

  FspsUpd->FspsConfig.PchSirqMode = PchContinuousMode;

  return EFI_SUCCESS;
}

