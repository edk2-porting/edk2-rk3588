/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PcdLib.h>
#include <Library/PciExpressLib.h>
#include <Library/PlatformPciLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciPlatform.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Regs/HisiPcieV1RegOffset.h>

#define INVALID_CAPABILITY_00       0x00
#define INVALID_CAPABILITY_FF       0xFF
#define PCI_CAPABILITY_POINTER_MASK 0xFC

STATIC
UINT64
GetPcieCfgAddress (
    UINT64 Ecam,
    UINTN Bus,
    UINTN Device,
    UINTN Function,
    UINTN Reg
    )
{
  return Ecam + PCI_EXPRESS_LIB_ADDRESS (Bus, Device, Function, Reg);
}

STATIC
PCI_ROOT_BRIDGE_RESOURCE_APPETURE *
GetAppetureByRootBridgeIo (
    IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *RootBridge
    )
{
  EFI_STATUS Status;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Configuration = NULL;
  UINTN Hb;
  UINTN Rb;

  Status = RootBridge->Configuration (
      RootBridge,
      (VOID **)&Configuration
      );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a:%d] RootBridgeIo->Configuration failed %r\n",
          __FUNCTION__, __LINE__, Status));
    return NULL;
  };

  while (Configuration->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    if (Configuration->ResType == ACPI_ADDRESS_SPACE_TYPE_BUS) {
      break;
    }
    Configuration++;
  }

  if (Configuration->Desc != ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((DEBUG_ERROR, "[%a:%d] Can't find bus descriptor\n", __FUNCTION__, __LINE__));
    return NULL;
  }

  for (Hb = 0; Hb < PCIE_MAX_HOSTBRIDGE; Hb++) {
    for (Rb = 0; Rb < PCIE_MAX_ROOTBRIDGE; Rb++) {
      if (RootBridge->SegmentNumber == mResAppeture[Hb][Rb].Segment &&
          Configuration->AddrRangeMin >= mResAppeture[Hb][Rb].BusBase &&
          Configuration->AddrRangeMax <= mResAppeture[Hb][Rb].BusLimit) {
        return &mResAppeture[Hb][Rb];
      }
    }
  }

  DEBUG ((DEBUG_ERROR, "[%a:%d] Can't find PCI appeture\n", __FUNCTION__, __LINE__));
  return NULL;
}

STATIC
VOID
SetAtuConfig0RW (
    PCI_ROOT_BRIDGE_RESOURCE_APPETURE *Private,
    UINT32 Index
    )
{
  UINTN RbPciBase = Private->RbPciBar;
  UINT64 MemLimit = GetPcieCfgAddress (Private->Ecam, Private->BusBase + 1, 1, 0, 0) - 1;
  UINT64 MemBase = GetPcieCfgAddress (Private->Ecam, Private->BusBase, 0, 0, 0);


  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_VIEW_POINT, Index);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LOW, (UINT32)MemBase);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_HIGH, (UINT32)(MemBase >> 32));
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LIMIT, (UINT32) MemLimit);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_LOW, 0);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_HIGH, 0);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL1, IATU_CTRL1_TYPE_CONFIG0);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL2, IATU_SHIIF_MODE);

  {
    UINTN i;
    for (i=0; i<0x20; i+=4) {
      DEBUG ((DEBUG_ERROR, "[%a:%d] - Base=%p value=%x\n", __FUNCTION__, __LINE__, RbPciBase + 0x900 + i, MmioRead32(RbPciBase + 0x900 + i)));
    }
  }
}

