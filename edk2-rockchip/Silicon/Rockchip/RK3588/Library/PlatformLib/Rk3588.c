/** @file
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2014-2016, Linaro Limited. All rights reserved.
 *  Copyright (c) 2014, Red Hat, Inc.
 *  Copyright (c) 2011-2013, ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/IoLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseVariableLib.h>
#include <Library/ResetUtilityLib.h>
#include <Library/SaradcLib.h>
#include <Library/SerialPortLib.h>
#include <Pi/PiBootMode.h>

#include <Ppi/ArmMpCoreInfo.h>

#define RECOVERY_KEY_SARADC_CHANNEL       1
#define RECOVERY_KEY_PRESS_MAX_THRESHOLD  100

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

  @return   Return the current Boot Mode of the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  This function is called by PrePeiCore, in the SEC phase.
**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN  MpId
  )
{
  EFI_STATUS     Status;
  RETURN_STATUS  ReturnStatus;
  UINT32         KeyAdcValue;
  UINTN          Size;
  UINT64         BaudRate;

  ReturnStatus = SaradcReadChannel (RECOVERY_KEY_SARADC_CHANNEL, &KeyAdcValue);
  if (ReturnStatus == RETURN_SUCCESS) {
    if (KeyAdcValue < RECOVERY_KEY_PRESS_MAX_THRESHOLD) {
      DEBUG ((DEBUG_INFO, "%a: Recovery key pressed - entering MASKROM.\n", __func__));
      ResetPlatformSpecificGuid (&gRockchipResetTypeMaskromGuid);
    }
  }

  Size   = sizeof (UINT64);
  Status = BaseGetVariable (
             L"DebugSerialPortBaudRate",
             &gRK3588DxeFormSetGuid,
             NULL,
             &Size,
             &BaudRate
             );
  if (EFI_ERROR (Status) || (BaudRate == 0)) {
    return RETURN_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "%a: Setting baud rate to %lu\n", __func__, BaudRate));

  PatchPcdSet64 (PcdUartDefaultBaudRate, BaudRate);
  SerialPortInitialize ();

  return RETURN_SUCCESS;
}

VOID
ArmPlatformInitializeSystemMemory (
  VOID
  )
{
}

STATIC ARM_CORE_INFO  mRk3588InfoTable[] = {
  { 0x0, 0x000 },             // Cluster 0, Core 0
  { 0x0, 0x100 },             // Cluster 0, Core 1
  { 0x0, 0x200 },             // Cluster 0, Core 2
  { 0x0, 0x300 },             // Cluster 0, Core 3
  { 0x0, 0x400 },             // Cluster 0, Core 4
  { 0x0, 0x500 },             // Cluster 0, Core 5
  { 0x0, 0x600 },             // Cluster 0, Core 6
  { 0x0, 0x700 },             // Cluster 0, Core 7
};

STATIC
EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN          *CoreCount,
  OUT ARM_CORE_INFO  **ArmCoreTable
  )
{
  // Only support one cluster
  *CoreCount    = sizeof (mRk3588InfoTable) / sizeof (ARM_CORE_INFO);
  *ArmCoreTable = mRk3588InfoTable;

  return EFI_SUCCESS;
}

STATIC ARM_MP_CORE_INFO_PPI    mMpCoreInfoPpi = {
  PrePeiCoreGetMpCoreInfo
};
STATIC EFI_PEI_PPI_DESCRIPTOR  mPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof (mPlatformPpiTable);
  *PpiList     = mPlatformPpiTable;
}
