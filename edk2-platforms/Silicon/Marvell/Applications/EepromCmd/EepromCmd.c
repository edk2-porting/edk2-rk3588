/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

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

#include <Protocol/Eeprom.h>
#include <Protocol/MvI2c.h>

CONST CHAR16 ShellEepromFileName[] = L"ShellCommand";
EFI_HANDLE ShellEepromHiiHandle = NULL;

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"-d", TypeValue},
  {L"-m", TypeValue},
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
ShellCommandGetManFileNameEeprom (
  VOID
  )
{
  return ShellEepromFileName;
}

STATIC
VOID
Usage (
  VOID
  )
{
  Print (L"Basic EEPROM commands:\n"
         "eeprom [read] [write] [list] [<Chip>] [<Bus>][<Address>] [<Length>] [-d <Data>] [-m <Source>]\n"
         "All modes except 'list' require Address, Length and Chip set.\n\n"
         "read    - read from EEPROM\n"
         "write   - write Data to EEPROM, requires -d\n"
         "list    - list available EEPROM devices\n\n"
         "-m      - transfer from/to RAM memory\n\n"
         "Chip    - EEPROM bus address\n"
         "Bus     - I2C bus address\n"
         "Address - address in EEPROM to read/write\n"
         "Data    - data byte to be written\n"
         "Length  - length of data to read/write/copy\n"
         "Source  - address of data in RAM to be copied\n"
         "Examples:\n"
         "List devices:\n"
         " eeprom list\n"
         "Read 16 bytes from address 0x0 in chip 0x57:\n"
         " eeprom read 0x57 0x0 0x0 0x10\n"
         "Fill 16 bytes with 0xab at address 0x0 in chip 0x57:\n"
         " eeprom write 0x57 0x0 0x0 0x10 -d 0xab\n"
         "Copy 32 bytes from 0x2000000 in RAM to EEPROM:\n"
         " eeprom write 0x57 0x0 0x0 0x20 -m 0x2000000\n"
         "Copy 32 bytes from EEPROM to RAM:\n"
         " eeprom read 0x57 0x0 0x0 0x20 -m 0x2000000\n"
  );
}

STATIC
EFI_STATUS
EepromList (
    )
{
  EFI_HANDLE *HandleBuffer;
  EFI_STATUS Status;
  UINTN ProtocolCount;
  MARVELL_EEPROM_PROTOCOL *EepromProtocol;
  UINTN i;
  Status = gBS->LocateHandleBuffer ( ByProtocol,
                                     &gMarvellEepromProtocolGuid,
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
                    &gMarvellEepromProtocolGuid,
                    (VOID **) &EepromProtocol,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL );
    Print (L"0x%x at bus %d\n", I2C_DEVICE_ADDRESS(EepromProtocol->Identifier),
        I2C_DEVICE_BUS(EepromProtocol->Identifier));
    Status = gBS->CloseProtocol ( HandleBuffer[i],
                                  &gMarvellEepromProtocolGuid,
                                  gImageHandle,
                                  NULL );
  }
  Print (L"\n");
  return Status;
}

STATIC
EFI_STATUS
EepromLocateProtocol (
 IN  UINT32 Identifier,
 OUT EFI_HANDLE *FoundHandle,
 OUT MARVELL_EEPROM_PROTOCOL **FoundEepromProtocol
 )
{
  EFI_HANDLE *HandleBuffer;
  EFI_STATUS Status;
  UINTN ProtocolCount;
  MARVELL_EEPROM_PROTOCOL *EepromProtocol;
  UINTN i;
  Status = gBS->LocateHandleBuffer ( ByProtocol,
                                     &gMarvellEepromProtocolGuid,
                                     NULL,
                                     &ProtocolCount,
                                     &HandleBuffer
                                   );
  if (EFI_ERROR(Status))
    return Status;

  for (i = 0; i < ProtocolCount; i++) {
    Status = gBS->OpenProtocol (
                    HandleBuffer[i],
                    &gMarvellEepromProtocolGuid,
                    (VOID **) &EepromProtocol,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL );
    if (EepromProtocol->Identifier == Identifier) {
      *FoundEepromProtocol = EepromProtocol;
      *FoundHandle = HandleBuffer[i];
      return EFI_SUCCESS;
    }
    Status = gBS->CloseProtocol ( HandleBuffer[i],
                                  &gMarvellEepromProtocolGuid,
                                  gImageHandle,
                                  NULL );
  }
  *FoundEepromProtocol = NULL;
  return EFI_UNSUPPORTED;
}

