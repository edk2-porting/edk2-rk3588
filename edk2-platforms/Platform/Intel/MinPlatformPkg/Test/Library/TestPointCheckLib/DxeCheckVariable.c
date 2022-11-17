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
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/VarCheck.h>

#define INIT_NAME_BUFFER_SIZE  128

CHAR8 *mAttributeShortName[] = {
  "NV",
  "BS",
  "RT",
  "HR",
  "AU",
  "AT",
  "AP",
};

VOID
DumpVarAttrName (
  IN UINT32            Attributes
  )
{
  UINTN   Index;
  BOOLEAN IsFirst;

  IsFirst = TRUE;
  DEBUG ((DEBUG_INFO, "("));
  for (Index = 0; Index < sizeof(mAttributeShortName)/sizeof(mAttributeShortName[0]); Index++) {
    if ((Attributes & (1 << Index)) != 0) {
      if (!IsFirst) {
        DEBUG ((DEBUG_INFO, "|"));
      }
      DEBUG ((DEBUG_INFO, mAttributeShortName[Index]));
      if (IsFirst) {
        IsFirst = FALSE;
      }
    }
  }
  DEBUG ((DEBUG_INFO, ")"));
}

VOID
DumpVarProperty (
  IN VAR_CHECK_VARIABLE_PROPERTY   *VariableProperty
  )
{
  if ((VariableProperty->Property & VAR_CHECK_VARIABLE_PROPERTY_READ_ONLY) != 0) {
    DEBUG ((DEBUG_INFO, " (RO)"));
  }
}

VOID
TestPointDumpVariable (
  VOID
  )
{
  CHAR16            *VariableName;
  UINTN             NameSize;
  UINTN             NameBufferSize;
  EFI_STATUS        Status;
  EFI_GUID          Guid;
  UINTN             VarSize;
  UINT32            Attributes;
  VOID              *VarData;
  EDKII_VAR_CHECK_PROTOCOL      *VarCheck;
  VAR_CHECK_VARIABLE_PROPERTY   VariableProperty;
  
  DEBUG ((DEBUG_INFO, "==== TestPointDumpVariable - Enter\n"));

  Status = gBS->LocateProtocol (&gEdkiiVarCheckProtocolGuid, NULL, (VOID **)&VarCheck);
  DEBUG ((DEBUG_INFO, "VarCheck - %r\n", Status));
  
  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "Variable List:\n"));
  NameBufferSize = INIT_NAME_BUFFER_SIZE;
  VariableName = AllocateZeroPool(NameBufferSize);
  if (VariableName == NULL) {
    goto Done ;
  }
  *VariableName = CHAR_NULL;

  while (!EFI_ERROR(Status)) {
    //
    // Get Name/Guid
    //
    NameSize = NameBufferSize;
    Status = gRT->GetNextVariableName(&NameSize, VariableName, &Guid);
    if (Status == EFI_NOT_FOUND){
      Status = EFI_SUCCESS;
      break;
    } else if (Status == EFI_BUFFER_TOO_SMALL) {
      NameBufferSize = NameSize > NameBufferSize * 2 ? NameSize : NameBufferSize * 2;
      if (VariableName != NULL) {
        FreePool (VariableName);
      }
      VariableName = AllocateZeroPool(NameBufferSize);
      if (VariableName == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        break;
      }
      NameSize = NameBufferSize;
      Status = gRT->GetNextVariableName(&NameSize, VariableName, &Guid);
    }
    
    if (EFI_ERROR(Status)) {
      break;
    }

    //
    // Get Attributes/Size
    //
    VarSize = 0;
    Attributes = 0;
    Status = gRT->GetVariable(VariableName, &Guid, &Attributes, &VarSize, NULL);
    if (Status == EFI_BUFFER_TOO_SMALL){
      VarData = AllocateZeroPool (VarSize);
      if (VarData == NULL) {
        goto Done;
      }
      Status = gRT->GetVariable(VariableName, &Guid, &Attributes, &VarSize, VarData);
      if (!EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO,
          "  Name=L\"%s\" Guid=%g Size=0x%x Attr=0x%x ",
          VariableName,
          &Guid,
          VarSize,
          Attributes
          ));
        DumpVarAttrName (Attributes);

        if (VarCheck != NULL) {
          Status = VarCheck->VariablePropertyGet (VariableName, &Guid, &VariableProperty);
          if (!EFI_ERROR(Status)) {
            DumpVarProperty (&VariableProperty);
          }
        }

        DEBUG ((DEBUG_INFO, "\n"));
        FreePool (VarData);
        Status = EFI_SUCCESS;
      }
    }
  }

  if (VariableName != NULL) {
    FreePool (VariableName);
  }

Done:
  DEBUG ((DEBUG_INFO, "==== TestPointDumpVariable - Exit\n"));

  return ;
}