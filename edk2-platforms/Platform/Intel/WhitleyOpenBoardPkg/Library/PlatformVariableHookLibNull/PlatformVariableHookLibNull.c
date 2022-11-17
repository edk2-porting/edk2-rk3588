/** @file

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi.h>
#include <Library/IoLib.h>




/*++
Description:

  This function is a hook for PlatformVariableInitPeiEntry

--*/
VOID PlatformVariableHookForEntry(
   VOID
){


}


/*++
Description:

  This function allow platform to generate variable hob base on different event.

Arguments:
  IN VOID  *Interface                  -point to EFI_PEI_READ_ONLY_VARIABLE2_PPI
   IN OUT   UINT8 *phobdata,        -pont to hob data
   IN OUT   UINT16 *pDefaultId      -pointer to defautlID

Returns:
  TRUE:platform have its own variable hob that need be createn
  FALSE:platform don;t need to create variable hob in this case


--*/
BOOLEAN PlatformVariableHookForHobGeneration(
   IN VOID  *Interface,
   IN OUT   UINT8 *phobdata,
   IN OUT   UINT16 *pDefaultId
){



  return FALSE;
}
