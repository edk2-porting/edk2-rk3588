/**@file
This utility is part of build process for IA32/X64 FD.
It generates FIT table.

Copyright (c) 2010-2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "FitGen.h"

//
// FIT spec
//
#pragma pack (1)
typedef struct {
  UINT64     Address;
  UINT8      Size[3];
  UINT8      Rsvd;
  UINT16     Version;
  UINT8      Type:7;
  UINT8      C_V:1;
  UINT8      Checksum;
} FIRMWARE_INTERFACE_TABLE_ENTRY;

//
// Struct for policy
//
typedef struct {
  UINT16     IndexPort;
  UINT16     DataPort;
  UINT8      Width;
  UINT8      Bit;
  UINT16     Index;
  UINT8      Size[3];
  UINT8      Rsvd;
  UINT16     Version; // == 0
  UINT8      Type:7;
  UINT8      C_V:1;
  UINT8      Checksum;
} FIRMWARE_INTERFACE_TABLE_ENTRY_PORT;

#define FIT_ALIGNMENT          0x3F // 0xF is required previously, but if we need exclude FIT, we must set 64 bytes alignment.
#define BIOS_MODULE_ALIGNMENT  0x3F  // 64 bytes for AnC
#define MICROCODE_ALIGNMENT    0x7FF

#define ACM_PKCS_1_5_RSA_SIGNATURE_SHA256_SIZE          256
#define ACM_PKCS_1_5_RSA_SIGNATURE_SHA384_SIZE          384

#define ACM_HEADER_VERSION_3  (3 << 16)
#define ACM_HEADER_VERSION_0  (0)
#define ACM_MODULE_TYPE_CHIPSET_ACM                     2
#define ACM_MODULE_SUBTYPE_CAPABLE_OF_EXECUTE_AT_RESET  0x1
#define ACM_MODULE_SUBTYPE_ANC_MODULE                   0x2
#define ACM_MODULE_FLAG_PREPRODUCTION                   0x4000
#define ACM_MODULE_FLAG_DEBUG_SIGN                      0x8000


typedef struct {
  UINT16     ModuleType;
  UINT16     ModuleSubType;
  UINT32     HeaderLen;
  UINT32     HeaderVersion;
  UINT16     ChipsetID;
  UINT16     Flags;
  UINT32     ModuleVendor;
  UINT32     Date;
  UINT32     Size;
  UINT16     TxtSvn;
  UINT16     SeSvn;
  UINT32     CodeControl;
  UINT32     ErrorEntryPoint;
  UINT32     GDTLimit;
  UINT32     GDTBasePtr;
  UINT32     SegSel;
  UINT32     EntryPoint;
  UINT8      Rsvd2[64];
  UINT32     KeySize; // 64
  UINT32     ScratchSize; // 2 * KeySize + 15
//UINT8      RSAPubKey[64 * 4]; // KeySize * 4
//UINT32     RSAPubExp;
//UINT8      RSASig[256];
  // End of AC module header
//UINT8      Scratch[(64 * 2 + 15) * 4]; // ScratchSize * 4
  // User Area
//UINT8      UserArea[1];
} ACM_FORMAT;

#define CHIPSET_ACM_INFORMATION_TABLE_VERSION_3  0x03
#define CHIPSET_ACM_INFORMATION_TABLE_VERSION_4  0x04

#define CHIPSET_ACM_INFORMATION_TABLE_VERSION    CHIPSET_ACM_INFORMATION_TABLE_VERSION_3

#define CHIPSET_ACM_INFORMATION_TABLE_GUID_V03 \
  { 0x7FC03AAA, 0x18DB46A7, 0x8F69AC2E, 0x5A7F418D }

#define CHIPSET_ACM_TYPE_BIOS   0
#define CHIPSET_ACM_TYPE_SINIT  1

typedef struct {
  UINT32    Guid0;
  UINT32    Guid1;
  UINT32    Guid2;
  UINT32    Guid3;
} ACM_GUID;

typedef struct {
  ACM_GUID   Guid;
  UINT8      ChipsetACMType;
  UINT8      Version;
  UINT16     Length;
  UINT32     ChipsetIDList;
  UINT32     OsSinitTableVer;
  UINT32     MinMleHeaderVer;
//#if (CHIPSET_ACM_INFORMATION_TABLE_VERSION >= CHIPSET_ACM_INFORMATION_TABLE_VERSION_3)
  UINT32     Capabilities;
  UINT8      AcmVersion;
  UINT8      AcmRevision[3];
//#if (CHIPSET_ACM_INFORMATION_TABLE_VERSION >= CHIPSET_ACM_INFORMATION_TABLE_VERSION_4)
  UINT32     ProcessorIDList;
//#endif
//#endif
} CHIPSET_ACM_INFORMATION_TABLE;

#define ACM_CHIPSET_ID_REVISION_ID_MAKE  0x1

typedef struct {
  UINT32     Flags;
  UINT16     VendorID;
  UINT16     DeviceID;
  UINT16     RevisionID;
  UINT8      Reserved[6];
} ACM_CHIPSET_ID;

typedef struct {
  UINT32           Count;
  ACM_CHIPSET_ID   ChipsetID[1];
} CHIPSET_ID_LIST;

typedef struct {
  UINT32     FMS;
  UINT32     FMSMask;
  UINT64     PlatformID;
  UINT64     PlatformMask;
} ACM_PROCESSOR_ID;

typedef struct {
  UINT32           Count;
  ACM_PROCESSOR_ID ProcessorID[1];
} PROCESSOR_ID_LIST;

#pragma pack ()


ACM_GUID mChipsetAcmInformationTableGuid03 = CHIPSET_ACM_INFORMATION_TABLE_GUID_V03;


//
// BIOS INFO data structure
// This is self contained data structure for BIOS info
//
#pragma pack (1)
#define BIOS_INFO_SIGNATURE  SIGNATURE_64 ('$', 'B', 'I', 'O', 'S', 'I', 'F', '$')
typedef struct {
  UINT64            Signature;
  UINT32            EntryCount;
  UINT32            Reserved;
//BIOS_INFO_STRUCT  Struct[EntryCount];
} BIOS_INFO_HEADER;

//
// BIOS_INFO_STRUCT attributes
// bits[0:3] means general attributes
// bits[4:7] means type specific attributes
//
#define BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT  0x01
#define BIOS_INFO_STRUCT_ATTRIBUTE_MICROCODE_WHOLE_REGION    0x10
#define BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB             0x10

typedef struct {
  //
  // FitTable entry type
  //
  UINT8    Type;
  //
  // BIOS_INFO_STRUCT attributes
  //
  UINT8    Attributes;
  //
  // FitTable entry version
  //
  UINT16   Version;
  //
  // FitTable entry real size
  //
  UINT32   Size;
  //
  // FitTable entry address
  //
  UINT64   Address;
} BIOS_INFO_STRUCT;

#pragma pack ()

#define MAX_BIOS_MODULE_ENTRY  0x20
#define MAX_MICROCODE_ENTRY    0x20
#define MAX_OPTIONAL_ENTRY     0x20
#define MAX_PORT_ENTRY         0x20

#define DEFAULT_FIT_TABLE_POINTER_OFFSET  0x40
#define DEFAULT_FIT_ENTRY_VERSION         0x0100

#define TOP_FLASH_ADDRESS  (gFitTableContext.TopFlashAddressRemapValue)

#define MEMORY_TO_FLASH(FileBuffer, FvBuffer, FvSize)  \
                 (UINTN)(TOP_FLASH_ADDRESS - ((UINTN)(FvBuffer) + (UINTN)(FvSize) - (UINTN)(FileBuffer)))
#define FLASH_TO_MEMORY(Address, FvBuffer, FvSize)  \
                 (VOID *)(UINTN)((UINTN)(FvBuffer) + (UINTN)(FvSize) - (TOP_FLASH_ADDRESS - (UINTN)(Address)))

#define FIT_TABLE_TYPE_HEADER                 0
#define FIT_TABLE_TYPE_MICROCODE              1
#define FIT_TABLE_TYPE_STARTUP_ACM            2
#define FIT_TABLE_TYPE_DIAGNST_ACM            3
#define FIT_TABLE_TYPE_BIOS_MODULE            7
#define FIT_TABLE_TYPE_TPM_POLICY             8
#define FIT_TABLE_TYPE_BIOS_POLICY            9
#define FIT_TABLE_TYPE_TXT_POLICY             10
#define FIT_TABLE_TYPE_KEY_MANIFEST           11
#define FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST   12
#define FIT_TABLE_TYPE_BIOS_DATA_AREA         13
#define FIT_TABLE_TYPE_CSE_SECURE_BOOT        16
#define FIT_TABLE_SUBTYPE_FIT_PATCH_MANIFEST  12
#define FIT_TABLE_SUBTYPE_ACM_MANIFEST        13

//
// With OptionalModule Address isn't known until free space has been
// identified and the optional module has been copied into the FLASH
// image buffer (or initialized to be populated later by another program).
// This is very dangerous code as it can truncate 64b pointers to
// allocated memory buffers.  The full pointer is in Buffer for that case.
//
typedef struct {
  UINT32  Type;
  UINT32  SubType; // Used by OptionalModule only
  UINT32  Address;
  UINT8   *Buffer; // Used by OptionalModule only
  UINT32  Size;
  UINT32  Version; // Used by OptionalModule and PortModule only
} FIT_TABLE_CONTEXT_ENTRY;

typedef struct {
  BOOLEAN                    Clear;
  UINT32                     FitTablePointerOffset;
  UINT32                     FitTablePointerOffset2;
  UINT32                     FitEntryNumber;
  UINT32                     BiosModuleNumber;
  UINT32                     MicrocodeNumber;
  UINT32                     OptionalModuleNumber;
  UINT32                     PortModuleNumber;
  UINT32                     GlobalVersion;
  UINT32                     FitHeaderVersion;
  FIT_TABLE_CONTEXT_ENTRY    StartupAcm;
  UINT32                     StartupAcmVersion;
  FIT_TABLE_CONTEXT_ENTRY    DiagnstAcm;
  UINT32                     DiagnstAcmVersion;
  FIT_TABLE_CONTEXT_ENTRY    BiosModule[MAX_BIOS_MODULE_ENTRY];
  UINT32                     BiosModuleVersion;
  FIT_TABLE_CONTEXT_ENTRY    Microcode[MAX_MICROCODE_ENTRY];
  BOOLEAN                    MicrocodeIsAligned;
  UINT32                     MicrocodeAlignValue;
  UINT32                     MicrocodeVersion;
  FIT_TABLE_CONTEXT_ENTRY    OptionalModule[MAX_OPTIONAL_ENTRY];
  FIT_TABLE_CONTEXT_ENTRY    PortModule[MAX_PORT_ENTRY];
  UINT64                     TopFlashAddressRemapValue;
} FIT_TABLE_CONTEXT;

FIT_TABLE_CONTEXT   gFitTableContext = {0};

unsigned int
xtoi (
  char  *str
  );

VOID
PrintUtilityInfo (
  VOID
  )
/*++

Routine Description:

  Displays the standard utility information to STDOUT

Arguments:

  None

Returns:

  None

--*/
{
  printf (
    "%s - Tiano IA32/X64 FIT table generation Utility for FIT spec revision 1.2."" Version %i.%i\n\n",
    UTILITY_NAME,
    UTILITY_MAJOR_VERSION,
    UTILITY_MINOR_VERSION
    );
}

VOID
PrintUsage (
  VOID
  )
/*++

Routine Description:

  Displays the utility usage syntax to STDOUT

Arguments:

  None

Returns:

  None

--*/
{
  printf ("Usage (generate): %s [-D] InputFvRecoveryFile OutputFvRecoveryFile\n"
          "\t[-V <FitEntryDefaultVersion>]\n"
          "\t[-F <FitTablePointerOffset>] [-F <FitTablePointerOffset>] [-V <FitHeaderVersion>]\n"
          "\t[-NA]\n"
          "\t[-A <MicrocodeAlignment>]\n"
          "\t[-REMAP <TopFlashAddress>\n"
          "\t[-CLEAR]\n"
          "\t[-L <MicrocodeSlotSize> <MicrocodeFfsGuid>]\n"
          "\t[-LF <MicrocodeSlotSize>]\n"
          "\t[-I <BiosInfoGuid>]\n"
          "\t[-S <StartupAcmAddress StartupAcmSize>|<StartupAcmGuid>] [-V <StartupAcmVersion>]\n"
          "\t[-U <DiagnstAcmAddress>|<DiagnstAcmGuid>]\n"
          "\t[-B <BiosModuleAddress BiosModuleSize>] [-B ...] [-V <BiosModuleVersion>]\n"
          "\t[-M <MicrocodeAddress MicrocodeSize>] [-M ...]|[-U <MicrocodeFv MicrocodeBase>|<MicrocodeRegionOffset MicrocodeRegionSize>|<MicrocodeGuid>] [-V <MicrocodeVersion>]\n"
          "\t[-O RecordType <RecordDataAddress RecordDataSize>|<RESERVE RecordDataSize>|<RecordDataGuid>|<RecordBinFile>|<CseRecordSubType RecordBinFile> [-V <RecordVersion>]] [-O ... [-V ...]]\n"
          "\t[-P RecordType <IndexPort DataPort Width Bit Index> [-V <RecordVersion>]] [-P ... [-V ...]]\n"
          , UTILITY_NAME);
  printf ("  Where:\n");
  printf ("\t-D                     - It is FD file instead of FV file. (The tool will search FV file)\n");
  printf ("\tInputFvRecoveryFile    - Name of the input FvRecovery.fv file.\n");
  printf ("\tOutputFvRecoveryFile   - Name of the output FvRecovery.fv file.\n");
  printf ("\tFitTablePointerOffset  - FIT table pointer offset. 0x%x as default. 0x18 for current soon to be obsoleted CPUs. User can set both.\n", DEFAULT_FIT_TABLE_POINTER_OFFSET);
  printf ("\tBiosInfoGuid           - Guid of BiosInfo Module. If this module exists, StartupAcm/Bios/Microcode can be optional.\n");
  printf ("\tStartupAcmAddress      - Address of StartupAcm.\n");
  printf ("\tStartupAcmSize         - Size of StartupAcm.\n");
  printf ("\tStartupAcmGuid         - Guid of StartupAcm Module, if StartupAcm is in a BiosModule, it will be excluded form that.\n");
  printf ("\tDiagnstAcmAddress      - Address of DiagnstAcm.\n");
  printf ("\tDiagnstAcmGuid         - Guid of DiagnstAcm Module, if DiagnstAcm is in a BiosModule, it will be excluded from that.\n");
  printf ("\tBiosModuleAddress      - Address of BiosModule. User should ensure there is no overlap.\n");
  printf ("\tBiosModuleSize         - Size of BiosModule.\n");
  printf ("\tMicrocodeAddress       - Address of Microcode.\n");
  printf ("\tMicrocodeSize          - Size of Microcode.\n");
  printf ("\tMicrocodeFv            - Name of Microcode.fv file.\n");
  printf ("\tMicrocodeBase          - The base address of Microcode.fv in final FD image.\n");
  printf ("\tMicrocodeRegionOffset  - Offset of Microcode region in input FD image.\n");
  printf ("\tMicrocodeRegionSize    - Size of Microcode region in input FD image.\n");
  printf ("\tMicrocodeGuid          - Guid of Microcode Module.\n");
  printf ("\tMicrocodeSlotSize      - Occupied region size of each Microcode binary.\n");
  printf ("\tMicrocodeFfsGuid       - Guid of FFS which is used to save Microcode binary");
  printf ("\t-LF                    - Microcode Slot mode without FFS check, treat all Microcode FV as slot mode. In this case the Microcode FV should only contain one FFS.\n");
  printf ("\t-NA                    - No 0x800 aligned Microcode requirement. No -NA means Microcode is aligned with option MicrocodeAlignment value.\n");
  printf ("\tMicrocodeAlignment     - HEX value of Microcode alignment. Ignored if \"-NA\" is specified. Default value is 0x800. The Microcode update data must start at a 16-byte aligned linear address.\n");
  printf ("\tRecordType             - FIT entry record type. User should ensure it is ordered.\n");
  printf ("\tRecordDataAddress      - FIT entry record data address.\n");
  printf ("\tRecordDataSize         - FIT entry record data size.\n");
  printf ("\tRecordDataGuid         - FIT entry record data GUID.\n");
  printf ("\tRecordBinFile          - FIT entry record data binary file.\n");
  printf ("\tCseRecordSubType       - FIT entry record subtype. Use to further distinguish CSE entries (see FIT spec revision 1.2 chapter 4.12).\n");
  printf ("\tFitEntryDefaultVersion - The default version for all FIT table entries. 0x%04x is used if this is not specified.\n", DEFAULT_FIT_ENTRY_VERSION);
  printf ("\tFitHeaderVersion       - The version for FIT header. (Override default version)\n");
  printf ("\tStartupAcmVersion      - The version for StartupAcm. (Override default version)\n");
  printf ("\tBiosModuleVersion      - The version for BiosModule. (Override default version)\n");
  printf ("\tMicrocodeVersion       - The version for Microcode. (Override default version)\n");
  printf ("\tRecordVersion          - The version for Record. (Override default version)\n");
  printf ("\tIndexPort              - The Index Port Number.\n");
  printf ("\tDataPort               - The Data Port Number.\n");
  printf ("\tWidth                  - The Width of the port.\n");
  printf ("\tBit                    - The Bit Number of the port.\n");
  printf ("\tIndex                  - The Index Number of the port.\n");
  printf ("\nUsage (view): %s [-view] InputFile -F <FitTablePointerOffset>\n", UTILITY_NAME);
  printf ("  Where:\n");
  printf ("\tInputFile              - Name of the input file.\n");
  printf ("\tFitTablePointerOffset  - FIT table pointer offset from end of file. 0x%x as default.\n", DEFAULT_FIT_TABLE_POINTER_OFFSET);
  printf ("\nTool return values:\n");
  printf ("\tSTATUS_SUCCESS=%d, STATUS_WARNING=%d, STATUS_ERROR=%d\n", STATUS_SUCCESS, STATUS_WARNING, STATUS_ERROR);
}

