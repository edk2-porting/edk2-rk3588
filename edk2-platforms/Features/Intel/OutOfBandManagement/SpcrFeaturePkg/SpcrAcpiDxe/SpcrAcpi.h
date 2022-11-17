/** @file
  Header file for the driver used to add SPCR device info to ACPI table.
  SPCR is abbreviation of Serial Port Console Redirection Table (SPCR).

  Copyright (c) 2004 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SPCR_ACPI_H_
#define _SPCR_ACPI_H_


#include <IndustryStandard/Acpi30.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
//#include <Guid/GlobalVariable.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SpcrDeviceLib.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/SerialIo.h>
#include <Protocol/SuperIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>

#pragma pack(1)

///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()
//
// Prototypes
//

/**

  Install Boot Device Selection Protocol

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval  EFI_SUCEESS  BDS has finished initializing.
                        Return the dispatcher and recall BDS.Entry
  @retval  Other        Return status from AllocatePool() or gBS->InstallProtocolInterface

**/
EFI_STATUS
EFIAPI
SpcrInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  );

/**
  Installs the Smbios Table to the System Table. This function gets called
  when the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

  @param  Event                The event to signal
  @param  Context              Event contex

**/
VOID
EFIAPI
OutOfBandACPITableConstruction (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );
#endif
