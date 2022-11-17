/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Tcg2Protocol.h>
#include <IndustryStandard/Tpm2Acpi.h>

VOID *
TestPointGetAcpi (
  IN UINT32  Signature
  );

EFI_STATUS
EFIAPI
TestPointCheckTcgTrustedBoot (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_TCG2_PROTOCOL                *Tcg2;
  EFI_TCG2_BOOT_SERVICE_CAPABILITY ProtocolCapability;
  VOID                             *Acpi;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckTcgTrustedBoot - Enter\n"));

  Status = gBS->LocateProtocol (&gEfiTcg2ProtocolGuid, NULL, (VOID **)&Tcg2);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Tcg2 Protocol - %r\n", Status));
    goto Done;
  }

  ZeroMem ((VOID *) &ProtocolCapability, sizeof (ProtocolCapability));
  ProtocolCapability.Size = (UINT8) sizeof (ProtocolCapability);
  Status = Tcg2->GetCapability (Tcg2, &ProtocolCapability);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Tcg2->GetCapability - %r\n", Status));
    goto Done;
  }

  DEBUG ((DEBUG_INFO, "Capability.Size     - 0x%x\n", ProtocolCapability.Size));
  DEBUG ((DEBUG_INFO, "StructureVersion    - %x.%x\n", ProtocolCapability.StructureVersion.Major, ProtocolCapability.StructureVersion.Minor));
  DEBUG ((DEBUG_INFO, "ProtocolVersion     - %x.%x\n", ProtocolCapability.ProtocolVersion.Major, ProtocolCapability.ProtocolVersion.Minor));
  DEBUG ((DEBUG_INFO, "HashAlgorithmBitmap - 0x%08x\n", ProtocolCapability.HashAlgorithmBitmap));
  DEBUG ((DEBUG_INFO, "SupportedEventLogs  - 0x%08x\n", ProtocolCapability.SupportedEventLogs));
  DEBUG ((DEBUG_INFO, "TPMPresentFlag      - %x\n", ProtocolCapability.TPMPresentFlag));
  DEBUG ((DEBUG_INFO, "MaxCommandSize      - 0x%04x\n", ProtocolCapability.MaxCommandSize));
  DEBUG ((DEBUG_INFO, "MaxResponseSize     - 0x%04x\n", ProtocolCapability.MaxResponseSize));
  DEBUG ((DEBUG_INFO, "ManufacturerID      - 0x%08x\n", ProtocolCapability.ManufacturerID));
  if ((ProtocolCapability.StructureVersion.Major > 1) ||
      (ProtocolCapability.StructureVersion.Minor > 0)) {
    DEBUG ((DEBUG_INFO, "NumberOfPCRBanks    - 0x%08x\n", ProtocolCapability.NumberOfPCRBanks));
    DEBUG ((DEBUG_INFO, "ActivePcrBanks      - 0x%08x\n", ProtocolCapability.ActivePcrBanks));
  }

  if (!ProtocolCapability.TPMPresentFlag) {
    DEBUG ((DEBUG_ERROR, "Tcg2 TPMPresentFlag FALSE\n"));
    Status = EFI_NOT_FOUND;
  }

  Acpi = TestPointGetAcpi (EFI_ACPI_5_0_TRUSTED_COMPUTING_PLATFORM_2_TABLE_SIGNATURE);
  if (Acpi == NULL) {
    DEBUG ((DEBUG_ERROR, "Tcg2 TPM2 table not found\n"));
    Status = EFI_NOT_FOUND;
  }

Done:
  if (EFI_ERROR(Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE5_READY_TO_BOOT_TCG_TRUSTED_BOOT_ENABLED_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE5_READY_TO_BOOT_TCG_TRUSTED_BOOT_ENABLED_ERROR_STRING
      );
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckTcgTrustedBoot - Exit\n"));
  return Status;
}
