/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/SmmCommunication.h>
#include <Guid/PiSmmCommunicationRegionTable.h>
#include <Guid/SmiHandlerProfile.h>

#define PROFILE_NAME_STRING_LENGTH  64
CHAR8 mNameString[PROFILE_NAME_STRING_LENGTH + 1];

VOID   *mSmiHandlerProfileDatabase;
UINTN  mSmiHandlerProfileDatabaseSize;

/**
  This function dump raw data.

  @param  Data  raw data
  @param  Size  raw data size
**/
VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  );

/**
  Get SMI handler profile database.
**/
VOID
GetSmiHandlerProfileDatabase(
  VOID
  )
{
  EFI_STATUS                                          Status;
  UINTN                                               CommSize;
  UINT8                                               *CommBuffer;
  EFI_SMM_COMMUNICATE_HEADER                          *CommHeader;
  SMI_HANDLER_PROFILE_PARAMETER_GET_INFO              *CommGetInfo;
  SMI_HANDLER_PROFILE_PARAMETER_GET_DATA_BY_OFFSET    *CommGetData;
  EFI_SMM_COMMUNICATION_PROTOCOL                      *SmmCommunication;
  UINTN                                               MinimalSizeNeeded;
  EDKII_PI_SMM_COMMUNICATION_REGION_TABLE             *PiSmmCommunicationRegionTable;
  UINT32                                              Index;
  EFI_MEMORY_DESCRIPTOR                               *Entry;
  VOID                                                *Buffer;
  UINTN                                               Size;
  UINTN                                               Offset;

  Status = gBS->LocateProtocol(&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **)&SmmCommunication);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "SmiHandlerProfile: Locate SmmCommunication protocol - %r\n", Status));
    return ;
  }

  MinimalSizeNeeded = EFI_PAGE_SIZE;

  Status = EfiGetSystemConfigurationTable(
             &gEdkiiPiSmmCommunicationRegionTableGuid,
             (VOID **)&PiSmmCommunicationRegionTable
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "SmiHandlerProfile: Get PiSmmCommunicationRegionTable - %r\n", Status));
    return ;
  }
  ASSERT(PiSmmCommunicationRegionTable != NULL);
  Entry = (EFI_MEMORY_DESCRIPTOR *)(PiSmmCommunicationRegionTable + 1);
  Size = 0;
  for (Index = 0; Index < PiSmmCommunicationRegionTable->NumberOfEntries; Index++) {
    if (Entry->Type == EfiConventionalMemory) {
      Size = EFI_PAGES_TO_SIZE((UINTN)Entry->NumberOfPages);
      if (Size >= MinimalSizeNeeded) {
        break;
      }
    }
    Entry = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)Entry + PiSmmCommunicationRegionTable->DescriptorSize);
  }
  ASSERT(Index < PiSmmCommunicationRegionTable->NumberOfEntries);
  CommBuffer = (UINT8 *)(UINTN)Entry->PhysicalStart;

  //
  // Get Size
  //
  CommHeader = (EFI_SMM_COMMUNICATE_HEADER *)&CommBuffer[0];
  CopyMem(&CommHeader->HeaderGuid, &gSmiHandlerProfileGuid, sizeof(gSmiHandlerProfileGuid));
  CommHeader->MessageLength = sizeof(SMI_HANDLER_PROFILE_PARAMETER_GET_INFO);

  CommGetInfo = (SMI_HANDLER_PROFILE_PARAMETER_GET_INFO *)&CommBuffer[OFFSET_OF(EFI_SMM_COMMUNICATE_HEADER, Data)];
  CommGetInfo->Header.Command = SMI_HANDLER_PROFILE_COMMAND_GET_INFO;
  CommGetInfo->Header.DataLength = sizeof(*CommGetInfo);
  CommGetInfo->Header.ReturnStatus = (UINT64)-1;
  CommGetInfo->DataSize = 0;

  CommSize = OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data) + (UINTN)CommHeader->MessageLength;
  Status = SmmCommunication->Communicate(SmmCommunication, CommBuffer, &CommSize);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "SmiHandlerProfile: SmmCommunication - %r\n", Status));
    return ;
  }

  if (CommGetInfo->Header.ReturnStatus != 0) {
    DEBUG ((DEBUG_INFO, "SmiHandlerProfile: GetInfo - 0x%0x\n", CommGetInfo->Header.ReturnStatus));
    return ;
  }

  mSmiHandlerProfileDatabaseSize = (UINTN)CommGetInfo->DataSize;

  //
  // Get Data
  //
  mSmiHandlerProfileDatabase = AllocateZeroPool(mSmiHandlerProfileDatabaseSize);
  if (mSmiHandlerProfileDatabase == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_INFO, "SmiHandlerProfile: AllocateZeroPool (0x%x) for dump buffer - %r\n", mSmiHandlerProfileDatabaseSize, Status));
    return ;
  }

  CommHeader = (EFI_SMM_COMMUNICATE_HEADER *)&CommBuffer[0];
  CopyMem(&CommHeader->HeaderGuid, &gSmiHandlerProfileGuid, sizeof(gSmiHandlerProfileGuid));
  CommHeader->MessageLength = sizeof(SMI_HANDLER_PROFILE_PARAMETER_GET_DATA_BY_OFFSET);

  CommGetData = (SMI_HANDLER_PROFILE_PARAMETER_GET_DATA_BY_OFFSET *)&CommBuffer[OFFSET_OF(EFI_SMM_COMMUNICATE_HEADER, Data)];
  CommGetData->Header.Command = SMI_HANDLER_PROFILE_COMMAND_GET_DATA_BY_OFFSET;
  CommGetData->Header.DataLength = sizeof(*CommGetData);
  CommGetData->Header.ReturnStatus = (UINT64)-1;

  CommSize = OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data) + (UINTN)CommHeader->MessageLength;
  Buffer = (UINT8 *)CommHeader + CommSize;
  Size -= CommSize;

  CommGetData->DataBuffer = (PHYSICAL_ADDRESS)(UINTN)Buffer;
  CommGetData->DataOffset = 0;
  while (CommGetData->DataOffset < mSmiHandlerProfileDatabaseSize) {
    Offset = (UINTN)CommGetData->DataOffset;
    if (Size <= (mSmiHandlerProfileDatabaseSize - CommGetData->DataOffset)) {
      CommGetData->DataSize = (UINT64)Size;
    } else {
      CommGetData->DataSize = (UINT64)(mSmiHandlerProfileDatabaseSize - CommGetData->DataOffset);
    }
    Status = SmmCommunication->Communicate(SmmCommunication, CommBuffer, &CommSize);
    ASSERT_EFI_ERROR(Status);

    if (CommGetData->Header.ReturnStatus != 0) {
      FreePool(mSmiHandlerProfileDatabase);
      mSmiHandlerProfileDatabase = NULL;
      DEBUG ((DEBUG_INFO, "SmiHandlerProfile: GetData - 0x%x\n", CommGetData->Header.ReturnStatus));
      return ;
    }
    CopyMem((UINT8 *)mSmiHandlerProfileDatabase + Offset, (VOID *)(UINTN)CommGetData->DataBuffer, (UINTN)CommGetData->DataSize);
  }

  DEBUG ((DEBUG_INFO, "SmiHandlerProfileSize - 0x%x\n", mSmiHandlerProfileDatabaseSize));

  return ;
}

