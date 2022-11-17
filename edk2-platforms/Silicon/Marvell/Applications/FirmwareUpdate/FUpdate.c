/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/FileHandleLib.h>
#include <Library/HandleParsingLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/ShellCommandLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/Spi.h>
#include <Protocol/SpiFlash.h>

#define CMD_NAME_STRING       L"fupdate"

#define SHELL_USE_DEVICE_PATH_PARAM  L"-p"
#define SHELL_DEVICE_NAME_PARAM      L"DeviceName"
#define SHELL_FILE_NAME_PARAM        L"LocalFileName"
#define SHELL_HELP_PARAM             L"help"
#define SHELL_LIST_PARAM             L"list"

#define MAIN_HDR_MAGIC        0xB105B002

STATIC EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL  *EfiDevicePathFromTextProtocol;
STATIC MARVELL_SPI_MASTER_PROTOCOL         *SpiMasterProtocol;
STATIC MARVELL_SPI_FLASH_PROTOCOL          *SpiFlashProtocol;
STATIC EFI_BLOCK_IO_PROTOCOL               *BlkIo;

STATIC CONST CHAR16 gShellFUpdateFileName[] = L"ShellCommands";
STATIC EFI_HANDLE gShellFUpdateHiiHandle = NULL;

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {SHELL_HELP_PARAM,            TypeFlag},
  {SHELL_LIST_PARAM,            TypeFlag},
  {SHELL_USE_DEVICE_PATH_PARAM, TypeFlag},
  {SHELL_FILE_NAME_PARAM,       TypePosition},
  {SHELL_DEVICE_NAME_PARAM,     TypePosition},
  {NULL ,                       TypeMax}
  };

typedef struct {              // Bytes
  UINT32  Magic;              //  0-3
  UINT32  PrologSize;         //  4-7
  UINT32  PrologChecksum;     //  8-11
  UINT32  BootImageSize;      // 12-15
  UINT32  BootImageChecksum;  // 16-19
  UINT32  Reserved0;          // 20-23
  UINT32  LoadAddr;           // 24-27
  UINT32  ExecAddr;           // 28-31
  UINT8   UartConfig;         //  32
  UINT8   Baudrate;           //  33
  UINT8   ExtCount;           //  34
  UINT8   AuxFlags;           //  35
  UINT32  IoArg0;             // 36-39
  UINT32  IoArg1;             // 40-43
  UINT32  IoArg2;             // 43-47
  UINT32  IoArg3;             // 48-51
  UINT32  Reserved1;          // 52-55
  UINT32  Reserved2;          // 56-59
  UINT32  Reserved3;          // 60-63
} MV_FIRMWARE_IMAGE_HEADER;

typedef
EFI_STATUS
(EFIAPI *FLASH_COMMAND) (
  UINT64           FileSize,
  UINTN            *FileBuffer,
  EFI_LBA          Offset
);