VOID *
SetMem (
  OUT     VOID                      *Buffer,
  IN      UINTN                     Length,
  IN      UINT8                     Value
  )
{
  //
  // Declare the local variables that actually move the data elements as
  // volatile to prevent the optimizer from replacing this function with
  // the intrinsic memset()
  //
  volatile UINT8                    *Pointer;

  Pointer = (UINT8*)Buffer;
  while (Length-- > 0) {
    *(Pointer++) = Value;
  }
  return Buffer;
}

BOOLEAN
CheckPath (
  IN CHAR8 * String
)
{
  //
  //Return FLASE if  input file path include % character or is NULL
  //
  CHAR8 *StrPtr;

  StrPtr = String;
  if (StrPtr == NULL) {
    return FALSE;
  }

  if (*StrPtr == 0) {
    return FALSE;
  }

  while (*StrPtr != '\0') {
    if (*StrPtr == '%') {
      return FALSE;
    }
    StrPtr++;
  }
  return TRUE;
}

STATUS
ReadInputFile (
  IN CHAR8    *FileName,
  OUT UINT8   **FileData,
  OUT UINT32  *FileSize,
  OUT UINT8   **FileBufferRaw OPTIONAL
  )
/*++

Routine Description:

  Read input file

Arguments:

  FileName      - The input file name
  FileData      - The input file data, the memory is aligned.
  FileSize      - The input file size
  FileBufferRaw - The memory to hold input file data. The caller must free the memory.

Returns:

  STATUS_SUCCESS - The file found and data read
  STATUS_ERROR   - The file data is not read
  STATUS_WARNING - The file is not found

--*/
{
  FILE                        *FpIn;
  UINT32                      TempResult;

  //
  //Check the File Path
  //
  if (!CheckPath(FileName)) {

    Error (NULL, 0, 0, "File path is invalid!", NULL);
    return STATUS_ERROR;
  }

  //
  // Open the Input FvRecovery.fv file
  //
  if ((FpIn = fopen (FileName, "rb")) == NULL) {
    //
    // Return WARNING, let caller make decision
    //
//    Error (NULL, 0, 0, "Unable to open file", FileName);
    return STATUS_WARNING;
  }
  //
  // Get the Input FvRecovery.fv file size
  //
  fseek (FpIn, 0, SEEK_END);
  *FileSize = ftell (FpIn);
  //
  // Read the contents of input file to memory buffer
  //
  if (FileBufferRaw != NULL) {
    *FileBufferRaw = (UINT8 *) malloc (*FileSize + 0x10000);
    if (NULL == *FileBufferRaw) {
      Error (NULL, 0, 0, "No sufficient memory to allocate!", NULL);
      fclose (FpIn);
      return STATUS_ERROR;
    }
    TempResult = 0x10000 - (UINT32) ((UINTN)*FileBufferRaw & 0x0FFFF);
    *FileData = (UINT8 *)((UINTN)*FileBufferRaw + TempResult);
  } else {
    *FileData = (UINT8 *) malloc (*FileSize);
     if (NULL == *FileData) {
      Error (NULL, 0, 0, "No sufficient memory to allocate!", NULL);
      fclose (FpIn);
      return STATUS_ERROR;
    }
  }
  fseek (FpIn, 0, SEEK_SET);
  TempResult = fread (*FileData, 1, *FileSize, FpIn);
  if (TempResult != *FileSize) {
    Error (NULL, 0, 0, "Read input file error!", NULL);
    if (FileBufferRaw != NULL) {
      free ((VOID *)*FileBufferRaw);
    } else {
      free ((VOID *)*FileData);
    }
    fclose (FpIn);
    return STATUS_ERROR;
  }

  //
  // Close the input FvRecovery.fv file
  //
  fclose (FpIn);

  return STATUS_SUCCESS;
}

UINT8 *
FindNextFvHeader (
  IN UINT8 *FileBuffer,
  IN UINTN  FileLength
  )
/*++

  Routine Description:
    Find next FvHeader in the FileBuffer

  Parameters:
    FileBuffer        -   The start FileBuffer which needs to be searched
    FileLength        -   The whole File Length.
  Return:
    FvHeader          -   The FvHeader is found successfully.
    NULL              -   The FvHeader is not found.

--*/
{
  UINT8                       *FileHeader;
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  UINT16                      FileChecksum;

  FileHeader = FileBuffer;
  for (; (UINTN)FileBuffer < (UINTN)FileHeader + FileLength; FileBuffer += 8) {
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FileBuffer;
    if (FvHeader->Signature == EFI_FVH_SIGNATURE) {
      //
      // potential candidate
      //

      //
      // Check checksum
      //
      if (FvHeader->FvLength > FileLength) {
        continue;
      }
      if (FvHeader->HeaderLength >= FileLength) {
        continue;
      }
      FileChecksum = CalculateChecksum16 ((UINT16 *)FileBuffer, FvHeader->HeaderLength / sizeof (UINT16));
      if (FileChecksum != 0) {
        continue;
      }

      //
      // Check revision and reserved field
      //
#if (PI_SPECIFICATION_VERSION < 0x00010000)
      if ((FvHeader->Revision == EFI_FVH_REVISION) &&
          (FvHeader->Reserved[0] == 0) &&
          (FvHeader->Reserved[1] == 0) &&
          (FvHeader->Reserved[2] == 0) ){
        return FileBuffer;
      }
#else
      if ((FvHeader->Revision == EFI_FVH_PI_REVISION) &&
          (FvHeader->Reserved[0] == 0) ){
        return FileBuffer;
      }
#endif
    }
  }

  return NULL;
}

UINT8  *
FindFileFromFvByGuid (
  IN UINT8     *FvBuffer,
  IN UINT32    FvSize,
  IN EFI_GUID  *Guid,
  OUT UINT32   *FileSize
  )
/*++

Routine Description:

  Find File with GUID in an FV

Arguments:

  FvBuffer       - FV binary buffer
  FvSize         - FV size
  Guid           - File GUID value to be searched
  FileSize       - Guid File size

Returns:

  FileLocation   - Guid File location.
  NULL           - Guid File is not found.

--*/
{
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  EFI_FFS_FILE_HEADER         *FileHeader;
  UINT64                      FvLength;
  EFI_GUID                    *TempGuid;
  UINT8                       *FixPoint;
  UINTN                       Offset;
  UINTN                       FileLength;
  UINTN                       FileOccupiedSize;

  //
  // Find the FFS file
  //
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FindNextFvHeader (FvBuffer, FvSize);
  if (NULL == FvHeader) {
    return NULL;
  }
  while (TRUE) {
    FvLength         = FvHeader->FvLength;

    //
    // Prepare to walk the FV image
    //
    InitializeFvLib (FvHeader, (UINT32)FvLength);

    FileHeader       = (EFI_FFS_FILE_HEADER *)((UINTN)FvHeader + FvHeader->HeaderLength);
    Offset           = (UINTN) FileHeader - (UINTN) FvHeader;

    while (Offset < FvLength) {
      TempGuid = (EFI_GUID *)&(FileHeader->Name);
      FileLength = (*(UINT32 *)(FileHeader->Size)) & 0x00FFFFFF;
      FileOccupiedSize = GETOCCUPIEDSIZE(FileLength, 8);
      if ((CompareGuid (TempGuid, Guid)) == 0) {
        //
        // Good! Find it.
        //
        FixPoint = ((UINT8 *)FileHeader + sizeof(EFI_FFS_FILE_HEADER));
        //
        // Find the position of file module, the offset
        // between the position and the end of FvRecovery.fv file
        // should not exceed 128kB to prevent reset vector from
        // outside legacy E and F segment
        //
        if ((UINTN)FvHeader + FvLength - (UINTN)FixPoint > 0x20000) {
  //        printf ("WARNING: The position of file module is not in E and F segment!\n");
  //        return NULL;
        }
        *FileSize = FileLength - sizeof(EFI_FFS_FILE_HEADER);
  #if (PI_SPECIFICATION_VERSION < 0x00010000)
        if (FileHeader->Attributes & FFS_ATTRIB_TAIL_PRESENT) {
          *FileSize -= sizeof(EFI_FFS_FILE_TAIL);
        }
  #endif
        return FixPoint;
      }
      FileHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FileHeader + FileOccupiedSize);
      Offset = (UINTN) FileHeader - (UINTN) FvHeader;
    }

    //
    // Not found, check next FV
    //
    if ((UINTN)FvBuffer + FvSize > (UINTN)FvHeader + FvLength) {
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FindNextFvHeader ((UINT8 *)FvHeader + (UINTN)FvLength, (UINTN)FvBuffer + FvSize - ((UINTN)FvHeader + (UINTN)FvLength));
      if (FvHeader == NULL) {
        break;
      }
    } else {
      break;
    }
  }

  //
  // Not found
  //
  return NULL;
}

BOOLEAN
IsGuidData (
  IN CHAR8     *StringData,
  OUT EFI_GUID *Guid
  )
/*++

Routine Description:

  Check whether a string is a GUID

Arguments:

  StringData  - the String
  Guid        - Guid to hold the value

Returns:

  TRUE  - StringData is a GUID, and Guid field is filled.
  FALSE - StringData is not a GUID

--*/
{
  if (strlen (StringData) != strlen ("00000000-0000-0000-0000-000000000000")) {
    return FALSE;
  }
  if ((StringData[8] != '-') ||
      (StringData[13] != '-') ||
      (StringData[18] != '-') ||
      (StringData[23] != '-') ) {
    return FALSE;
  }

  StringToGuid (StringData, Guid);

  return TRUE;
}

VOID
CheckOverlap (
  IN UINT32 Address,
  IN UINT32 Size
  )
{
  INTN  Index;

  for (Index = 0; Index < (INTN)gFitTableContext.BiosModuleNumber; Index ++) {
    if ((gFitTableContext.BiosModule[Index].Address <= Address) &&
        ((gFitTableContext.BiosModule[Index].Size - Size) >= (Address - gFitTableContext.BiosModule[Index].Address))) {
      UINT32  TempSize;
      INT32   SubIndex;

      //
      // Found overlap, split BiosModuleEntry
      // Currently only support StartupAcm in 1 BiosModule. It does not support StartupAcm across 2 BiosModule or more.
      //
      if (gFitTableContext.BiosModuleNumber >= MAX_BIOS_MODULE_ENTRY) {
        Error (NULL, 0, 0, "Too many Bios Module!", NULL);
        return ;
      }

      if (Address != gFitTableContext.BiosModule[Index].Address) {
        //
        // Skip the entry whose start address is same as StartupAcm
        //
        gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Type    = FIT_TABLE_TYPE_BIOS_MODULE;
        gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Address = gFitTableContext.BiosModule[Index].Address;
        gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Size    = Address - gFitTableContext.BiosModule[Index].Address;
        gFitTableContext.BiosModuleNumber ++;
        gFitTableContext.FitEntryNumber++;
      }

      TempSize = gFitTableContext.BiosModule[Index].Address + gFitTableContext.BiosModule[Index].Size;
      gFitTableContext.BiosModule[Index].Address = Address + Size;
      gFitTableContext.BiosModule[Index].Size    = TempSize - gFitTableContext.BiosModule[Index].Address;

      if (gFitTableContext.BiosModule[Index].Size == 0) {
        //
        // remove the entry if size is 0
        //
        for (SubIndex = Index; SubIndex < (INTN)gFitTableContext.BiosModuleNumber - 1; SubIndex ++) {
          gFitTableContext.BiosModule[SubIndex].Address = gFitTableContext.BiosModule[SubIndex + 1].Address;
          gFitTableContext.BiosModule[SubIndex].Size    = gFitTableContext.BiosModule[SubIndex + 1].Size;
        }
        gFitTableContext.BiosModuleNumber --;
        gFitTableContext.FitEntryNumber--;
      }
      break;
    }
  }
}

UINT8 *
GetMicrocodeBufferFromFv (
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader
  )
{
  UINT8 *MicrocodeBuffer;
  EFI_FFS_FILE_HEADER *FfsHeader;

  MicrocodeBuffer = NULL;
  //
  // Skip FV header + FV extension header + FFS header
  //
  FfsHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *) FvHeader + FvHeader->HeaderLength);
  while ((UINT8 *) FfsHeader < (UINT8 *) FvHeader + FvHeader->FvLength) {
    if (FfsHeader->Type == EFI_FV_FILETYPE_RAW) {
      //
      // Find the first RAW ffs file as Microcode Buffer
      //
      MicrocodeBuffer = (UINT8 *)(FfsHeader + 1);
      break;
    }
    if (GetFfsFileLength (FfsHeader) == 0xFFFFFF) {
      // spare space is found, and exit
      break;
    }
    FfsHeader = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FfsHeader + ((GetFfsFileLength (FfsHeader)+7)&~7));
  }

  return MicrocodeBuffer;
}

UINT32
GetFitEntryNumber (
  IN INTN   argc,
  IN CHAR8  **argv,
  IN UINT8  *FdBuffer,
  IN UINT32 FdSize
  )
