/*
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
*/
#ifndef __RDK_BOOT_MANAGER_LIB_H__
#define __RDK_BOOT_MANAGER_LIB_H__

#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/ShellLib.h>
#include <Library/DevicePathLib.h>
#include <Library/FileHandleLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/DiskIo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/LoadFile.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/AndroidFastbootPlatform.h>
#include <Guid/ImageAuthentication.h>
#include <Guid/AuthenticatedVariableFormat.h>
#include <HttpBootDxe/HttpBootDxe.h>
#include <Include/Guid/AuthenticatedVariableFormat.h>

extern
EFI_STATUS
PartitionRead (
  IN CHAR8  *PartitionName,
  IN VOID   *Image,
  IN UINTN  Size
  );

extern
EFI_STATUS
PartitionWrite (
  IN CHAR8  *PartitionName,
  IN VOID   *Image,
  IN UINTN  Size
  );

extern
EFI_STATUS
GetRdkVariable (
  IN  CONST CHAR16  *Name,
  OUT CONST CHAR16  **Value
  );

extern
EFI_STATUS
RdkSecureBoot (
  EFI_HANDLE		ImageHandle,
  EFI_BOOT_SERVICES     *BootServices
  );

extern
EFI_STATUS
RdkReadFile (
  IN  	CONST CHAR16 	*Path,
  IN OUT  VOID          **BufferPtr,
  OUT  	UINTN           *FileSize
  );

extern
EFI_STATUS
RdkWriteFile (
  IN  	CONST CHAR16    *Path,
  IN OUT  VOID          **BufferPtr,
  OUT  	UINTN           *FileSize
  );

extern
EFI_STATUS
GetFileHandler (
  OUT 	EFI_FILE_HANDLE *FileHandle,
  IN 	CONST CHAR16    *Path,
  IN  	UINT64          OpenMode
  );

extern
EFI_STATUS
RdkHttpBoot (
  VOID
  );

#endif /* __RDK_BOOT_MANAGER_LIB_H__ */