/**
  Get the file name portion of the Pdb File Name.

  The portion of the Pdb File Name between the last backslash and
  either a following period or the end of the string is copied into
  AsciiBuffer.  The name is truncated, if necessary, to ensure that
  AsciiBuffer is not overrun.

  @param[in]  PdbFileName     Pdb file name.
  @param[out] AsciiBuffer     The resultant Ascii File Name.

**/
VOID
GetShortPdbFileName (
  IN  CHAR8     *PdbFileName,
  OUT CHAR8     *AsciiBuffer
  )
{
  UINTN IndexPdb;     // Current work location within a Pdb string.
  UINTN IndexBuffer;  // Current work location within a Buffer string.
  UINTN StartIndex;
  UINTN EndIndex;

  ZeroMem (AsciiBuffer, PROFILE_NAME_STRING_LENGTH + 1);

  if (PdbFileName == NULL) {
    AsciiStrnCpyS (AsciiBuffer, PROFILE_NAME_STRING_LENGTH + 1, " ", 1);
  } else {
    StartIndex = 0;
    for (EndIndex = 0; PdbFileName[EndIndex] != 0; EndIndex++);
    for (IndexPdb = 0; PdbFileName[IndexPdb] != 0; IndexPdb++) {
      if ((PdbFileName[IndexPdb] == '\\') || (PdbFileName[IndexPdb] == '/')) {
        StartIndex = IndexPdb + 1;
      }

      if (PdbFileName[IndexPdb] == '.') {
        EndIndex = IndexPdb;
      }
    }

    IndexBuffer = 0;
    for (IndexPdb = StartIndex; IndexPdb < EndIndex; IndexPdb++) {
      AsciiBuffer[IndexBuffer] = PdbFileName[IndexPdb];
      IndexBuffer++;
      if (IndexBuffer >= PROFILE_NAME_STRING_LENGTH) {
        AsciiBuffer[PROFILE_NAME_STRING_LENGTH] = 0;
        break;
      }
    }
  }
}