/*++

Routine Description:

  Get FIT entry number and fill global FIT table context, from argument

Arguments:

  argc           - Number of command line parameters.
  argv           - Array of pointers to parameter strings.
  FdBuffer       - FD binary buffer
  FdSize         - FD size

Returns:

  FitEntryNumber - The FIT entry number
  0              - Argument parse fail

*/
{
  EFI_GUID  Guid;
  EFI_GUID  MicrocodeFfsGuid;
  INTN      Index;
  UINT8     *FileBuffer;
  UINT32    FileSize;
  UINT32    Type;
  UINT32    SubType;
  UINT8     *MicrocodeFileBuffer;
  UINT8     *MicrocodeFileBufferRaw;
  UINT32    MicrocodeFileSize;
  UINT32    MicrocodeBase;
  UINT32    MicrocodeSize;
  UINT8     *MicrocodeBuffer;
  UINT32    MicrocodeBufferSize;
  UINT8     *Walker;
  UINT32    MicrocodeRegionOffset;
  UINT32    MicrocodeRegionSize;
  UINT32    SlotSize;
  STATUS    Status;
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  UINTN                       FitEntryNumber;
  BOOLEAN                     BiosInfoExist;
  BOOLEAN                     SlotMode;
  BOOLEAN                     SlotModeForce;
  BIOS_INFO_HEADER            *BiosInfo;
  BIOS_INFO_STRUCT            *BiosInfoStruct;
  UINTN                       BiosInfoIndex;

  SlotMode      = FALSE;
  SlotModeForce = FALSE;

  //
  // Init index
  //
  Index = 3;
  if (((strcmp (argv[1], "-D") == 0) ||
       (strcmp (argv[1], "-d") == 0)) ) {
    Index ++;
  }

  //
  // Fill Global Version
  //
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-V") != 0) &&
       (strcmp (argv[Index], "-v") != 0)) ) {
    gFitTableContext.GlobalVersion = DEFAULT_FIT_ENTRY_VERSION;
  } else {
    gFitTableContext.GlobalVersion = xtoi (argv[Index + 1]);
    Index += 2;
  }

  //
  // 0. FIT Header
  //
  gFitTableContext.FitEntryNumber = 1;
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-F") != 0) &&
       (strcmp (argv[Index], "-f") != 0)) ) {
    //
    // Use default address
    //
    gFitTableContext.FitTablePointerOffset = DEFAULT_FIT_TABLE_POINTER_OFFSET;
  } else {
    //
    // Get offset from parameter
    //
    gFitTableContext.FitTablePointerOffset = xtoi (argv[Index + 1]);
    Index += 2;
  }

  //
  // 0.1 FIT Header 2
  //
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-F") != 0) &&
       (strcmp (argv[Index], "-f") != 0)) ) {
    //
    // Bypass
    //
    gFitTableContext.FitTablePointerOffset2 = 0;
  } else {
    //
    // Get offset from parameter
    //
    gFitTableContext.FitTablePointerOffset2 = xtoi (argv[Index + 1]);
    Index += 2;
  }

  //
  // 0.2 FIT Header version
  //
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-V") != 0) &&
       (strcmp (argv[Index], "-v") != 0)) ) {
    //
    // Bypass
    //
    gFitTableContext.FitHeaderVersion = gFitTableContext.GlobalVersion;
  } else {
    //
    // Get offset from parameter
    //
    gFitTableContext.FitHeaderVersion = xtoi (argv[Index + 1]);
    Index += 2;
  }

  //
  // 0.3 Microcode alignment
  //
  if ((Index >= argc) ||
      ((strcmp (argv[Index], "-NA") != 0) &&
       (strcmp (argv[Index], "-na") != 0) &&
       (strcmp (argv[Index], "-A") != 0) &&
       (strcmp (argv[Index], "-a") != 0))) {
    //
    // by pass
    //
    gFitTableContext.MicrocodeIsAligned = TRUE;
    gFitTableContext.MicrocodeAlignValue = 0x800;
  } else if ((strcmp (argv[Index], "-NA") == 0) || (strcmp (argv[Index], "-na") == 0)) {
    gFitTableContext.MicrocodeIsAligned = FALSE;
    gFitTableContext.MicrocodeAlignValue = 1;
    Index += 1;
  } else if ((strcmp (argv[Index], "-A") == 0) || (strcmp (argv[Index], "-a") == 0)) {
    gFitTableContext.MicrocodeIsAligned = TRUE;
    //
    // Get alignment from parameter
    //
    gFitTableContext.MicrocodeAlignValue = xtoi (argv[Index + 1]);;
    Index += 2;
  }

  if ((Index >= argc) ||
      ((strcmp (argv[Index], "-REMAP") == 0) ||
       (strcmp (argv[Index], "-remap") == 0)) ) {
    //
    // by pass
    //
    gFitTableContext.TopFlashAddressRemapValue = xtoi (argv[Index + 1]);
    Index += 2;
  } else {
    //
    // no remapping
    //
    gFitTableContext.TopFlashAddressRemapValue = 0x100000000;
  }
  printf ("Top Flash Address Value : 0x%llx\n", (unsigned long long) gFitTableContext.TopFlashAddressRemapValue);
  //
  // 0.4 Clear FIT table related memory
  //
  if ((Index >= argc) ||
      ((strcmp (argv[Index], "-CLEAR") != 0) &&
       (strcmp (argv[Index], "-clear") != 0)) ) {
    //
    // by pass
    //
    gFitTableContext.Clear = FALSE;
  } else {
    //
    // Clear FIT table
    //
    gFitTableContext.Clear = TRUE;
    //
    // Do not parse any more
    //
    return 0;
  }

  //
  // 0.5 SlotSize
  //
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-L") != 0) &&
       (strcmp (argv[Index], "-l") != 0) &&
       (strcmp (argv[Index], "-LF") != 0) &&
       (strcmp (argv[Index], "-lf") != 0))) {
    //
    // Bypass
    //
    SlotSize = 0;
  } else {
    SlotSize = xtoi (argv[Index + 1]);

    if (SlotSize == 0 || SlotSize & 0xF) {
      printf ("Invalid slotsize = 0x%x, slot size should not be zero, or start at a non-16-byte aligned linear address!\n", SlotSize);
      return 0;
    }
    if (strcmp (argv[Index], "-LF") == 0 || strcmp (argv[Index], "-lf") == 0) {
      SlotModeForce = TRUE;
      Index += 2;
    } else {
      SlotMode = IsGuidData(argv[Index + 2], &MicrocodeFfsGuid);
      if (!SlotMode) {
        printf ("Need a ffs GUID for search uCode ffs\n");
        return 0;
      }
      Index += 3;
    }
  }

  //
  // 0.6 BiosInfo
  //
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-I") != 0) &&
       (strcmp (argv[Index], "-i") != 0)) ) {
    //
    // Bypass
    //
    BiosInfoExist = FALSE;
  } else {
    //
    // Get offset from parameter
    //
    BiosInfoExist = TRUE;
    if (IsGuidData (argv[Index + 1], &Guid)) {
      FileBuffer = FindFileFromFvByGuid (FdBuffer, FdSize, &Guid, &FileSize);
      if (FileBuffer == NULL) {
        Error (NULL, 0, 0, "-I Parameter incorrect, GUID not found!", "%s", argv[Index + 1]);
        // not found
        return 0;
      }
      BiosInfo = (BIOS_INFO_HEADER *)FileBuffer;
      for (BiosInfoIndex = 0; BiosInfoIndex < FileSize; BiosInfoIndex++) {
        if (((BIOS_INFO_HEADER *)(FileBuffer + BiosInfoIndex))->Signature == BIOS_INFO_SIGNATURE) {
          BiosInfo = (BIOS_INFO_HEADER *)(FileBuffer + BiosInfoIndex);
        }
      }
      if (BiosInfo->Signature != BIOS_INFO_SIGNATURE) {
        Error (NULL, 0, 0, "-I Parameter incorrect, Signature Error!", NULL);
        // not found
        return 0;
      }
      BiosInfoStruct = (BIOS_INFO_STRUCT *)(BiosInfo + 1);

      for (BiosInfoIndex = 0; BiosInfoIndex < BiosInfo->EntryCount; BiosInfoIndex++) {
        if ((BiosInfoStruct[BiosInfoIndex].Attributes & BIOS_INFO_STRUCT_ATTRIBUTE_GENERAL_EXCLUDE_FROM_FIT) != 0) {
          continue;
        }
        switch (BiosInfoStruct[BiosInfoIndex].Type) {
        case FIT_TABLE_TYPE_HEADER:
          Error (NULL, 0, 0, "-I Parameter incorrect, Header Type unsupported!", NULL);
          return 0;
        case FIT_TABLE_TYPE_STARTUP_ACM:
          if (gFitTableContext.StartupAcm.Type != 0) {
            Error (NULL, 0, 0, "-I Parameter incorrect, Duplicated StartupAcm!", NULL);
            return 0;
          }
          gFitTableContext.StartupAcm.Type    = FIT_TABLE_TYPE_STARTUP_ACM;
          gFitTableContext.StartupAcm.Address = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
          gFitTableContext.StartupAcm.Size    = (UINT32)BiosInfoStruct[BiosInfoIndex].Size;
          gFitTableContext.StartupAcmVersion  = BiosInfoStruct[BiosInfoIndex].Version;
          gFitTableContext.FitEntryNumber ++;
          break;
        case FIT_TABLE_TYPE_DIAGNST_ACM:
          if (gFitTableContext.DiagnstAcm.Type != 0) {
            Error (NULL, 0, 0, "-U Parameter incorrect, Duplicated DiagnosticsAcm!", NULL);
            return 0;
          }
          gFitTableContext.DiagnstAcm.Type    = FIT_TABLE_TYPE_DIAGNST_ACM;
          gFitTableContext.DiagnstAcm.Address = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
          gFitTableContext.DiagnstAcm.Size    = 0;
          gFitTableContext.DiagnstAcmVersion  = DEFAULT_FIT_ENTRY_VERSION;
          gFitTableContext.FitEntryNumber ++;
          break;
        case FIT_TABLE_TYPE_BIOS_MODULE:
          if ((BiosInfoStruct[BiosInfoIndex].Attributes & BIOS_INFO_STRUCT_ATTRIBUTE_BIOS_POST_IBB) != 0) {
            continue;
          }
          if (gFitTableContext.BiosModuleNumber >= MAX_BIOS_MODULE_ENTRY) {
            Error (NULL, 0, 0, "-I Parameter incorrect, Too many Bios Module!", NULL);
            return 0;
          }
          gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Type    = FIT_TABLE_TYPE_BIOS_MODULE;
          gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Address = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
          gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Size    = (UINT32)BiosInfoStruct[BiosInfoIndex].Size;
          gFitTableContext.BiosModuleVersion = BiosInfoStruct[BiosInfoIndex].Version;
          gFitTableContext.BiosModuleNumber ++;
          gFitTableContext.FitEntryNumber ++;
          break;
        case FIT_TABLE_TYPE_MICROCODE:
          if ((BiosInfoStruct[BiosInfoIndex].Attributes & BIOS_INFO_STRUCT_ATTRIBUTE_MICROCODE_WHOLE_REGION) == 0) {
            if (gFitTableContext.MicrocodeNumber >= MAX_MICROCODE_ENTRY) {
              Error (NULL, 0, 0, "-I Parameter incorrect, Too many Microcode!", NULL);
              return 0;
            }
            gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Type    = FIT_TABLE_TYPE_MICROCODE;
            gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Address = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
            gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Size    = (UINT32)BiosInfoStruct[BiosInfoIndex].Size;
            gFitTableContext.MicrocodeVersion = BiosInfoStruct[BiosInfoIndex].Version;
            gFitTableContext.MicrocodeNumber++;
            gFitTableContext.FitEntryNumber++;
          } else {
            MicrocodeRegionOffset = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
            MicrocodeRegionSize   = (UINT32)BiosInfoStruct[BiosInfoIndex].Size;
            if (MicrocodeRegionOffset == 0) {
              Error (NULL, 0, 0, "-I Parameter incorrect, MicrocodeRegionOffset is 0", NULL);
              return 0;
            }
            if (MicrocodeRegionSize == 0) {
              Error (NULL, 0, 0, "-I Parameter incorrect, MicrocodeRegionSize is 0", NULL);
              return 0;
            }
            if (MicrocodeRegionSize > FdSize) {
              Error (NULL, 0, 0, "-I Parameter incorrect, MicrocodeRegionSize too large", NULL);
              return 0;
            }

            MicrocodeFileBuffer = FLASH_TO_MEMORY (MicrocodeRegionOffset, FdBuffer, FdSize);
            MicrocodeFileSize = MicrocodeRegionSize;
            MicrocodeBase = MicrocodeRegionOffset;

            FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)MicrocodeFileBuffer;
            if (FvHeader->Signature == EFI_FVH_SIGNATURE) {
              MicrocodeBuffer = GetMicrocodeBufferFromFv (FvHeader);
            } else {
              MicrocodeBuffer = MicrocodeFileBuffer;
            }

            if (SlotMode) {
              MicrocodeBuffer = FindFileFromFvByGuid(MicrocodeFileBuffer, MicrocodeFileSize, &MicrocodeFfsGuid, &MicrocodeBufferSize);
              if (MicrocodeBuffer == NULL) {
                printf ("-L Parameter incorrect, GUID not found\n");
                // not found
                return 0;
              }
            }

            while ((UINT32)(MicrocodeBuffer - MicrocodeFileBuffer) < MicrocodeFileSize) {
              if (*(UINT32 *)(MicrocodeBuffer) != 0x1) { // HeaderVersion
                break;
              }
              if (*(UINT32 *)(MicrocodeBuffer + 20) != 0x1) { // LoaderVersion
                break;
              }
              if (*(UINT32 *)(MicrocodeBuffer + 28) == 0) { // DataSize
                MicrocodeSize = 2048;
              } else {
                //
                // MCU might be put at 2KB alignment, if so, we need to adjust the size as 2KB alignment.
                //
                if (gFitTableContext.MicrocodeIsAligned) {
                  if (gFitTableContext.MicrocodeAlignValue & 0xF) {
                    printf ("-A Parameter incorrect, Microcode data must start at a 16-byte aligned linear address!\n");
                    return 0;
                  }
                  MicrocodeSize = ROUNDUP (*(UINT32 *)(MicrocodeBuffer + 32), gFitTableContext.MicrocodeAlignValue);
                } else {
                  MicrocodeSize = (*(UINT32 *)(MicrocodeBuffer + 32));
                }
              }

              //
              // Add Microcode
              //
              if (gFitTableContext.MicrocodeNumber >= MAX_MICROCODE_ENTRY) {
                printf ("-I Parameter incorrect, Too many Microcode!\n");
                return 0;
              }
              gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Type = FIT_TABLE_TYPE_MICROCODE;
              gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Address = MicrocodeBase + (UINT32)((UINTN) MicrocodeBuffer - (UINTN) MicrocodeFileBuffer);
              gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Size = MicrocodeSize;
              gFitTableContext.MicrocodeNumber++;
              gFitTableContext.FitEntryNumber++;

              if (SlotSize != 0) {
                if (SlotSize < MicrocodeSize) {
                  printf ("Parameter incorrect, Slot size: %x is too small for Microcode size: %x!\n", SlotSize, MicrocodeSize);
                  return 0;
                }
                MicrocodeBuffer += SlotSize;
              } else {
                MicrocodeBuffer += MicrocodeSize;
              }
            }

            ///
            /// Check the remaining buffer
            ///
            if (((UINT32)(MicrocodeBuffer - MicrocodeFileBuffer) < MicrocodeFileSize) && (SlotMode || SlotModeForce)) {
              for (Walker = MicrocodeBuffer; Walker < MicrocodeFileBuffer + MicrocodeFileSize; Walker++) {
                if (*Walker != 0xFF) {
                  printf ("Error: detect non-spare space after uCode array, please check uCode array!\n");
                  return 0;
                }
              }

              ///
              /// Split the spare space as empty buffer for save uCode patch.
              ///
              while (MicrocodeBuffer + SlotSize <= MicrocodeFileBuffer + MicrocodeFileSize) {
                gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Type = FIT_TABLE_TYPE_MICROCODE;
                gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Address = MicrocodeBase + (UINT32)((UINTN) MicrocodeBuffer - (UINTN) MicrocodeFileBuffer);
                gFitTableContext.MicrocodeNumber++;
                gFitTableContext.FitEntryNumber++;

                MicrocodeBuffer += SlotSize;
              }
            }
          }
          break;
        case FIT_TABLE_TYPE_TPM_POLICY:
        case FIT_TABLE_TYPE_BIOS_POLICY:
        case FIT_TABLE_TYPE_TXT_POLICY:
        case FIT_TABLE_TYPE_KEY_MANIFEST:
        case FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST:
        case FIT_TABLE_TYPE_BIOS_DATA_AREA:
        case FIT_TABLE_TYPE_CSE_SECURE_BOOT:
        default :
          if (BiosInfoStruct[BiosInfoIndex].Version != 0) {
            if (gFitTableContext.OptionalModuleNumber >= MAX_OPTIONAL_ENTRY) {
              Error (NULL, 0, 0, "-I Parameter incorrect, Too many Optional Module!", NULL);
              return 0;
            }
            gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Type    = BiosInfoStruct[BiosInfoIndex].Type;
            gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Address = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
            gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Size    = (UINT32)BiosInfoStruct[BiosInfoIndex].Size;
            gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Version = BiosInfoStruct[BiosInfoIndex].Version;
            gFitTableContext.OptionalModuleNumber++;
            gFitTableContext.FitEntryNumber++;
          } else {
            if (gFitTableContext.PortModuleNumber >= MAX_PORT_ENTRY) {
              Error (NULL, 0, 0, "-I Parameter incorrect, Too many Port Module!", NULL);
              return 0;
            }
            gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Type    = BiosInfoStruct[BiosInfoIndex].Type;
            gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Address = (UINT32)BiosInfoStruct[BiosInfoIndex].Address;
            gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Size    = (UINT32)(BiosInfoStruct[BiosInfoIndex].Address >> 32);
            gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Version = BiosInfoStruct[BiosInfoIndex].Version;
            gFitTableContext.PortModuleNumber++;
            gFitTableContext.FitEntryNumber++;
          }
          break;
        }
      }

    } else {
      Error (NULL, 0, 0, "-I Parameter incorrect, expect GUID!", NULL);
      return 0;
    }
    Index += 2;
  }

  //
  // 1. StartupAcm
  //
  do {
    if ((Index + 1 >= argc) ||
        ((strcmp (argv[Index], "-S") != 0) &&
         (strcmp (argv[Index], "-s") != 0)) ) {
      if (BiosInfoExist && (gFitTableContext.StartupAcm.Type == FIT_TABLE_TYPE_STARTUP_ACM)) {
        break;
      }
//      Error (NULL, 0, 0, "-S Parameter incorrect, expect -S!", NULL);
//      return 0;
      printf ("-S not found. WARNING!\n");
      break;
    }
    if (IsGuidData (argv[Index + 1], &Guid)) {
      FileBuffer = FindFileFromFvByGuid (FdBuffer, FdSize, &Guid, &FileSize);
      if (FileBuffer == NULL) {
        Error (NULL, 0, 0, "-S Parameter incorrect, GUID not found!", "%s", argv[Index + 1]);
        // not found
        return 0;
      }
      FileBuffer = (UINT8 *)MEMORY_TO_FLASH (FileBuffer, FdBuffer, FdSize);
      Index += 2;
    } else {
      if (Index + 2 >= argc) {
        Error (NULL, 0, 0, "-S Parameter incorrect, expect Address Size!", NULL);
        return 0;
      }
      FileBuffer = (UINT8 *) (UINTN) xtoi (argv[Index + 1]);
      FileSize = xtoi (argv[Index + 2]);
      Index += 3;
    }
    if (gFitTableContext.StartupAcm.Type != 0) {
      Error (NULL, 0, 0, "-S Parameter incorrect, Duplicated StartupAcm!", NULL);
      return 0;
    }
    gFitTableContext.StartupAcm.Type = FIT_TABLE_TYPE_STARTUP_ACM;
    gFitTableContext.StartupAcm.Address = (UINT32) (UINTN) FileBuffer;
    gFitTableContext.StartupAcm.Size = FileSize;
    gFitTableContext.FitEntryNumber ++;

    //
    // 1.1 StartupAcm version
    //
    if ((Index + 1 >= argc) ||
        ((strcmp (argv[Index], "-V") != 0) &&
         (strcmp (argv[Index], "-v") != 0)) ) {
      //
      // Bypass
      //
      gFitTableContext.StartupAcmVersion = gFitTableContext.GlobalVersion;
    } else {
      //
      // Get offset from parameter
      //
      gFitTableContext.StartupAcmVersion = xtoi (argv[Index + 1]);
      Index += 2;
    }
  } while (FALSE);

  //
  // 1.5. DiagnosticsAcm
  //
  do {
    if ((Index + 1 >= argc) ||
        ((strcmp (argv[Index], "-U") != 0) &&
         (strcmp (argv[Index], "-u") != 0)) ) {
      if (BiosInfoExist && (gFitTableContext.DiagnstAcm.Type == FIT_TABLE_TYPE_DIAGNST_ACM)) {
        break;
      }
      break;
    }
    if (IsGuidData (argv[Index + 1], &Guid)) {
      FileBuffer = FindFileFromFvByGuid (FdBuffer, FdSize, &Guid, &FileSize);
      if (FileBuffer == NULL) {
        Error (NULL, 0, 0, "-U Parameter incorrect, GUID not found!", "%s", argv[Index + 1]);
        return 0;
      }
      FileBuffer = (UINT8 *)MEMORY_TO_FLASH (FileBuffer, FdBuffer, FdSize);
      Index += 2;
    } else {
      FileBuffer = (UINT8 *) (UINTN) xtoi (argv[Index + 1]);
      Index += 2;
    }
    if (gFitTableContext.DiagnstAcm.Type != 0) {
      Error (NULL, 0, 0, "-U Parameter incorrect, Duplicated DiagnosticsAcm!", NULL);
      return 0;
    }
    gFitTableContext.DiagnstAcm.Type = FIT_TABLE_TYPE_DIAGNST_ACM;
    gFitTableContext.DiagnstAcm.Address = (UINT32) (UINTN) FileBuffer;
    gFitTableContext.DiagnstAcm.Size = 0;
    gFitTableContext.FitEntryNumber ++;
    gFitTableContext.DiagnstAcmVersion = DEFAULT_FIT_ENTRY_VERSION;
  } while (FALSE);

  // 2. BiosModule
  //
  do {
    if ((Index + 2 >= argc) ||
        ((strcmp (argv[Index], "-B") != 0) &&
         (strcmp (argv[Index], "-b") != 0)) ) {
      if (BiosInfoExist && (gFitTableContext.BiosModuleNumber != 0)) {
        break;
      }
//      Error (NULL, 0, 0, "-B Parameter incorrect, expect -B!", NULL);
//      return 0;
      printf ("-B not found. WARNING!\n");
      break;
    }

    FileBuffer = (UINT8 *) (UINTN) xtoi (argv[Index + 1]);
    FileSize = xtoi (argv[Index + 2]);
    gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Type = FIT_TABLE_TYPE_BIOS_MODULE;
    gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Address = (UINT32) (UINTN) FileBuffer;
    gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Size = FileSize;
    gFitTableContext.BiosModuleNumber ++;
    gFitTableContext.FitEntryNumber ++;

    while (TRUE) {
      Index += 3;
      if (Index + 2 >= argc) {
        break;
      }
      if ((strcmp (argv[Index], "-B") != 0) &&
          (strcmp (argv[Index], "-b") != 0) ) {
        break;
      }
      if (gFitTableContext.BiosModuleNumber >= MAX_BIOS_MODULE_ENTRY) {
        Error (NULL, 0, 0, "-B Parameter incorrect, Too many Bios Module!", NULL);
        return 0;
      }
      FileBuffer = (UINT8 *) (UINTN) xtoi (argv[Index + 1]);
      FileSize = xtoi (argv[Index + 2]);
      gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Type = FIT_TABLE_TYPE_BIOS_MODULE;
      gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Address = (UINT32) (UINTN) FileBuffer;
      gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Size = FileSize;
      gFitTableContext.BiosModuleNumber ++;
      gFitTableContext.FitEntryNumber++;
    }

    //
    // 2.1 BiosModule version
    //
    if ((Index + 1 >= argc) ||
        ((strcmp (argv[Index], "-V") != 0) &&
         (strcmp (argv[Index], "-v") != 0)) ) {
      //
      // Bypass
      //
      gFitTableContext.BiosModuleVersion = gFitTableContext.GlobalVersion;
    } else {
      //
      // Get offset from parameter
      //
      gFitTableContext.BiosModuleVersion = xtoi (argv[Index + 1]);
      Index += 2;
    }
  } while (FALSE);

  //
  // 3. Microcode
  //
  while (TRUE) {
    if (Index + 1 >= argc) {
      break;
    }
    if ((strcmp (argv[Index], "-M") != 0) &&
        (strcmp (argv[Index], "-m") != 0) ) {
      break;
    }
    if (IsGuidData (argv[Index + 2], &Guid)) {
      Error (NULL, 0, 0, "-M Parameter incorrect, GUID unsupported!", NULL);
      return 0;
    } else {
      if (Index + 2 >= argc) {
        break;
      }
      FileBuffer = (UINT8 *) (UINTN) xtoi (argv[Index + 1]);
      FileSize = xtoi (argv[Index + 2]);
      Index += 3;
    }
    if (gFitTableContext.MicrocodeNumber >= MAX_MICROCODE_ENTRY) {
      Error (NULL, 0, 0, "-M Parameter incorrect, Too many Microcode!", NULL);
      return 0;
    }
    gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Type = FIT_TABLE_TYPE_MICROCODE;
    gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Address = (UINT32) (UINTN) FileBuffer;
    gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Size = FileSize;
    gFitTableContext.MicrocodeNumber++;
    gFitTableContext.FitEntryNumber++;
  }

  //
  // 3.1 MicrocodeFv
  //
  while (TRUE) {
    if (Index + 1 >= argc) {
      break;
    }
    if ((strcmp (argv[Index], "-U") != 0) &&
        (strcmp (argv[Index], "-u") != 0) ) {
      break;
    }
    //
    // Get Fv
    //
    if (IsGuidData (argv[Index + 1], &Guid)) {
      MicrocodeFileBuffer = FindFileFromFvByGuid (FdBuffer, FdSize, &Guid, &MicrocodeFileSize);
      if (MicrocodeFileBuffer == NULL) {
        Error (NULL, 0, 0, "-U Parameter incorrect, GUID not found!", "%s", argv[Index + 1]);
        // not found
        return 0;
      }
      Index += 2;

      MicrocodeBuffer = MicrocodeFileBuffer;
      MicrocodeFileBufferRaw = NULL;
      MicrocodeRegionOffset = MEMORY_TO_FLASH (MicrocodeFileBuffer, FdBuffer, FdSize);
      MicrocodeRegionSize   = 0;
      MicrocodeBase = MicrocodeRegionOffset;

    } else {
      if (Index + 2 >= argc) {
        break;
      }
      Status = ReadInputFile (argv[Index + 1], &MicrocodeFileBuffer, &MicrocodeFileSize, &MicrocodeFileBufferRaw);
      if (Status != STATUS_SUCCESS) {
        MicrocodeRegionOffset = xtoi (argv[Index + 1]);
        MicrocodeRegionSize   = xtoi (argv[Index + 2]);

        if (MicrocodeRegionOffset == 0) {
          Error (NULL, 0, 0, "-U Parameter incorrect, MicrocodeRegionOffset is 0, or unable to open file", "%s", argv[Index + 1]);
          return 0;
        }
        if (MicrocodeRegionSize == 0) {
          Error (NULL, 0, 0, "-U Parameter incorrect, MicrocodeRegionSize is 0", NULL);
          return 0;
        }
        if (MicrocodeRegionSize > FdSize) {
          Error (NULL, 0, 0, "-U Parameter incorrect, MicrocodeRegionSize too large", NULL);
          return 0;
        }

        Index += 3;

        MicrocodeFileBufferRaw = NULL;
        MicrocodeFileBuffer = FLASH_TO_MEMORY (MicrocodeRegionOffset, FdBuffer, FdSize);
        MicrocodeFileSize = MicrocodeRegionSize;
        MicrocodeBase = MicrocodeRegionOffset;

        FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)MicrocodeFileBuffer;
        if (FvHeader->Signature == EFI_FVH_SIGNATURE) {
          MicrocodeBuffer = GetMicrocodeBufferFromFv (FvHeader);
        } else {
          MicrocodeBuffer = MicrocodeFileBuffer;
        }
      } else {
        MicrocodeBase = xtoi (argv[Index + 2]);
        Index += 3;
        MicrocodeRegionOffset = 0;
        MicrocodeRegionSize   = 0;

        FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)MicrocodeFileBuffer;
        if (FvHeader->Signature == EFI_FVH_SIGNATURE) {
          MicrocodeBuffer = GetMicrocodeBufferFromFv (FvHeader);
        } else {
          MicrocodeBuffer = MicrocodeFileBuffer;
        }
      }
    }
    while ((UINT32)(MicrocodeBuffer - MicrocodeFileBuffer) < MicrocodeFileSize) {
      if (*(UINT32 *)(MicrocodeBuffer) != 0x1) { // HeaderVersion
        break;
      }
      if (*(UINT32 *)(MicrocodeBuffer + 20) != 0x1) { // LoaderVersion
        break;
      }
      if (*(UINT32 *)(MicrocodeBuffer + 28) == 0) { // DataSize
        MicrocodeSize = 2048;
      } else {
        //
        // MCU might be put at 2KB alignment, if so, we need to adjust the size as 2KB alignment.
        //
        if (gFitTableContext.MicrocodeIsAligned) {
          MicrocodeSize = (*(UINT32 *)(MicrocodeBuffer + 32) + (gFitTableContext.MicrocodeAlignValue - 1)) & ~(gFitTableContext.MicrocodeAlignValue - 1);
        } else {
          MicrocodeSize = (*(UINT32 *)(MicrocodeBuffer + 32));
        }
      }

      //
      // Add Microcode
      //
      if (gFitTableContext.MicrocodeNumber >= MAX_MICROCODE_ENTRY) {
        printf ("-U Parameter incorrect, Too many Microcode!\n");
        return 0;
      }
      gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Type = FIT_TABLE_TYPE_MICROCODE;
      gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Address = MicrocodeBase + (UINT32)((UINTN) MicrocodeBuffer - (UINTN) MicrocodeFileBuffer);
      gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Size = MicrocodeSize;
      gFitTableContext.MicrocodeNumber++;
      gFitTableContext.FitEntryNumber++;

      MicrocodeBuffer += MicrocodeSize;
    }

    if (MicrocodeFileBufferRaw != NULL) {
      free ((VOID *)MicrocodeFileBufferRaw);
      MicrocodeFileBufferRaw = NULL;
    }
  }

  //
  // 3.3 Microcode version
  //
  if ((Index + 1 >= argc) ||
      ((strcmp (argv[Index], "-V") != 0) &&
       (strcmp (argv[Index], "-v") != 0)) ) {
    //
    // Bypass
    //
    gFitTableContext.MicrocodeVersion = gFitTableContext.GlobalVersion;
  } else {
    //
    // Get offset from parameter
    //
    gFitTableContext.MicrocodeVersion = xtoi (argv[Index + 1]);
    Index += 2;
  }

  //
  // 4. Optional type
  //
  while (TRUE) {
    if (Index + 2 >= argc) {
      break;
    }
    if ((strcmp (argv[Index], "-O") != 0) &&
        (strcmp (argv[Index], "-o") != 0) ) {
      break;
    }
    Type = xtoi (argv[Index + 1]);
    //
    // 1st, try CSE entry sub-type
    //
    SubType = 0;
    if (Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT) {
      if (Index + 3 >= argc) {
        break;
      }
      SubType = xtoi (argv[Index + 2]);
      //
      // try file
      //
      if (SubType != FIT_TABLE_SUBTYPE_FIT_PATCH_MANIFEST && SubType != FIT_TABLE_SUBTYPE_ACM_MANIFEST) {
        Error (NULL, 0, 0, "-O Parameter incorrect, SubType unsupported!", NULL);
        return 0;
      }
      Status = ReadInputFile (argv[Index + 3], &FileBuffer, &FileSize, NULL);
      if (Status == STATUS_SUCCESS) {
        if (FileSize >= 0x80000000) {
          Error (NULL, 0, 0, "-O Parameter incorrect, FileSize too large!", NULL);
          free (FileBuffer);
          return 0;
        }
        //
        // Set the most significant bit
        // It means the data in memory, not in flash yet.
        // Assume the file size should < 2G.
        //
        FileSize |= 0x80000000;
        Index += 4;
      } else {
        if (Status == STATUS_WARNING) {
          Error (NULL, 0, 0, "-O Parameter incorrect, Unable to open file", argv[Index + 3]);
        }
        return 0;
      }
    } else {
      //
      // 2nd, try GUID
      //
      if (IsGuidData (argv[Index + 2], &Guid)) {
        FileBuffer = FindFileFromFvByGuid (FdBuffer, FdSize, &Guid, &FileSize);
        if (FileBuffer == NULL) {
          Error (NULL, 0, 0, "-O Parameter incorrect, GUID not found!", "%s", argv[Index + 2]);
          // not found
          return 0;
        }
        if (FileSize >= 0x80000000) {
          Error (NULL, 0, 0, "-O Parameter incorrect, FileSize too large!", NULL);
          return 0;
        }
      FileBuffer = (UINT8 *)MEMORY_TO_FLASH (FileBuffer, FdBuffer, FdSize);
        Index += 3;
      } else {
        //
        // 3rd, try file
        //
        Status = ReadInputFile (argv[Index + 2], &FileBuffer, &FileSize, NULL);
        if (Status == STATUS_SUCCESS) {
          if (FileSize >= 0x80000000) {
            Error (NULL, 0, 0, "-O Parameter incorrect, FileSize too large!", NULL);
            free (FileBuffer);
            return 0;
          }
          //
          // Set the most significant bit
          // It means the data in memory, not in flash yet.
          // Assume the file size should < 2G.
          //
          FileSize |= 0x80000000;
          Index += 3;
        } else {
          //
          // 4th, try <RESERVE, Length>
          //
          if (Index + 3 >= argc) {
            break;
          }
          if ((strcmp (argv[Index + 2], "RESERVE") == 0) ||
              (strcmp (argv[Index + 2], "reserve") == 0)) {
            FileSize = xtoi (argv[Index + 3]);
            if (FileSize >= 0x80000000) {
              Error (NULL, 0, 0, "-O Parameter incorrect, FileSize too large!", NULL);
              return 0;
            }
            FileBuffer = malloc (FileSize);
            if (FileBuffer == NULL) {
              Error (NULL, 0, 0, "No sufficient memory to allocate!", NULL);
              return 0;
            }
            SetMem (FileBuffer, FileSize, 0xFF);
            //
            // Set the most significant bit
            // It means the data in memory, not in flash yet.
            // Assume the file size should < 2G.
            //
            FileSize |= 0x80000000;
            Index += 4;
          } else {
            //
            // 5th, try <Address, Length>
            //
            if (Index + 3 >= argc) {
              break;
            }
            FileBuffer = (UINT8 *) (UINTN) xtoi (argv[Index + 2]);
            FileSize = xtoi (argv[Index + 3]);
            if (FileSize >= 0x80000000) {
              Error (NULL, 0, 0, "-O Parameter incorrect, FileSize too large!", NULL);
              return 0;
            }
            Index += 4;
          }
        }
      }
    }
    if (gFitTableContext.OptionalModuleNumber >= MAX_OPTIONAL_ENTRY) {
      Error (NULL, 0, 0, "-O Parameter incorrect, Too many Optional Module!", NULL);
      free (FileBuffer);
      return 0;
    }
    gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Type = Type;
    if (gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT) {
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].SubType = SubType;
    }
    gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Address = (UINT32) (UINTN) FileBuffer;
    gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Buffer = FileBuffer;
    gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Size = FileSize;

    //
    // 4.1 Optional Module version
    //
    if ((Index + 1 >= argc) ||
        ((strcmp (argv[Index], "-V") != 0) &&
         (strcmp (argv[Index], "-v") != 0)) ) {
      //
      // Bypass
      //
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Version = gFitTableContext.GlobalVersion;
    } else {
      //
      // Get offset from parameter
      //
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Version = xtoi (argv[Index + 1]);
      Index += 2;
    }

    gFitTableContext.OptionalModuleNumber ++;
    gFitTableContext.FitEntryNumber++;
  }

  //
  // 5. Port type
  //
  while (TRUE) {
    if (Index + 6 >= argc) {
      break;
    }
    if ((strcmp (argv[Index], "-P") != 0) &&
        (strcmp (argv[Index], "-p") != 0) ) {
      break;
    }

    Type = xtoi (argv[Index + 1]);
    if (gFitTableContext.PortModuleNumber >= MAX_PORT_ENTRY) {
      printf ("-P Parameter incorrect, Too many Port Module!\n");
      return 0;
    }

    gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Type = Type;
    gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Address = (UINT16)xtoi (argv[Index + 2]) + ((UINT16)xtoi (argv[Index + 3]) << 16);
    gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Size = (UINT8)xtoi (argv[Index + 4]) + ((UINT8)xtoi (argv[Index + 5]) << 8) + ((UINT16)xtoi (argv[Index + 6]) << 16);
    Index += 7;

    //
    // 5.1 Port Module version
    //
    if ((Index + 1 >= argc) ||
        ((strcmp (argv[Index], "-V") != 0) &&
         (strcmp (argv[Index], "-v") != 0)) ) {
      //
      // Bypass
      //
      gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Version = 0;
    } else {
      //
      // Get offset from parameter
      //
      gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Version = xtoi (argv[Index + 1]);
      Index += 2;
    }

    gFitTableContext.PortModuleNumber++;
    gFitTableContext.FitEntryNumber++;
  }

  //
  // Final: Check StartupAcm in BiosModule.
  //
  CheckOverlap (gFitTableContext.StartupAcm.Address, gFitTableContext.StartupAcm.Size);
  FitEntryNumber = gFitTableContext.FitEntryNumber;
  for (Index = 0; Index < (INTN)gFitTableContext.OptionalModuleNumber; Index++) {
    if ((gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_POLICY) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_KEY_MANIFEST) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_DATA_AREA) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT)) {
      // NOTE: It might be virtual address now. Just put a place holder.
      FitEntryNumber ++;
    }
  }

  return FitEntryNumber;
}

