/** @file
  PCH Reset Protocol

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_RESET_H_
#define _PCH_RESET_H_

#include <Ppi/PchReset.h>

#define EFI_CAPSULE_VARIABLE_NAME           L"CapsuleUpdateData"
extern EFI_GUID                             gEfiCapsuleVendorGuid;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                             gPchResetProtocolGuid;
extern EFI_GUID                             gPchResetCallbackProtocolGuid;
extern EFI_GUID                             gPchPowerCycleResetGuid;
extern EFI_GUID                             gPchGlobalResetGuid;
extern EFI_GUID                             gPchGlobalResetWithEcGuid;
//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_RESET_PROTOCOL     PCH_RESET_PROTOCOL;

typedef        PCH_RESET_CALLBACK_PPI  PCH_RESET_CALLBACK_PROTOCOL;

//
// Related Definitions
//
//
// PCH Platform Specific ResetData
//
#define PCH_POWER_CYCLE_RESET_GUID \
  { \
    0x8d8ee25b, 0x66dd, 0x4ed8, { 0x8a, 0xbd, 0x14, 0x16, 0xe8, 0x8e, 0x1d, 0x24 } \
  }

#define PCH_GLOBAL_RESET_GUID \
  { \
    0x9db31b4c, 0xf5ef, 0x48bb, { 0x94, 0x2b, 0x18, 0x1f, 0x7e, 0x3a, 0x3e, 0x40 } \
  }

#define PCH_GLOBAL_RESET_WITH_EC_GUID \
  { \
    0xd22e6b72, 0x53cd, 0x4158, { 0x83, 0x3f, 0x6f, 0xd8, 0x7e, 0xbe, 0xa9, 0x93 } \
  }

#define PCH_PLATFORM_SPECIFIC_RESET_STRING L"PCH_RESET"
#define PCH_RESET_DATA_STRING_MAX_LENGTH   sizeof (PCH_PLATFORM_SPECIFIC_RESET_STRING)

typedef struct _RESET_DATA {
  CHAR16   Description[PCH_RESET_DATA_STRING_MAX_LENGTH];
  EFI_GUID Guid;
} PCH_RESET_DATA;


//
// Member functions
//
/**
  Execute Pch Reset from the Host controller.

  @param[in] This                 Pointer to the PCH_RESET_PROTOCOL instance.
  @param[in] ResetType            UEFI defined reset type.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET) (
  IN PCH_RESET_PROTOCOL *This,
  IN PCH_RESET_TYPE     ResetType,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  );

/**
  Retrieve PCH platform specific ResetData

  @param[in]  Guid      PCH platform specific reset GUID.
  @param[out] DataSize  The size of ResetData in bytes.

  @retval ResetData     A platform specific reset that the exact type of
                        the reset is defined by the EFI_GUID that follows
                        the Null-terminated Unicode string.
  @retval NULL          If Guid is not defined in PCH platform specific reset.
**/
typedef
VOID *
(EFIAPI *PCH_RESET_GET_RESET_DATA) (
  IN  EFI_GUID  *Guid,
  OUT UINTN     *DataSize
  );

/**
  Interface structure to execute Pch Reset from the Host controller.
**/
struct _PCH_RESET_PROTOCOL {
  PCH_RESET                Reset;
  PCH_RESET_GET_RESET_DATA GetResetData;
};

#endif
