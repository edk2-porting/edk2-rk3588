/** @file
  This driver module adds SATA controller support.

  Copyright 2017-2018, 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

 **/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define SATA_PPCFG             0xA8
#define SATA_PTC               0xC8
#define SATA_PAXIC             0xC0

#define PORT_PHYSICAL          0xA003FFFE
#define PORT_TRANSPORT         0x08000025
#define PORT_RXWM              0x08000029
#define ENABLE_NONZERO_4MB_PRD 0x10000000

/**
  This function gets registered as a callback to perform USB controller intialization

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context.

**/
STATIC
VOID
EFIAPI
SataEndOfDxeCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS               Status;
  UINT32                   NumSataController;
  UINT32                   Index;
  UINT32                   Data;
  UINTN                    ControllerAddr;

  NumSataController = PcdGet32 (PcdNumSataController);

  for (Index = 0; Index < NumSataController; Index++) {
    ControllerAddr = PcdGet64 (PcdSataBaseAddr) +
                     (Index * PcdGet32 (PcdSataSize));

    //
    // configuring Physical Control Layer parameters for Port 0
    //
    MmioWrite32 ((UINTN)(ControllerAddr + SATA_PPCFG), PORT_PHYSICAL);

    //
    // This register controls the configuration of the
    // Transport Layer for  Port 0
    // Errata Description : The default Rx watermark value may be insufficient
    // for some hard drives and result in a false CRC or internal errors.
    // Workaround: Change PTC[RXWM] field at offset 0xC8 to 0x29. Do not change
    // the other reserved fields of the register.
    //

    Data = MmioRead32 ((UINTN)(ControllerAddr + SATA_PTC));
    if (PcdGetBool (PcdSataErratumA009185)) {
      Data |= PORT_RXWM;
    } else {
      Data |= PORT_TRANSPORT;
    }
    MmioWrite32 ((UINTN)(ControllerAddr + SATA_PTC), Data);

    //
    // Enable Non-Zero 4 MB PRD entries.
    //
    MmioOr32 ((UINTN)(ControllerAddr + SATA_PAXIC), ENABLE_NONZERO_4MB_PRD);

    Status = RegisterNonDiscoverableMmioDevice (
               NonDiscoverableDeviceTypeAhci,
               NonDiscoverableDeviceDmaTypeNonCoherent,
               NULL,
               NULL,
               1,
               ControllerAddr, PcdGet32 (PcdSataSize)
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to register SATA device 0x%x, error 0x%r \n",
        ControllerAddr, Status));
    }
  }
}

/**
  The Entry Point of module. It follows the standard UEFI driver model.

  @param[in] ImageHandle       The firmware allocated handle for the EFI image.
  @param[in] SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS          The entry point is executed successfully.
  @retval Other                Some error occurs when executing this entry point

**/
EFI_STATUS
EFIAPI
InitializeSataController (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS               Status;
  EFI_EVENT                EndOfDxeEvent;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SataEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );

  return Status;
}
