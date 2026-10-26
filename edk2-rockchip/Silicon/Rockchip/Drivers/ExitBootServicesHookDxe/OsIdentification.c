/** @file
 *
 *  Copyright (c) 2024-2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/PeImage.h>
#include <Library/PeCoffGetEntryPointLib.h>

#include "ExitBootServicesHook.h"

STATIC CHAR8  *mOsTypeStrings[] = {
  [ExitBootServicesOsUnknown] = "Unknown",
  [ExitBootServicesOsWindows] = "Windows",
  [ExitBootServicesOsLinux]   = "Linux",
};
STATIC_ASSERT (ARRAY_SIZE (mOsTypeStrings) == ExitBootServicesOsMax);

#define LINUX_ARM64_MAGIC  0x644d5241
#define LINUX_PE_MAGIC     0x818223cd

STATIC
BOOLEAN
IsPeImageVmlinuz (
  IN VOID  *PeImage
  )
{
  UINT8  *Buf = PeImage;

  switch (*(UINT32 *)(Buf + 0x38)) {
    case LINUX_ARM64_MAGIC:
    case LINUX_PE_MAGIC:
      return TRUE;
  }

  return FALSE;
}

//
// Stale or junk pages starting with "MZ" can carry any e_lfanew
// (e.g. 0xFFFFF000). Only follow it if the whole PE header then lies
// within the same page; otherwise the page is not treated as a header.
//
STATIC
BOOLEAN
IsPeHeaderOffsetInPage (
  IN UINT32  PeHeaderOffset
  )
{
  return (PeHeaderOffset >= sizeof (EFI_IMAGE_DOS_HEADER)) &&
         (PeHeaderOffset <= EFI_PAGE_SIZE - sizeof (EFI_IMAGE_NT_HEADERS64)) &&
         ((PeHeaderOffset & (sizeof (UINT32) - 1)) == 0);
}

//
// PeCoffLoaderGetPdbPointer () follows the debug directory RVA and the
// CodeView record RVAs without bounds checks. Make sure everything it can
// dereference lies within SizeOfImage, which always holds for well-formed
// images. The PE32/PE32+ selection mirrors PeCoffLoaderGetPdbPointer ().
// Returns FALSE where PeCoffLoaderGetPdbPointer () would return NULL anyway.
//
STATIC
BOOLEAN
IsPeDebugDataInImage (
  IN VOID  *PeImage
  )
{
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;
  EFI_IMAGE_DATA_DIRECTORY             *DirectoryEntry;
  EFI_IMAGE_DEBUG_DIRECTORY_ENTRY      *DebugEntry;
  UINT32                               NumberOfRvaAndSizes;
  UINT64                               SizeOfImage;
  UINT64                               Offset;
  UINT64                               End;
  UINT64                               DataSize;
  UINT16                               Magic;

  DosHdr = (EFI_IMAGE_DOS_HEADER *)PeImage;
  if ((DosHdr->e_magic != EFI_IMAGE_DOS_SIGNATURE) ||
      !IsPeHeaderOffsetInPage (DosHdr->e_lfanew))
  {
    return FALSE;
  }

  Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINTN)PeImage + DosHdr->e_lfanew);
  if (Hdr.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
    return FALSE;
  }

  switch (Hdr.Pe32->FileHeader.Machine) {
    case IMAGE_FILE_MACHINE_I386:
      Magic = EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC;
      break;
    case IMAGE_FILE_MACHINE_X64:
    case IMAGE_FILE_MACHINE_IA64:
      Magic = EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC;
      break;
    default:
      Magic = Hdr.Pe32->OptionalHeader.Magic;
      break;
  }

  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    NumberOfRvaAndSizes = Hdr.Pe32->OptionalHeader.NumberOfRvaAndSizes;
    DirectoryEntry      = &Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_DEBUG];
    SizeOfImage         = Hdr.Pe32->OptionalHeader.SizeOfImage;
  } else if (Hdr.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    NumberOfRvaAndSizes = Hdr.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
    DirectoryEntry      = &Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_DEBUG];
    SizeOfImage         = Hdr.Pe32Plus->OptionalHeader.SizeOfImage;
  } else {
    return FALSE;
  }

  if (NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY_DEBUG) {
    return FALSE;
  }

  Offset = DirectoryEntry->VirtualAddress;
  End    = Offset + DirectoryEntry->Size;
  if (End > SizeOfImage) {
    return FALSE;
  }

  for ( ; Offset < End; Offset += sizeof (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY)) {
    if (Offset + sizeof (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY) > SizeOfImage) {
      return FALSE;
    }

    DebugEntry = (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY *)((UINTN)PeImage + (UINTN)Offset);
    if ((DebugEntry->Type == EFI_IMAGE_DEBUG_TYPE_CODEVIEW) && (DebugEntry->SizeOfData > 0)) {
      DataSize = DebugEntry->SizeOfData;
      if (DataSize < sizeof (UINT32)) {
        DataSize = sizeof (UINT32);
      }

      if ((UINT64)DebugEntry->RVA + DataSize > SizeOfImage) {
        return FALSE;
      }
    }
  }

  return TRUE;
}

STATIC CHAR8  mWinLoadNameStr[] = "winload";
#define PDB_NAME_MAX_LENGTH  256

STATIC
BOOLEAN
IsPeImageWinLoader (
  IN VOID  *PeImage
  )
{
  CHAR8  *PdbStr;
  UINTN  WinLoadNameStrLen;
  UINTN  Index;

  if (!IsPeDebugDataInImage (PeImage)) {
    return FALSE;
  }

  PdbStr = (CHAR8 *)PeCoffLoaderGetPdbPointer (PeImage);
  if (PdbStr == NULL) {
    return FALSE;
  }

  WinLoadNameStrLen = sizeof (mWinLoadNameStr) - sizeof (CHAR8);

  for (Index = 0; Index < PDB_NAME_MAX_LENGTH && PdbStr[Index] != '\0'; Index++) {
    if (AsciiStrnCmp (PdbStr + Index, mWinLoadNameStr, WinLoadNameStrLen) == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_PHYSICAL_ADDRESS
FindPeImageBase (
  IN EFI_PHYSICAL_ADDRESS  Base
  )
{
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;

  Base &= ~(EFI_PAGE_SIZE - 1);

  while (Base != 0) {
    DosHdr = (EFI_IMAGE_DOS_HEADER *)Base;
    if ((DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) &&
        IsPeHeaderOffsetInPage (DosHdr->e_lfanew))
    {
      Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)(Base + DosHdr->e_lfanew);
      if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE) {
        break;
      }
    }

    Base -= EFI_PAGE_SIZE;
  }

  return Base;
}

EXIT_BOOT_SERVICES_OS_TYPE
IdentifyOsType (
  IN EFI_PHYSICAL_ADDRESS  OsLoaderAddress
  )
{
  VOID  *OsLoaderImage;

  if (OsLoaderAddress == 0) {
    return ExitBootServicesOsUnknown;
  }

  OsLoaderImage = (VOID *)OsLoaderAddress;

  if (IsPeImageVmlinuz (OsLoaderImage)) {
    return ExitBootServicesOsLinux;
  }

  if (IsPeImageWinLoader (OsLoaderImage)) {
    return ExitBootServicesOsWindows;
  }

  return ExitBootServicesOsUnknown;
}

CHAR8 *
OsTypeToString (
  IN EXIT_BOOT_SERVICES_OS_TYPE  OsType
  )
{
  if ((OsType < ExitBootServicesOsUnknown) || (OsType >= ExitBootServicesOsMax)) {
    ASSERT (FALSE);
    return mOsTypeStrings[ExitBootServicesOsUnknown];
  }

  return mOsTypeStrings[OsType];
}
