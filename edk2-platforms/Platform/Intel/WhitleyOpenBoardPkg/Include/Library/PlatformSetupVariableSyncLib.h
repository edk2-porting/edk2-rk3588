/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PLATFORM_SETUP_VAR_SYNC
#define __PLATFORM_SETUP_VAR_SYNC
#include <Base.h>
#include "Uefi/UefiBaseType.h"

/**
  THis function can sync PC/OEM setup variable value to Rp setup variable before variable service is ready
    IN EFI_PEI_SERVICES           **PeiServices,
  IN OUT VOID* Header,  -   The input paramter should be   VARIABLE_STORE_HEADER  *VarStoreHeader.
                            Since we don't know whether SECURE_BOOT_ENABLE is used, we don't know to include which ***VariableFormat.h
                            So just use VOID* to pass to platform library
  IN BOOLEAN CreateHobDataForRpDefaults - whether need to create a hob for RP setup variable only,
                                           in normal boot case, we should set this to TRUE to make sure RP setup variable can always sync latest PC variable value



  @return The number of Unicode characters.

**/
EFI_STATUS SyncSetupVariable  (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN OUT VOID* Header,
  IN BOOLEAN CreateHobDataForRpDefaults
);


/*++
Description:

  This function finds the matched default data and create GUID hob only for RP variable .
  This is used to sync Pc variable to RP variable value

Arguments:

  DefaultId - Specifies the type of defaults to retrieve.
  BoardId   - Specifies the platform board of defaults to retrieve.

Returns:

  EFI_SUCCESS - The matched default data is found.
  EFI_NOT_FOUND - The matched default data is not found.
  EFI_OUT_OF_RESOURCES - No enough resource to create HOB.

--*/

EFI_STATUS
CreateRPVariableHob (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  );

#endif