/**
  Get a human readable name for an image.
  The following methods will be tried orderly:
    1. Image PDB
    2. FFS UI section
    3. Image GUID

  @param[in] ImageStruct  Point to the image structure.

  @return The resulting Ascii name string is stored in the mNameString global array.

**/
CHAR8 *
GetDriverNameString (
  IN SMM_CORE_IMAGE_DATABASE_STRUCTURE  *ImageStruct
  )
{
  EFI_STATUS                  Status;
  CHAR16                      *NameString;
  UINTN                       StringSize;

  if (ImageStruct == NULL) {
    return "???";
  }

  //
  // Method 1: Get the name string from image PDB
  //
  if (ImageStruct->PdbStringOffset != 0) {
    GetShortPdbFileName ((CHAR8 *) ((UINTN) ImageStruct + ImageStruct->PdbStringOffset), mNameString);
    return mNameString;
  }

  if (!IsZeroGuid (&ImageStruct->FileGuid)) {
    //
    // Try to get the image's FFS UI section by image GUID
    //
    NameString = NULL;
    StringSize = 0;
    Status = GetSectionFromAnyFv (
              &ImageStruct->FileGuid,
              EFI_SECTION_USER_INTERFACE,
              0,
              (VOID **) &NameString,
              &StringSize
              );
    if (!EFI_ERROR (Status)) {
      //
      // Method 2: Get the name string from FFS UI section
      //
      if (StrLen (NameString) > PROFILE_NAME_STRING_LENGTH) {
        NameString[PROFILE_NAME_STRING_LENGTH] = 0;
      }
      UnicodeStrToAsciiStrS (NameString, mNameString, sizeof (mNameString));
      FreePool (NameString);
      return mNameString;
    }
  }

  //
  // Method 3: Get the name string from image GUID
  //
  AsciiSPrint (mNameString, sizeof (mNameString), "%g", &ImageStruct->FileGuid);
  return mNameString;
}

/**
  Get image structure from reference index.

  @param ImageRef   the image reference index

  @return image structure
**/
SMM_CORE_IMAGE_DATABASE_STRUCTURE *
GetImageFromRef (
  IN UINTN ImageRef
  )
{
  SMM_CORE_IMAGE_DATABASE_STRUCTURE  *ImageStruct;

  ImageStruct = (VOID *)mSmiHandlerProfileDatabase;
  while ((UINTN)ImageStruct < (UINTN)mSmiHandlerProfileDatabase + mSmiHandlerProfileDatabaseSize) {
    if (ImageStruct->Header.Signature == SMM_CORE_IMAGE_DATABASE_SIGNATURE) {
      if (ImageStruct->ImageRef == ImageRef) {
        return ImageStruct;
      }
    }
    ImageStruct = (VOID *)((UINTN)ImageStruct + ImageStruct->Header.Length);
  }

  return NULL;
}

