/** @file
  Aspire VN7-572G Board Initialization DXE library

  Copyright (c) 2021, Baruch Binyamin Doron
  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BoardInitLib.h>
#include <Library/DebugLib.h>
#include <Library/EcLib.h>
#include <Library/BoardEcLib.h>

/**
  Update the EC's clock?

**/
VOID
EcSendTime (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_TIME    EfiTime;
  // Time could be negative (before 2016)
  INTN        EcTime;
  UINT8       EcTimeByte;
  INTN        Index;
  UINT8       EcResponse;

  Status = gRT->GetTime (&EfiTime, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to retrieve current time\n"));
    return;
  }

  // Time since year of release. Note that "century" is ignored.
  EcTime = ((EfiTime.Year << 26) + (EfiTime.Month << 22) + (EfiTime.Day << 17)
         + (EfiTime.Hour << 12) + (EfiTime.Minute << 6) + (EfiTime.Second)
         /* 16 years */
         - 0x40000000);

  DEBUG ((DEBUG_INFO, "EC: reporting present time 0x%x\n", EcTime));
  SendEcCommand (0xE0);
  for (Index = 0; Index < 4; Index++) {
    // Shift bytes
    EcTimeByte = (UINT8) (EcTime >> (Index * 8));
    DEBUG ((DEBUG_INFO, "EC: Sending 0x%x (iteration %d)\n", EcTimeByte, Index));
    SendEcData (EcTimeByte);
  }

  Status = ReceiveEcData (&EcResponse);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EC: response 0x%x\n", EcResponse));
  }
}

/**
  Configure EC

**/
VOID
EcRequestsTime (
  VOID
  )
{
  UINT8           Dat;

  /* This is executed as protocol notify in vendor's RtKbcDriver when *CommonService
   * protocol is installed. Effectively, this code could execute from the entrypoint */
  EcCmd90Read (0x79, &Dat);
  if (Dat & BIT0) {
    EcSendTime ();
  }
}

/**
  A hook for board-specific initialization after PCI enumeration.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitAfterPciEnumeration (
  VOID
  )
{
  EcRequestsTime ();
  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ReadyToBoot event.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitReadyToBoot (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ExitBootServices event.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitEndOfFirmware (
  VOID
  )
{
  return EFI_SUCCESS;
}
