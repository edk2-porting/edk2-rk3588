/** @file
  Smbios basic entry point.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SmbiosBasic.h"

EFI_STATUS
EFIAPI
BiosVendorFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

EFI_STATUS
EFIAPI
SystemManufacturerFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

EFI_STATUS
EFIAPI
BaseBoardManufacturerFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

EFI_STATUS
EFIAPI
ChassisManufacturerFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

EFI_STATUS
EFIAPI
BootInfoStatusFunction(
  IN  EFI_SMBIOS_PROTOCOL   *Smbios
  );

typedef
EFI_STATUS
(EFIAPI EFI_BASIC_SMBIOS_DATA_FUNCTION) (
  IN  EFI_SMBIOS_PROTOCOL  *Smbios
  );

typedef struct {
  EFI_BASIC_SMBIOS_DATA_FUNCTION *Function;
} EFI_BASIC_SMBIOS_DATA;

EFI_BASIC_SMBIOS_DATA mSmbiosBasicDataFuncTable[] = {
  {&BiosVendorFunction},
  {&SystemManufacturerFunction},
  {&BaseBoardManufacturerFunction},
  {&ChassisManufacturerFunction},
  {&BootInfoStatusFunction},
};

/**
  Standard EFI driver point.  This driver parses the mSmbiosMiscDataTable
  structure and reports any generated data using SMBIOS protocol.

  @param  ImageHandle     Handle for the image of this driver
  @param  SystemTable     Pointer to the EFI System Table

  @retval  EFI_SUCCESS    The data was successfully stored.

**/
EFI_STATUS
EFIAPI
SmbiosBasicEntryPoint(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINTN                Index;
  EFI_STATUS           EfiStatus;
  EFI_SMBIOS_PROTOCOL  *Smbios;

  EfiStatus = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**)&Smbios);
  if (EFI_ERROR(EfiStatus)) {
    DEBUG((DEBUG_ERROR, "Could not locate SMBIOS protocol.  %r\n", EfiStatus));
    return EfiStatus;
  }

  for (Index = 0; Index < sizeof(mSmbiosBasicDataFuncTable)/sizeof(mSmbiosBasicDataFuncTable[0]); ++Index) {
    EfiStatus = (*mSmbiosBasicDataFuncTable[Index].Function) (Smbios);
    if (EFI_ERROR(EfiStatus)) {
      DEBUG((DEBUG_ERROR, "Basic smbios store error.  Index=%d, ReturnStatus=%r\n", Index, EfiStatus));
      return EfiStatus;
    }
  }

  return EfiStatus;
}

/**
  Add an SMBIOS record.

  @param  Smbios                The EFI_SMBIOS_PROTOCOL instance.
  @param  SmbiosHandle          A unique handle will be assigned to the SMBIOS record.
  @param  Record                The data for the fixed portion of the SMBIOS record. The format of the record is
                                determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined
                                by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or
                                a set of null terminated strings and a null.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN EFI_SMBIOS_PROTOCOL        *Smbios,
  OUT EFI_SMBIOS_HANDLE         *SmbiosHandle,
  IN EFI_SMBIOS_TABLE_HEADER    *Record
  )
{
  *SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  return Smbios->Add (
                   Smbios,
                   NULL,
                   SmbiosHandle,
                   Record
                   );
}
