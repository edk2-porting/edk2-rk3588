/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.
Copyright (c) 2020, Intel Corporation. All rights reserved.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/HiiLib.h>
#include <Library/FileHandleLib.h>

#include <Protocol/Spi.h>
#include <Protocol/SpiFlash.h>

MARVELL_SPI_FLASH_PROTOCOL *SpiFlashProtocol;
MARVELL_SPI_MASTER_PROTOCOL *SpiMasterProtocol;

CONST CHAR16 gShellSpiFlashFileName[] = L"ShellCommand";
EFI_HANDLE gShellSfHiiHandle = NULL;

BOOLEAN InitFlag = 1;

STATIC SPI_DEVICE *mSlave;

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"read", TypeFlag},
  {L"readfile", TypeFlag},
  {L"write", TypeFlag},
  {L"writefile", TypeFlag},
  {L"erase", TypeFlag},
  {L"update", TypeFlag},
  {L"updatefile", TypeFlag},
  {L"probe", TypeFlag},
  {L"help", TypeFlag},
  {NULL , TypeMax}
  };

typedef enum {
  PROBE       = 1,
  READ        = 2,
  READ_FILE   = 4,
  WRITE       = 8,
  WRITE_FILE  = 16,
  ERASE       = 32,
  UPDATE      = 64,
  UPDATE_FILE = 128,
} Flags;

/**
  Return the file name of the help text file if not using HII.

  @return The string pointer to the file name.
**/
CONST CHAR16*
EFIAPI
ShellCommandGetManFileNameSpiFlash (
  VOID
  )
{

  return gShellSpiFlashFileName;
}

VOID
SfUsage (
  VOID
  )
{
  Print (L"\nBasic SPI command\n"
         "sf [probe | read | readfile | write | writefile | erase |"
         "update | updatefile]"
         "[<Address> | <FilePath>] <Offset> <Length>\n\n"
         "Length   - Number of bytes to send\n"
         "Address  - Address in RAM to store/load data\n"
         "FilePath - Path to file to read/write data from/to\n"
         "Offset   - Offset from beginning of SPI flash to store/load data\n"
         "Examples:\n"
         "Check if there is response from SPI flash\n"
         "  sf probe\n"
         "Read 32 bytes from 0xe00000 of SPI flash into RAM at address 0x100000\n"
         "  sf read 0x100000 0xe00000 32\n"
         "Read 0x20 bytes from 0x200000 of SPI flash into RAM at address 0x300000\n"
         "  sf read 0x300000 0x200000 0x20\n"
         "Erase 0x10000 bytes from offset 0x100000 of SPI flash\n"
         "  sf erase 0x100000 0x100000\n"
         "Write 16 bytes from 0x200000 at RAM into SPI flash at address 0x4000000\n"
         "  sf write 0x200000 0x4000000 16\n"
         "Update 100 bytes from 0x100000 at RAM in SPI flash at address 0xe00000\n"
         "  sf update 0x100000 0xe00000 100\n"
         "Read 0x3000 bytes from 0x0 of SPI flash into file fs2:file.bin\n"
         "  sf readfile fs2:file.bin 0x0 0x3000 \n"
         "Update data in SPI flash at 0x3000000 from file Linux.efi\n"
         "  sf updatefile Linux.efi 0x3000000\n"
  );
}