SHELL_STATUS
EFIAPI
ShellCommandRunEeprom (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  LIST_ENTRY              *CheckPackage;
  CHAR16                  *ProblemParam;
  CONST CHAR16            *ValueStr;
  UINTN                   Address, Length, Chip, Source, Bus;
  UINT8                   Data;
  UINT8                   *Buffer;
  BOOLEAN                 ReadMode, MemMode;
  EFI_HANDLE              Handle, ProtHandle;
  MARVELL_EEPROM_PROTOCOL *EepromProtocol = NULL;
  UINTN                   Count, HandleSize;

  Handle = NULL;
  Source = 0;
  HandleSize = 2 * sizeof (EFI_HANDLE);

  Status = gBS->LocateHandle (ByProtocol, &gMarvellEepromProtocolGuid, NULL,
    &HandleSize, &ProtHandle);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "No Eeprom protocol, connect I2C stack\n"));
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
    EepromList();
    return SHELL_SUCCESS;
  }

  if (ShellCommandLineGetFlag (CheckPackage, L"help")) {
    Usage();
    return SHELL_SUCCESS;
  }

  if (ShellCommandLineGetCount(CheckPackage) < 4) {
    Print (L"Not enough arguments given.\n");
    Usage();
  }

  ReadMode = ShellCommandLineGetFlag (CheckPackage, L"read");
  if (ReadMode == ShellCommandLineGetFlag (CheckPackage, L"write")) {
    Print (L"Error - type read, write, list or help.\n");
    return SHELL_INVALID_PARAMETER;
  }

  MemMode = ShellCommandLineGetFlag (CheckPackage, L"-m");
  Data = 0;
  if (!ReadMode && !MemMode) {
    ValueStr = ShellCommandLineGetValue (CheckPackage, L"-d");
    if (ValueStr == NULL) {
      Print (L"Error - no data to write.\n");
      return SHELL_INVALID_PARAMETER;
    }
    Data = ShellHexStrToUintn (ValueStr);
  }

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 1);
  Chip = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 2);
  Bus = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 3);
  Address = ShellHexStrToUintn (ValueStr);

  ValueStr = ShellCommandLineGetRawValue(CheckPackage, 4);
  Length = ShellHexStrToUintn (ValueStr);

  if (MemMode) {
    ValueStr = ShellCommandLineGetValue (CheckPackage, L"-m");
    if (ValueStr == NULL) {
      Print (L"Error - no memory address given.\n");
      return SHELL_INVALID_PARAMETER;
    }
    Source = ShellHexStrToUintn (ValueStr);
  }

  EepromLocateProtocol (I2C_DEVICE_INDEX(Bus, Chip), &Handle, &EepromProtocol);
  if (EepromProtocol == NULL) {
    Print (L"Failed to locate EEPROM protocol.\n");
    return SHELL_INVALID_PARAMETER;
  }

  if (MemMode) {
    Buffer = (VOID *) Source;
  } else {
    Buffer = AllocateZeroPool (Length);
    if (Buffer == NULL) {
      Status = SHELL_OUT_OF_RESOURCES;
      Print (L"Error - out of resources.\n");
      goto out_close;
    }
    if (!ReadMode) {
      for (Count = 0; Count < Length; Count++)
        Buffer[Count] = Data;
    }
  }
  EepromProtocol->Transfer(EepromProtocol, Address, Length, Buffer,
      ReadMode ? EEPROM_READ : EEPROM_WRITE);

  if (MemMode) {
  Print (L"Transfered succesfully.\n");
  } else {
    Print (L"Transfered:\n");
    for (Count = 0; Count < Length; Count++) {
      Print(L"0x%x ", Buffer[Count]);
      if (Count % 8 == 7)
        Print(L"\n");
    }
    Print (L"\n");
  }

  Status = SHELL_SUCCESS;

  if (!MemMode)
    FreePool(Buffer);
out_close:
  gBS->CloseProtocol ( Handle,
                       &gMarvellEepromProtocolGuid,
                       gImageHandle,
                       NULL );

  return Status;
}

EFI_STATUS
EFIAPI
ShellEepromCmdLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  ShellEepromHiiHandle = NULL;

  ShellEepromHiiHandle = HiiAddPackages (
                          &gShellEepromHiiGuid, gImageHandle,
                          UefiShellEepromLibStrings, NULL
                          );
  if (ShellEepromHiiHandle == NULL) {
    Print (L"Filed to add Hii package\n");
    return EFI_DEVICE_ERROR;
  }
  ShellCommandRegisterCommandName (
     L"eeprom", ShellCommandRunEeprom, ShellCommandGetManFileNameEeprom, 0,
     L"eeprom", TRUE , ShellEepromHiiHandle, STRING_TOKEN (STR_GET_HELP_EEPROM)
     );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ShellEepromCmdLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  if (ShellEepromHiiHandle != NULL) {
    HiiRemovePackages (ShellEepromHiiHandle);
  }
  return EFI_SUCCESS;
}
