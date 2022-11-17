/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Protocol/PciPlatform.h>


/*++

Routine Description:

  Perform Platform initialization first in PciPlatform.

Arguments:

Returns:

 VOID.

--*/
VOID
EFIAPI
PciInitPlatform (
  VOID
  )
{
  return;
}

/*++

Routine Description:

  Perform Platform initialization by the phase indicated.

Arguments:

  HostBridge    -  The associated PCI host bridge handle.
  Phase         -  The phase of the PCI controller enumeration.
  ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

Returns:

--*/
VOID
EFIAPI
PhaseNotifyPlatform (
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE  Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  return;
}

