/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.
Copyright (c) 2021-2022, Rockchip Limited. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellCommandLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>

#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/I2cDemo.h>
#include <Protocol/I2c.h>

CONST CHAR16 ShellI2cDemoFileName[] = L"I2cDemoTestShellCommand";
EFI_HANDLE ShellI2cDemoHiiHandle = NULL;

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"read", TypeFlag},
  {L"write", TypeFlag},
  {L"list", TypeFlag},
  {L"help", TypeFlag},
  {NULL , TypeMax}
  };

/**
  Return the file name of the help text file if not using HII.

  @return The string pointer to the file name.
**/
STATIC
CONST CHAR16*
EFIAPI
ShellCommandGetManFileNameI2cDemo (
  VOID
  )
{
  return ShellI2cDemoFileName;
}

STATIC
VOID
Usage (
  VOID
  )
{
  Print (L"I2C DEMO TEST commands:\n"
         "i2cdemo [read] [write] [list] [<Bus>][<Address>] [<Length>] [<RegAddress>] [<RegAddressLength>] [<Data>] \n"
         "All modes except 'list' require Address, Length and Chip set.\n\n"
         "read    - read from i2cdemo device\n"
         "write   - write Data to i2cdemo device\\n"
         "list    - list available i2cdemo devices\n\n"
         "Bus    - I2C bus address\n"
	 "Address    - i2cdemo bus address\n"
         "Length	  - data byte length to read/write\\n"
         "RegAddress - address in i2cdemo to read/write\n"
         "RegAddressLength - address in i2cdemo length\n"
         "Data    - data byte to be written\n"
         "Examples:\n"
         "List devices:\n"
         " i2cdemo list\n"
         "Read 2 bytes from address 0x10 in chip 0x51@bus2:\n"
         " i2cdemo read 2 0x51 2 0x10 1\n"
         "Fill 16 bytes with 0xab at address 0x0 in chip 0x57:\n"
         " i2cdemo write 2 0x51 1 0x10 1 0x00\n"
  );
}

STATIC
EFI_STATUS
I2cDemoList (
    )
{
  EFI_HANDLE *HandleBuffer;
  EFI_STATUS Status;
  UINTN ProtocolCount;
  ROCKCHIP_I2CDEMO_PROTOCOL *I2cDemoProtocol;
  UINTN i;
  Status = gBS->LocateHandleBuffer ( ByProtocol,
                                     &gRockchipI2cDemoProtocolGuid,
                                     NULL,
                                     &ProtocolCount,
                                     &HandleBuffer
                                   );
  if (ProtocolCount == 0) {
    Print (L"0 devices found.\n");
  } else {
    Print (L"%u devices found: ", ProtocolCount);
  }

  for (i = 0; i < ProtocolCount; i++) {
    Status = gBS->OpenProtocol (
                    HandleBuffer[i],
                    &gRockchipI2cDemoProtocolGuid,
                    (VOID **) &I2cDemoProtocol,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL );
    Print (L"0x%x at bus %d\n", I2C_DEVICE_ADDRESS(I2cDemoProtocol->Identifier),
        I2C_DEVICE_BUS(I2cDemoProtocol->Identifier));
    Status = gBS->CloseProtocol ( HandleBuffer[i],
                                  &gRockchipI2cDemoProtocolGuid,
                                  gImageHandle,
                                  NULL );
  }
  Print (L"\n");
  return Status;
}

