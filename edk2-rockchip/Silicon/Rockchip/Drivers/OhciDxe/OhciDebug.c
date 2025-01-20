/** @file
This file provides the information dump support for OHCI when in debug mode.

Copyright (c) 2013-2015 Intel Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Ohci.h"

/*++

  Print the data of ED and the TDs attached to the ED

  @param  Uhc                   Pointer to OHCI private data
  @param  Ed                    Pointer to a ED to free
  @param  Td                    Pointer to the Td head

  @retval EFI_SUCCESS           ED

**/
EFI_STATUS
OhciDumpEdTdInfo (
  IN USB_OHCI_HC_DEV  *Uhc,
  IN ED_DESCRIPTOR    *Ed,
  IN TD_DESCRIPTOR    *Td,
  BOOLEAN             Stage
  )
{
  UINT32  Index;

  if (Stage) {
    DEBUG ((DEBUG_INFO, "\n Before executing command\n"));
  } else {
    DEBUG ((DEBUG_INFO, "\n after executing command\n"));
  }

  if (Ed != NULL) {
    DEBUG ((DEBUG_INFO, "\nED Address:%p, ED buffer:\n", Ed));
    DEBUG ((DEBUG_INFO, "DWord0  :TD Tail :TD Head :Next ED\n"));
    for (Index = 0; Index < sizeof (ED_DESCRIPTOR)/4; Index++) {
      DEBUG ((DEBUG_INFO, "%8x ", *((UINT32 *)(Ed) + Index)));
    }

    DEBUG ((DEBUG_INFO, "\nNext TD buffer:%p\n", Td));
  }

  while (Td != NULL) {
    if (Td->Word0.DirPID == TD_SETUP_PID) {
      DEBUG ((DEBUG_INFO, "\nSetup PID "));
    } else if (Td->Word0.DirPID == TD_OUT_PID) {
      DEBUG ((DEBUG_INFO, "\nOut PID "));
    } else if (Td->Word0.DirPID == TD_IN_PID) {
      DEBUG ((DEBUG_INFO, "\nIn PID "));
    } else if (Td->Word0.DirPID == TD_NODATA_PID) {
      DEBUG ((DEBUG_INFO, "\nNo data PID "));
    }

    DEBUG ((DEBUG_INFO, "TD Address:%p, TD buffer:\n", Td));
    DEBUG ((DEBUG_INFO, "DWord0  :CuBuffer:Next TD :Buff End:Next TD :DataBuff:ActLength\n"));
    for (Index = 0; Index < sizeof (TD_DESCRIPTOR)/4; Index++) {
      DEBUG ((DEBUG_INFO, "%8x ", *((UINT32 *)(Td) + Index)));
    }

    DEBUG ((DEBUG_INFO, "\nCurrent TD Data buffer(size%d)\n", (UINT32)Td->ActualSendLength));
    for (Index = 0; Index < Td->ActualSendLength; Index++) {
      DEBUG ((DEBUG_INFO, "%2x ", *(UINT8 *)(UINTN)(Td->DataBuffer + Index)));
    }

    Td = (TD_DESCRIPTOR *)(UINTN)(Td->NextTDPointer);
  }

  DEBUG ((DEBUG_INFO, "\n TD buffer End\n"));

  return EFI_SUCCESS;
}

VOID
OhciDumpReg (
  IN USB_OHCI_HC_DEV  *Ohc
  )
{
  UINT32  Data;

  for (int i = 0; i < 22; i++) {
    Data = MmioRead32 (Ohc->UsbHcBaseAddress + 0x04*i);
    DEBUG ((DEBUG_INFO, "OhcDumpReg 0x%x = 0x%x\n", Ohc->UsbHcBaseAddress +0x04*i, Data));
  }
}
