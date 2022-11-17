/** @file
  This is the driver that initializes the Intel System Agent.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "SaInitDxe.h"
#include "SaInit.h"
#include <SaConfigHob.h>
#include <Protocol/PciEnumerationComplete.h>
#include <MemInfoHob.h>
#include <Protocol/SaIotrapSmi.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT16                   mPcieIoTrapAddress;

///
/// Global Variables
///
extern SA_CONFIG_HOB         *mSaConfigHob;


/**
  SystemAgent Dxe Initialization.

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SaInitEntryPointDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  VOID                      *Registration;
  EFI_EVENT                 ReadyToBoot;


  DEBUG ((DEBUG_INFO, "SaInitDxe Start\n"));

  SaInitEntryPoint ();

  Status = SaAcpiInit (ImageHandle);
  ///
  /// Create PCI Enumeration Completed callback for CPU PCIe
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    CpuPciEnumCompleteCallback,
    NULL,
    &Registration
    );

  //
  // Register a Ready to boot event to config PCIE power management setting after OPROM executed
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             SaOnReadyToBoot,
             NULL,
             &ReadyToBoot
             );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SaInitDxe End\n"));

  return EFI_SUCCESS;
}

/**
  Do PCIE power management while resume from S3
**/
VOID
ReconfigureCpuPciePowerManagementForS3 (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Data32;
  SA_IOTRAP_SMI_PROTOCOL                *CpuPcieIoTrapProtocol;

  Status = gBS->LocateProtocol (&gCpuPcieIoTrapProtocolGuid, NULL, (VOID **) &CpuPcieIoTrapProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }
  mPcieIoTrapAddress = CpuPcieIoTrapProtocol->SaIotrapSmiAddress;
  DEBUG ((DEBUG_INFO, "PcieIoTrapAddress: %0x\n", mPcieIoTrapAddress));

  if (mPcieIoTrapAddress != 0) {
    //
    // Save PCH PCIE IoTrap address to re-config PCIE power management setting after resume from S3
    //
    Data32 = CpuPciePmTrap;
    S3BootScriptSaveIoWrite (
      S3BootScriptWidthUint32,
      (UINTN) (mPcieIoTrapAddress),
      1,
      &Data32
      );
  } else {
    ASSERT (FALSE);
  }
}


/**
  SA initialization before boot to OS

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SaOnReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  DEBUG ((DEBUG_INFO, "Uefi SaOnReadyToBoot() Start\n"));

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  //
  // Trigger an Iotrap SMI to config PCIE power management setting after PCI enumrate is done
  //
#if FixedPcdGetBool(PcdCpuPcieEnable) == 1
  if (mPcieIoTrapAddress != 0) {
    IoWrite32 ((UINTN) mPcieIoTrapAddress, CpuPciePmTrap);
  } else {
    ASSERT (FALSE);
  }
#endif
  DEBUG ((DEBUG_INFO, "Uefi SaOnReadyToBoot() End\n"));
}


/**
  This function gets registered as a callback to perform CPU PCIe initialization before EndOfDxe

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
CpuPciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  VOID                *ProtocolPointer;

  DEBUG ((DEBUG_INFO, "CpuPciEnumCompleteCallback Start\n"));
  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  ReconfigureCpuPciePowerManagementForS3();
  //
  // Routine for update DMAR
  //
  UpdateDmarEndOfPcieEnum ();

  UpdateSaGnvsForMmioResourceBaseLength ();
  DEBUG ((DEBUG_INFO, "CpuPciEnumCompleteCallback End\n"));
  return;
}
