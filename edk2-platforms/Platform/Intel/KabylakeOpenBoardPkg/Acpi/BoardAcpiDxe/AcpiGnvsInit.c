/** @file
  Acpi Gnvs Init Library.

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <PchAccess.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/MpService.h>
#include <Protocol/SaGlobalNvsArea.h>

/**
  A protocol callback which updates MMIO Base and Length in SA GNVS area

  @param[in] Event    - The triggered event.
  @param[in] Context  - Context for this event.

**/
VOID
EFIAPI
UpdateSaGnvsForMmioResourceBaseLength (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                            Status;
  SYSTEM_AGENT_GLOBAL_NVS_AREA_PROTOCOL *SaGlobalNvsAreaProtocol;

  Status = gBS->LocateProtocol (&gSaGlobalNvsAreaProtocolGuid, NULL, (VOID **) &SaGlobalNvsAreaProtocol);
  if (Status != EFI_SUCCESS) {
    return;
  }
  gBS->CloseEvent (Event);

  //
  // Configure MMIO Base/Length. This logic is only valid for platforms that use PciHostBridgeLibSimple.
  //
  DEBUG ((DEBUG_INFO, "[BoardAcpiDxe] Update SA GNVS Area.\n"));
  SaGlobalNvsAreaProtocol->Area->Mmio32Base = PcdGet32 (PcdPciReservedMemBase);
  if (PcdGet32 (PcdPciReservedMemLimit) != 0) {
    SaGlobalNvsAreaProtocol->Area->Mmio32Length = PcdGet32 (PcdPciReservedMemLimit) - PcdGet32 (PcdPciReservedMemBase) + 1;
  } else {
    SaGlobalNvsAreaProtocol->Area->Mmio32Length = ((UINT32) PcdGet64 (PcdPciExpressBaseAddress)) - PcdGet32 (PcdPciReservedMemBase);
  }
  if (PcdGet64 (PcdPciReservedMemAbove4GBLimit) > PcdGet64 (PcdPciReservedMemAbove4GBBase)) {
    SaGlobalNvsAreaProtocol->Area->Mmio64Base   = PcdGet64 (PcdPciReservedMemAbove4GBBase);
    SaGlobalNvsAreaProtocol->Area->Mmio64Length = PcdGet64 (PcdPciReservedMemAbove4GBLimit) - PcdGet64 (PcdPciReservedMemAbove4GBBase) + 1;
  }
}

/**
@brief
  Global NVS initialize.

  @param[in] GlobalNvs         - Pointer of Global NVS area

  @retval EFI_SUCCESS          - Allocate Global NVS completed.
  @retval EFI_OUT_OF_RESOURCES - Failed to allocate required page for GNVS.
**/
EFI_STATUS
EFIAPI
AcpiGnvsInit (
  IN OUT VOID                   **GlobalNvs
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GNVS;
  EFI_MP_SERVICES_PROTOCOL      *MpService;
  UINTN                         NumberOfCPUs;
  UINTN                         NumberOfEnabledCPUs;
  VOID                          *SaGlobalNvsRegistration;

  Pages = EFI_SIZE_TO_PAGES (sizeof (EFI_GLOBAL_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Locate the MP services protocol
  // Find the MP Protocol. This is an MP platform, so MP protocol must be there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpService
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
              MpService,
              &NumberOfCPUs,
              &NumberOfEnabledCPUs
              );

  *GlobalNvs = (VOID *) (UINTN) Address;
  SetMem (*GlobalNvs, sizeof (EFI_GLOBAL_NVS_AREA), 0);

  //
  // GNVS default value init here...
  //
  GNVS = (EFI_GLOBAL_NVS_AREA_PROTOCOL *) &Address;

  GNVS->Area->ThreadCount = (UINT8)NumberOfEnabledCPUs;

  //
  // Miscellaneous
  //
  GNVS->Area->PL1LimitCS = 0;
  GNVS->Area->PL1LimitCSValue = 4500;

  //
  // Update SA GNVS with MMIO Base/Length
  //
  EfiCreateProtocolNotifyEvent (
    &gSaGlobalNvsAreaProtocolGuid,
    TPL_CALLBACK,
    UpdateSaGnvsForMmioResourceBaseLength,
    NULL,
    &SaGlobalNvsRegistration
    );

  return EFI_SUCCESS;
}