STATIC
EFI_STATUS
I2cDemoLocateProtocol (
 IN  UINT32 Identifier,
 OUT EFI_HANDLE *FoundHandle,
 OUT ROCKCHIP_I2CDEMO_PROTOCOL **FoundI2cDemoProtocol
 )
{
  EFI_HANDLE *HandleBuffer;
  EFI_STATUS Status;
  UINTN ProtocolCount;
  ROCKCHIP_I2CDEMO_PROTOCOL *I2cDemoProtocol;
  UINTN i;
  Status = gBS->LocateHandleBuffer ( ByProtocol,
                                     &gRockchipI2cDemoProtocolGuid,
                                     NULL,
                                     &ProtocolCount,
                                     &HandleBuffer
                                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  for (i = 0; i < ProtocolCount; i++) {
    Status = gBS->OpenProtocol (
                    HandleBuffer[i],
                    &gRockchipI2cDemoProtocolGuid,
                    (VOID **) &I2cDemoProtocol,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL );

    if (I2cDemoProtocol->Identifier == Identifier) {
      *FoundI2cDemoProtocol = I2cDemoProtocol;
      *FoundHandle = HandleBuffer[i];
      return EFI_SUCCESS;
    }
    Status = gBS->CloseProtocol ( HandleBuffer[i],
                                  &gRockchipI2cDemoProtocolGuid,
                                  gImageHandle,
                                  NULL );
  }
  *FoundI2cDemoProtocol = NULL;

  return EFI_UNSUPPORTED;
}

SHELL_STATUS
EFIAPI
ShellCommandRunI2cDemo (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  LIST_ENTRY              *CheckPackage;
  CHAR16                  *ProblemParam;
  CONST CHAR16            *ValueStr;
  UINTN                   Bus, Address, XferLength, RegAddress, RegAddressLength, Source;
  UINT8                   *Buffer;
  BOOLEAN                 ReadMode, WriteMode;
  EFI_HANDLE              Handle, ProtHandle;
  ROCKCHIP_I2CDEMO_PROTOCOL *I2cDemoProtocol = NULL;
  UINTN                   HandleSize, i;
  UINTN                   TxData;

  Handle = NULL;
  Source = 0;
  HandleSize = 2 * sizeof (EFI_HANDLE);

  Status = gBS->LocateHandle (ByProtocol, &gRockchipI2cDemoProtocolGuid, NULL,
    &HandleSize, &ProtHandle);
  if (EFI_ERROR(Status)) {
    Print (L"No I2cDemo protocol, connect I2C stack\n");
    Status = gBS->LocateHandle (ByProtocol, &gEfiI2cMasterProtocolGuid, NULL,
      &HandleSize, &ProtHandle);
    if (EFI_ERROR(Status)) {
      Print (L"Failed to locate I2cMaster protocol, abort!\n");
      return SHELL_ABORTED;
    }
    Status = gBS->ConnectController (ProtHandle, NULL, NULL, TRUE);
    if (EFI_ERROR(Status)) {
      Print (L"Cannot connect I2C stack, abort!\n");
      return SHELL_ABORTED;
    }
  }

  Status = ShellInitialize ();
  if (EFI_ERROR (Status)) {
    Print (L"Error - failed to initialize shell\n");
    return SHELL_ABORTED;
  }

  Status = ShellCommandLineParse (ParamList, &CheckPackage, &ProblemParam, TRUE);
  if (EFI_ERROR (Status)) {
    Print (L"Error - failed to parse command line\n");
    return SHELL_ABORTED;
  }

  if (ShellCommandLineGetFlag (CheckPackage, L"list")) {
    I2cDemoList();
    return SHELL_SUCCESS;
  }

  if (ShellCommandLineGetFlag (CheckPackage, L"help")) {
    Usage();
    return SHELL_SUCCESS;
  }

  ReadMode = ShellCommandLineGetFlag (CheckPackage, L"read");
  WriteMode = ShellCommandLineGetFlag (CheckPackage, L"write");

  if (!ReadMode && !WriteMode) {
    Print (L"Not support mode given.\n");
    Usage();
    return SHELL_ABORTED;
  }

  if (ShellCommandLineGetCount(CheckPackage) != 6 && ReadMode) {
    Print (L"Not enough arguments given.\n");
    Usage();
    return SHELL_ABORTED;
  }

  if (ShellCommandLineGetCount(CheckPackage) != 7 && WriteMode) {
    Print (L"Not enough arguments given.\n");
    Usage();
    return SHELL_ABORTED;
  }

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 1);
  Bus = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 2);
  Address = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 3);
  XferLength = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 4);
  RegAddress = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 5);
  RegAddressLength = ShellHexStrToUintn (ValueStr);

  if (WriteMode) {
    ValueStr = ShellCommandLineGetRawValue(CheckPackage, 6);
    TxData = ShellHexStrToUintn (ValueStr);
  }

  I2cDemoLocateProtocol (I2C_DEVICE_INDEX(Bus, Address), &Handle, &I2cDemoProtocol);
  if (I2cDemoProtocol == NULL) {
    Print (L"Failed to locate I2CDEMO protocol.\n");
    return SHELL_INVALID_PARAMETER;
  }

    Buffer = AllocateZeroPool (XferLength);
    if (Buffer == NULL) {
      Status = SHELL_OUT_OF_RESOURCES;
      Print (L"Error - out of resources.\n");
      goto out_close;
    }

  if (ReadMode) {
    Status = I2cDemoProtocol->Read(I2cDemoProtocol, (UINT8 *)&RegAddress, RegAddressLength,  Buffer, XferLength);
    if (!EFI_ERROR(Status)) {
      Print (L"Read data[0 ~ %d]:", XferLength - 1);
      for (i = 0; i <XferLength; i++ ) {
        Print (L" 0x%x", Buffer[i]);
      }
      Print (L"\n");
    }
  } else {
    Status = I2cDemoProtocol->Write(I2cDemoProtocol, (UINT8 *)&RegAddress, RegAddressLength,  (UINT8 *)&TxData, XferLength);
  }

  if (EFI_ERROR(Status)) {
    Print (L"I2c Operation failed %d.\n", Status);
  } else {
    Print (L"I2c Operation successfully.\n");
  }

  Status = SHELL_SUCCESS;

    FreePool(Buffer);
out_close:
  gBS->CloseProtocol ( Handle,
                       &gRockchipI2cDemoProtocolGuid,
                       gImageHandle,
                       NULL );

  return Status;
}

EFI_STATUS
EFIAPI
ShellI2cDemoTestLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  Print (L"~Filed to add Hii package\n");
  ShellI2cDemoHiiHandle = NULL;

  ShellI2cDemoHiiHandle = HiiAddPackages (
                          &gShellI2cDemoHiiGuid, gImageHandle,
                          UefiShellI2cDemoLibStrings, NULL
                          );
  if (ShellI2cDemoHiiHandle == NULL) {
    Print (L"Filed to add Hii package\n");
    return EFI_DEVICE_ERROR;
  }
  ShellCommandRegisterCommandName (
     L"i2cdemo", ShellCommandRunI2cDemo, ShellCommandGetManFileNameI2cDemo, 0,
     L"i2cdemo", TRUE , ShellI2cDemoHiiHandle, STRING_TOKEN (STR_GET_HELP_I2CDEMO)
     );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ShellI2cDemoTestLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  if (ShellI2cDemoHiiHandle != NULL) {
    HiiRemovePackages (ShellI2cDemoHiiHandle);
  }
  return EFI_SUCCESS;
}
