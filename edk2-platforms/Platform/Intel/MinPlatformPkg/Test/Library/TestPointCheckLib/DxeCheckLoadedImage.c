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
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Protocol/LoadedImage.h>

BOOLEAN
IsRuntimeImage (
  IN VOID  *Pe32Data
  )
{
  EFI_IMAGE_DOS_HEADER                  *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION   Hdr;
  UINT16                                Magic;
  UINT16                                Subsystem;

  ASSERT (Pe32Data   != NULL);

  DosHdr = (EFI_IMAGE_DOS_HEADER *)Pe32Data;
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    //
    // DOS image header is present, so read the PE header after the DOS image header.
    //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINTN) Pe32Data + (UINTN) ((DosHdr->e_lfanew) & 0x0ffff));
  } else {
    //
    // DOS image header is not present, so PE header is at the image base.
    //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)Pe32Data;
  }

  if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE) {
    //
    // NOTE: We use Machine field to identify PE32/PE32+, instead of Magic.
    //       It is due to backward-compatibility, for some system might
    //       generate PE32+ image with PE32 Magic.
    //
    switch (Hdr.Pe32->FileHeader.Machine) {
    case IMAGE_FILE_MACHINE_I386:
      //
      // Assume PE32 image with IA32 Machine field.
      //
      Magic = EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC;
      break;
    case IMAGE_FILE_MACHINE_X64:
    case IMAGE_FILE_MACHINE_IA64:
      //
      // Assume PE32+ image with x64 or IA64 Machine field
      //
      Magic = EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC;
      break;
    default:
      //
      // For unknow Machine field, use Magic in optional Header
      //
      Magic = Hdr.Pe32->OptionalHeader.Magic;
    }

    Subsystem = 0;

    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      Subsystem = Hdr.Pe32->OptionalHeader.Subsystem;
    } else if (Hdr.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
      Subsystem = Hdr.Pe32Plus->OptionalHeader.Subsystem;
    } else {
      ASSERT (FALSE);
    }
    if (Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER) {
      return TRUE;
    }
  }

  return FALSE;
}

VOID
DumpLoadedImage (
  IN UINTN                                  Index,
  IN EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage,
  IN EFI_DEVICE_PATH_PROTOCOL               *DevicePath,
  IN EFI_DEVICE_PATH_PROTOCOL               *LoadedImageDevicePath
  )
{
  CHAR16                            *Str;
  CHAR8                             *PdbPointer;

  DEBUG ((DEBUG_INFO, "[0x%04x]:", Index));
  if (IsRuntimeImage (LoadedImage->ImageBase)) {
    DEBUG ((DEBUG_INFO, " (RT)"));
  } else {
    DEBUG ((DEBUG_INFO, "     "));
  }
  DEBUG ((DEBUG_INFO, " 0x%016lx-0x%016lx", (UINT64)(UINTN)LoadedImage->ImageBase, LoadedImage->ImageSize));

  if (LoadedImageDevicePath != NULL) {
    Str = ConvertDevicePathToText(LoadedImageDevicePath, TRUE, TRUE);
    DEBUG ((DEBUG_INFO, " LoadedImageDevicePath=%s", Str));
    if (Str != NULL) {
      FreePool (Str);
    }
  } else {
    if (LoadedImage->FilePath != NULL) {
      Str = ConvertDevicePathToText(LoadedImage->FilePath, TRUE, TRUE);
      DEBUG ((DEBUG_INFO, " FilePath=%s", Str));
      if (Str != NULL) {
        FreePool (Str);
      }
    }

    if (DevicePath != NULL) {
      Str = ConvertDevicePathToText(DevicePath, TRUE, TRUE);
      DEBUG ((DEBUG_INFO, " DevicePath=%s", Str));
      if (Str != NULL) {
        FreePool (Str);
      }
    }
  }

  PdbPointer = PeCoffLoaderGetPdbPointer (LoadedImage->ImageBase);
  if (PdbPointer != NULL) {
    DEBUG ((DEBUG_INFO, " (pdb - %a)", PdbPointer));
  }

  DEBUG ((DEBUG_INFO, "\n"));
}

EFI_STATUS
TestPointCheckNon3rdPartyImage (
  IN EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage,
  IN EFI_DEVICE_PATH_PROTOCOL               *DevicePath,
  IN EFI_DEVICE_PATH_PROTOCOL               *LoadedImageDevicePath
  )
{
  if (LoadedImageDevicePath != NULL) {
    // LoadedImageDevicePath should be Fv()/FvFile()
    if (DevicePathType (LoadedImageDevicePath) == MEDIA_DEVICE_PATH &&
        DevicePathSubType (LoadedImageDevicePath) == MEDIA_PIWG_FW_VOL_DP) {
      return EFI_SUCCESS;
    }
  } else {
    if (LoadedImage->FilePath != NULL) {
      // LoadedImage->FilePath should be FvFile()
      if (DevicePathType (LoadedImage->FilePath) == MEDIA_DEVICE_PATH &&
          DevicePathSubType (LoadedImage->FilePath) == MEDIA_PIWG_FW_FILE_DP) {
        return EFI_SUCCESS;
      }
    }
    if (DevicePath != NULL) {
      // DevicePath should be Fv()
      if (DevicePathType (DevicePath) == MEDIA_DEVICE_PATH &&
          DevicePathSubType (DevicePath) == MEDIA_PIWG_FW_VOL_DP) {
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_INVALID_PARAMETER;
}

EFI_STATUS
TestPointCheckLoadedImage (
  VOID
  )
{
  EFI_STATUS                             Status;
  EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage;
  UINTN                                  Index;
  EFI_HANDLE                             *HandleBuf;
  UINTN                                  HandleCount;
  EFI_DEVICE_PATH_PROTOCOL               *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL               *LoadedImageDevicePath;
  EFI_STATUS                             ReturnStatus;
  
  ReturnStatus = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "==== TestPointCheckLoadedImage - Enter\n"));
  HandleBuf = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadedImageProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuf
                  );
  if (EFI_ERROR (Status)) {
    goto Done ;
  }
  
  DEBUG ((DEBUG_INFO, "LoadedImage (%d):\n", HandleCount));
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuf[Index],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID **)&LoadedImage
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = gBS->HandleProtocol (LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath);
    if (EFI_ERROR(Status)) {
      DevicePath = NULL;
    }

    Status = gBS->HandleProtocol (HandleBuf[Index], &gEfiLoadedImageDevicePathProtocolGuid, (VOID **)&LoadedImageDevicePath);
    if (EFI_ERROR(Status)) {
      LoadedImageDevicePath = NULL;
    }

    DumpLoadedImage (Index, LoadedImage, DevicePath, LoadedImageDevicePath);

    Status = TestPointCheckNon3rdPartyImage (LoadedImage, DevicePath, LoadedImageDevicePath);
    if (EFI_ERROR(Status)) {
      ReturnStatus = Status;
      DEBUG ((DEBUG_ERROR, "3rd Party Image found - Index (%d)\n", Index));
      TestPointLibAppendErrorString (
        PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
        NULL,
        TEST_POINT_BYTE3_END_OF_DXE_NO_THIRD_PARTY_PCI_OPTION_ROM_ERROR_CODE \
          TEST_POINT_END_OF_DXE \
          TEST_POINT_BYTE3_END_OF_DXE_NO_THIRD_PARTY_PCI_OPTION_ROM_ERROR_STRING
        );
    }
  }

Done:

  if (HandleBuf != NULL) {
    FreePool (HandleBuf);
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckLoadedImage - Exit\n"));

  return ReturnStatus;
}
