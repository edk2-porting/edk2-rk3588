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
    if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
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
