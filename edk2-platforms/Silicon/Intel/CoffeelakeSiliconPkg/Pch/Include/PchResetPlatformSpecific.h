/** @file
  PCH Reset Platform Specific definitions.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_RESET_PLATFORM_SPECIFIC_H_
#define _PCH_RESET_PLATFORM_SPECIFIC_H_

#define PCH_PLATFORM_SPECIFIC_RESET_STRING   L"PCH_RESET"
#define PCH_RESET_DATA_STRING_MAX_LENGTH     (sizeof (PCH_PLATFORM_SPECIFIC_RESET_STRING) / sizeof (UINT16))

extern EFI_GUID gPchGlobalResetGuid;

typedef struct _RESET_DATA {
  CHAR16   Description[PCH_RESET_DATA_STRING_MAX_LENGTH];
  EFI_GUID Guid;
} PCH_RESET_DATA;

#endif // _PCH_RESET_PLATFORM_SPECIFIC_H_

