/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/NonDiscoverableInitLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/BoardDesc.h>
#include <Protocol/EmbeddedExternalDevice.h>

//
// NonDiscoverable devices registration
//
STATIC
EFI_STATUS
NonDiscoverableInitXhci (
  IN MV_BOARD_XHCI_DESC *Desc
  )
{
  EFI_STATUS Status;
  UINT8 i;

  for (i = 0; i < Desc->XhciDevCount; i++) {
    Status = RegisterNonDiscoverableMmioDevice (
                     NonDiscoverableDeviceTypeXhci,
                     Desc[i].SoC->XhciDmaType,
                     NonDiscoverableDeviceInitializerGet (
                       NonDiscoverableDeviceTypeXhci,
                       i),
                     NULL,
                     1,
                     Desc[i].SoC->XhciBaseAddress,
                     Desc[i].SoC->XhciMemSize
                   );

    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "NonDiscoverable: Cannot install Xhci device %d\n", i));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
NonDiscoverableInitAhci (
  IN MV_BOARD_AHCI_DESC *Desc
  )
{
  EFI_STATUS Status;
  UINT8 i;

  for (i = 0; i < Desc->AhciDevCount; i++) {
    Status = RegisterNonDiscoverableMmioDevice (
                     NonDiscoverableDeviceTypeAhci,
                     Desc[i].SoC->AhciDmaType,
                     NonDiscoverableDeviceInitializerGet (
                       NonDiscoverableDeviceTypeAhci,
                       i),
                     NULL,
                     1,
                     Desc[i].SoC->AhciBaseAddress,
                     Desc[i].SoC->AhciMemSize
                   );

    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "NonDiscoverable: Cannot install Ahci device %d\n", i));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
NonDiscoverableInitSdhci (
  IN MV_BOARD_SDMMC_DESC *Desc
  )
{
  EFI_STATUS Status;
  UINT8 i;

  for (i = 0; i < Desc->SdMmcDevCount; i++) {
    Status = RegisterNonDiscoverableMmioDevice (
                     NonDiscoverableDeviceTypeSdhci,
                     Desc[i].SoC->SdMmcDmaType,
                     NonDiscoverableDeviceInitializerGet (
                       NonDiscoverableDeviceTypeSdhci,
                       i),
                     NULL,
                     1,
                     Desc[i].SoC->SdMmcBaseAddress,
                     Desc[i].SoC->SdMmcMemSize
                   );

    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "NonDiscoverable: Cannot install Sdhci device %d\n", i));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

//
// Entry point
//
EFI_STATUS
EFIAPI
NonDiscoverableEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  MARVELL_BOARD_DESC_PROTOCOL *BoardDescProtocol;
  MV_BOARD_SDMMC_DESC *SdMmcBoardDesc;
  MV_BOARD_AHCI_DESC *AhciBoardDesc;
  MV_BOARD_XHCI_DESC *XhciBoardDesc;
  EFI_STATUS Status;

  /* Obtain list of available controllers */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&BoardDescProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Xhci */
  Status = BoardDescProtocol->BoardDescXhciGet (BoardDescProtocol,
                                &XhciBoardDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = NonDiscoverableInitXhci (XhciBoardDesc);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  BoardDescProtocol->BoardDescFree (XhciBoardDesc);

  /* Ahci */
  Status = BoardDescProtocol->BoardDescAhciGet (BoardDescProtocol,
                                &AhciBoardDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = NonDiscoverableInitAhci (AhciBoardDesc);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  BoardDescProtocol->BoardDescFree (AhciBoardDesc);

  /* SdMmc */
  Status = BoardDescProtocol->BoardDescSdMmcGet (BoardDescProtocol,
                                &SdMmcBoardDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = NonDiscoverableInitSdhci (SdMmcBoardDesc);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
