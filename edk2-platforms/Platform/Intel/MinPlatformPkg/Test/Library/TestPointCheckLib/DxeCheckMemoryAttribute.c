/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Guid/MemoryAttributesTable.h>
#include <Protocol/Runtime.h>

CHAR8 *
ShortNameOfMemoryType(
  IN UINT32 Type
  );

VOID
TestPointDumpMemoryAttributesTable (
  IN EFI_MEMORY_ATTRIBUTES_TABLE                     *MemoryAttributesTable
  )
{
  UINTN                 Index;
  EFI_MEMORY_DESCRIPTOR *Entry;
  UINT64                Pages[EfiMaxMemoryType];

  ZeroMem (Pages, sizeof(Pages));

  DEBUG ((DEBUG_INFO, "MemoryAttributesTable:"));
  DEBUG ((DEBUG_INFO, " Version=0x%x", MemoryAttributesTable->Version));
  DEBUG ((DEBUG_INFO, " Count=0x%x", MemoryAttributesTable->NumberOfEntries));
  DEBUG ((DEBUG_INFO, " DescriptorSize=0x%x\n", MemoryAttributesTable->DescriptorSize));
  Entry = (EFI_MEMORY_DESCRIPTOR *)(MemoryAttributesTable + 1);
  DEBUG ((DEBUG_INFO, "Type       Start            End              # Pages          Attributes\n"));
  for (Index = 0; Index < MemoryAttributesTable->NumberOfEntries; Index++) {
    DEBUG ((DEBUG_INFO, ShortNameOfMemoryType(Entry->Type)));
    DEBUG ((DEBUG_INFO, " %016LX-%016LX %016LX %016LX\n",
      Entry->PhysicalStart,
      Entry->PhysicalStart+MultU64x64 (SIZE_4KB,Entry->NumberOfPages) - 1,
      Entry->NumberOfPages,
      Entry->Attribute
      ));
    if (Entry->Type < EfiMaxMemoryType) {
      Pages[Entry->Type] += Entry->NumberOfPages;
    }
    Entry = NEXT_MEMORY_DESCRIPTOR (Entry, MemoryAttributesTable->DescriptorSize);
  }
  
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "  RT_Code   : %14ld Pages (%ld Bytes)\n", Pages[EfiRuntimeServicesCode],     MultU64x64(SIZE_4KB, Pages[EfiRuntimeServicesCode])));
  DEBUG ((DEBUG_INFO, "  RT_Data   : %14ld Pages (%ld Bytes)\n", Pages[EfiRuntimeServicesData],     MultU64x64(SIZE_4KB, Pages[EfiRuntimeServicesData])));
  DEBUG ((DEBUG_INFO, "              -------------- \n"));
}