VOID *
FindSpaceSkipApVector (
  IN UINT8     *FvBuffer,
  IN UINT8     *Address,
  IN UINTN     Size
  )
/*++

Routine Description:

  No enough space - it might happen that it is occupied by AP wake vector.
  Last chance - skip this and search again.

Arguments:

  FvBuffer       - FvRecovery binary buffer
  Address        - Address to be searched from
  Size           - Size need to be filled

Returns:

  FitTableOffset - The FIT table offset
  NULL           - No enough space for FIT table

*/
{
  UINT8        *ApVector;
  UINT8        *NewAddress;
  UINTN        Index;

  ApVector = (UINT8 *)((UINTN)Address & ~0xFFF);
  if ((UINTN)ApVector <= (UINTN)FvBuffer) {
    return NULL;
  }

  NewAddress = (UINT8 *)(ApVector - Size);
  for (Index = 0; Index < Size; Index ++) {
    if (NewAddress[Index] != 0xFF) {
      return NULL;
    }
  }
  return NewAddress;
}

VOID *
GetFreeSpaceFromFv (
  IN UINT8     *FvBuffer,
  IN UINT32    FvSize,
  IN UINT32    FitEntryNumber
  )
/*++

Routine Description:

  Get free space for FIT table from FvRecovery

Arguments:

  FvBuffer       - FvRecovery binary buffer
  FvSize         - FvRecovery size
  FitEntryNumber - The FIT entry number

Returns:

  FitTableOffset - The offset of FIT table in FvRecovery file
  NULL           - Free space not found

--*/
{
  UINT8       *FitTableOffset;
  INTN        Index;
  INTN        SubIndex;
  UINT8       *OptionalModuleAddress;
  EFI_GUID    VTFGuid = EFI_FFS_VOLUME_TOP_FILE_GUID;
  UINT32      AlignedSize;
  UINT32      FitTableSize;

  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  EFI_FFS_FILE_HEADER         *FileHeader;
  UINT64                      FvLength;
  UINT32                      Offset;
  UINT32                      FileLength;
  UINT32                      FileOccupiedSize;

  //
  // Check 4G - FitTablePointerOffset
  //
  if ((*(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset) != 0xFFFFFFFFFFFFFFFFull) &&
      (*(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset) != 0) &&
      (*(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset) != 0xEEEEEEEEEEEEEEEEull)) {
    Error (NULL, 0, 0, "4G - FitTablePointerOffset is occupied!", NULL);
    return NULL;
  }
  if (gFitTableContext.FitTablePointerOffset2 != 0) {
    if ((*(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset2) != 0xFFFFFFFFFFFFFFFFull) &&
        (*(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset2) != 0) &&
        (*(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset2) != 0xEEEEEEEEEEEEEEEEull)) {
      Error (NULL, 0, 0, "4G - FitTablePointerOffset is occupied!", NULL);
      return NULL;
    }
  }

  //
  // Get EFI_FFS_VOLUME_TOP_FILE_GUID location
  //
  FitTableOffset = NULL;

  FvHeader         = (EFI_FIRMWARE_VOLUME_HEADER *)FvBuffer;
  FvLength         = FvHeader->FvLength;
  FileHeader       = (EFI_FFS_FILE_HEADER *)(FvBuffer + FvHeader->HeaderLength);
  Offset           = (UINTN)FileHeader - (UINTN)FvBuffer;

  while (Offset < FvLength) {
    FileLength = (*(UINT32 *)(FileHeader->Size)) & 0x00FFFFFF;
    FileOccupiedSize = GETOCCUPIEDSIZE(FileLength, 8);
    if ((CompareGuid (&(FileHeader->Name), &VTFGuid)) == 0) {
      // find it
      FitTableOffset = (UINT8 *)FileHeader;
      break;
    }
    FileHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FileHeader + FileOccupiedSize);
    Offset = (UINTN)FileHeader - (UINTN)FvBuffer;
  }

  if (FitTableOffset == NULL) {
    Error (NULL, 0, 0, "EFI_FFS_VOLUME_TOP_FILE_GUID not found!", NULL);
    return NULL;
  }

  FitTableSize = FitEntryNumber * sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY);
  FitTableSize += FIT_ALIGNMENT;
  FitTableSize &= ~FIT_ALIGNMENT;

  FitTableOffset = (UINT8 *)((UINTN)FitTableOffset & ~FIT_ALIGNMENT);
  FitTableOffset = (UINT8 *)(FitTableOffset - FitTableSize);

  //
  // Check it
  //
  for (Index = 0; Index < (INTN)(FitTableSize); Index ++) {
    if (FitTableOffset[Index] != 0xFF) {
      //
      // No enough space - it might happen that it is occupied by AP wake vector.
      // Last chance - skip this and search again.
      //
      FitTableOffset = FindSpaceSkipApVector (FvBuffer, &FitTableOffset[Index], FitTableSize);
      if (FitTableOffset == NULL) {
        Error (NULL, 0, 0, "No enough space for FIT table!", NULL);
        return NULL;
      }
    }
  }

  //
  // Check space for Optional module
  //
  OptionalModuleAddress = FitTableOffset;
  for (Index = 0; Index < (INTN)gFitTableContext.OptionalModuleNumber; Index++) {
    AlignedSize = gFitTableContext.OptionalModule[Index].Size;
    if ((gFitTableContext.OptionalModule[Index].Size & 0x80000000) != 0) {

      //
      // Need copy binary to file.
      //
      gFitTableContext.OptionalModule[Index].Size &= ~0x80000000;

      AlignedSize = gFitTableContext.OptionalModule[Index].Size;
      if ((gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_POLICY) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_KEY_MANIFEST) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_DATA_AREA) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT)) {
        // Let it 64 byte align
        AlignedSize += BIOS_MODULE_ALIGNMENT;
        AlignedSize &= ~BIOS_MODULE_ALIGNMENT;
      }

      OptionalModuleAddress -= AlignedSize;

      if ((gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_POLICY) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_KEY_MANIFEST) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_DATA_AREA) ||
          (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT)) {
        // Let it 64 byte align
        OptionalModuleAddress = (UINT8 *)((UINTN)OptionalModuleAddress & ~BIOS_MODULE_ALIGNMENT);
      }

      for (SubIndex = 0; SubIndex < (INTN)(AlignedSize); SubIndex ++) {
        if (OptionalModuleAddress[SubIndex] != 0xFF) {
          //
          // No enough space - it might happen that it is occupied by AP wake vector.
          // Last chance - skip this and search again.
          //
          OptionalModuleAddress = FindSpaceSkipApVector (FvBuffer, &OptionalModuleAddress[SubIndex], AlignedSize);
          if (OptionalModuleAddress == NULL) {
            Error (NULL, 0, 0, "No enough space for OptionalModule!", NULL);
            return NULL;
          }
        }
      }
      memcpy (OptionalModuleAddress, gFitTableContext.OptionalModule[Index].Buffer, gFitTableContext.OptionalModule[Index].Size);
      free (gFitTableContext.OptionalModule[Index].Buffer);
      gFitTableContext.OptionalModule[Index].Address = MEMORY_TO_FLASH (OptionalModuleAddress, FvBuffer, FvSize);
    }
    //
    // Final: Check BiosPolicyData in BiosModule.
    //
    if ((gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_POLICY) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_KEY_MANIFEST) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_BIOS_DATA_AREA) ||
        (gFitTableContext.OptionalModule[Index].Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT)) {
      CheckOverlap (gFitTableContext.OptionalModule[Index].Address, AlignedSize);
    }
  }

  return FitTableOffset;
}