STATIC
VOID
SetAtuConfig1RW (
    PCI_ROOT_BRIDGE_RESOURCE_APPETURE *Private,
    UINT32 Index
    )
{
  UINTN RbPciBase = Private->RbPciBar;
  UINT64 MemLimit = GetPcieCfgAddress (Private->Ecam, Private->BusLimit + 1, 0, 0, 0) - 1;
  UINT64 MemBase = GetPcieCfgAddress (Private->Ecam, Private->BusBase + 2, 0, 0, 0);


  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_VIEW_POINT, Index);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL1, IATU_CTRL1_TYPE_CONFIG1);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LOW, (UINT32)MemBase);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_HIGH, (UINT32)(MemBase >> 32));
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LIMIT, (UINT32) MemLimit);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_LOW, 0);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_HIGH, 0);
  MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL2, IATU_SHIIF_MODE);

  {
    UINTN i;
    for (i=0; i<0x20; i+=4) {
      DEBUG ((DEBUG_ERROR, "[%a:%d] - Base=%p value=%x\n", __FUNCTION__, __LINE__, RbPciBase + 0x900 + i, MmioRead32(RbPciBase + 0x900 + i)));
    }
  }
}

STATIC
VOID
SetAtuIoRW (UINT64 RbPciBase,UINT64 IoBase,UINT64 CpuIoRegionLimit, UINT64 CpuIoRegionBase, UINT32 Index)
{

    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_VIEW_POINT, Index);
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL1, IATU_CTRL1_TYPE_IO);

    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LOW, (UINT32)(CpuIoRegionBase));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_HIGH, (UINT32)((UINT64)CpuIoRegionBase >> 32));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LIMIT, (UINT32)(CpuIoRegionLimit));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_LOW, (UINT32)(IoBase));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_HIGH, (UINT32)((UINT64)(IoBase) >> 32));

    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL2, IATU_NORMAL_MODE);

    {
      UINTN i;
      for (i=0; i<0x20; i+=4) {
        DEBUG ((DEBUG_ERROR, "[%a:%d] - Base=%p value=%x\n", __FUNCTION__, __LINE__, RbPciBase + 0x900 + i, MmioRead32(RbPciBase + 0x900 + i)));
      }
    }
}

STATIC
VOID
SetAtuMemRW(UINT64 RbPciBase,UINT64 MemBase,UINT64 CpuMemRegionLimit, UINT64 CpuMemRegionBase, UINT32 Index)
{

    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_VIEW_POINT, Index);
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL1, IATU_CTRL1_TYPE_MEM);

    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LOW, (UINT32)(CpuMemRegionBase));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_HIGH, (UINT32)((UINT64)(CpuMemRegionBase) >> 32));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LIMIT, (UINT32)(CpuMemRegionLimit));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_LOW, (UINT32)(MemBase));
    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_TARGET_HIGH, (UINT32)((UINT64)(MemBase) >> 32));

    MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_CTRL2, IATU_NORMAL_MODE);

    {
      UINTN i;
      for (i=0; i<0x20; i+=4) {
        DEBUG ((DEBUG_ERROR, "[%a:%d] - Base=%p value=%x\n", __FUNCTION__, __LINE__, RbPciBase + 0x900 + i, MmioRead32(RbPciBase + 0x900 + i)));
      }
    }
}

VOID
InitAtu (PCI_ROOT_BRIDGE_RESOURCE_APPETURE *Private)
{
  SetAtuMemRW (Private->RbPciBar, Private->PciRegionBase, Private->PciRegionLimit, Private->CpuMemRegionBase, 0);
  SetAtuConfig0RW (Private, 1);
  SetAtuConfig1RW (Private, 2);
  SetAtuIoRW (Private->RbPciBar, Private->IoBase, Private->IoLimit, Private->CpuIoRegionBase, 3);
}

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
  UINT32             Port;
  UINT32             HostBridgeNum = 0;

  for (HostBridgeNum = 0; HostBridgeNum < PCIE_MAX_HOSTBRIDGE; HostBridgeNum++) {
    for (Port = 0; Port < PCIE_MAX_ROOTBRIDGE; Port++) {
      InitAtu (&mResAppeture[HostBridgeNum][Port]);
    }
  }

  return;
}

