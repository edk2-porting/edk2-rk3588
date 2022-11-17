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
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>

VOID
TestPointDumpDevicePath (
  VOID
  )
{
  UINTN                             Index;
  EFI_HANDLE                        *HandleBuf;
  UINTN                             HandleCount;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  EFI_STATUS                        Status;
  CHAR16                            *Str;
  
  DEBUG ((DEBUG_INFO, "==== TestPointDumpDevicePath - Enter\n"));
  HandleBuf = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuf
                  );
  if (EFI_ERROR (Status)) {
    goto Done ;
  }

  DEBUG ((DEBUG_INFO, "DeviceList:\n"));
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                  HandleBuf[Index],
                  &gEfiDevicePathProtocolGuid,
                  (VOID**)&DevicePath
                 );
    if (EFI_ERROR (Status)) {
      continue;
    }
    Str = ConvertDevicePathToText(DevicePath, TRUE, TRUE);
    DEBUG ((DEBUG_INFO, "  %s\n", Str));
    if (Str != NULL) {
      FreePool (Str);
    }
  }

Done:
  if (HandleBuf != NULL) {
    FreePool (HandleBuf);
  }

  DEBUG ((DEBUG_INFO, "==== TestPointDumpDevicePath - Exit\n"));

  return ;
}

/**
  The DevicePathNode is a single instance.
  The DevicePathNodeSize must not include END_DEVICE_PATH.
**/
BOOLEAN
IsDevicePathNodeExist (
  IN EFI_DEVICE_PATH_PROTOCOL          *DevicePathNode,
  IN UINTN                             DevicePathNodeSize
  )
{
  UINTN                             Index;
  EFI_HANDLE                        *HandleBuf;
  UINTN                             HandleCount;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  EFI_STATUS                        Status;
  BOOLEAN                           Result;
  UINTN                             DevicePathSize;

  if (DevicePathType (DevicePathNode) == HARDWARE_DEVICE_PATH ||
      DevicePathType (DevicePathNode) == ACPI_DEVICE_PATH) {
    Result = FALSE;
    HandleBuf = NULL;
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiDevicePathProtocolGuid,
                    NULL,
                    &HandleCount,
                    &HandleBuf
                    );
    if (EFI_ERROR (Status)) {
      goto Done ;
    }

    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->HandleProtocol (
                    HandleBuf[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID**)&DevicePath
                   );
      if (EFI_ERROR (Status)) {
        continue;
      }
      DevicePathSize = GetDevicePathSize(DevicePath);
      if (DevicePathSize != DevicePathNodeSize + sizeof(EFI_DEVICE_PATH_PROTOCOL)) {
        continue;
      }
      if (CompareMem (DevicePath, DevicePathNode, DevicePathNodeSize) == 0) {
        Result = TRUE;
        break;
      }
    }

  Done:
    if (HandleBuf != NULL) {
      FreePool (HandleBuf);
    }
    return Result;
  }

  if (DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH) {
    return TRUE;
  }
  
  if (DevicePathType (DevicePathNode) == MEDIA_DEVICE_PATH) {
    return TRUE;
  }
  
  if (DevicePathType (DevicePathNode) == BBS_DEVICE_PATH) {
    return TRUE;
  }
  
  return TRUE;
}

/**
  The DevicePathTarget might be multiple instance.
  The DevicePathTarget must be ended with END_ENTIRE_DEVICE_PATH_SUBTYPE.
**/
BOOLEAN
IsDevicePathExist (
  IN EFI_DEVICE_PATH_PROTOCOL          *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL          *TmpDevicePath;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePathNode;
  BOOLEAN                           Result;

  TmpDevicePath = DevicePath;
  DevicePathNode = DevicePath;
  while (!IsDevicePathEnd (TmpDevicePath)) {
    if (IsDevicePathEndInstance (TmpDevicePath)) {
      //
      // DevicePath is a multi-instance device path
      //
      Result = IsDevicePathNodeExist (DevicePathNode, (UINTN)TmpDevicePath - (UINTN)DevicePathNode);
      if (!Result) {
        return FALSE;
      }
      DevicePathNode = NextDevicePathNode (TmpDevicePath);
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }
  Result = IsDevicePathNodeExist (DevicePathNode, (UINTN)TmpDevicePath - (UINTN)DevicePathNode);
  if (!Result) {
    return FALSE;
  }

  return TRUE;
}