VOID
PrintFitData (
  VOID
  )
/*++

Routine Description:

  Output FIT table information

Arguments:

  None

Returns:

  None

--*/
{
  UINT32                          Index;

  printf ("FIT Table Pointer Offset: 0x%x\n", gFitTableContext.FitTablePointerOffset);
  if (gFitTableContext.FitTablePointerOffset2 != 0) {
    printf ("FIT Table Pointer Offset: 0x%x\n", gFitTableContext.FitTablePointerOffset2);
  }
  printf ("Total FIT Entry number: 0x%x\n", gFitTableContext.FitEntryNumber);
  printf ("FitHeader version: 0x%04x\n", gFitTableContext.FitHeaderVersion);
  if (gFitTableContext.StartupAcm.Address != 0) {
    printf ("StartupAcm - (0x%08x, 0x%08x, 0x%04x)\n", gFitTableContext.StartupAcm.Address, gFitTableContext.StartupAcm.Size, gFitTableContext.StartupAcmVersion);
  }
  if (gFitTableContext.DiagnstAcm.Address != 0) {
    printf ("DiagnosticAcm - (0x%08x, 0x%08x, 0x%04x)\n", gFitTableContext.DiagnstAcm.Address, gFitTableContext.DiagnstAcm.Size, gFitTableContext.DiagnstAcmVersion);
  }
  for (Index = 0; Index < gFitTableContext.BiosModuleNumber; Index++) {
    printf ("BiosModule[%d] - (0x%08x, 0x%08x, 0x%04x)\n", Index, gFitTableContext.BiosModule[Index].Address, gFitTableContext.BiosModule[Index].Size, gFitTableContext.BiosModuleVersion);
  }
  for (Index = 0; Index < gFitTableContext.MicrocodeNumber; Index++) {
    printf ("Microcode[%d] - (0x%08x, 0x%08x, 0x%04x)\n", Index, gFitTableContext.Microcode[Index].Address, gFitTableContext.Microcode[Index].Size, gFitTableContext.MicrocodeVersion);
  }
  for (Index = 0; Index < gFitTableContext.OptionalModuleNumber; Index++) {
    printf ("OptionalModule[%d] - (0x%08x, 0x%08x, 0x%02x, 0x%04x)\n", Index, gFitTableContext.OptionalModule[Index].Address, gFitTableContext.OptionalModule[Index].Size, gFitTableContext.OptionalModule[Index].Type, gFitTableContext.OptionalModule[Index].Version);
  }
  for (Index = 0; Index < gFitTableContext.PortModuleNumber; Index++) {
    printf ("PortModule[%d] - (0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%04x, 0x%02x, 0x%04x)\n", Index,
      (UINT16)gFitTableContext.PortModule[Index].Address, (UINT16)(gFitTableContext.PortModule[Index].Address >> 16),
      (UINT8)gFitTableContext.PortModule[Index].Size, (UINT8)(gFitTableContext.PortModule[Index].Size >> 8), (UINT16)(gFitTableContext.PortModule[Index].Size >> 16),
      gFitTableContext.PortModule[Index].Type, gFitTableContext.PortModule[Index].Version);
  }

  printf ("\n");
  return ;
}