/**
  Dump SMM loaded image information.
**/
VOID
DumpSmmLoadedImage(
  VOID
  )
{
  SMM_CORE_IMAGE_DATABASE_STRUCTURE  *ImageStruct;
  CHAR8                              *PdbString;
  CHAR8                              *NameString;

  ImageStruct = (VOID *)mSmiHandlerProfileDatabase;
  while ((UINTN)ImageStruct < (UINTN)mSmiHandlerProfileDatabase + mSmiHandlerProfileDatabaseSize) {
    if (ImageStruct->Header.Signature == SMM_CORE_IMAGE_DATABASE_SIGNATURE) {
      NameString = GetDriverNameString (ImageStruct);
      DEBUG ((DEBUG_INFO, "  <Image Name=\"%a\"", NameString));
      DEBUG ((DEBUG_INFO, " Base=\"0x%lx\" Size=\"0x%lx\"", ImageStruct->ImageBase, ImageStruct->ImageSize));
      if (ImageStruct->EntryPoint != 0) {
        DEBUG ((DEBUG_INFO, " EntryPoint=\"0x%lx\"", ImageStruct->EntryPoint));
      }
      DEBUG ((DEBUG_INFO, " FvFile=\"%g\"", &ImageStruct->FileGuid));
      DEBUG ((DEBUG_INFO, " RefId=\"0x%x\"", ImageStruct->ImageRef));
      DEBUG ((DEBUG_INFO, ">\n"));
      if (ImageStruct->PdbStringOffset != 0) {
        PdbString = (CHAR8 *)((UINTN)ImageStruct + ImageStruct->PdbStringOffset);
        DEBUG ((DEBUG_INFO, "    <Pdb>%a</Pdb>\n", PdbString));
      }
      DEBUG ((DEBUG_INFO, "  </Image>\n"));
    }

    ImageStruct = (VOID *)((UINTN)ImageStruct + ImageStruct->Header.Length);
  }

  return;
}

CHAR8 *mSxTypeString[] = {
  "SxS0",
  "SxS1",
  "SxS2",
  "SxS3",
  "SxS4",
  "SxS5",
};

/**
  Convert SxType to a string.

  @param Type SxType

  @return SxType string
**/
CHAR8 *
SxTypeToString (
  IN EFI_SLEEP_TYPE  Type
  )
{
  if (Type >= 0 && Type <= ARRAY_SIZE(mSxTypeString)) {
    return mSxTypeString[Type];
  } else {
    AsciiSPrint (mNameString, sizeof(mNameString), "0x%x", Type);
    return mNameString;
  }
}

CHAR8 *mSxPhaseString[] = {
  "SxEntry",
  "SxExit",
};

/**
  Convert SxPhase to a string.

  @param Phase SxPhase

  @return SxPhase string
**/
CHAR8 *
SxPhaseToString (
  IN EFI_SLEEP_PHASE Phase
  )
{
  if (Phase >= 0 && Phase <= ARRAY_SIZE(mSxPhaseString)) {
    return mSxPhaseString[Phase];
  } else {
    AsciiSPrint (mNameString, sizeof(mNameString), "0x%x", Phase);
    return mNameString;
  }
}

CHAR8 *mPowerButtonPhaseString[] = {
  "PowerButtonEntry",
  "PowerButtonExit",
};

/**
  Convert PowerButtonPhase to a string.

  @param Phase PowerButtonPhase

  @return PowerButtonPhase string
**/
CHAR8 *
PowerButtonPhaseToString (
  IN EFI_POWER_BUTTON_PHASE  Phase
  )
{
  if (Phase >= 0 && Phase <= ARRAY_SIZE(mPowerButtonPhaseString)) {
    return mPowerButtonPhaseString[Phase];
  } else {
    AsciiSPrint (mNameString, sizeof(mNameString), "0x%x", Phase);
    return mNameString;
  }
}

CHAR8 *mStandbyButtonPhaseString[] = {
  "StandbyButtonEntry",
  "StandbyButtonExit",
};

