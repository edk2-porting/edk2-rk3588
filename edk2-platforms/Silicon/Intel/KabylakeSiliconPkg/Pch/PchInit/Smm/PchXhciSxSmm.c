/** @file
  PCH xHCI Sx handler implementation.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/TimerLib.h>
#include "PchInitSmm.h"

/**
  xHCI Controller Configurations Prior to Entering S3
**/
VOID
PchXhciS3Callback (
  VOID
  )
{
  UINTN      XhciPciMmBase;
  UINT8      OrgCmdByte;
  UINT16     OrgPmcs;
  UINT32     OrgMmioAddr;
  UINT32     OrgMmioHAddr;
  UINT32     XhciMmioBase;

  XhciPciMmBase = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_XHCI,
                    PCI_FUNCTION_NUMBER_PCH_XHCI
                    );

  ///
  /// Save Cmd and XhciBar and Pmcs registers
  ///
  OrgCmdByte     = MmioRead8  (XhciPciMmBase + PCI_COMMAND_OFFSET);
  OrgPmcs        = MmioRead16 (XhciPciMmBase + R_PCH_XHCI_PWR_CNTL_STS);
  OrgMmioAddr    = MmioRead32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE) & 0xFFFF0000;
  OrgMmioHAddr   = MmioRead32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE + 4);

  ///
  /// Use the reserved MMIO
  /// Clear MSE before changing MMIO address
  ///
  MmioAnd8 (XhciPciMmBase + PCI_COMMAND_OFFSET, (UINT8)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
  MmioWrite32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE, (UINT32) mXhciMmioBaseAddr);
  MmioWrite32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE + 4, 0);

  ///
  /// Set MSE
  ///
  MmioOr8 (XhciPciMmBase + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));

  XhciMmioBase = (UINT32) mXhciMmioBaseAddr;

  ///
  /// Restore Cmd and XhciBar and Pmcs registers
  ///
  MmioWrite32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE + 4, OrgMmioHAddr);
  MmioWrite32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE, OrgMmioAddr);
  MmioWrite8 (XhciPciMmBase + PCI_COMMAND_OFFSET, OrgCmdByte);
  ///
  /// Bring device to D3
  ///
  MmioOr8 (XhciPciMmBase + R_PCH_XHCI_PWR_CNTL_STS, (UINT8)(V_PCH_XHCI_PWR_CNTL_STS_PWR_STS_D3));

}