/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
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
  PCI_TYPE00                          *Pci,
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
  
  Dummy=0xFFFFFFFF;
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
  if ((Pci->Hdr).VendorId == 0xffff) {
    /// PCIe card could have been assigned a temporary bus number. 
    /// An write cycle can be used to try to rewrite the Bus number in the card
    /// Try to write the Vendor Id register, and recheck if the card is present. 
    Status = PciRootBridgeIo->Pci.Write(
                                    PciRootBridgeIo, 
                                    EfiPciWidthUint32, 
                                    Address, 
                                    1,							  
                                    &Dummy
                                    ); 
									
    // Retry the previous read after the PCI cycle has been tried. 
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    1,
                                    Pci
                                    );
  }

  if (!EFI_ERROR (Status) && (Pci->Hdr).VendorId != 0xffff) {

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