STATIC
BOOLEAN
PcieCheckAriFwdEn (
  UINTN  PciBaseAddr
  )
{
  UINT8   PciPrimaryStatus;
  UINT8   CapabilityOffset;
  UINT8   CapId;
  UINT8   TempData;

  PciPrimaryStatus = MmioRead16 (PciBaseAddr + PCI_PRIMARY_STATUS_OFFSET);

  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {
    CapabilityOffset = MmioRead8 (PciBaseAddr + PCI_CAPBILITY_POINTER_OFFSET);
    CapabilityOffset &= PCI_CAPABILITY_POINTER_MASK;

    while ((CapabilityOffset != INVALID_CAPABILITY_00) && (CapabilityOffset != INVALID_CAPABILITY_FF)) {
      CapId = MmioRead8 (PciBaseAddr + CapabilityOffset);
      if (CapId == EFI_PCI_CAPABILITY_ID_PCIEXP) {
        break;
      }
      CapabilityOffset = MmioRead8 (PciBaseAddr + CapabilityOffset + 1);
      CapabilityOffset &= PCI_CAPABILITY_POINTER_MASK;
    }
  } else {
    return FALSE;
  }

  if ((CapabilityOffset == INVALID_CAPABILITY_FF) || (CapabilityOffset == INVALID_CAPABILITY_00)) {
    return FALSE;
  }

  TempData = MmioRead16 (PciBaseAddr + CapabilityOffset +
                          EFI_PCIE_CAPABILITY_DEVICE_CONTROL_2_OFFSET);
  TempData &= EFI_PCIE_CAPABILITY_DEVICE_CAPABILITIES_2_ARI_FORWARDING;

  if (TempData == EFI_PCIE_CAPABILITY_DEVICE_CAPABILITIES_2_ARI_FORWARDING) {
    return TRUE;
  } else {
    return FALSE;
  }
}

VOID
EnlargeAtuConfig0 (
  IN EFI_HANDLE HostBridge
  )
{
  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL    *ResAlloc = NULL;
  EFI_STATUS                                          Status;
  EFI_HANDLE RootBridgeHandle = NULL;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *RootBridgeIo = NULL;
  PCI_ROOT_BRIDGE_RESOURCE_APPETURE *Appeture;
  UINTN                           RbPciBase;
  UINT64                          MemLimit;

  DEBUG ((DEBUG_INFO, "In Enlarge RP iATU Config 0.\n"));

  Status = gBS->HandleProtocol (
      HostBridge,
      &gEfiPciHostBridgeResourceAllocationProtocolGuid,
      (VOID **)&ResAlloc
      );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a:%d] - HandleProtocol failed %r\n", __FUNCTION__,
          __LINE__, Status));
    return;
  }

  while (TRUE) {
    Status = ResAlloc->GetNextRootBridge (
        ResAlloc,
        &RootBridgeHandle
        );
    if (EFI_ERROR (Status)) {
      break;
    }
    Status = gBS->HandleProtocol (
        RootBridgeHandle,
        &gEfiPciRootBridgeIoProtocolGuid,
        (VOID **)&RootBridgeIo
        );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a:%d] - HandleProtocol failed %r\n", __FUNCTION__, __LINE__, Status));
      // This should never happen so that it is a fatal error and we don't try
      // to continue
      break;
    }

    Appeture = GetAppetureByRootBridgeIo (RootBridgeIo);
    if (Appeture == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a:%d] Get appeture failed\n", __FUNCTION__,
            __LINE__));
      continue;
    }

    RbPciBase = Appeture->RbPciBar;
    // Those ARI FWD Enable Root Bridge, need enlarge iATU window.
    if (PcieCheckAriFwdEn (RbPciBase)) {
      MemLimit = GetPcieCfgAddress (Appeture->Ecam, Appeture->BusBase + 2, 0, 0, 0) - 1;
      MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_VIEW_POINT, 1);
      MmioWrite32 (RbPciBase + IATU_OFFSET + IATU_REGION_BASE_LIMIT, (UINT32) MemLimit);
    }
  }
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
  switch (Phase) {
  case EfiPciHostBridgeEndEnumeration:
    // Only do once
    if (ChipsetPhase == ChipsetEntry) {
      DEBUG ((DEBUG_INFO, "PCI end enumeration platform hook\n"));
      EnlargeAtuConfig0 (HostBridge);
    }
    break;
  default:
    break;
  }

  return ;
}

