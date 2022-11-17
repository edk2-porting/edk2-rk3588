/*++

  Copyright (c) 2004  - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PLAT_OVER_MNGR_H_
#define _PLAT_OVER_MNGR_H_

#include <PiDxe.h>

#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/PciIo.h>
#include <Protocol/BusSpecificDriverOverride.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PlatformDriverOverride.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/VariableFormat.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HiiLib.h>
#include <Library/BiosIdLib.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>
#include <IndustryStandard/Pci22.h>

#include "Guid/SetupVariable.h"
#include "Guid/OsSelection.h"

#include <CpuType.h>
#include <Guid/PlatformCpuInfo.h>
#include <Protocol/SimpleTextIn.h>

typedef UINT16  STRING_REF;

extern EFI_HII_HANDLE   mHiiHandle;

VOID
SwapEntries (
  IN  CHAR8 *Data
  );

VOID
AsciiToUnicode (
  IN    CHAR8     *AsciiString,
  IN    CHAR16    *UnicodeString
  );

VOID
EFIAPI
SetupInfo (
  );


extern EFI_HANDLE mImageHandle;

#endif