/**
  Convert StandbyButtonPhase to a string.

  @param Phase StandbyButtonPhase

  @return StandbyButtonPhase string
**/
CHAR8 *
StandbyButtonPhaseToString (
  IN EFI_STANDBY_BUTTON_PHASE  Phase
  )
{
  if (Phase >= 0 && Phase <= ARRAY_SIZE(mStandbyButtonPhaseString)) {
    return mStandbyButtonPhaseString[Phase];
  } else {
    AsciiSPrint (mNameString, sizeof(mNameString), "0x%x", Phase);
    return mNameString;
  }
}

CHAR8 *mIoTrapTypeString[] = {
  "WriteTrap",
  "ReadTrap",
  "ReadWriteTrap",
};

/**
  Convert IoTrapType to a string.

  @param Type IoTrapType

  @return IoTrapType string
**/
CHAR8 *
IoTrapTypeToString (
  IN EFI_SMM_IO_TRAP_DISPATCH_TYPE  Type
  )
{
  if (Type >= 0 && Type <= ARRAY_SIZE(mIoTrapTypeString)) {
    return mIoTrapTypeString[Type];
  } else {
    AsciiSPrint (mNameString, sizeof(mNameString), "0x%x", Type);
    return mNameString;
  }
}

CHAR8 *mUsbTypeString[] = {
  "UsbLegacy",
  "UsbWake",
};

/**
  Convert UsbType to a string.

  @param Type UsbType

  @return UsbType string
**/
CHAR8 *
UsbTypeToString (
  IN EFI_USB_SMI_TYPE          Type
  )
{
  if (Type >= 0 && Type <= ARRAY_SIZE(mUsbTypeString)) {
    return mUsbTypeString[Type];
  } else {
    AsciiSPrint (mNameString, sizeof(mNameString), "0x%x", Type);
    return mNameString;
  }
}

/**
  Dump SMI child context.

  @param HandlerType  the handler type
  @param Context      the handler context
  @param ContextSize  the handler context size
**/
VOID
DumpSmiChildContext (
  IN EFI_GUID   *HandlerType,
  IN VOID       *Context,
  IN UINTN      ContextSize
  )
{
  CHAR16        *Str;

  if (CompareGuid (HandlerType, &gEfiSmmSwDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " SwSmi=\"0x%lx\"", ((SMI_HANDLER_PROFILE_SW_REGISTER_CONTEXT *)Context)->SwSmiInputValue));
  } else if (CompareGuid (HandlerType, &gEfiSmmSxDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " SxType=\"%a\"", SxTypeToString(((EFI_SMM_SX_REGISTER_CONTEXT *)Context)->Type)));
    DEBUG ((DEBUG_INFO, " SxPhase=\"%a\"", SxPhaseToString(((EFI_SMM_SX_REGISTER_CONTEXT *)Context)->Phase)));
  } else if (CompareGuid (HandlerType, &gEfiSmmPowerButtonDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " PowerButtonPhase=\"%a\"", PowerButtonPhaseToString(((EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT *)Context)->Phase)));
  } else if (CompareGuid (HandlerType, &gEfiSmmStandbyButtonDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " StandbyButtonPhase=\"%a\"", StandbyButtonPhaseToString(((EFI_SMM_STANDBY_BUTTON_REGISTER_CONTEXT *)Context)->Phase)));
  } else if (CompareGuid (HandlerType, &gEfiSmmPeriodicTimerDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " PeriodicTimerPeriod=\"%ld\"", ((EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT *)Context)->Period));
    DEBUG ((DEBUG_INFO, " PeriodicTimerSmiTickInterval=\"%ld\"", ((EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT *)Context)->SmiTickInterval));
  } else if (CompareGuid (HandlerType, &gEfiSmmGpiDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " GpiNum=\"0x%lx\"", ((EFI_SMM_GPI_REGISTER_CONTEXT *)Context)->GpiNum));
  } else if (CompareGuid (HandlerType, &gEfiSmmIoTrapDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " IoTrapAddress=\"0x%x\"", ((EFI_SMM_IO_TRAP_REGISTER_CONTEXT *)Context)->Address));
    DEBUG ((DEBUG_INFO, " IoTrapLength=\"0x%x\"", ((EFI_SMM_IO_TRAP_REGISTER_CONTEXT *)Context)->Length));
    DEBUG ((DEBUG_INFO, " IoTrapType=\"%a\"", IoTrapTypeToString(((EFI_SMM_IO_TRAP_REGISTER_CONTEXT *)Context)->Type)));
  } else if (CompareGuid (HandlerType, &gEfiSmmUsbDispatch2ProtocolGuid)) {
    DEBUG ((DEBUG_INFO, " UsbType=\"0x%x\"", UsbTypeToString(((SMI_HANDLER_PROFILE_USB_REGISTER_CONTEXT *)Context)->Type)));
    Str = ConvertDevicePathToText((EFI_DEVICE_PATH_PROTOCOL *)(((SMI_HANDLER_PROFILE_USB_REGISTER_CONTEXT *)Context) + 1), TRUE, TRUE);
    DEBUG ((DEBUG_INFO, " UsbDevicePath=\"%s\"", Str));
    if (Str != NULL) {
      FreePool (Str);
    }
  } else {
    DEBUG ((DEBUG_INFO, " Context=\""));
    InternalDumpData (Context, ContextSize);
    DEBUG ((DEBUG_INFO, "\""));
  }
}

