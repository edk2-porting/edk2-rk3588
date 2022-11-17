/** @file
  Copyright (c) 2017, Linaro Limited. All rights reserved.
  Copyright (c) 2017, Marvell International Ltd. and its affiliates

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Guid/EventGroup.h>

#include <IndustryStandard/MvSmc.h>

#include <Library/ArmadaIcuLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/DebugLib.h>
#include <Library/MppLib.h>
#include <Library/MvComPhyLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UtmiPhyLib.h>

STATIC EFI_EVENT mArmada7k8kExitBootServicesEvent;

/**
  Disable extra EL3 handling of the PMU interrupts for DT world.

  @param[in]   Event                  Event structure
  @param[in]   Context                Notification context

**/
STATIC
VOID
EFIAPI
Armada7k8kExitBootServicesHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  ARM_SMC_ARGS SmcRegs = {0};

  SmcRegs.Arg0 = MV_SMC_ID_PMU_IRQ_DISABLE;
  ArmCallSmc (&SmcRegs);
}

/**
  Check if we boot with DT and trigger EBS event in such case.

  @param[in]   Event                  Event structure
  @param[in]   Context                Notification context

**/
STATIC
VOID
EFIAPI
Armada7k8kOnReadyToBootHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS    Status;
  VOID          *Interface;

  // Ensure the event will be triggered only once
  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gEdkiiPlatformHasAcpiGuid,
                  NULL,
                  (VOID **)&Interface);
  if (!EFI_ERROR (Status)) {
    /* ACPI is enabled, so leave the current settings intact. */
    return;
  }

  //
  // In case DT is selected, create EBS event for disabling
  // extra EL3 handling of the PMU interrupts in EL3.
  //
  Status = gBS->CreateEvent (EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_NOTIFY,
                  Armada7k8kExitBootServicesHandler,
                  NULL,
                  &mArmada7k8kExitBootServicesEvent);
  ASSERT_EFI_ERROR (Status);
}

EFI_STATUS
EFIAPI
ArmadaPlatInitDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  ARM_SMC_ARGS  SmcRegs = {0};
  EFI_STATUS    Status;
  EFI_EVENT     Event;

  DEBUG ((DEBUG_ERROR,
    "\n%a %a Init\n\n",
    (CHAR8 *)PcdGetPtr (PcdProductManufacturer),
    (CHAR8 *)PcdGetPtr (PcdProductPlatformName)));

  Status = gBS->InstallProtocolInterface (&ImageHandle,
                  &gMarvellPlatformInitCompleteProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL);
  ASSERT_EFI_ERROR (Status);

  MvComPhyInit ();
  UtmiPhyInit ();
  MppInitialize ();
  ArmadaIcuInitialize ();
  Status = ArmadaBoardInit ();
  ASSERT_EFI_ERROR (Status);

  /*
   * Enable EL3 PMU interrupt handler and
   * register the ReadyToBoot event.
   */
  SmcRegs.Arg0 = MV_SMC_ID_PMU_IRQ_ENABLE;
  ArmCallSmc (&SmcRegs);

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  Armada7k8kOnReadyToBootHandler,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