CHAR8 *mFitCseSubTypeStr[] = {
  "CSE_RSVD   ",
  "CSE_K_HASH1",
  "CSE_M_HASH ",
  "CSE_BPOLICY",
  "CSE_OTHR_BP",
  "CSE_OEMSMIP",
  "CSE_MRCDATA",
  "CSE_IBBL_H ",
  "CSE_IBB_H  ",
  "CSE_OEM_ID ",
  "CSEOEMSKUID",
  "CSE_BD_IND ",
  "CSE_FPM    ",
  "CSE_ACMM   "
};

CHAR8 *mFitTypeStr[] = {
  "           ",
  "MICROCODE  ",
  "STARTUP_ACM",
  "DIAGNST_ACM",
  "           ",
  "           ",
  "           ",
  "           ",
  "BIOS_MODULE",
  "TPM_POLICY ",
  "BIOS_POLICY",
  "TXT_POLICY ",
  "KEYMANIFEST",
  "BP_MANIFEST",
  "BIOS_DATA_A",
  " ",
  " ",
  "CSE_SECUREB"
};

CHAR8  mFitSignature[] = "'_FIT_   ' ";
CHAR8  mFitSignatureInHeader[] = "'        ' ";
CHAR8 *
FitTypeToStr (
  IN FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry
  )
/*++

Routine Description:

  Convert FitEntry type to a string

Arguments:

  FitEntry - Fit entry

Returns:

  String

--*/
{
  if (FitEntry->Type == FIT_TABLE_TYPE_HEADER) {
    CopyMem (&mFitSignatureInHeader[1], &FitEntry->Address, sizeof(FitEntry->Address));
    return mFitSignatureInHeader;
  }
  if (FitEntry->Type < sizeof (mFitTypeStr)/sizeof(mFitTypeStr[0])) {
    if (FitEntry->Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT) {
      //
      // "Reserved" field is used to distinguish CSE Secure Boot entries (see FIT spec revision 1.2)
      //
      if (FitEntry->Rsvd < sizeof (mFitCseSubTypeStr)/sizeof(mFitCseSubTypeStr[0])) {
        return mFitCseSubTypeStr[FitEntry->Rsvd];
      }
    }
    return mFitTypeStr[FitEntry->Type];
  } else {
    return "           ";
  }
}

VOID
PrintFitTable (
  IN UINT8                       *FvBuffer,
  IN UINT32                      FvSize
  )
/*++

Routine Description:

  Print Fit table in flash image

Arguments:

  FvBuffer       - FvRecovery binary buffer
  FvSize         - FvRecovery size

Returns:

  None

--*/
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  UINT32                          EntryNum;
  UINT32                          Index;
  UINT32                          FitTableOffset;
  FIRMWARE_INTERFACE_TABLE_ENTRY_PORT   *FitEntryPort;

  printf ("##############\n");
  printf ("# FIT Table: #\n");
  printf ("##############\n");

  printf ("FIT Pointer Offset: 0x%x\n", gFitTableContext.FitTablePointerOffset);
  FitTableOffset = *(UINT32 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset);
  printf ("FIT Table Address:  0x%x\n", FitTableOffset);
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FLASH_TO_MEMORY(FitTableOffset, FvBuffer, FvSize);

  //
  // Check FitEntry is 16 byte aligned
  //
  if (((UINTN)FitEntry & 0xF) != 0) {
    printf("ERROR: invalid FitEntry address 0x%X!\n", (UINT32) (UINTN) FitEntry);
    return;
  }

  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;
  printf ("====== ================ ====== ======== ============== ==== ======== (====== ==== ====== ==== ======)\n");
  printf ("Index:      Address      Size  Version       Type      C_V  Checksum (Index  Data Width  Bit  Offset)\n");
  printf ("====== ================ ====== ======== ============== ==== ======== (====== ==== ====== ==== ======)\n");
  for (Index = 0; Index < EntryNum; Index++) {
    printf (" %02d:   %016llx %06x   %04x   %02x-%s  %02x     %02x   ",
      Index,
      (unsigned long long) FitEntry[Index].Address,
      *(UINT32 *)(&FitEntry[Index].Size[0]) & 0xFFFFFF,
      FitEntry[Index].Version,
      FitEntry[Index].Type,
      FitTypeToStr(&FitEntry[Index]),
      FitEntry[Index].C_V,
      FitEntry[Index].Checksum
      );

    if (Index == 0) {
      if (FitEntry[Index].Type != FIT_TABLE_TYPE_HEADER) {
        printf("ERROR: FIT Entry 0 is not Header Type %d!\n", FIT_TABLE_TYPE_HEADER);
        return;
      }
      if (strcmp(mFitSignatureInHeader, mFitSignature) != 0) {
        printf("ERROR: FIT Entry 0 signature invalid (%s, expected %s)!\n", mFitSignatureInHeader, mFitSignature);
        return;
      }

    }

    switch (FitEntry[Index].Type) {
    case FIT_TABLE_TYPE_TPM_POLICY:
    case FIT_TABLE_TYPE_TXT_POLICY:
      if (FitEntry[Index].Version == 0) {
        FitEntryPort = (FIRMWARE_INTERFACE_TABLE_ENTRY_PORT *)&FitEntry[Index];
        printf (" ( %04x  %04x   %02x    %02x   %04x )\n",
          FitEntryPort->IndexPort,
          FitEntryPort->DataPort,
          FitEntryPort->Width,
          FitEntryPort->Bit,
          FitEntryPort->Index
          );
        break;
      }
      // Not Port Configure, pass through
    default:
      printf ("\n");
      break;
    }
  }
  printf ("====== ================ ====== ======== ============== ==== ======== (====== ==== ====== ==== ======)\n");
  printf ("Index:      Address      Size  Version       Type      C_V  Checksum (Index  Data Width  Bit  Offset)\n");
  printf ("====== ================ ====== ======== ============== ==== ======== (====== ==== ====== ==== ======)\n");
}

/**

  This function dump raw data.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
DumpData (
  IN UINT8  *Data,
  IN UINT32 Size
  )
{
  UINT32 Index;
  for (Index = 0; Index < Size; Index++) {
    printf ("%02x", Data[Index]);
  }
}

/**

  This function dump raw data with colume format.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
DumpHex (
  IN UINT8  *Data,
  IN UINT32 Size
  )
{
  UINT32  Index;
  UINT32  Count;
  UINT32  Left;

#define COLUME_SIZE  (16 * 2)

  Count = Size / COLUME_SIZE;
  Left  = Size % COLUME_SIZE;
  for (Index = 0; Index < Count; Index++) {
    printf ("%04x: ", Index * COLUME_SIZE);
    DumpData (Data + Index * COLUME_SIZE, COLUME_SIZE);
    printf ("\n");
  }

  if (Left != 0) {
    printf ("%04x: ", Index * COLUME_SIZE);
    DumpData (Data + Index * COLUME_SIZE, Left);
    printf ("\n");
  }
}

//
// This table defines the ACM type string
//
CHAR8 *mAcmTypeStr[] = {
  "BIOS ACM",
  "SINIT ACM",
};

//
// This table defines the ACM capability string
//
CHAR8 *mCapabilityStr[] = {
  "GETSEC[WAKEUP] for RLP   ",
  "MONITOR address for RLP  ",
  "ECX for MLE PageTable    ",
  "STM support              ",
};

VOID
DumpAcm (
  IN ACM_FORMAT                    *Acm
  )
/*++

Routine Description:

  DumpAcm information

Arguments:

  Acm       - ACM buffer

Returns:

  None

--*/
{
  CHIPSET_ACM_INFORMATION_TABLE *ChipsetAcmInformationTable;
  CHIPSET_ID_LIST               *ChipsetIdList;
  PROCESSOR_ID_LIST             *ProcessorIdList;
  UINT32                        Index;
  UINT8                         *Buffer;

  printf (
    "*****************************************************************************\n"
    "*         ACM                                                               *\n"
    "*****************************************************************************\n"
    );

  printf ("ACM: (%08x)\n", (UINT32) (UINTN) Acm);
  printf ("  ModuleType                 - %04x\n", Acm->ModuleType);
  if (Acm->ModuleType == ACM_MODULE_TYPE_CHIPSET_ACM) {
    printf ("    Chipset ACM\n");
  }
  printf ("  ModuleSubType              - %04x\n", Acm->ModuleSubType);
  if ((Acm->ModuleSubType & ACM_MODULE_SUBTYPE_CAPABLE_OF_EXECUTE_AT_RESET) != 0) {
    printf ("    Capable of be Executed at Reset\n");
  }
  if ((Acm->ModuleSubType & ACM_MODULE_SUBTYPE_ANC_MODULE) != 0) {
    printf ("    AnC Module\n");
  }
  printf ("  HeaderLen                  - %08x\n", Acm->HeaderLen);
  printf ("  HeaderVersion              - %08x\n", Acm->HeaderVersion);
  printf ("  ChipsetID                  - %04x\n", Acm->ChipsetID);
  printf ("  Flags                      - %04x\n", Acm->Flags);
  printf ("    PreProduction            - %04x\n", Acm->Flags & ACM_MODULE_FLAG_PREPRODUCTION);
  printf ("    Debug Signed             - %04x\n", Acm->Flags & ACM_MODULE_FLAG_DEBUG_SIGN);
  printf ("  ModuleVendor               - %08x\n", Acm->ModuleVendor);
  printf ("  Date                       - %08x\n", Acm->Date);
  printf ("  Size                       - %08x\n", Acm->Size);
  printf ("  TxtSvn                     - %04x\n", Acm->TxtSvn);
  printf ("  SeSvn                      - %04x\n", Acm->SeSvn);
  printf ("  CodeControl                - %08x\n", Acm->CodeControl);
  printf ("  ErrorEntryPoint            - %08x\n", Acm->ErrorEntryPoint);
  printf ("  GDTLimit                   - %08x\n", Acm->GDTLimit);
  printf ("  GDTBasePtr                 - %08x\n", Acm->GDTBasePtr);
  printf ("  SegSel                     - %08x\n", Acm->SegSel);
  printf ("  EntryPoint                 - %08x\n", Acm->EntryPoint);
  printf ("  KeySize                    - %08x\n", Acm->KeySize);
  printf ("  ScratchSize                - %08x\n", Acm->ScratchSize);

  Buffer = (UINT8 *)(Acm + 1);
  printf ("  RSAPubKey                  - \n");
  DumpHex (Buffer, Acm->KeySize * 4);
  printf ("\n");
  Buffer += Acm->KeySize * 4;
  //
  // To simplify the tool and making it independent of ACM header change,
  // the rest of ACM parsing  will be skipped starting ACM_HEADER_VERSION4
  //
  if((Acm->HeaderVersion != ACM_HEADER_VERSION_3) && (Acm->HeaderVersion != ACM_HEADER_VERSION_0)){
     printf (
        "*****************************************************************************\n\n"
        );
    return;
  }
  if (Acm->HeaderVersion == ACM_HEADER_VERSION_3) {
    printf ("  RSASig                     - \n");
    DumpHex (Buffer, ACM_PKCS_1_5_RSA_SIGNATURE_SHA384_SIZE); // PKCS #1.5 RSA Signature
    printf ("\n");
    Buffer += ACM_PKCS_1_5_RSA_SIGNATURE_SHA384_SIZE;
  } else {
    printf ("  RSAPubExp                  - %08x\n", *(UINT32 *)Buffer);
    Buffer += 4;

    printf ("  RSASig                     - \n");
    DumpHex (Buffer, ACM_PKCS_1_5_RSA_SIGNATURE_SHA256_SIZE); // PKCS #1.5 RSA Signature
    printf ("\n");
    Buffer += ACM_PKCS_1_5_RSA_SIGNATURE_SHA256_SIZE;
  }
  Buffer += Acm->ScratchSize * 4;

  if ((Acm->ModuleSubType & ACM_MODULE_SUBTYPE_ANC_MODULE) == 0) {
    ChipsetAcmInformationTable = (CHIPSET_ACM_INFORMATION_TABLE *)Buffer;
    printf ("Chipset ACM info:\n");
    printf (
      "  Guid                       - {%08x-%08x-%08x-%08x}\n",
      ChipsetAcmInformationTable->Guid.Guid0,
      ChipsetAcmInformationTable->Guid.Guid1,
      ChipsetAcmInformationTable->Guid.Guid2,
      ChipsetAcmInformationTable->Guid.Guid3
      );
    printf ("  ChipsetACMType             - %02x\n", ChipsetAcmInformationTable->ChipsetACMType);
    if (ChipsetAcmInformationTable->ChipsetACMType < sizeof(mAcmTypeStr)/sizeof(mAcmTypeStr[0])) {
      printf ("    %s\n", mAcmTypeStr[ChipsetAcmInformationTable->ChipsetACMType]);
    }
    printf ("  Version                    - %02x\n", ChipsetAcmInformationTable->Version);
    printf ("  Length                     - %04x\n", ChipsetAcmInformationTable->Length);
    printf ("  ChipsetIDList              - %08x\n", ChipsetAcmInformationTable->ChipsetIDList);
    printf ("  OsSinitTableVer            - %08x\n", ChipsetAcmInformationTable->OsSinitTableVer);
    printf ("  MinMleHeaderVer            - %08x\n", ChipsetAcmInformationTable->MinMleHeaderVer);
    if (ChipsetAcmInformationTable->Version >= CHIPSET_ACM_INFORMATION_TABLE_VERSION_3) {
      printf ("  Capabilities               - %08x\n", ChipsetAcmInformationTable->Capabilities);
      for (Index = 0; Index < sizeof(mCapabilityStr)/sizeof(mCapabilityStr[0]); Index++) {
        if (mCapabilityStr[Index] == NULL) {
          continue;
        }
        printf (
          "    %s- %08x\n",
          mCapabilityStr[Index],
          (ChipsetAcmInformationTable->Capabilities & (1 << Index))
          );
      }
      printf ("  AcmVersion                 - %02x\n", ChipsetAcmInformationTable->AcmVersion);
      printf ("  AcmRevision                - %02x.%02x.%02x\n", ChipsetAcmInformationTable->AcmRevision[0], ChipsetAcmInformationTable->AcmRevision[1], ChipsetAcmInformationTable->AcmRevision[2]);
    }
    if (ChipsetAcmInformationTable->Version >= CHIPSET_ACM_INFORMATION_TABLE_VERSION_4) {
      printf ("  ProcessorIDList            - %08x\n", ChipsetAcmInformationTable->ProcessorIDList);
    }

    ChipsetIdList = (CHIPSET_ID_LIST *)((UINTN)Acm + ChipsetAcmInformationTable->ChipsetIDList);
    printf ("Chipset ID List info:\n");
    printf ("  Count                      - %08x\n", ChipsetIdList->Count);
    for (Index = 0; Index < ChipsetIdList->Count; Index++) {
      printf ("  ID[%d]:\n", Index);
      printf ("    Flags                    - %08x\n", ChipsetIdList->ChipsetID[Index].Flags);
      printf ("      RevisionIdMask         - %08x\n", ChipsetIdList->ChipsetID[Index].Flags & ACM_CHIPSET_ID_REVISION_ID_MAKE);
      printf ("    VendorID                 - %04x\n", ChipsetIdList->ChipsetID[Index].VendorID);
      printf ("    DeviceID                 - %04x\n", ChipsetIdList->ChipsetID[Index].DeviceID);
      printf ("    RevisionID               - %04x\n", ChipsetIdList->ChipsetID[Index].RevisionID);
    }
    if (ChipsetAcmInformationTable->Version < CHIPSET_ACM_INFORMATION_TABLE_VERSION_4) {
      goto End;
    }
    ProcessorIdList = (PROCESSOR_ID_LIST *)((UINTN)Acm + ChipsetAcmInformationTable->ProcessorIDList);
    printf ("Processor ID List info:\n");
    printf ("  Count                      - %08x\n", ProcessorIdList->Count);
    for (Index = 0; Index < ProcessorIdList->Count; Index++) {
      printf ("  ID[%d]:\n", Index);
      printf ("    FMS                      - %08x\n", ProcessorIdList->ProcessorID[Index].FMS);
      printf ("    FMSMask                  - %08x\n", ProcessorIdList->ProcessorID[Index].FMSMask);
      printf ("    PlatformID               - %016llx\n", (unsigned long long) ProcessorIdList->ProcessorID[Index].PlatformID);
      printf ("    PlatformMask             - %016llx\n", (unsigned long long) ProcessorIdList->ProcessorID[Index].PlatformMask);
    }
  }

End:
  printf (
    "*****************************************************************************\n\n"
    );
}