/**
  Dump SMI handler in HandlerCategory.

  @param HandlerCategory  SMI handler category
**/
VOID
DumpSmiHandler(
  IN UINT32 HandlerCategory
  )
{
  SMM_CORE_SMI_DATABASE_STRUCTURE    *SmiStruct;
  SMM_CORE_SMI_HANDLER_STRUCTURE     *SmiHandlerStruct;
  UINTN                              Index;
  SMM_CORE_IMAGE_DATABASE_STRUCTURE  *ImageStruct;
  CHAR8                              *NameString;

  SmiStruct = (VOID *)mSmiHandlerProfileDatabase;
  while ((UINTN)SmiStruct < (UINTN)mSmiHandlerProfileDatabase + mSmiHandlerProfileDatabaseSize) {
    if ((SmiStruct->Header.Signature == SMM_CORE_SMI_DATABASE_SIGNATURE) && (SmiStruct->HandlerCategory == HandlerCategory)) {
      SmiHandlerStruct = (VOID *)(SmiStruct + 1);
      DEBUG ((DEBUG_INFO, "  <SmiEntry"));
      if (!IsZeroGuid (&SmiStruct->HandlerType)) {
        DEBUG ((DEBUG_INFO, " HandlerType=\"%g\"", &SmiStruct->HandlerType));
      }
      DEBUG ((DEBUG_INFO, ">\n"));
      for (Index = 0; Index < SmiStruct->HandlerCount; Index++) {
        DEBUG ((DEBUG_INFO, "    <SmiHandler"));
        if (SmiHandlerStruct->ContextBufferSize != 0) {
          DumpSmiChildContext (&SmiStruct->HandlerType, (UINT8 *)SmiHandlerStruct + SmiHandlerStruct->ContextBufferOffset, SmiHandlerStruct->ContextBufferSize);
        }
        DEBUG ((DEBUG_INFO, ">\n"));
        ImageStruct = GetImageFromRef((UINTN)SmiHandlerStruct->ImageRef);
        NameString = GetDriverNameString (ImageStruct);
        DEBUG ((DEBUG_INFO, "      <Module RefId=\"0x%x\" Name=\"%a\">\n", SmiHandlerStruct->ImageRef, NameString));
        if ((ImageStruct != NULL) && (ImageStruct->PdbStringOffset != 0)) {
          DEBUG ((DEBUG_INFO, "      <Pdb>%a</Pdb>\n", (UINT8 *)ImageStruct + ImageStruct->PdbStringOffset));
        }
        DEBUG ((DEBUG_INFO, "      </Module>\n"));
        DEBUG ((DEBUG_INFO, "      <Handler Address=\"0x%lx\">\n", SmiHandlerStruct->Handler));
        if (ImageStruct != NULL) {
          DEBUG ((DEBUG_INFO, "         <RVA>0x%x</RVA>\n", (UINTN) (SmiHandlerStruct->Handler - ImageStruct->ImageBase)));
        }
        DEBUG ((DEBUG_INFO, "      </Handler>\n", SmiHandlerStruct->Handler));
        DEBUG ((DEBUG_INFO, "      <Caller Address=\"0x%lx\">\n", SmiHandlerStruct->CallerAddr));
        if (ImageStruct != NULL) {
          DEBUG ((DEBUG_INFO, "         <RVA>0x%x</RVA>\n", (UINTN) (SmiHandlerStruct->CallerAddr - ImageStruct->ImageBase)));
        }
        DEBUG ((DEBUG_INFO, "      </Caller>\n", SmiHandlerStruct->Handler));
        SmiHandlerStruct = (VOID *)((UINTN)SmiHandlerStruct + SmiHandlerStruct->Length);
        DEBUG ((DEBUG_INFO, "    </SmiHandler>\n"));
      }
      DEBUG ((DEBUG_INFO, "  </SmiEntry>\n"));
    }
    SmiStruct = (VOID *)((UINTN)SmiStruct + SmiStruct->Header.Length);
  }

  return;
}

