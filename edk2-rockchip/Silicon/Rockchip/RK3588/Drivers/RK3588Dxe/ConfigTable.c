/** @file
 *
 *  Copyright (c) 2020, Jeremy Linton
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/AcpiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "ConfigTable.h"

STATIC CONST EFI_GUID mAcpiTableFile = {
  0x7E374E25, 0x8E01, 0x4FEE, { 0x87, 0xf2, 0x39, 0x0C, 0x23, 0xC6, 0x06, 0xCD }
};

typedef struct {
  CHAR8 Name[4];
  UINTN PcdToken;
} AML_NAME_OP_REPLACE;

#define SSDT_PATTERN_LEN 5
#define AML_NAMEOP_8   0x0A
#define AML_NAMEOP_16  0x0B
#define AML_NAMEOP_32  0x0C
#define AML_NAMEOP_STR 0x0D
//
// Scan the given namespace table (DSDT/SSDT) for AML NameOps
// listed in the NameOpReplace structure. If one is found then
// update the value in the table from the specified Pcd
//
// This allows us to have conditionals in AML controlled
// by options in the BDS or detected during firmware bootstrap.
// We could extend this concept for strings/etc but due to len
// variations its probably easier to encode the strings
// in the ASL and pick the correct one based off a variable.
//
STATIC
VOID
AcpiUpdateSdtNameOps (
  EFI_ACPI_DESCRIPTION_HEADER  *AcpiTable,
  CONST AML_NAME_OP_REPLACE    *NameOpReplace
  )
{
  UINTN  Idx;
  UINTN  Index;
  CHAR8  Pattern[SSDT_PATTERN_LEN];
  UINTN  PcdVal;
  UINT8  *SdtPtr;
  UINT32 SdtSize;

  SdtSize = AcpiTable->Length;

  if (SdtSize > 0) {
    SdtPtr = (UINT8 *)AcpiTable;

    for (Idx = 0; NameOpReplace && NameOpReplace[Idx].PcdToken; Idx++) {
      //
      // Do a single NameOp variable replacement these are of the
      // form 08 XXXX SIZE VAL, where SIZE is 0A=byte, 0B=word, 0C=dword
      // and XXXX is the name and VAL is the value
      //
      Pattern[0] = 0x08;
      Pattern[1] = NameOpReplace[Idx].Name[0];
      Pattern[2] = NameOpReplace[Idx].Name[1];
      Pattern[3] = NameOpReplace[Idx].Name[2];
      Pattern[4] = NameOpReplace[Idx].Name[3];

      for (Index = 0; Index < (SdtSize - SSDT_PATTERN_LEN); Index++) {
        if (CompareMem (SdtPtr + Index, Pattern, SSDT_PATTERN_LEN) == 0) {
          PcdVal = LibPcdGet32 (NameOpReplace[Idx].PcdToken);
          switch (SdtPtr[Index + SSDT_PATTERN_LEN]) {
          case AML_NAMEOP_32:
            SdtPtr[Index + SSDT_PATTERN_LEN + 4] = (PcdVal >> 24) & 0xFF;
            SdtPtr[Index + SSDT_PATTERN_LEN + 3] = (PcdVal >> 16) & 0xFF;
            // Fallthrough
          case AML_NAMEOP_16:
            SdtPtr[Index + SSDT_PATTERN_LEN + 2] = (PcdVal >> 8) & 0xFF;
            // Fallthrough
          case AML_NAMEOP_8:
            SdtPtr[Index + SSDT_PATTERN_LEN + 1] = PcdVal & 0xFF;
            break;
          case 0:
          case 1:
            SdtPtr[Index + SSDT_PATTERN_LEN + 1] = !!PcdVal;
            break;
          case AML_NAMEOP_STR:
            //
            // If the string val is added to the NameOpReplace, we can
            // dynamically update things like _HID too as long as the
            // string length matches.
            //
            break;
          }
          break;
        }
      }
    }
  }
}

STATIC
BOOLEAN
AcpiVerifyUpdateTable (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader
  )
{
  BOOLEAN              Result;
  AML_NAME_OP_REPLACE  *NameOpReplace;

  Result = TRUE;
  NameOpReplace = NULL;

  switch (AcpiHeader->OemTableId) {
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '3', '4', 'L', '0'):
      Result = FixedPcdGetBool (PcdPcie30Supported)
                && PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED;
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '3', '2', 'L', '0'):
      Result = FALSE; // not supported yet
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '2', '1', 'L', '0'):
      Result = PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_PCIE;
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '2', '1', 'L', '1'):
      Result = PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_PCIE;
      break;
    case SIGNATURE_64 ('P', 'C', 'I', 'E', '2', '1', 'L', '2'):
      Result = PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_PCIE;
      break;
    case SIGNATURE_64 ('A', 'H', 'C', 'S', 'A', 'T', 'A', '0'):
      Result = PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_SATA;
      break;
    case SIGNATURE_64 ('A', 'H', 'C', 'S', 'A', 'T', 'A', '1'):
      Result = PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_SATA;
      break;
    case SIGNATURE_64 ('A', 'H', 'C', 'S', 'A', 'T', 'A', '2'):
      Result = PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_SATA;
      break;
    case SIGNATURE_64 ('U', 'S', 'B', '2', 'H', 'O', 'S', 'T'):
      Result = PcdGet32 (PcdAcpiUsb2State) == ACPI_USB2_STATE_ENABLED;
      break;
  }

  if (Result && NameOpReplace) {
    AcpiUpdateSdtNameOps (AcpiHeader, NameOpReplace);
  }

  return Result;
}

//
// Monitor the ACPI tables being installed and when
// a DSDT/SSDT is detected validate that we want to
// install it, and if so update any "NameOp" defined
// variables contained in the table from PCD values
//
STATIC
BOOLEAN
AcpiHandleDynamicNamespace (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader
  )
{
  switch (AcpiHeader->Signature) {
    case SIGNATURE_32 ('D', 'S', 'D', 'T'):
    case SIGNATURE_32 ('S', 'S', 'D', 'T'):
      return AcpiVerifyUpdateTable (AcpiHeader);
  }

  return TRUE;
}

VOID
EFIAPI
ApplyConfigTableVariables (
  VOID
  )
{
  if (PcdGet32 (PcdConfigTableMode) & CONFIG_TABLE_MODE_ACPI) {
    LocateAndInstallAcpiFromFvConditional (&mAcpiTableFile, &AcpiHandleDynamicNamespace);
  }
}

VOID
EFIAPI
SetupConfigTableVariables (
  VOID
  )
{
  UINTN      Size;
  UINT32     Var32;
  UINT8      Var8;
  EFI_STATUS Status;

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"ConfigTableMode",
                            &gRK3588DxeFormSetGuid,
                            NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdConfigTableMode, PcdGet32 (PcdConfigTableMode));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"AcpiUsb2State",
                            &gRK3588DxeFormSetGuid,
                            NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdAcpiUsb2State, FixedPcdGet32 (PcdAcpiUsb2StateDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT8);
  Status = gRT->GetVariable (L"FdtSupportOverrides",
                            &gRK3588DxeFormSetGuid,
                            NULL, &Size, &Var8);
  if (EFI_ERROR (Status)) {
    Status = PcdSet8S (PcdFdtSupportOverrides, PcdGet8 (PcdFdtSupportOverrides));
    ASSERT_EFI_ERROR (Status);
  }
}