BOOLEAN
CheckAcm (
  IN ACM_FORMAT                        *Acm,
  IN UINTN                             AcmMaxSize
  )
/*++

Routine Description:

  Check Acm information

Arguments:

  Acm        - ACM buffer
  AcmMaxSize - ACM max size

Returns:

  TRUE  - ACM is valid
  FALSE - ACM is invalid

--*/
{
  CHIPSET_ACM_INFORMATION_TABLE *ChipsetAcmInformationTable;
  CHIPSET_ID_LIST               *ChipsetIdList;
  PROCESSOR_ID_LIST             *ProcessorIdList;
  UINT8                         *Buffer;

  if (Acm->ModuleType != ACM_MODULE_TYPE_CHIPSET_ACM) {
    printf ("ACM invalid : ModuleType!\n");
    return FALSE;
  }
  if (Acm->Size * 4 > AcmMaxSize) {
    printf ("ACM invalid : Size!\n");
    return FALSE;
  }

  //
  // To simplify the tool and making it independent of ACM header change,
  // the following check will be skipped starting ACM_HEADER_VERSION3
  //
  if((Acm->HeaderVersion != ACM_HEADER_VERSION_3) && (Acm->HeaderVersion != ACM_HEADER_VERSION_0)){
    printf ("ACM header Version 4 or higher, bypassing other checks!\n");
    return TRUE;
  }
  Buffer = (UINT8 *)(Acm + 1);
  Buffer += Acm->KeySize * 4;
  if (Acm->HeaderVersion == ACM_HEADER_VERSION_3) {
    Buffer += ACM_PKCS_1_5_RSA_SIGNATURE_SHA384_SIZE;
  } else {
    Buffer += 4;
    Buffer += ACM_PKCS_1_5_RSA_SIGNATURE_SHA256_SIZE;
  }
  Buffer += Acm->ScratchSize * 4;

  if ((Acm->ModuleSubType & ACM_MODULE_SUBTYPE_ANC_MODULE) == 0) {
    ChipsetAcmInformationTable = (CHIPSET_ACM_INFORMATION_TABLE *)Buffer;
    if ((UINTN)ChipsetAcmInformationTable >= (UINTN)Acm + AcmMaxSize) {
      printf ("ACM invalid : ChipsetAcmInformationTable!\n");
      return FALSE;
    }

    if (CompareGuid ((EFI_GUID *)&ChipsetAcmInformationTable->Guid, (EFI_GUID *)&mChipsetAcmInformationTableGuid03) != 0) {
      printf ("ACM invalid : ChipsetACMGuid!\n");
      return FALSE;
    }
    if (ChipsetAcmInformationTable->ChipsetACMType != CHIPSET_ACM_TYPE_BIOS) {
      printf ("ACM invalid : ChipsetACMType!\n");
      return FALSE;
    }
    if (ChipsetAcmInformationTable->Version < CHIPSET_ACM_INFORMATION_TABLE_VERSION_3) {
      printf ("ACM invalid : ChipsetACMVersion!\n");
      return FALSE;
    }
    if ((UINTN)ChipsetAcmInformationTable + ChipsetAcmInformationTable->Length > (UINTN)Acm + AcmMaxSize) {
      printf ("ACM invalid : ChipsetACMLength!\n");
      return FALSE;
    }

    if (ChipsetAcmInformationTable->ChipsetIDList >= AcmMaxSize) {
      printf ("ACM invalid : ChipsetACMChipsetIDList!\n");
      return FALSE;
    }
    ChipsetIdList = (CHIPSET_ID_LIST *)((UINTN)Acm + ChipsetAcmInformationTable->ChipsetIDList);
    if (ChipsetIdList->Count == 0) {
      printf ("ACM invalid : ChipsetACMChipsetIDListCount!\n");
      return FALSE;
    }
    if (ChipsetAcmInformationTable->ChipsetIDList + sizeof(CHIPSET_ID_LIST) + (ChipsetIdList->Count - 1) * sizeof(ACM_CHIPSET_ID) > AcmMaxSize) {
      printf ("ACM invalid : ChipsetACMChipsetIDList!\n");
      return FALSE;
    }

    if (ChipsetAcmInformationTable->Version < CHIPSET_ACM_INFORMATION_TABLE_VERSION_4) {
      goto End;
    }

    if (ChipsetAcmInformationTable->ProcessorIDList >= AcmMaxSize) {
      printf ("ACM invalid : ChipsetACMProcessorIDList!\n");
      return FALSE;
    }
    ProcessorIdList = (PROCESSOR_ID_LIST *)((UINTN)Acm + ChipsetAcmInformationTable->ProcessorIDList);
    if (ProcessorIdList->Count == 0) {
      printf ("ACM invalid : ChipsetACMProcessorIdListCount!\n");
      return FALSE;
    }
    if (ChipsetAcmInformationTable->ChipsetIDList + sizeof(PROCESSOR_ID_LIST) + (ChipsetIdList->Count - 1) * sizeof(ACM_PROCESSOR_ID) > AcmMaxSize) {
      printf ("ACM invalid : ChipsetACMProcessorIdList!\n");
      return FALSE;
    }
  }

End:

  return TRUE;
}

VOID
FillFitTable (
  IN UINT8     *FvBuffer,
  IN UINT32    FvSize,
  IN UINT8     *FitTableOffset
  )
/*++

Routine Description:

  Fill the FIT table information to FvRecovery

Arguments:

  FvBuffer       - FvRecovery binary buffer
  FvSize         - FvRecovery size
  FitTableOffset - The offset of FIT table in FvRecovery file

Returns:

  None

--*/
{
  FIRMWARE_INTERFACE_TABLE_ENTRY *FitEntry;
  UINT32                          FitIndex;
  UINT32                          Index;
  UINT8                           Checksum;
  UINTN                           SubIndex;
  FIT_TABLE_CONTEXT_ENTRY         TempContextEntry;
  FIRMWARE_INTERFACE_TABLE_ENTRY  TempTableEntry;

  //
  // 1. FitPointer
  //
  *(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset) = (UINT64)(UINTN)MEMORY_TO_FLASH (FitTableOffset, FvBuffer, FvSize);
  if (gFitTableContext.FitTablePointerOffset2 != 0) {
    *(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset2) = (UINT64)(UINTN)MEMORY_TO_FLASH (FitTableOffset, FvBuffer, FvSize);
  }

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitTableOffset;
  FitIndex = 0;

  //
  // 2. FitHeader
  //
  FitEntry[FitIndex].Address             = *(UINT64 *)"_FIT_   ";
  *(UINT32 *)&FitEntry[FitIndex].Size[0] = gFitTableContext.FitEntryNumber;
  FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.FitHeaderVersion;
  FitEntry[FitIndex].Type                = FIT_TABLE_TYPE_HEADER;
  FitEntry[FitIndex].C_V                 = 1;
  //
  // Checksum will be updated later...
  //
  FitEntry[FitIndex].Checksum            = 0;

  //
  // 3. Microcode
  //
  FitIndex++;
  for (Index = 0; Index < gFitTableContext.MicrocodeNumber; Index++) {
    FitEntry[FitIndex].Address             = gFitTableContext.Microcode[Index].Address;
    *(UINT32 *)&FitEntry[FitIndex].Size[0] = 0; //gFitTableContext.Microcode[Index].Size / 16;
    FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.MicrocodeVersion;
    FitEntry[FitIndex].Type                = FIT_TABLE_TYPE_MICROCODE;
    FitEntry[FitIndex].C_V                 = 0;
    FitEntry[FitIndex].Checksum            = 0;
    FitIndex++;
  }

  //
  // 4. StartupAcm
  //
  if (gFitTableContext.StartupAcm.Address != 0) {
    FitEntry[FitIndex].Address             = gFitTableContext.StartupAcm.Address;
    *(UINT32 *)&FitEntry[FitIndex].Size[0] = 0; //gFitTableContext.StartupAcm.Size / 16;
    FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.StartupAcmVersion;
    FitEntry[FitIndex].Type                = FIT_TABLE_TYPE_STARTUP_ACM;
    FitEntry[FitIndex].C_V                 = 0;
    FitEntry[FitIndex].Checksum            = 0;
    FitIndex++;
  }

  //
  // 4.5. DiagnosticAcm
  //
  if (gFitTableContext.DiagnstAcm.Address != 0) {
    FitEntry[FitIndex].Address             = gFitTableContext.DiagnstAcm.Address;
    *(UINT32 *)&FitEntry[FitIndex].Size[0] = 0;
    FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.DiagnstAcmVersion;
    FitEntry[FitIndex].Type                = FIT_TABLE_TYPE_DIAGNST_ACM;
    FitEntry[FitIndex].C_V                 = 0;
    FitEntry[FitIndex].Checksum            = 0;
    FitIndex++;
  }
  //
  // 5. BiosModule
  //
  //
  // BiosModule segments order needs to be put from low addresss to high for Btg requirement
  //
  if (gFitTableContext.BiosModuleNumber > 1) {
    for (Index = 0; Index < (UINTN)gFitTableContext.BiosModuleNumber - 1; Index++){
      for (SubIndex = 0; SubIndex < gFitTableContext.BiosModuleNumber - Index - 1; SubIndex++) {
        if (gFitTableContext.BiosModule[SubIndex].Address > gFitTableContext.BiosModule[SubIndex + 1].Address) {
          CopyMem (&TempContextEntry, &gFitTableContext.BiosModule[SubIndex], sizeof(FIT_TABLE_CONTEXT_ENTRY));
          CopyMem (&gFitTableContext.BiosModule[SubIndex], &gFitTableContext.BiosModule[SubIndex + 1], sizeof(FIT_TABLE_CONTEXT_ENTRY));
          CopyMem (&gFitTableContext.BiosModule[SubIndex + 1], &TempContextEntry, sizeof(FIT_TABLE_CONTEXT_ENTRY));
        }
      }
    }
  }
  for (Index = 0; Index < gFitTableContext.BiosModuleNumber; Index++) {
    FitEntry[FitIndex].Address             = gFitTableContext.BiosModule[Index].Address;
    *(UINT32 *)&FitEntry[FitIndex].Size[0] = gFitTableContext.BiosModule[Index].Size / 16;
    FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.BiosModuleVersion;
    FitEntry[FitIndex].Type                = FIT_TABLE_TYPE_BIOS_MODULE;
    FitEntry[FitIndex].C_V                 = 0;
    FitEntry[FitIndex].Checksum            = 0;
    FitIndex++;
  }

  //
  // 6. Optional module
  //
  for (Index = 0; Index < gFitTableContext.OptionalModuleNumber; Index++) {
    FitEntry[FitIndex].Address             = gFitTableContext.OptionalModule[Index].Address;
    *(UINT32 *)&FitEntry[FitIndex].Size[0] = gFitTableContext.OptionalModule[Index].Size;
    FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.OptionalModule[Index].Version;
    FitEntry[FitIndex].Type                = (UINT8)gFitTableContext.OptionalModule[Index].Type;
    if (FitEntry[FitIndex].Type == FIT_TABLE_TYPE_CSE_SECURE_BOOT) {
      FitEntry[FitIndex].Rsvd              = (UINT8)gFitTableContext.OptionalModule[Index].SubType;
    }
    FitEntry[FitIndex].C_V                 = 0;
    FitEntry[FitIndex].Checksum            = 0;
    FitIndex++;
  }

  //
  // 7. Port module
  //
  for (Index = 0; Index < gFitTableContext.PortModuleNumber; Index++) {
    FitEntry[FitIndex].Address             = gFitTableContext.PortModule[Index].Address + ((UINT64)gFitTableContext.PortModule[Index].Size << 32);
    *(UINT32 *)&FitEntry[FitIndex].Size[0] = 0;
    FitEntry[FitIndex].Version             = (UINT16)gFitTableContext.PortModule[Index].Version;
    FitEntry[FitIndex].Type                = (UINT8)gFitTableContext.PortModule[Index].Type;
    FitEntry[FitIndex].C_V                 = 0;
    FitEntry[FitIndex].Checksum            = 0;
    FitIndex++;
  }

  //
  // The FIT records must always be arranged in the ascending order of their type attribute in the FIT.
  //
  for (Index = 0; Index < (UINTN)FitIndex - 1; Index++){
    for (SubIndex = 0; SubIndex < FitIndex - Index - 1; SubIndex++) {
      if (FitEntry[SubIndex].Type > FitEntry[SubIndex + 1].Type) {
        CopyMem (&TempTableEntry, &FitEntry[SubIndex], sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY));
        CopyMem (&FitEntry[SubIndex], &FitEntry[SubIndex + 1], sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY));
        CopyMem (&FitEntry[SubIndex + 1], &TempTableEntry, sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY));
      }
    }
  }

  //
  // Update FIT header signature as final step
  //
  Checksum = CalculateChecksum8 ((UINT8 *)&FitEntry[0], sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY) * FitIndex);
  FitEntry[0].Checksum = Checksum;
}

VOID
ClearFitTable (
  IN UINT8     *FvBuffer,
  IN UINT32    FvSize
  )
/*++

Routine Description:

  Clear the FIT table information to Fvrecovery

Arguments:

  FvBuffer       - Fvrecovery binary buffer
  FvSize         - Fvrecovery size

Returns:

  None

--*/
{
  FIRMWARE_INTERFACE_TABLE_ENTRY *FitEntry;
  UINT32                          EntryNum;
  UINT32                          FitIndex;
  UINT64                          FitTablePointer;
  UINT8                           *Buffer;
  UINT32                          BufferSize;

  FitTablePointer = *(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset);
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FLASH_TO_MEMORY (FitTablePointer, FvBuffer, FvSize);

  //
  // Clear FIT pointer
  //
  *(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset) = 0xEEEEEEEEEEEEEEEEull;
  if (gFitTableContext.FitTablePointerOffset2 != 0) {
    *(UINT64 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset2) = 0xEEEEEEEEEEEEEEEEull;
  }

  //
  // Clear FIT table
  //
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;
  for (FitIndex = 0; FitIndex < EntryNum; FitIndex++) {
    switch (FitEntry[FitIndex].Type) {
    case FIT_TABLE_TYPE_BIOS_POLICY:
    case FIT_TABLE_TYPE_KEY_MANIFEST:
    case FIT_TABLE_TYPE_BOOT_POLICY_MANIFEST:
    case FIT_TABLE_TYPE_BIOS_DATA_AREA:
    case FIT_TABLE_TYPE_CSE_SECURE_BOOT:
      //
      // Clear FIT table data buffer
      //
      Buffer = FLASH_TO_MEMORY (FitEntry[FitIndex].Address, FvBuffer, FvSize);
      BufferSize = (*(UINT32 *)FitEntry[FitIndex].Size) & 0xFFFFFF;
      SetMem (Buffer, BufferSize, 0xFF);
      break;
    default:
      break;
    }
    //
    // Clear FIT table itself
    //
    SetMem (&FitEntry[FitIndex], sizeof(FitEntry[FitIndex]), 0xFF);
  }
}

STATUS
WriteOutputFile (
  IN CHAR8   *FileName,
  IN UINT8   *FileData,
  IN UINT32  FileSize
  )
