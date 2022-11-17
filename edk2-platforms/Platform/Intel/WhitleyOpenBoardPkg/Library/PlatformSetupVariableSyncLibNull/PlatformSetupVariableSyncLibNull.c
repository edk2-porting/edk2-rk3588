/** @file

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PlatformSetupVariableSyncLib.h>


/*++
    Description:

      This function will parse the variable hob and find three vairables:
      RP variable
      PC common variable
      PC generation variable
      This is used to sync Pc variable to RP variable value

    Arguments:
      PeiServices - PeiServices
      Header -  VARIABLE_STORE_HEADER
      CreateHobDataForRpDefaults - will create a hob for RP defaults,
                                   this is used in normal post case,
                                   cannot be used in specicfic hob event


    Returns:

      EFI_SUCCESS -  Sync to RP variable Success
      Other -Sync to RP variable  Failure


    --*/

EFI_STATUS SyncSetupVariable  (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN OUT VOID* Header,
  IN BOOLEAN CreateHobDataForRpDefaults
)
{
   EFI_STATUS Status = EFI_SUCCESS;



   return Status;

}



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
  ){
   EFI_STATUS Status = EFI_SUCCESS;

   return Status;

  }
