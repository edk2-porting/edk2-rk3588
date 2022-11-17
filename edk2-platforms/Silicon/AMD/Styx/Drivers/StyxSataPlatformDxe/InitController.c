/** @file
  Initialize SATA Phy, Serdes, and Controller.

  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>
  Copyright (c) 2016, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SataRegisters.h"

#include <Library/AmdSataInitLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>

#include <Protocol/NonDiscoverableDevice.h>

#include <SocVersion.h>

STATIC
VOID
ResetSataController (
  EFI_PHYSICAL_ADDRESS    AhciBaseAddr
  )
{
  // Make a minimal global reset for HBA regiser
  MmioOr32 (AhciBaseAddr + EFI_AHCI_GHC_OFFSET, EFI_AHCI_GHC_RESET);

  // Clear all interrupts
  MmioWrite32 (AhciBaseAddr + EFI_AHCI_PORT_IS, EFI_AHCI_PORT_IS_CLEAR);

  // Turn on interrupts and ensure that the HBA is working in AHCI mode
  MmioOr32 (AhciBaseAddr + EFI_AHCI_GHC_OFFSET,
    EFI_AHCI_GHC_IE | EFI_AHCI_GHC_ENABLE);
}

STATIC
VOID
SetSataCapabilities (
  EFI_PHYSICAL_ADDRESS    AhciBaseAddr
  )
{
  UINT32                  Capability;

  Capability = 0;
  if (FixedPcdGetBool (PcdSataSssSupport)) // Staggered Spin-Up Support bit
    Capability |= EFI_AHCI_CAP_SSS;
  if (FixedPcdGetBool (PcdSataSmpsSupport)) // Mechanical Presence Support bit
    Capability |= EFI_AHCI_CAP_SMPS;

  MmioOr32 (AhciBaseAddr + EFI_AHCI_CAPABILITY_OFFSET, Capability);
}

STATIC
VOID
InitializeSataPorts (
  EFI_PHYSICAL_ADDRESS    AhciBaseAddr,
  UINTN                   PortCount
  )
{
  INTN                    PortNum;
  BOOLEAN                 IsCpd;
  BOOLEAN                 IsMpsp;
  UINT32                  PortRegAddr;
  UINT32                  RegVal;

  // Set Ports Implemented (PI)
  MmioWrite32 (AhciBaseAddr + EFI_AHCI_PI_OFFSET, (1 << PortCount) - 1);

  IsCpd = FixedPcdGetBool (PcdSataPortCpd);
  IsMpsp = FixedPcdGetBool (PcdSataPortMpsp);
  if (!IsCpd && !IsMpsp) {
    return;
  }

  for (PortNum = 0; PortNum < PortCount; PortNum++) {
    PortRegAddr = EFI_AHCI_PORT_OFFSET (PortNum) + EFI_AHCI_PORT_CMD;
    RegVal = MmioRead32(AhciBaseAddr + PortRegAddr);
    if (IsCpd)
      RegVal |= EFI_AHCI_PORT_CMD_CPD;
    else
      RegVal &= ~EFI_AHCI_PORT_CMD_CPD;
    if (IsMpsp)
      RegVal |= EFI_AHCI_PORT_CMD_MPSP;
    else
      RegVal &= ~EFI_AHCI_PORT_CMD_MPSP;
    RegVal |= EFI_AHCI_PORT_CMD_HPCP;
    MmioWrite32(AhciBaseAddr + PortRegAddr, RegVal);
  }
}

STATIC
EFI_STATUS
InitializeSataController (
  EFI_PHYSICAL_ADDRESS    AhciBaseAddr,
  UINTN                   SataPortCount,
  UINTN                   StartPort
  )
{
  UINT8                   SataChPerSerdes;
  UINT32                  PortNum;
  UINT32                  EvenPort;
  UINT32                  OddPort;

  SataChPerSerdes = FixedPcdGet8 (PcdSataNumChPerSerdes);

  //
  // SataChPerSerdes must be 2 for the Even/Odd logic in the loop below
  //
  ASSERT(SataChPerSerdes == 2);

  for (PortNum = StartPort; PortNum < SataPortCount + StartPort; PortNum += SataChPerSerdes) {
    EvenPort = (UINT32)(FixedPcdGet32 (PcdSataPortMode) >> (PortNum * 2)) & 3;
    OddPort = (UINT32)(FixedPcdGet32 (PcdSataPortMode) >> ((PortNum+1) * 2)) & 3;
    SataPhyInit (PortNum / SataChPerSerdes, EvenPort, OddPort);
  }

  //
  // Reset SATA controller
  //
  ResetSataController (AhciBaseAddr);

  //
  // Set SATA capabilities
  //
  SetSataCapabilities (AhciBaseAddr);

  //
  // Set and intialize the Sata ports
  //
  InitializeSataPorts (AhciBaseAddr, SataPortCount);

  return RegisterNonDiscoverableMmioDevice (
           NonDiscoverableDeviceTypeAhci,
           NonDiscoverableDeviceDmaTypeCoherent,
           NULL,
           NULL,
           1,
           AhciBaseAddr, SIZE_4KB);
}

EFI_STATUS
EFIAPI
StyxSataPlatformDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINT32                  PortNum;
  EFI_STATUS              Status;

  //
  // Perform SATA workarounds
  //
  for (PortNum = 0; PortNum < FixedPcdGet8(PcdSata0PortCount); PortNum++) {
      SetCwMinSata0 (PortNum);
  }

  Status = InitializeSataController (FixedPcdGet32(PcdSata0CtrlAxiSlvPort),
             FixedPcdGet8(PcdSata0PortCount), 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: failed to initialize primary SATA controller!\n",
      __FUNCTION__));
    return Status;
  }

  for (PortNum = 0; PortNum < FixedPcdGet8(PcdSata0PortCount); PortNum++) {
      SetPrdSingleSata0 (PortNum);
  }

  //
  // Ignore the second SATA controller on pre-B1 silicon
  //
  if ((PcdGet32 (PcdSocCpuId) & STYX_SOC_VERSION_MASK) < STYX_SOC_VERSION_B1) {
    return EFI_SUCCESS;
  }

  if (FixedPcdGet8(PcdSata1PortCount) > 0) {
    for (PortNum = 0; PortNum < FixedPcdGet8(PcdSata1PortCount); PortNum++) {
        SetCwMinSata1 (PortNum);
    }

    Status = InitializeSataController (FixedPcdGet32(PcdSata1CtrlAxiSlvPort),
               FixedPcdGet8(PcdSata1PortCount),
               FixedPcdGet8(PcdSata0PortCount));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: failed to initialize secondary SATA controller!\n",
        __FUNCTION__));
    } else {
      for (PortNum = 0; PortNum < FixedPcdGet8(PcdSata1PortCount); PortNum++) {
          SetPrdSingleSata1 (PortNum);
      }
    }
  }
  return EFI_SUCCESS;
}
