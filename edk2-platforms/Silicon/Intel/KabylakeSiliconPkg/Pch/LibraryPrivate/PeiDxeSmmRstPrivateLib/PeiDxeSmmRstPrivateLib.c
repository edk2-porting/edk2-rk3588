/** @file
 Routines for Rst remapping

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/RstPrivateLib.h>
#include <PchPcieStorageDetectHob.h>

/**
  Checks whether device on given Root Port is remap capable

  @param[in] RpNumber                   Root Port Number
  @param[in] TempPciBus                 Temporary Pci Bus

  @retval UINT8                         Device Programming Interface
**/
UINT8
RstGetProgInterfaceForRemapDevice (
  IN UINT32        RpNumber,
  IN UINT32        TempPciBus
)
{
  UINTN           RpDevice;
  UINTN           RpFunction;
  UINT8           BusNumber;
  UINTN           EpBase;
  UINT8           PiInterface;
  UINTN           RpBase;
  UINT8           ClassCode;
  UINT8           SubClassCode;


  DEBUG ((DEBUG_INFO, "Detect storage device on Rp: %d\n", RpNumber));

  GetPchPcieRpDevFun (RpNumber, &RpDevice, &RpFunction);
  BusNumber = DEFAULT_PCI_BUS_NUMBER_PCH;
  PiInterface = RST_PCIE_STORAGE_INTERFACE_NONE;
  RpBase = MmPciBase (BusNumber, (UINT32) RpDevice, (UINT32) RpFunction);

  if (MmioRead16 (RpBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG((DEBUG_INFO, "RstGetProgInterfaceForRemapDevices: RootPort does not exists Rp = %d\n", RpNumber));

    return PiInterface;
  }

  //
  // Assign Temporary Bus Number
  //
  MmioAndThenOr32 (
    RpBase + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET,
    (UINT32) ~B_PCI_BRIDGE_BNUM_SBBN_SCBN,
    ((UINT32) (TempPciBus << 8)) | ((UINT32) (TempPciBus << 16))
    );

  //
  // A config write is required in order for the device to re-capture the Bus number,
  // according to PCI Express Base Specification, 2.2.6.2
  // Write to a read-only register VendorID to not cause any side effects.
  //
  EpBase  = MmPciBase (TempPciBus, 0, 0);
  MmioWrite16 (EpBase + PCI_VENDOR_ID_OFFSET, 0);

  //
  // Read Vendor Id to check if device exists
  // if not clear bus number and return RST_PCIE_STORAGE_INTERFACE_NONE
  //
  if (MmioRead16 (EpBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    MmioAnd32 (RpBase + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, (UINT32) ~B_PCI_BRIDGE_BNUM_SBBN_SCBN);
    return PiInterface;
  }

  ClassCode = MmioRead8 (EpBase + R_PCI_BCC_OFFSET);
  SubClassCode = MmioRead8 (EpBase + R_PCI_SCC_OFFSET);
  PiInterface = MmioRead8 (EpBase + R_PCI_PI_OFFSET);

  if ( ClassCode == PCI_CLASS_MASS_STORAGE) {
    DEBUG ((DEBUG_INFO, "RstGetProgInterfaceForRemapDevice: ClassCode = %X, SubClassCode = %X,  PiInterface = %X\n", ClassCode, SubClassCode, PiInterface));

    if (SubClassCode == PCI_CLASS_MASS_STORAGE_AHCI) {
      if (PiInterface == RST_PCIE_STORAGE_INTERFACE_AHCI) {
        DEBUG ((DEBUG_INFO, "RstGetProgInterfaceForRemapDevice: AHCI Card found on Rp: %d\n", RpNumber));

      }
    } else if (SubClassCode == PCI_CLASS_MASS_STORAGE_NVME) {
        if (PiInterface == RST_PCIE_STORAGE_INTERFACE_NVME) {
          DEBUG ((DEBUG_INFO, "RstGetProgInterfaceForRemapDevice: NVMe Card found on Rp: %d\n", RpNumber));

        }
    } else if (SubClassCode == PCI_CLASS_MASS_STORAGE_RAID) {
      DEBUG ((DEBUG_INFO, "RstGetProgInterfaceForRemapDevice: RAID Card found on Rp: %d\n", RpNumber));

      PiInterface = RST_PCIE_STORAGE_INTERFACE_NVME;
    }
  }
  //
  // Clear temporary bus number
  //
  MmioAnd32 (RpBase + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, (UINT32) ~B_PCI_BRIDGE_BNUM_SBBN_SCBN);

  return PiInterface;

}

/**
  Check the lane occupied by certain root port according to the root port number and configuration strap
  Return 8-bit bitmap where each bit represents the lane number (e.g.: return 00000011b means the root port owns 2 lane)

  @param[in] RootPortNum            Root Port Number

  @retval UINT8                     Lane Occupied by the Root Port (bitmap)
**/
UINT8
RstGetRpLaneOccupyMask (
  IN  UINT32                   RootPortNum
  )
{
  EFI_STATUS              Status;
  UINT32                  CtrlNum;
  UINT32                  CtrlFirstRpNum;
  UINT32                  Data32;
  UINT8                   LaneOccupied;

  LaneOccupied          = 0;

  CtrlNum = RootPortNum / PCH_PCIE_CONTROLLER_PORTS;
  CtrlFirstRpNum = CtrlNum * PCH_PCIE_CONTROLLER_PORTS;

  //
  // Read the Root Port Configuration Straps for the link width, and return LaneOccupied by the Root Port accordingly
  //
  Status = PchSbiRpPciRead32 (CtrlFirstRpNum, R_PCH_PCIE_STRPFUSECFG, &Data32);
  ASSERT_EFI_ERROR (Status);

  switch ((Data32 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC) {
    case V_PCH_PCIE_STRPFUSECFG_RPC_4:
      if (RootPortNum % 4 == 0) {
        LaneOccupied = (BIT3|BIT2|BIT1|BIT0);
      }
      break;
    case V_PCH_PCIE_STRPFUSECFG_RPC_2_2:
      if ((RootPortNum % 2 == 0)) {
        LaneOccupied = (BIT1|BIT0);
      }
      break;
    case V_PCH_PCIE_STRPFUSECFG_RPC_2_1_1:
      if (RootPortNum % 4 == 0) {
        LaneOccupied = (BIT1|BIT0);
      } else if (RootPortNum % 4 != 1) {
        LaneOccupied = BIT0;
      }
      break;
    case V_PCH_PCIE_STRPFUSECFG_RPC_1_1_1_1:
      LaneOccupied = BIT0;
      break;
    default:
      break;
  }

  return LaneOccupied;
}

/**
  Checks PCH generation and returns SATA's GCR.PLS bit number according to root port number with a PCIe storage device
  connected to.

  @param[in] RootPortNum        Root port number which PCIe storage device is connected to

  @retval UINT32                Number of GCR.PLS bit representing root port
**/
UINT32
RstPlsBitNumber (
  IN UINT32           RootPortNum
  )
{
  if ((GetPchGeneration () == KblPch) && (RstGetCycleRouterNumber (RootPortNum) == RST_PCIE_STORAGE_CR_2)) {
    return RootPortNum - 2 * PCH_PCIE_CONTROLLER_PORTS;
  } else {
    return RootPortNum;
  }
}

/**
  Checks if device with given PCI config space address is Intel's Stony Beach.

  @param[in] EndPointPciBase    Address of device's PCI config space

  @retval BOOLEAN               TRUE if device is Intel's Stony Beach, FALSE othrwise
**/
BOOLEAN
RstIsStonyBeach (
  IN UINTN            EndPointPciBase
  )
{
  UINT16              DeviceVendorId;
  UINT16              DeviceDeviceId;

  DeviceVendorId = MmioRead16 (EndPointPciBase + PCI_VENDOR_ID_OFFSET);
  DeviceDeviceId = MmioRead16 (EndPointPciBase + PCI_DEVICE_ID_OFFSET);

  if (DeviceVendorId == RST_STONY_BEACH_VENDOR_ID && DeviceDeviceId == RST_STONY_BEACH_DEVICE_ID) {
    return TRUE;
  }

  return FALSE;
}