/*++

Routine Description:

  Read input file

Arguments:

  FileName      - The input file name
  FileData      - The input file data
  FileSize      - The input file size

Returns:

  STATUS_SUCCESS - Write file data successfully
  STATUS_ERROR   - The file data is not written

--*/
{
  FILE                        *FpOut;

  //
  //Check the File Path
  //
  if (!CheckPath(FileName)) {

    Error (NULL, 0, 0, "File path is invalid!", NULL);
    return STATUS_ERROR;
  }

  //
  // Open the output FvRecovery.fv file
  //
  if ((FpOut = fopen (FileName, "w+b")) == NULL) {
    Error (NULL, 0, 0, "Unable to open file", "%s", FileName);
    return STATUS_ERROR;
  }
  //
  // Write the output FvRecovery.fv file
  //
  if ((fwrite (FileData, 1, FileSize, FpOut)) != FileSize) {
    Error (NULL, 0, 0, "Write output file error!", NULL);
    fclose (FpOut);
    return STATUS_ERROR;
  }

  //
  // Close the output FvRecovery.fv file
  //
  fclose (FpOut);

  return STATUS_SUCCESS;
}

UINT32
GetFvRecoveryInfoFromFd (
  IN UINT8                       *FdBuffer,
  IN UINT32                      FdFileSize,
  OUT UINT8                      **FvRecovery
  )
/*++

Routine Description:

  Get FvRecovery information from Fd file.

Arguments:

  FdBuffer     - Fd file buffer.
  FdFileSize   - Fd file size.
  FvRecovery   - FvRecovery pointer in Fd file buffer

Returns:
  FvRecovery file size

--*/
{
  UINT8                         *FileBuffer = NULL;
  UINT32                        FvRecoveryFileSize =0;
  EFI_GUID                      VTFGuid = EFI_FFS_VOLUME_TOP_FILE_GUID;
  UINT32                        FvLength;
  UINT32                        FileLength;

  *FvRecovery = NULL;
  FileBuffer = FindNextFvHeader (FdBuffer, FdFileSize);
  if (FileBuffer == NULL) {
    return 0;
  }

  while ((UINTN)FileBuffer < (UINTN)FdBuffer + FdFileSize) {
    FvLength         = (UINT32)((EFI_FIRMWARE_VOLUME_HEADER *)FileBuffer)->FvLength;

    if (FindFileFromFvByGuid (FileBuffer, FvLength, &VTFGuid, &FileLength) != NULL) {
      //
      // Found the VTF
      //
      FvRecoveryFileSize = FvLength;
      *FvRecovery = FileBuffer;
    }

    //
    // Next fv
    //
    FileBuffer = (UINT8 *)FileBuffer + FvLength;
    if ((UINTN)FileBuffer >= (UINTN)FdBuffer + FdFileSize) {
      break;
    }
    FileBuffer = FindNextFvHeader (FileBuffer, (UINTN)FdBuffer + FdFileSize - (UINTN)FileBuffer);
    if (FileBuffer == NULL) {
      break;
    }

  }

  //
  // Return
  //
  return FvRecoveryFileSize;
}

UINT32
GetFitEntryInfo (
  IN UINT8     *FvBuffer,
  IN UINT32    FvSize
  )
/*++

Routine Description:

  Fill the FIT table information to Fvrecovery

Arguments:

  FvBuffer       - Fvrecovery binary buffer
  FvSize         - Fvrecovery size

Returns:

  0 - Fit Table not found

--*/
{
  FIRMWARE_INTERFACE_TABLE_ENTRY *FitEntry;
  UINT32                          FitIndex;
  UINT32                          FitTableOffset;

  //
  // 1. FitPointer
  //
  if (gFitTableContext.FitTablePointerOffset == 0) {
    gFitTableContext.FitTablePointerOffset = DEFAULT_FIT_TABLE_POINTER_OFFSET;
  }
  gFitTableContext.FitTablePointerOffset2 = 0;

  FitTableOffset = *(UINT32 *)(FvBuffer + FvSize - gFitTableContext.FitTablePointerOffset);

  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FLASH_TO_MEMORY(FitTableOffset, FvBuffer, FvSize);
  FitIndex = 0;

  //
  // 2. FitHeader
  //
  if (FitEntry[FitIndex].Address != *(UINT64 *)"_FIT_   ") {
    return 0;
  }
  if (FitEntry[FitIndex].Type != FIT_TABLE_TYPE_HEADER) {
    return 0;
  }
  gFitTableContext.FitEntryNumber = *(UINT32 *)&FitEntry[FitIndex].Size[0];
  gFitTableContext.FitHeaderVersion = FitEntry[FitIndex].Version;

  //
  // 3. FitEntry
  //
  FitIndex++;
  for (; FitIndex < gFitTableContext.FitEntryNumber; FitIndex++) {
    switch (FitEntry[FitIndex].Type) {
    case FIT_TABLE_TYPE_MICROCODE:
      gFitTableContext.Microcode[gFitTableContext.MicrocodeNumber].Address = (UINT32)FitEntry[FitIndex].Address;
      gFitTableContext.MicrocodeVersion                                    = FitEntry[FitIndex].Version;
      gFitTableContext.MicrocodeNumber ++;
      break;
    case FIT_TABLE_TYPE_STARTUP_ACM:
      gFitTableContext.StartupAcm.Address = (UINT32)FitEntry[FitIndex].Address;
      gFitTableContext.StartupAcmVersion  = FitEntry[FitIndex].Version;
      break;
    case FIT_TABLE_TYPE_BIOS_MODULE:
      gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Address = (UINT32)FitEntry[FitIndex].Address;
      gFitTableContext.BiosModule[gFitTableContext.BiosModuleNumber].Size    = *(UINT32 *)&FitEntry[FitIndex].Size[0] * 16;
      gFitTableContext.BiosModuleVersion                                     = FitEntry[FitIndex].Version;
      gFitTableContext.BiosModuleNumber ++;
      break;
    case FIT_TABLE_TYPE_TPM_POLICY:
    case FIT_TABLE_TYPE_TXT_POLICY:
      if (FitEntry[FitIndex].Version == 0) {
        gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Address = (UINT32)FitEntry[FitIndex].Address;
        gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Size    = (UINT32)(FitEntry[FitIndex].Address >> 32);
        gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Version = FitEntry[FitIndex].Version;
        gFitTableContext.PortModule[gFitTableContext.PortModuleNumber].Type    = FitEntry[FitIndex].Type;
        gFitTableContext.PortModuleNumber ++;
        break;
      }
      // Not Port Configure, pass through
    default: // Others
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Address = (UINT32)FitEntry[FitIndex].Address;
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Size    = *(UINT32 *)&FitEntry[FitIndex].Size[0];
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Version = FitEntry[FitIndex].Version;
      gFitTableContext.OptionalModule[gFitTableContext.OptionalModuleNumber].Type    = FitEntry[FitIndex].Type;
      gFitTableContext.OptionalModuleNumber ++;
      break;
    }
  }

  return gFitTableContext.FitEntryNumber;
}

STATUS
FitGen (
  IN INTN   argc,
  IN CHAR8  **argv
  )
/*++

Routine Description:

  Main function for FitGen.

Arguments:

  argc - Number of command line parameters.
  argv - Array of pointers to parameter strings.

Returns:
  STATUS_SUCCESS - Utility exits successfully.
  STATUS_ERROR   - Some error occurred during execution.

--*/
{
  UINT32                      FvRecoveryFileSize;
  UINT8                       *FileBuffer;
  UINT8                       *FileBufferRaw;
  UINTN                       FitEntryNumber;
  UINT8                       *FitTableOffset;
  STATUS                      Status;
  UINT32                      FitTableSize;

  BOOLEAN                     IsFv;
  UINT8                       *FdFileBuffer;
  UINT32                      FdFileSize;

  UINT8                       *AcmBuffer;

  FileBufferRaw = NULL;
  //
  // Step 0: Check FV or FD
  //
  if (((strcmp (argv[1], "-D") == 0) ||
       (strcmp (argv[1], "-d") == 0)) ) {
    IsFv = FALSE;
  } else {
    IsFv = TRUE;
  }

  //
  // Step 1: Read InputFvRecovery.fv data
  //
  if (IsFv) {
    Status = ReadInputFile (argv[1], &FileBuffer, &FvRecoveryFileSize, &FileBufferRaw);
    if (Status != STATUS_SUCCESS) {
      Error (NULL, 0, 0, "Unable to open file", "%s", argv[1]);
      goto exitFunc;
    }
    FdFileBuffer = FileBuffer;
    FdFileSize = FvRecoveryFileSize;
  } else {
    Status = ReadInputFile (argv[2], &FdFileBuffer, &FdFileSize, &FileBufferRaw);
    if (Status != STATUS_SUCCESS) {
      Error (NULL, 0, 0, "Unable to open file", "%s", argv[2]);
      goto exitFunc;
    }

    //
    // Get Fvrecovery information
    //
    FvRecoveryFileSize = GetFvRecoveryInfoFromFd (FdFileBuffer, FdFileSize, &FileBuffer);
    if ((FvRecoveryFileSize == 0) || (FileBuffer == NULL)) {
      Error (NULL, 0, 0, "FvRecovery not found in Fd file!", NULL);
      Status = STATUS_ERROR;
      goto exitFunc;
    }
  }

  //
  // Step 2: Calculate FIT entry number.
  //
  FitEntryNumber = GetFitEntryNumber (argc, argv, FdFileBuffer, FdFileSize);
  if (!gFitTableContext.Clear) {
    if (FitEntryNumber == 0) {
      Status = STATUS_ERROR;
      goto exitFunc;
    }

    //
    // For debug
    //
    PrintFitData ();

    //
    // Add 1 more FitEntry as place holder, because we need exclude FIT table itself
    //
    FitEntryNumber++;

    //
    // Step 3: Get enough space in FvRecovery.fv
    //
    FitTableOffset = GetFreeSpaceFromFv (FileBuffer, FvRecoveryFileSize, FitEntryNumber);
    if (FitTableOffset == NULL) {
      return STATUS_ERROR;
    }
    FitTableSize = FitEntryNumber * sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY);
    FitTableSize += FIT_ALIGNMENT;
    FitTableSize &= ~FIT_ALIGNMENT;

    CheckOverlap (
      MEMORY_TO_FLASH (FitTableOffset, FdFileBuffer, FdFileSize),
      FitTableSize
      );

    //
    // Get ACM buffer
    //
    if (gFitTableContext.StartupAcm.Address != 0) {
      AcmBuffer = FLASH_TO_MEMORY(gFitTableContext.StartupAcm.Address, FdFileBuffer, FdFileSize);
      if ((AcmBuffer < FdFileBuffer) || (AcmBuffer + gFitTableContext.StartupAcm.Size > FdFileBuffer + FdFileSize)) {
        printf ("ACM out of range - can not validate it\n");
        AcmBuffer = NULL;
      }

      if (AcmBuffer != NULL) {
        if (CheckAcm ((ACM_FORMAT *)AcmBuffer, gFitTableContext.StartupAcm.Size)) {
          DumpAcm ((ACM_FORMAT *)AcmBuffer);
        } else {
          Status = STATUS_ERROR;
          goto exitFunc;
        }
      }

    }

    //
    // Step 4: Fill the FIT table one by one
    //
    FillFitTable (FdFileBuffer, FdFileSize, FitTableOffset);

    //
    // For debug
    //
    PrintFitTable (FdFileBuffer, FdFileSize);
  } else {
    printf ("Clear FIT table ...\n");
    //
    // Step 3: Get FIT table info
    //
    FitEntryNumber = GetFitEntryInfo (FdFileBuffer, FdFileSize);
    if (FitEntryNumber == 0) {
      Error (NULL, 0, 0, "No FIT table found", NULL);
      return STATUS_ERROR;
    }

    //
    // For debug
    //
    PrintFitTable (FdFileBuffer, FdFileSize);

    //
    // Step 4: Clear FIT table
    //
    ClearFitTable (FdFileBuffer, FdFileSize);
    printf ("Clear FIT table Done!\n");
  }

  //
  // Step 5: Write OutputFvRecovery.fv data
  //
  if (IsFv) {
    Status = WriteOutputFile (argv[2], FileBuffer, FvRecoveryFileSize);
  } else {
    Status = WriteOutputFile (argv[3], FdFileBuffer, FdFileSize);
  }

exitFunc:
  if (FileBufferRaw != NULL) {
    free ((VOID *)FileBufferRaw);
  }
  return Status;
}

STATUS
FitView (
  IN INTN   argc,
  IN CHAR8  **argv
  )
/*++

Routine Description:

  View function for FitGen.

Arguments:

  argc - Number of command line parameters.
  argv - Array of pointers to parameter strings.

Returns:
  STATUS_SUCCESS - Utility exits successfully.
  STATUS_ERROR   - Some error occurred during execution.

--*/
{
  UINT32                      FvRecoveryFileSize;
  UINT8                       *FileBuffer;
  UINT8                       *FileBufferRaw = NULL;
  STATUS                      Status;

  //
  // Step 1: Read input file
  //
  Status = ReadInputFile (argv[2], &FileBuffer, &FvRecoveryFileSize, &FileBufferRaw);
  if (Status != STATUS_SUCCESS) {
    Error (NULL, 0, 0, "Unable to open file", "%s", argv[2]);
    goto exitFunc;
  }

  // no -f option, use default FIT pointer offset
  if (argc == 3) {
    //
    // Use default address
    //
    gFitTableContext.FitTablePointerOffset = DEFAULT_FIT_TABLE_POINTER_OFFSET;
  } else if (stricmp (argv[3], "-f") == 0) {
    if (argc == 5) {
      //
      // Get offset from parameter
      //
      gFitTableContext.FitTablePointerOffset = xtoi (argv[3 + 1]);
    } else {
      Error (NULL, 0, 0, "FIT offset not specified!", NULL);
      goto exitFunc;
    }
  } else {
    Error (NULL, 0, 0, "Invalid view option: ", "%s", argv[3]);
    goto exitFunc;
  }

  //
  // For debug
  //
  PrintFitTable (FileBuffer, FvRecoveryFileSize);

exitFunc:
  if (FileBufferRaw != NULL) {
    free ((VOID *)FileBufferRaw);
  }
  return Status;
}

int
main (
  int   argc,
  char  **argv
  )
/*++

Routine Description:

  Main function.

Arguments:

  argc - Number of command line parameters.
  argv - Array of pointers to parameter strings.

Returns:
  STATUS_SUCCESS - Utility exits successfully.
  STATUS_ERROR   - Some error occurred during execution.

--*/
{
  SetUtilityName (UTILITY_NAME);

  //
  // Display utility information
  //
  PrintUtilityInfo ();

  //
  // Verify the correct number of arguments
  //
  if (argc >= MIN_VIEW_ARGS && stricmp (argv[1], "-view") == 0) {
    return FitView (argc, argv);
  } else if (argc >= MIN_ARGS) {
    return FitGen (argc, argv);
  } else {
    Error (NULL, 0, 0, "invalid number of input parameters specified", NULL);
    PrintUsage ();
    return STATUS_ERROR;
  }
}

unsigned int
xtoi (
  char  *str
  )
/*++

Routine Description:

  Convert hex string to uint

Arguments:

  str  -  The string

Returns:

--*/
{
  unsigned int u;
  char         c;
  unsigned int m;

  if (str == NULL) {
    return 0;
  }

  m = (unsigned int) -1 >> 4;
  //
  // skip preceeding white space
  //
  while (*str && *str == ' ') {
    str += 1;
  }
  //
  // skip preceeding zeros
  //
  while (*str && *str == '0') {
    str += 1;
  }
  //
  // skip preceeding x/X character
  //
  if (*str && (*str == 'x' || *str == 'X')) {
    str += 1;
  }
  //
  // convert hex digits
  //
  u = 0;
  c = *(str++);
  while (c) {
    if (c >= 'a' && c <= 'f') {
      c -= 'a' - 'A';
    }

    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {
      if (u > m) {
        return (unsigned int) -1;
      }

      u = (u << 4) | (c - (c >= 'A' ? 'A' - 10 : '0'));
    } else {
      //
      // Let application exit immediately
      //
      Error (NULL, 0, 0, "Hex value is expected!", NULL);
      exit (0);
      break;
    }

    c = *(str++);
  }

  return u;
}