STATIC
EFI_STATUS
OpenAndPrepareFile (
  IN CHAR16 *FilePath,
  SHELL_FILE_HANDLE *FileHandle
  )
{
  EFI_STATUS Status;
  UINT64 OpenMode;

  OpenMode = EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;

  Status = ShellOpenFileByName (FilePath, FileHandle, OpenMode, 0);
  if (EFI_ERROR (Status)) {
    Print (L"sf: Cannot open file\n");
    return Status;
  }

  Status = FileHandleSetPosition(*FileHandle, 0);

  if (EFI_ERROR(Status)) {
    Print (L"sf: Cannot set file position to first byte\n");
    ShellCloseFile (FileHandle);
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
FlashProbe (
  IN SPI_DEVICE       *Slave
  )
{
  EFI_STATUS Status;

  Status = SpiFlashProtocol->ReadId (Slave, FALSE);
  if (EFI_ERROR (Status)) {
    return SHELL_ABORTED;
  }

  Status = SpiFlashProtocol->Init (SpiFlashProtocol, Slave);
  if (EFI_ERROR (Status)) {
    Print (L"sf: Cannot initialize flash device\n");
    return SHELL_ABORTED;
  }

  InitFlag = 0;

  return SHELL_SUCCESS;
}

SHELL_STATUS
EFIAPI
ShellCommandRunSpiFlash (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
EFI_STATUS              Status;
  LIST_ENTRY            *CheckPackage;
  EFI_PHYSICAL_ADDRESS  Address = 0, Offset = 0;
  SHELL_FILE_HANDLE     FileHandle = NULL;
  UINTN                 ByteCount, I;
  UINT64                FileSize;
  UINT8                 *Buffer = NULL, *FileBuffer = NULL;
  CHAR16                *ProblemParam, *FilePath;
  CONST CHAR16          *AddressStr = NULL, *OffsetStr = NULL;
  CONST CHAR16          *LengthStr = NULL, *FileStr = NULL;
  BOOLEAN               AddrFlag = FALSE, LengthFlag = TRUE, FileFlag = FALSE;
  UINT8                 Flag = 0, CheckFlag = 0;
  UINT8                 Mode, Cs;

  Status = gBS->LocateProtocol (
    &gMarvellSpiFlashProtocolGuid,
    NULL,
    (VOID **)&SpiFlashProtocol
  );
  if (EFI_ERROR(Status)) {
    Print (L"sf: Cannot locate SpiFlash protocol\n");
    return SHELL_ABORTED;
  }

  Status = gBS->LocateProtocol (
    &gMarvellSpiMasterProtocolGuid,
    NULL,
    (VOID **)&SpiMasterProtocol
  );
  if (EFI_ERROR(Status)) {
    Print (L"sf: Cannot locate SpiMaster protocol\n");
    return SHELL_ABORTED;
  }

  // Parse Shell command line
  Status = ShellInitialize ();
  if (EFI_ERROR (Status)) {
    Print (L"sf: Cannot initialize Shell\n");
    ASSERT_EFI_ERROR (Status);
    return SHELL_ABORTED;
  }

  Status = ShellCommandLineParse (ParamList, &CheckPackage, &ProblemParam, TRUE);
  if (EFI_ERROR (Status)) {
    Print (L"sf: Error while parsing command line\n");
    return SHELL_ABORTED;
  }

  if (ShellCommandLineGetFlag (CheckPackage, L"help")) {
    SfUsage();
    return EFI_SUCCESS;
  }

  // Check flags provided by user
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"probe") << 0);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"read") << 1);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"readfile") << 2);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"write") << 3);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"writefile") << 4);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"erase") << 5);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"update") << 6);
  Flag |= (ShellCommandLineGetFlag (CheckPackage, L"updatefile") << 7);

  if (InitFlag && !(Flag & PROBE)) {
    Print (L"Please run sf probe\n");
    return EFI_SUCCESS;
  }

  CheckFlag = Flag;
  for (I = 0; CheckFlag; CheckFlag >>= 1) {
    I += CheckFlag & 1;
    if (I > 1) {
      Print (L"sf: Too many flags\n");
      SfUsage();
      return SHELL_ABORTED;
    }
  }

  Mode = PcdGet32 (PcdSpiFlashMode);
  Cs = PcdGet32 (PcdSpiFlashCs);

  // Setup new spi device
  mSlave = SpiMasterProtocol->SetupDevice (SpiMasterProtocol, mSlave, Cs, Mode);
    if (mSlave == NULL) {
      Print(L"sf: Cannot allocate SPI device!\n");
      return SHELL_ABORTED;
    }

  switch (Flag) {
  case PROBE:
    // Probe spi bus
    Status = FlashProbe (mSlave);
    if (EFI_ERROR(Status)) {
      // No supported spi flash detected
      SpiMasterProtocol->FreeDevice(mSlave);
      mSlave = NULL;
      return SHELL_ABORTED;
    } else {
      return Status;
    }
    break;
  // Fall through
  case READ:
  case WRITE:
  case UPDATE:
    AddressStr = ShellCommandLineGetRawValue (CheckPackage, 1);
    OffsetStr = ShellCommandLineGetRawValue (CheckPackage, 2);
    LengthStr = ShellCommandLineGetRawValue (CheckPackage, 3);
    AddrFlag = TRUE;
    break;
  case ERASE:
    OffsetStr = ShellCommandLineGetRawValue (CheckPackage, 1);
    LengthStr = ShellCommandLineGetRawValue (CheckPackage, 2);
    break;
  case READ_FILE:
    FileStr = ShellCommandLineGetRawValue (CheckPackage, 1);
    OffsetStr = ShellCommandLineGetRawValue (CheckPackage, 2);
    LengthStr = ShellCommandLineGetRawValue (CheckPackage, 3);
    FileFlag = TRUE;
    break;
  case WRITE_FILE:
  case UPDATE_FILE:
    FileStr = ShellCommandLineGetRawValue (CheckPackage, 1);
    OffsetStr = ShellCommandLineGetRawValue (CheckPackage, 2);
    LengthFlag = FALSE;
    FileFlag = TRUE;
    break;
  }

  // Read address parameter
  if ((AddressStr == NULL) & AddrFlag) {
    Print (L"sf: No address parameter!\n");
    return SHELL_ABORTED;
  } else if (AddrFlag) {
    Address = ShellHexStrToUintn (AddressStr);
    if (Address == (UINTN)(-1)) {
      Print (L"sf: Wrong address parameter\n");
      return SHELL_ABORTED;
    }
  }

  // Read offset parameter
  if (OffsetStr == NULL) {
    Print (L"sf: No offset Parameter!\n");
    return SHELL_ABORTED;
  } else {
    Offset = ShellHexStrToUintn (OffsetStr);
    if (Offset < 0) {
      Print (L"sf: Wrong offset parameter: %s", OffsetStr);
      return SHELL_ABORTED;
    }
  }

  // Read length parameter
  if ((LengthStr == NULL) & LengthFlag) {
    Print (L"sf: No lenght parameter!\n");
    return SHELL_ABORTED;
  } else if (LengthFlag) {
    ByteCount = ShellStrToUintn (LengthStr);
    if (ByteCount < 0) {
      Print (L"sf: Wrong length parameter %s!\n", LengthStr);
      return SHELL_ABORTED;
    }
  }

  if (FileFlag) {
    // Read FilePath parameter
    if (FileStr == NULL) {
      Print (L"sf: No FilePath parameter!\n");
      return SHELL_ABORTED;
    } else {
      FilePath = (CHAR16 *) FileStr;
      Status = ShellIsFile (FilePath);
      // When read file into flash, file doesn't have to exist
      if (EFI_ERROR (Status) && !(Flag & READ_FILE)) {
        Print (L"sf: Wrong FilePath parameter!\n");
        return SHELL_ABORTED;
      }
    }

    Status = OpenAndPrepareFile (FilePath, &FileHandle);
    if (EFI_ERROR(Status)) {
      Print (L"sf: Error while preparing file\n");
      return SHELL_ABORTED;
    }

    // Get file size in order to check correctness at the end of transfer
    if (Flag & (WRITE_FILE | UPDATE_FILE)) {
      Status = FileHandleGetSize (FileHandle, &FileSize);
      if (EFI_ERROR (Status)) {
        Print (L"sf: Cannot get file size\n");
      }
      ByteCount = (UINTN) FileSize;
    }

    FileBuffer = AllocateZeroPool ((UINTN) ByteCount);
    if (FileBuffer == NULL) {
      Print (L"sf: Cannot allocate memory\n");
      goto Error_Close_File;
    }

    // Read file content and store it in FileBuffer
    if (Flag & (WRITE_FILE | UPDATE_FILE)) {
      Status = FileHandleRead (FileHandle, &ByteCount, FileBuffer);
      if (EFI_ERROR (Status)) {
        Print (L"sf: Read from file error\n");
        goto Error_Free_Buffer;
      } else if (ByteCount != (UINTN) FileSize) {
        Print (L"sf: Not whole file read. Abort\n");
        goto Error_Free_Buffer;
      }
    }
  }

  Buffer = (UINT8 *)(UINTN)Address;
  if (FileFlag) {
    Buffer = FileBuffer;
  }

  switch (Flag) {
  case READ:
  case READ_FILE:
    Status = SpiFlashProtocol->Read (mSlave, Offset, ByteCount, Buffer);
    break;
  case ERASE:
    Status = SpiFlashProtocol->Erase (mSlave, Offset, ByteCount);
    break;
  case WRITE:
  case WRITE_FILE:
    Status = SpiFlashProtocol->Write (mSlave, Offset, ByteCount, Buffer);
    break;
  case UPDATE:
  case UPDATE_FILE:
    Status = SpiFlashProtocol->Update (mSlave, Offset, ByteCount, Buffer);
    break;
  }

  if (EFI_ERROR (Status)) {
    Print (L"sf: Error while performing spi transfer\n");
    return SHELL_ABORTED;
  }

  switch (Flag) {
  case ERASE:
    Print (L"sf: %d bytes succesfully erased at offset 0x%x\n", ByteCount,
      Offset);
    break;
  case WRITE:
  case WRITE_FILE:
    Print (L"sf: Write %d bytes at offset 0x%x\n", ByteCount, Offset);
    break;
  case UPDATE:
  case UPDATE_FILE:
    Print (L"sf: Update %d bytes at offset 0x%x\n", ByteCount, Offset);
    break;
  case READ:
    Print (L"sf: Read %d bytes from offset 0x%x\n", ByteCount, Offset);
    break;
  case READ_FILE:
    Status = FileHandleWrite (FileHandle, &ByteCount, FileBuffer);
    if (EFI_ERROR(Status)) {
      Print (L"sf: Error while writing into file\n");
      goto Error_Free_Buffer;
    }
    break;
  }

  if (FileFlag) {
    FreePool (FileBuffer);

    if (FileHandle != NULL) {
      ShellCloseFile (&FileHandle);
    }
  }

  return EFI_SUCCESS;

Error_Free_Buffer:
  FreePool (FileBuffer);
Error_Close_File:
  ShellCloseFile (&FileHandle);
  return SHELL_ABORTED;
}

EFI_STATUS
EFIAPI
ShellSpiFlashLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  gShellSfHiiHandle = NULL;

  gShellSfHiiHandle = HiiAddPackages (
                        &gShellSfHiiGuid, gImageHandle,
                        UefiShellSpiFlashLibStrings, NULL
                        );
  if (gShellSfHiiHandle == NULL) {
    return EFI_DEVICE_ERROR;
  }

  ShellCommandRegisterCommandName (
     L"sf", ShellCommandRunSpiFlash, ShellCommandGetManFileNameSpiFlash, 0,
     L"sf", TRUE , gShellSfHiiHandle, STRING_TOKEN (STR_GET_HELP_SF)
     );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ShellSpiFlashLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  if (gShellSfHiiHandle != NULL) {
    HiiRemovePackages (gShellSfHiiHandle);
  }
  return EFI_SUCCESS;
}