STATIC
EFI_STATUS
SpiFlashProbe (
  IN SPI_DEVICE    *Slave
  )
{
  EFI_STATUS       Status;

  // Read SPI flash ID
  Status = SpiFlashProtocol->ReadId (Slave, FALSE);
  if (EFI_ERROR (Status)) {
    return SHELL_ABORTED;
  }

  Status = SpiFlashProtocol->Init (SpiFlashProtocol, Slave);
  if (EFI_ERROR(Status)) {
    Print (L"%s: Cannot initialize flash device\n", CMD_NAME_STRING);
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Verify if selected device is valid for the firmware update.

  @param[in]   Handle            Handle of verified device
  @param[in]   Offset            [OPTIONAL] Additional parameter
                                 required for BlkIo->WriteBlocks,
                                 filled depending on detected device
                                 type.

**/
STATIC
EFI_STATUS
IsDeviceSupported (
  IN  EFI_HANDLE   Handle,
  OUT EFI_LBA     *Offset OPTIONAL
  )
{
  EFI_STATUS                  Status;
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath;

  //
  // Skip handles that do not have device path protocol
  //
  Status = gBS->OpenProtocol (Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID**)&DevicePath,
                  gImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Skip handles that are not block devices
  //
  Status = gBS->OpenProtocol (Handle,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH) {
      //
      // Search for SD/MMC devices.
      //
      if (DevicePath->SubType == MSG_SD_DP) {
        //
        // Only flashing in the beginning of SD card makes sense.
        //
        DevicePath = NextDevicePathNode (DevicePath);
        if (!IsDevicePathEnd (DevicePath)) {
          return EFI_UNSUPPORTED;
        }
        if (Offset != NULL) {
          *Offset = 1;
        }
        return EFI_SUCCESS;
      }
      if (DevicePath->SubType == MSG_EMMC_DP) {
        //
        // Filter out entire MMC device (ctrl(0x0))
        // and boot partitions (ctrl(0x1)/ctrl(0x2)) as valid for
        // the firmware update.
        //
        DevicePath = NextDevicePathNode (DevicePath);
        if (IsDevicePathEnd (DevicePath)) {
          return EFI_UNSUPPORTED;
        }
        DevicePath = NextDevicePathNode (DevicePath);
        if (!IsDevicePathEnd (DevicePath)) {
          return EFI_UNSUPPORTED;
        }
        if (Offset != NULL) {
          *Offset = 0;
        }
        return EFI_SUCCESS;
      }
    }
    DevicePath = NextDevicePathNode (DevicePath);
  }

  return EFI_UNSUPPORTED;
}

/**
  Print information about single device supporting the firmware update.

  @param[in]   Handle            Handle of verified device

**/
STATIC
EFI_STATUS
PrintSupportedDevice (
  IN EFI_HANDLE  Handle
  )
{
  CHAR16         *Name;

  gEfiShellProtocol->GetDeviceName (Handle,
    EFI_DEVICE_NAME_USE_DEVICE_PATH,
    NULL,
    &Name);
  if (Name != NULL) {
    ShellPrintEx (-1,
      -1,
      L"%H%02x%N      %s\n",
      ConvertHandleToHandleIndex (Handle),
      Name);
  }

  return EFI_SUCCESS;
}

/**
  Print information about all devices supporting the firmware update.

**/
STATIC
EFI_STATUS
ListSupportedDevices (
  )
{
  UINTN          LoopVar;
  EFI_HANDLE     Handle;
  EFI_STATUS     Status;

  ShellPrintEx (-1, -1, L"%BHandle  Path%N\n");
  ShellPrintEx (-1, -1, L"%Hspi%N     spi\n");

  for (LoopVar = 1; ; LoopVar++) {
    Handle = ConvertHandleIndexToHandle (LoopVar);
    if (Handle == NULL) {
      break;
    }

    Status = IsDeviceSupported (Handle, NULL);
    if (!EFI_ERROR (Status)) {
      PrintSupportedDevice (Handle);
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CheckImageHeader (
  IN OUT UINTN *ImageHeader
  )
{
  MV_FIRMWARE_IMAGE_HEADER *Header;
  UINT32 HeaderLength, Checksum, ChecksumBackup;

  Header = (MV_FIRMWARE_IMAGE_HEADER *)ImageHeader;
  HeaderLength = Header->PrologSize;
  ChecksumBackup = Header->PrologChecksum;

  // Compare magic number
  if (Header->Magic != MAIN_HDR_MAGIC) {
    Print (L"%s: Bad Image magic 0x%08x != 0x%08x\n", CMD_NAME_STRING, Header->Magic, MAIN_HDR_MAGIC);
    return EFI_DEVICE_ERROR;
  }

  // The checksum field is discarded from calculation
  Header->PrologChecksum = 0;

  Checksum = CalculateSum32 ((UINT32 *)Header, HeaderLength);
  if (Checksum != ChecksumBackup) {
    Print (L"%s: Bad Image checksum. 0x%x != 0x%x\n", CMD_NAME_STRING, Checksum, ChecksumBackup);
    return EFI_DEVICE_ERROR;
  }

  // Restore checksum backup
  Header->PrologChecksum = ChecksumBackup;

  return 0;
}

STATIC
EFI_STATUS
PrepareFirmwareImage (
  IN LIST_ENTRY             *CheckPackage,
  IN OUT SHELL_FILE_HANDLE  *FileHandle,
  IN OUT UINTN              **FileBuffer,
  IN OUT UINT64             *FileSize,
  IN UINT64                 Alignment
  )
{
  CONST CHAR16         *FileStr;
  EFI_STATUS           Status;
  UINT64               OpenMode;
  UINTN                *Buffer;
  UINTN                AlignedFileSize;

  // Parse string from commandline
  FileStr = ShellCommandLineGetRawValue (CheckPackage, 1);
  if (FileStr == NULL) {
    Print (L"%s: No image specified\n", CMD_NAME_STRING);
    return EFI_INVALID_PARAMETER;
  } else {
    Status = ShellIsFile (FileStr);
    if (EFI_ERROR(Status)) {
      Print (L"%s: File not found\n", CMD_NAME_STRING);
      return EFI_INVALID_PARAMETER;
    }
  }

  // Obtain file size
  OpenMode = EFI_FILE_MODE_READ;

  Status = ShellOpenFileByName (FileStr, FileHandle, OpenMode, 0);
    if (EFI_ERROR (Status)) {
      Print (L"%s: Cannot open Image file\n", CMD_NAME_STRING);
      return EFI_DEVICE_ERROR;
    }

  Status = FileHandleGetSize (*FileHandle, FileSize);
    if (EFI_ERROR (Status)) {
      Print (L"%s: Cannot get Image file size\n", CMD_NAME_STRING);
    }

  // Allocate aligned buffer in case of updating firmware in a BlockDevice
  if (Alignment > 0) {
    AlignedFileSize = *FileSize + (Alignment - (*FileSize % Alignment));
  } else {
    AlignedFileSize = *FileSize;
  }

  // Read Image header into buffer
  Buffer = AllocateZeroPool (AlignedFileSize);
  if (Buffer == NULL) {
    Print (L"%s: Fail to allocate buffer\n", CMD_NAME_STRING);
    ShellCloseFile (FileHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = FileHandleRead (*FileHandle, (UINTN *)FileSize, Buffer);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Cannot read Image file header\n", CMD_NAME_STRING);
    ShellCloseFile (FileHandle);
    FreePool (Buffer);
    return EFI_DEVICE_ERROR;
  }

  *FileBuffer = Buffer;
  *FileSize = AlignedFileSize;

  return EFI_SUCCESS;
}

/**
  Update firmware image in the block device.

  @param[in]   FileSize            Size of the file to be flashed
  @param[in]  *FileBuffer          Pointer to the file in memory
  @param[in]   Offset              First logical block to be updated.

**/
STATIC
EFI_STATUS
EFIAPI
ProgramBlockDevice (
  IN UINT64      FileSize,
  IN UINTN      *FileBuffer,
  IN EFI_LBA     Offset
  )
{
  EFI_STATUS     Status;

  Print (L"Updating image in BlockDevice\n");

  Status = BlkIo->WriteBlocks (BlkIo,
                    BlkIo->Media->MediaId,
                    Offset,
                    FileSize,
                    FileBuffer);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Cannot write to device (Status: %r)\n",
      CMD_NAME_STRING,
      Status);
    return Status;
  }

  Status = BlkIo->FlushBlocks (BlkIo);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Cannot flush to device (Status: %r)\n",
      CMD_NAME_STRING,
      Status);
    return Status;
  }

  return EFI_SUCCESS;
}


/**
  Update firmware image in the SPI flash.

  @param[in]   FileSize            Size of the file to be flashed
  @param[in]  *FileBuffer          Pointer to the file in memory
  @param[in]   Offset              First logical block to be updated.
                                   Irrelevant for SPI.

**/
STATIC
EFI_STATUS
EFIAPI
ProgramSpiFlash (
  IN UINT64      FileSize,
  IN UINTN      *FileBuffer,
  IN EFI_LBA     Offset
  )
{
  SPI_DEVICE    *SpiFlash;
  EFI_STATUS     Status;

  // Locate SPI protocols
  Status = gBS->LocateProtocol (&gMarvellSpiFlashProtocolGuid,
                  NULL,
                  (VOID **)&SpiFlashProtocol);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Cannot locate SpiFlash protocol\n", CMD_NAME_STRING);
    return Status;
  }

  Status = gBS->LocateProtocol (&gMarvellSpiMasterProtocolGuid,
                  NULL,
                  (VOID **)&SpiMasterProtocol);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Cannot locate SpiMaster protocol\n", CMD_NAME_STRING);
    return Status;
  }

  // Setup and probe SPI flash
  SpiFlash = NULL;
  SpiFlash = SpiMasterProtocol->SetupDevice (SpiMasterProtocol,
                                  SpiFlash,
                                  PcdGet32 (PcdSpiFlashCs),
                                  PcdGet32 (PcdSpiFlashMode));
  if (SpiFlash == NULL) {
    Print(L"%s: Cannot allocate SPI device!\n", CMD_NAME_STRING);
    return EFI_ABORTED;
  }

  Status = SpiFlashProbe (SpiFlash);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Error while performing SPI flash probe\n", CMD_NAME_STRING);
    goto FlashProbeError;
  }

  // Update firmware image in flash at offset 0x0
  Status = SpiFlashProtocol->Update (SpiFlash,
                               0x0,
                               FileSize,
                               (UINT8 *)FileBuffer);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Error while performing flash update\n", CMD_NAME_STRING);
    goto FlashProbeError;
  }

  // Release resources
  SpiMasterProtocol->FreeDevice (SpiFlash);

  return EFI_SUCCESS;

