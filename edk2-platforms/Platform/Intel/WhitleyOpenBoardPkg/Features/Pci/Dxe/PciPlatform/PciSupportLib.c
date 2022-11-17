/** @file
  Support PCI chipset initialization.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PiDxe.h"
#include <Base.h>
#include <Guid/SocketIioVariable.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "IndustryStandard/Pci.h"
#include "PciSupportLib.h"

PCIE_STACK  mPcieStack;


/**

  This routine is used to check whether the pci device is present

  @retval None

**/
BOOLEAN
IsPciDevicePresent (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *PciRootBridgeIo,
  OUT PCI_TYPE00                      *Pci,
  UINT8                               Bus,
  UINT8                               Device,
  UINT8                               Func
  )
// TODO:    PciRootBridgeIo - add argument and description to function comment
// TODO:    Pci - add argument and description to function comment
// TODO:    Bus - add argument and description to function comment
// TODO:    Device - add argument and description to function comment
// TODO:    Func - add argument and description to function comment
// TODO:    EFI_SUCCESS - add return value to function comment
// TODO:    EFI_NOT_FOUND - add return value to function comment
{
  UINT64      Address;
  UINT32      Dummy;
  EFI_STATUS  Status;

  Dummy = 0xFFFFFFFF;
  //
  // Create PCI address map in terms of Bus, Device and Func
  //
  Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);

  //
  // Read the Vendor Id register
  //
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32,
                                  Address,
                                  1,
                                  Pci
                                  );
  if ((Pci->Hdr).VendorId == 0xFFFF) {
    //
    // The PCIe card could have been assigned a temporary bus number earlier in
    // the boot flow. Performing a write cycle can be used to cause the PCIe
    // card to latch the new bus number. Try to writing the Vendor Id register,
    // then recheck if the card is present.
    //
    Status = PciRootBridgeIo->Pci.Write(
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    1,
                                    &Dummy
                                    );

    //
    // Retry the previous read after the PCI write cycle.
    //
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    1,
                                    Pci
                                    );
  }

  if (!EFI_ERROR (Status) && (Pci->Hdr).VendorId != 0xFFFF) {
    //
    // Read the entire config header for the device
    //
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    sizeof (PCI_TYPE00) / sizeof (UINT32),
                                    Pci
                                    );

    return TRUE;
  }

  return FALSE;
}