EFI_STATUS
EFIAPI
TestPointCheckSmiHandlerInstrument (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmiHandlerInstrument - Enter\n"));

  GetSmiHandlerProfileDatabase();

  if (mSmiHandlerProfileDatabase == NULL) {
    return EFI_SUCCESS;
  }

  //
  // Dump all image
  //
  DEBUG ((DEBUG_INFO, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
  DEBUG ((DEBUG_INFO, "<SmiHandlerProfile>\n"));
  DEBUG ((DEBUG_INFO, "<ImageDatabase>\n"));
  DEBUG ((DEBUG_INFO, "  <!-- SMM image loaded -->\n"));
  DumpSmmLoadedImage();
  DEBUG ((DEBUG_INFO, "</ImageDatabase>\n\n"));

  //
  // Dump SMI Handler
  //
  DEBUG ((DEBUG_INFO, "<SmiHandlerDatabase>\n"));
  DEBUG ((DEBUG_INFO, "  <!-- SMI Handler registered -->\n\n"));
  DEBUG ((DEBUG_INFO, "  <SmiHandlerCategory Name=\"RootSmi\">\n"));
  DEBUG ((DEBUG_INFO, "  <!-- The root SMI Handler registered by SmmCore -->\n"));
  DumpSmiHandler(SmmCoreSmiHandlerCategoryRootHandler);
  DEBUG ((DEBUG_INFO, "  </SmiHandlerCategory>\n\n"));

  DEBUG ((DEBUG_INFO, "  <SmiHandlerCategory Name=\"GuidSmi\">\n"));
  DEBUG ((DEBUG_INFO, "  <!-- The GUID SMI Handler registered by SmmCore -->\n"));
  DumpSmiHandler(SmmCoreSmiHandlerCategoryGuidHandler);
  DEBUG ((DEBUG_INFO, "  </SmiHandlerCategory>\n\n"));

  DEBUG ((DEBUG_INFO, "  <SmiHandlerCategory Name=\"HardwareSmi\">\n"));
  DEBUG ((DEBUG_INFO, "  <!-- The hardware SMI Handler registered by SmmChildDispatcher -->\n"));
  DumpSmiHandler(SmmCoreSmiHandlerCategoryHardwareHandler);
  DEBUG ((DEBUG_INFO, "  </SmiHandlerCategory>\n\n"));

  DEBUG ((DEBUG_INFO, "</SmiHandlerDatabase>\n"));
  DEBUG ((DEBUG_INFO, "</SmiHandlerProfile>\n"));

  if (mSmiHandlerProfileDatabase != NULL) {
    FreePool(mSmiHandlerProfileDatabase);
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmiHandlerInstrument - Exit\n"));
  return EFI_SUCCESS;
}
