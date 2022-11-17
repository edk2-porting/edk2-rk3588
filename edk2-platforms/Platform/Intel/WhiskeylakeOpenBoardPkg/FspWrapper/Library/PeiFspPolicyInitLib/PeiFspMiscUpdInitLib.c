/** @file
  Implementation of Fsp Misc UPD Initialization.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>

#define STATUS_CODE_USE_RAM        BIT0
#define STATUS_CODE_USE_ISA_SERIAL BIT1
#define STATUS_CODE_USE_USB        BIT2
#define STATUS_CODE_USE_USB3       BIT3
#define STATUS_CODE_USE_SERIALIO   BIT4
#define STATUS_CODE_USE_TRACEHUB   BIT5
#define STATUS_CODE_CMOS_INVALID   BIT6
#define STATUS_CODE_CMOS_VALID     BIT7
/**
  Performs FSP Misc UPD initialization.

  @param[in][out]  FspmUpd             Pointer to FSPM_UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
**/
EFI_STATUS
EFIAPI
PeiFspMiscUpdInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_HOB_POINTERS              Hob;
  DEBUG_CONFIG_DATA_HOB             *DebugConfigData;
  UINT8                             DebugInterfaces;

  FspmUpd->FspmArchUpd.StackBase = (VOID *)(UINTN)(PcdGet32(PcdTemporaryRamBase) + PcdGet32(PcdTemporaryRamSize) - (PcdGet32(PcdFspTemporaryRamSize) + PcdGet32(PcdFspReservedBufferSize)));
  FspmUpd->FspmArchUpd.StackSize = PcdGet32(PcdFspTemporaryRamSize);

  Status = PeiServicesGetBootMode (&(FspmUpd->FspmArchUpd.BootMode));
  if (EFI_ERROR (Status)) {
    FspmUpd->FspmArchUpd.BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }

  FspmUpd->FspmArchUpd.BootLoaderTolumSize = 0x0;

  //
  // Initialize DebugConfigData
  //
  DebugInterfaces = 0x00;
  Hob.Guid = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (Hob.Guid != NULL) {
    DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (Hob.Guid);
    if (DebugConfigData != NULL) {
      // Debug Interfaces
      if (DebugConfigData->RamDebugInterface)      { DebugInterfaces |= STATUS_CODE_USE_RAM; }
      if (DebugConfigData->UartDebugInterface)     { DebugInterfaces |= STATUS_CODE_USE_ISA_SERIAL; }
      if (DebugConfigData->Usb3DebugInterface)     { DebugInterfaces |= STATUS_CODE_USE_USB3; }
      if (DebugConfigData->SerialIoDebugInterface) { DebugInterfaces |= STATUS_CODE_USE_SERIALIO; }
      if (DebugConfigData->TraceHubDebugInterface) { DebugInterfaces |= STATUS_CODE_USE_TRACEHUB; }
      FspmUpd->FspmConfig.PcdDebugInterfaceFlags  = DebugInterfaces;
      // Serial debug message baud rate
      FspmUpd->FspmConfig.PcdSerialDebugBaudRate  = DebugConfigData->SerialDebugBaudRate;
      //Serial debug message level
      FspmUpd->FspmConfig.PcdSerialDebugLevel     = DebugConfigData->SerialDebug;
    }
  }
  DEBUG ((DEBUG_INFO, "FspmConfig.PcdDebugInterfaceFlags is 0x%X\n", FspmUpd->FspmConfig.PcdDebugInterfaceFlags));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.PcdSerialDebugBaudRate is 0x%X\n", FspmUpd->FspmConfig.PcdSerialDebugBaudRate));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.PcdSerialDebugLevel is 0x%X\n", FspmUpd->FspmConfig.PcdSerialDebugLevel));

  return EFI_SUCCESS;
}

