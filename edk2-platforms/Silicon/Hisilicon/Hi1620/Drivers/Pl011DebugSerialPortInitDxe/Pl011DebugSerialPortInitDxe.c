/** @file

    Copyright (c) 2016 - 2018, Hisilicon Limited. All rights reserved.
    Copyright (c) 2016 - 2018, Linaro Limited. All rights reserved.

    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PL011UartLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>

RETURN_STATUS
EFIAPI
DebugSerialPortInitialize (
  VOID
  )
{
  UINT64              BaudRate;
  UINT32              ReceiveFifoDepth;
  EFI_PARITY_TYPE     Parity;
  UINT8               DataBits;
  EFI_STOP_BITS_TYPE  StopBits;

  BaudRate = FixedPcdGet64 (PcdUartDefaultBaudRate);
  ReceiveFifoDepth = 0;         // Use default FIFO depth
  Parity = (EFI_PARITY_TYPE)FixedPcdGet8 (PcdUartDefaultParity);
  DataBits = FixedPcdGet8 (PcdUartDefaultDataBits);
  StopBits = (EFI_STOP_BITS_TYPE) FixedPcdGet8 (PcdUartDefaultStopBits);
  return PL011UartInitializePort (
           (UINTN)FixedPcdGet64 (PcdSerialDbgRegisterBase),
           FixedPcdGet32 (PL011UartClkInHz),
           &BaudRate,
           &ReceiveFifoDepth,
           &Parity,
           &DataBits,
           &StopBits
           );
}

EFI_STATUS
SerialPortEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  Status = DebugSerialPortInitialize ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CPU1 TB serial port init ERROR: %r\n", Status));
  }
  return EFI_SUCCESS;
}