FlashProbeError:
  SpiMasterProtocol->FreeDevice (SpiFlash);

  return Status;
}

/**
  Parse commandline parameters and pick device for the firmware update.

  @param[in]     *CheckPackage        Shell command argument list.
  @param[in/out] *FlashCommand        Pointer to firmware update command.
                                      Picked, depending on the device type.
  @param[in/out] *Alignment           File buffer alignment value.
  @param[in/out] *Offset              First logical block to be updated.

**/
STATIC
EFI_STATUS
EFIAPI
SelectDevice (
  IN LIST_ENTRY             *CheckPackage,
  IN OUT FLASH_COMMAND      *FlashCommand,
  IN OUT UINT64             *Alignment,
  IN OUT EFI_LBA            *Offset
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  CONST CHAR16              *DeviceName;
  EFI_HANDLE                Handle;
  UINTN                     HandleIndex;
  EFI_STATUS                Status;

  //
  // SPI flash
  //
  DeviceName = ShellCommandLineGetRawValue (CheckPackage, 2);
  if (DeviceName == NULL || StrnCmp(DeviceName, L"spi", 4) == 0) {
    *FlashCommand = ProgramSpiFlash;
    *Alignment = 0;
    *Offset = 0;
    return EFI_SUCCESS;
  }

  if (ShellCommandLineGetFlag (CheckPackage, SHELL_USE_DEVICE_PATH_PARAM)) {
    //
    // SD/MMC is selected using device path string.
    //
    Status = gBS->LocateProtocol (&gEfiDevicePathFromTextProtocolGuid,
                    NULL,
                    (VOID **)&EfiDevicePathFromTextProtocol);
    if (EFI_ERROR (Status)) {
      Print (L"%s: cannot locate EfiDevicePathFromText protocol\n",
        CMD_NAME_STRING);
      return EFI_INVALID_PARAMETER;
    }

    DevicePath = EfiDevicePathFromTextProtocol->ConvertTextToDevicePath (DeviceName);
    if (DevicePath == NULL) {
      Print (L"%s: cannot locate EfiDevicePathFromText protocol\n",
        CMD_NAME_STRING);
      return EFI_INVALID_PARAMETER;
    }

    Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid,
                    &DevicePath,
                    &Handle);
    if (EFI_ERROR (Status)) {
      Print (L"%s: cannot locate selected block device\n", CMD_NAME_STRING);
      return Status;
    }
  } else {
    //
    // SD/MMC device is selected using handle number.
    //
    HandleIndex = ShellHexStrToUintn (DeviceName);
    if (HandleIndex == (UINTN)(-1)) {
      Print (L"%s: handle to device have to be hex number\n", CMD_NAME_STRING);
      return EFI_INVALID_PARAMETER;
    }

    Handle = ConvertHandleIndexToHandle (HandleIndex);
    if (Handle == NULL) {
      Print (L"%s: %x is not correct device handle\n",
        CMD_NAME_STRING,
        HandleIndex);
      return EFI_INVALID_PARAMETER;
    }
  }

  Status = IsDeviceSupported (Handle, Offset);
  if (EFI_ERROR (Status)) {
    Print (L"%s: device not supported\n", CMD_NAME_STRING);
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->OpenProtocol (Handle,
                  &gEfiBlockIoProtocolGuid,
                  (VOID**)&BlkIo,
                  gImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Cannot open device BlokIo protocol\n", CMD_NAME_STRING);
    return Status;
  }
  *FlashCommand = ProgramBlockDevice;
  *Alignment = BlkIo->Media->BlockSize;

  return EFI_SUCCESS;
}

