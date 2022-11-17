/** @file
  Header file for UserAuthentication2Dxe.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _USER_AUTHENTICATION_DXE_H_
#define _USER_AUTHENTICATION_DXE_H_


#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/HiiConfigAccess.h>

#include <Guid/MdeModuleHii.h>
#include <Guid/HiiPlatformSetupFormset.h>
#include <Guid/UserAuthentication.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UserPasswordLib.h>

#include "UserAuthenticationDxeFormset.h"

extern UINT8  UserAuthenticationDxeVfrBin[];
extern UINT8  UserAuthentication2DxeStrings[];

typedef struct {
  EFI_HII_CONFIG_ACCESS_PROTOCOL       ConfigAccess;
  EFI_HANDLE                           DriverHandle;
  EFI_HII_HANDLE                       HiiHandle;
  UINT8                                PasswordState;
  CHAR16                               OldPassword[PASSWORD_MAX_SIZE];
} USER_AUTHENTICATION_PRIVATE_DATA;

#pragma pack(1)
///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

#endif