EFI_STATUS
TestPointCheckMemoryAttribute (
  IN EFI_MEMORY_ATTRIBUTES_TABLE     *MemoryAttributesTable,
  IN EFI_PHYSICAL_ADDRESS            Base,
  IN UINT64                          Size,
  IN BOOLEAN                         IsCode,
  IN BOOLEAN                         IsFromSmm
  )
{
  UINTN                 Index;
  EFI_MEMORY_DESCRIPTOR *Entry;
  
  DEBUG ((DEBUG_ERROR, "Attribute Checking 0x%lx - 0x%lx\n", Base, Size));
  Entry = (EFI_MEMORY_DESCRIPTOR *)(MemoryAttributesTable + 1);
  for (Index = 0; Index < MemoryAttributesTable->NumberOfEntries; Index++) {
    if (Base >= Entry->PhysicalStart && Base+Size <= Entry->PhysicalStart+MultU64x64 (SIZE_4KB,Entry->NumberOfPages)) {
      if (IsFromSmm) {
        if (IsCode) {
          if (Entry->Type != EfiRuntimeServicesCode) {
            DEBUG ((DEBUG_ERROR, "Invalid Entry->Type %d\n", Entry->Type));
            return EFI_INVALID_PARAMETER;
          }
          if ((Entry->Attribute & (EFI_MEMORY_RO | EFI_MEMORY_XP)) != EFI_MEMORY_RO) {
            DEBUG ((DEBUG_ERROR, "Invalid Code Entry->Attribute 0x%lx\n", Entry->Attribute));
            return EFI_INVALID_PARAMETER;
          }
        } else {
          if (Entry->Type != EfiRuntimeServicesData) {
            DEBUG ((DEBUG_ERROR, "Invalid Entry->Type %d\n", Entry->Type));
            return EFI_INVALID_PARAMETER;
          }
          if ((Entry->Attribute & (EFI_MEMORY_RO | EFI_MEMORY_XP)) != EFI_MEMORY_XP) {
            DEBUG ((DEBUG_ERROR, "Invalid Data Entry->Attribute 0x%lx\n", Entry->Attribute));
            return EFI_INVALID_PARAMETER;
          }
        }
      } else {
        if (Entry->Type != EfiRuntimeServicesCode) {
          DEBUG ((DEBUG_ERROR, "Invalid Entry->Type %d\n", Entry->Type));
          return EFI_INVALID_PARAMETER;
        }
        if (IsCode) {
          if ((Entry->Attribute & (EFI_MEMORY_RO | EFI_MEMORY_XP)) != EFI_MEMORY_RO) {
            DEBUG ((DEBUG_ERROR, "Invalid Code Entry->Attribute 0x%lx\n", Entry->Attribute));
            return EFI_INVALID_PARAMETER;
          }
        } else {
          if ((Entry->Attribute & (EFI_MEMORY_RO | EFI_MEMORY_XP)) != EFI_MEMORY_XP) {
            DEBUG ((DEBUG_ERROR, "Invalid Data Entry->Attribute 0x%lx\n", Entry->Attribute));
            return EFI_INVALID_PARAMETER;
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
TestPointCheckImageMemoryAttribute (
  IN EFI_MEMORY_ATTRIBUTES_TABLE     *MemoryAttributesTable,
  IN EFI_PHYSICAL_ADDRESS            ImageBase,
  IN UINT64                          ImageSize,
  IN BOOLEAN                         IsFromSmm
  )
{
  EFI_STATUS                           Status;
  EFI_STATUS                           ReturnStatus;
  VOID                                 *ImageAddress;
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  UINT32                               PeCoffHeaderOffset;
  UINT32                               SectionAlignment;
  EFI_IMAGE_SECTION_HEADER             *Section;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;
  UINT8                                *Name;
  UINTN                                Index;
  CHAR8                                *PdbPointer;

  ReturnStatus = EFI_SUCCESS;
  //
  // Check whole region
  //
  ImageAddress = (VOID *)(UINTN)ImageBase;

  PdbPointer = PeCoffLoaderGetPdbPointer (ImageAddress);
  if (PdbPointer != NULL) {
    DEBUG ((EFI_D_INFO, "  Image - %a\n", PdbPointer));
  }

  //
  // Check PE/COFF image
  //
  DosHdr = (EFI_IMAGE_DOS_HEADER *) (UINTN) ImageAddress;
  PeCoffHeaderOffset = 0;
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    PeCoffHeaderOffset = DosHdr->e_lfanew;
  }

  Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINT8 *) (UINTN) ImageAddress + PeCoffHeaderOffset);
  if (Hdr.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
    DEBUG ((EFI_D_INFO, "Hdr.Pe32->Signature invalid - 0x%x\n", Hdr.Pe32->Signature));
    return EFI_INVALID_PARAMETER;
  }
  
  //
  // Measuring PE/COFF Image Header;
  // But CheckSum field and SECURITY data directory (certificate) are excluded
  //
  if (Hdr.Pe32->FileHeader.Machine == IMAGE_FILE_MACHINE_IA64 && Hdr.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // NOTE: Some versions of Linux ELILO for Itanium have an incorrect magic value 
    //       in the PE/COFF Header.
    //
    SectionAlignment  = Hdr.Pe32->OptionalHeader.SectionAlignment;
  } else {
    //
    // Get the section alignment value from the PE/COFF Optional Header
    //
    SectionAlignment  = Hdr.Pe32Plus->OptionalHeader.SectionAlignment;
  }

  if ((SectionAlignment & (RUNTIME_PAGE_ALLOCATION_GRANULARITY - 1)) != 0) {
    DEBUG ((EFI_D_INFO, "!!!!!!!!  RecordImageMemoryMap - Section Alignment(0x%x) is not %dK  !!!!!!!!\n", SectionAlignment, RUNTIME_PAGE_ALLOCATION_GRANULARITY >> 10));
    PdbPointer = PeCoffLoaderGetPdbPointer ((VOID*) (UINTN) ImageAddress);
    if (PdbPointer != NULL) {
      DEBUG ((EFI_D_INFO, "!!!!!!!!  Image - %a  !!!!!!!!\n", PdbPointer));
    }
    return EFI_INVALID_PARAMETER;
  }

  Section = (EFI_IMAGE_SECTION_HEADER *) (
               (UINT8 *) (UINTN) ImageAddress +
               PeCoffHeaderOffset +
               sizeof(UINT32) +
               sizeof(EFI_IMAGE_FILE_HEADER) +
               Hdr.Pe32->FileHeader.SizeOfOptionalHeader
               );

  Status = TestPointCheckMemoryAttribute (
             MemoryAttributesTable,
             (UINTN)ImageAddress,
             (UINTN)&Section[Hdr.Pe32->FileHeader.NumberOfSections] - (UINTN)ImageAddress,
             FALSE,
             IsFromSmm
             );
  if (EFI_ERROR(Status)) {
    ReturnStatus = Status;
  }

  for (Index = 0; Index < Hdr.Pe32->FileHeader.NumberOfSections; Index++) {
    Name = Section[Index].Name;
    DEBUG ((
      EFI_D_INFO,
      "  Section - '%c%c%c%c%c%c%c%c'\n",
      Name[0],
      Name[1],
      Name[2],
      Name[3],
      Name[4],
      Name[5],
      Name[6],
      Name[7]
      ));
      
    DEBUG ((EFI_D_INFO, "    VirtualSize          - 0x%08x\n", Section[Index].Misc.VirtualSize));
    DEBUG ((EFI_D_INFO, "    VirtualAddress       - 0x%08x\n", Section[Index].VirtualAddress));
    DEBUG ((EFI_D_INFO, "    SizeOfRawData        - 0x%08x\n", Section[Index].SizeOfRawData));
    DEBUG ((EFI_D_INFO, "    PointerToRawData     - 0x%08x\n", Section[Index].PointerToRawData));
    DEBUG ((EFI_D_INFO, "    PointerToRelocations - 0x%08x\n", Section[Index].PointerToRelocations));
    DEBUG ((EFI_D_INFO, "    PointerToLinenumbers - 0x%08x\n", Section[Index].PointerToLinenumbers));
    DEBUG ((EFI_D_INFO, "    NumberOfRelocations  - 0x%08x\n", Section[Index].NumberOfRelocations));
    DEBUG ((EFI_D_INFO, "    NumberOfLinenumbers  - 0x%08x\n", Section[Index].NumberOfLinenumbers));
    DEBUG ((EFI_D_INFO, "    Characteristics      - 0x%08x\n", Section[Index].Characteristics));
    if ((Section[Index].Characteristics & EFI_IMAGE_SCN_CNT_CODE) != 0) {
      //
      // Check code section
      //
      Status = TestPointCheckMemoryAttribute (
                 MemoryAttributesTable,
                 (UINTN)ImageAddress + Section[Index].VirtualAddress,
                 Section[Index].SizeOfRawData,
                 TRUE,
                 IsFromSmm
                 );
    } else {
      //
      // Check data section
      //
      Status = TestPointCheckMemoryAttribute (
                 MemoryAttributesTable,
                 (UINTN)ImageAddress + Section[Index].VirtualAddress,
                 Section[Index].SizeOfRawData,
                 FALSE,
                 IsFromSmm
                 );
    }
    if (EFI_ERROR(Status)) {
      ReturnStatus = Status;
    }
  }

  return ReturnStatus;
}

EFI_STATUS
TestPointCheckUefiMemoryAttributesTable (
  IN EFI_MEMORY_ATTRIBUTES_TABLE                     *MemoryAttributesTable
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 ReturnStatus;
  EFI_RUNTIME_ARCH_PROTOCOL  *RuntimeArch;
  LIST_ENTRY                 *Link;
  EFI_RUNTIME_IMAGE_ENTRY    *RuntimeImage;

  Status = gBS->LocateProtocol (
                  &gEfiRuntimeArchProtocolGuid,
                  NULL,
                  (VOID **)&RuntimeArch
                  );
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  ReturnStatus = EFI_SUCCESS;
  for (Link = RuntimeArch->ImageHead.ForwardLink; Link != &(RuntimeArch->ImageHead); Link = Link->ForwardLink) {
    RuntimeImage = BASE_CR (Link, EFI_RUNTIME_IMAGE_ENTRY, Link);
    Status = TestPointCheckImageMemoryAttribute (
               MemoryAttributesTable,
               (EFI_PHYSICAL_ADDRESS)(UINTN)RuntimeImage->ImageBase,
               RuntimeImage->ImageSize,
               FALSE
               );
    if (EFI_ERROR(Status)) {
      ReturnStatus = Status;
    }
  }

  return ReturnStatus;
}

EFI_STATUS
TestPointCheckUefiMemAttribute (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *MemoryAttributesTable;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckUefiMemAttribute - Enter\n"));
  Status = EfiGetSystemConfigurationTable (&gEfiMemoryAttributesTableGuid, (VOID **)&MemoryAttributesTable);
  if (!EFI_ERROR (Status)) {
    TestPointDumpMemoryAttributesTable(MemoryAttributesTable);
    Status = TestPointCheckUefiMemoryAttributesTable(MemoryAttributesTable);
  }

  if (EFI_ERROR (Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_STRING
      );
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckUefiMemAttribute - Exit\n"));

  return Status;
}