/**
  Return the file name of the help text file if not using HII.

  @return The string pointer to the file name.
**/
STATIC
CONST CHAR16*
EFIAPI
ShellCommandGetManFileNameFUpdate (
  VOID
  )
{
  return gShellFUpdateFileName;
}

STATIC
VOID
FUpdateUsage (
  VOID
  )
{
  Print (L"\nFirmware update command\n"
         "fupdate <LocalFilePath> [-p] [Device]\n\n"
         "LocalFilePath - path to local firmware image file\n"
         "-p            - When flag is selected Device is interpreted\n"
         "                as device path, not device handle.\n"
         "Device        - Select device which will be flashed.\n"
         "                Supported devices can be listed using \"fupdate list\"\n"
         "                command. Device is represented by its handle.\n"
         "                The default value is spi.\n"
         "EXAMPLES:\n"
         " * Update firmware in SPI flash from file fs2:flash-image.bin\n"
         "     fupdate fs2:flash-image.bin\n"
         " * Update firmware in device with handle 5F from file flash-image.bin\n"
         "     fupdate flash-image.bin 5F\n"
         " * Update firmware in device with selected path from file flash.bin\n"
         "     fupdate flash.bin -p VenHw(0D51905B-B77E-452A-A2C0-ECA0CC8D514A,000078F20000000000)/SD(0x0)\n"
         " * List supported devices\n"
         "     fupdate list\n"
  );
}

STATIC
SHELL_STATUS
EFIAPI
ShellCommandRunFUpdate (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  SHELL_FILE_HANDLE  FileHandle;
  FLASH_COMMAND      FlashCommand;
  LIST_ENTRY         *CheckPackage;
  EFI_LBA            Offset;
  UINT64             Alignment;
  UINT64             FileSize;
  UINTN              *FileBuffer;
  CHAR16             *ProblemParam;
  EFI_STATUS         Status;

  FileBuffer = NULL;

  // Parse command line
  Status = ShellInitialize ();
  if (EFI_ERROR (Status)) {
    Print (L"%s: Error while initializing Shell\n", CMD_NAME_STRING);
    ASSERT_EFI_ERROR (Status);
    return SHELL_ABORTED;
  }

  Status = ShellCommandLineParse (ParamList, &CheckPackage, &ProblemParam, TRUE);
  if (EFI_ERROR (Status)) {
    Print (L"%s: Invalid parameter\n", CMD_NAME_STRING);
    return SHELL_ABORTED;
  }

  if (ShellCommandLineGetFlag (CheckPackage, SHELL_HELP_PARAM)) {
    FUpdateUsage ();
    return EFI_SUCCESS;
  }

  if (ShellCommandLineGetFlag (CheckPackage, SHELL_LIST_PARAM)) {
    ListSupportedDevices ();
    return EFI_SUCCESS;
  }

  // Select device to flash
  Status = SelectDevice (CheckPackage, &FlashCommand, &Alignment, &Offset);
  if (EFI_ERROR (Status)) {
    return SHELL_ABORTED;
  }

  // Prepare local file to be burned into flash
  Status = PrepareFirmwareImage (CheckPackage,
             &FileHandle,
             &FileBuffer,
             &FileSize,
             Alignment);
  if (EFI_ERROR (Status)) {
    return SHELL_ABORTED;
  }

  // Check image checksum and magic
  Status = CheckImageHeader (FileBuffer);
  if (EFI_ERROR(Status)) {
    goto HeaderError;
  }

  // Update firmware image
  Status = FlashCommand (FileSize, FileBuffer, Offset);
  if (EFI_ERROR (Status)) {
    goto HeaderError;
  }

  FreePool (FileBuffer);
  ShellCloseFile (&FileHandle);

  Print (L"%s: Update %d bytes at offset 0x%x succeeded!\n",
    CMD_NAME_STRING,
    FileSize,
    Offset * Alignment);

  return EFI_SUCCESS;

HeaderError:
  FreePool (FileBuffer);
  ShellCloseFile (&FileHandle);

  return SHELL_ABORTED;
}

EFI_STATUS
EFIAPI
ShellFUpdateCommandConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;

  gShellFUpdateHiiHandle = NULL;

  gShellFUpdateHiiHandle = HiiAddPackages (
                                  &gShellFUpdateHiiGuid,
                                  gImageHandle,
                                  UefiShellFUpdateCommandLibStrings,
                                  NULL
                                );

  if (gShellFUpdateHiiHandle == NULL) {
    Print (L"%s: Cannot add Hii package\n", CMD_NAME_STRING);
    return EFI_DEVICE_ERROR;
  }

  Status = ShellCommandRegisterCommandName (
                           CMD_NAME_STRING,
                           ShellCommandRunFUpdate,
                           ShellCommandGetManFileNameFUpdate,
                           0,
                           CMD_NAME_STRING,
                           TRUE,
                           gShellFUpdateHiiHandle,
                           STRING_TOKEN (STR_GET_HELP_FUPDATE)
                         );

  if (EFI_ERROR(Status)) {
    Print (L"%s: Error while registering command\n", CMD_NAME_STRING);
    return SHELL_ABORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ShellFUpdateCommandDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  if (gShellFUpdateHiiHandle != NULL) {
    HiiRemovePackages (gShellFUpdateHiiHandle);
  }

  return EFI_SUCCESS;
}
