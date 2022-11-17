/** @file
  Configures the remapping for PCH Intel RST PCie Storage
  In order to use this feature, Intel RST Driver is required

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchInit.h"
#include <Library/RstPrivateLib.h>
#include <PchPcieStorageDetectHob.h>

//
// Variables below indicate the memory range to be allocated to the PCIe storage device BAR usage when it is HIDDEN,
// thus it does not overlapped with the SGREG_BAR from the host perspective
//
#define RST_PCIE_STORAGE_MEMORY_START_RANGE         PCH_PCR_BASE_ADDRESS
#define RST_PCIE_STORAGE_MEMORY_END_RANGE           (PCH_PCR_BASE_ADDRESS + PCH_PCR_MMIO_SIZE - 1)

#define PCI_CARD_PM_CAP_ID              0x01
#define PCI_CARD_MSIX_CAP_ID            0x11
#define PCI_CARD_SATA_CAP_ID            0x12
#define PCI_CARD_BAR_TOTAL              6
#define PCI_CARD_LINK_SPEED_GEN1_GEN2   0
#define PCI_CARD_LINK_SPEED_GEN3        1
#define PCI_CARD_LINK_WIDTH_1           0
#define PCI_CARD_LINK_WIDTH_2           1
#define PCI_CARD_LINK_WIDTH_4           2
#define PCI_CARD_REVISION               0x08
#define PCI_CARD_BASE_ADDR0             0x10
#define PCI_CARD_BASE_ADDR1             0x14
#define PCI_CARD_BASE_ADDR5             0x24

GLOBAL_REMOVE_IF_UNREFERENCED PCH_RST_PCIE_STORAGE_DETECTION mRstPcieStorageDetection [PCH_MAX_RST_PCIE_STORAGE_CR];

UINT32 PchRstPcieStorageCurrentMemoryRange = RST_PCIE_STORAGE_MEMORY_START_RANGE;


/**
  Function to perform dump for PCH_RST_PCIE_STORAGE_DETECTION

  @param[in] Index      Index for RST PCIe Storage Cycle Router Instance

  @retval None
**/
VOID
PchRstPcieStorageRemappingDump (
  IN        UINTN              Index
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "PchRstPcieStorageRemappingDump() Started\n"));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].SupportRstPcieStoragRemapping    =  %x\n", Index, mRstPcieStorageDetection[Index].SupportRstPcieStoragRemapping));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].RootPortNum                      =  %x\n", Index, mRstPcieStorageDetection[Index].RootPortNum));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].RootPortLane                     =  %x\n", Index, mRstPcieStorageDetection[Index].RootPortLane));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].DeviceInterface                  =  %x\n", Index, mRstPcieStorageDetection[Index].DeviceInterface));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].IsMsixSupported                  =  %x\n", Index, mRstPcieStorageDetection[Index].IsMsixSupported));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].MsixStartingVector               =  %x\n", Index, mRstPcieStorageDetection[Index].MsixStartingVector));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].MsixEndingVector                 =  %x\n", Index, mRstPcieStorageDetection[Index].MsixEndingVector));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointBarSize                  =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointBarSize));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointUniqueMsixTableBar       =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointUniqueMsixTableBar));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointUniqueMsixTableBarValue  =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointUniqueMsixTableBarValue));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointUniqueMsixPbaBar         =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointUniqueMsixPbaBar));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointUniqueMsixPbaBarValue    =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointUniqueMsixPbaBarValue));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointBcc                      =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointBcc));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointScc                      =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointScc));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].EndPointPi                       =  %x\n", Index, mRstPcieStorageDetection[Index].EndPointPi));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.PmCapPtr                    =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.PmCapPtr));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.PcieCapPtr                  =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.PcieCapPtr));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.L1ssCapPtr                  =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.L1ssCapPtr));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.EndpointL1ssControl2        =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointL1ssControl2));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.EndpointL1ssControl1        =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointL1ssControl1));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.LtrCapPtr                   =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.LtrCapPtr));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.EndpointLtrData             =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointLtrData));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.EndpointLctlData16          =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointLctlData16));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.EndpointDctlData16          =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointDctlData16));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.EndpointDctl2Data16         =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointDctl2Data16));
  DEBUG ((DEBUG_INFO, "mRstPcieStorageDetection[%d].PchRstPcieStorageSaveRestore.RootPortDctl2Data16         =  %x\n", Index, mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.RootPortDctl2Data16));
  DEBUG ((DEBUG_INFO, "PchRstPcieStorageRemappingDump() Ended\n"));
  DEBUG_CODE_END ();
}

/**
  Perform  memory space allocation or free memory pool for AHCI BAR

  @param[in]      Operation             True:  Perform memory space allocation for AHCI BAR
                                        False: Perform memory pool free for AHCI BAR
  @param[in,out]  MemBaseAddress        The base memory address used by AHCI BAR
**/
VOID
AbarMemorySpaceOperation (
  IN      BOOLEAN                             Operation,
  IN OUT  EFI_PHYSICAL_ADDRESS                *MemBaseAddress
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  BaseAddress;
  UINTN                 AhciBarAlignment;
  UINTN                 AhciBarLength;

  //
  // Assign a 512k size memory space to the ABAR when NVM Remapping is enabled
  //
  AhciBarAlignment = N_PCH_SATA_AHCI_BAR_ALIGNMENT_512K;     // 2^19: 512K Alignment
  AhciBarLength = V_PCH_SATA_AHCI_BAR_LENGTH_512K;           // 512K Length


  if (Operation == TRUE) {
    ///
    /// If MemBaseAddress is not allocated yet, allocate the AHCI BAR
    ///
    if ((PcdGet8 (PcdEfiGcdAllocateType) == EfiGcdAllocateMaxAddressSearchBottomUp) || (PcdGet8 (PcdEfiGcdAllocateType) == EfiGcdAllocateMaxAddressSearchTopDown)) {
      BaseAddress = 0x0ffffffff;
    }
    Status = gDS->AllocateMemorySpace (
                    PcdGet8 (PcdEfiGcdAllocateType),
                    EfiGcdMemoryTypeMemoryMappedIo,
                    AhciBarAlignment,
                    AhciBarLength,
                    &BaseAddress,
                    mImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
    *MemBaseAddress = BaseAddress;
  } else {
    ///
    /// Else, free the GCD pool
    ///
    gDS->FreeMemorySpace (
           *MemBaseAddress,
           AhciBarLength
           );
  }
}

/**
  Function to perform memory allocation for PCIe storage device that support unique BAR

  @param[in]        BarSize         The BAR size required for memory allocation
  @param[in,out]    AllocAddr       The Address that been allocated by this function

  @retval EFI_SUCCESS               The function completed successfully
  @retval EFI_OUT_OF_RESOURCES      Memory or storage is not enough
**/
EFI_STATUS
PchRstPcieStorageMemAllocation (
  IN        UINT32              BarSize,
  IN OUT    UINT32              *AllocAddr
  )
{
  if ((PchRstPcieStorageCurrentMemoryRange + BarSize) > RST_PCIE_STORAGE_MEMORY_END_RANGE) {
    return EFI_OUT_OF_RESOURCES;
  }

  if ((PchRstPcieStorageCurrentMemoryRange & (BarSize - 1)) != 0) {
    *AllocAddr      = (PchRstPcieStorageCurrentMemoryRange + BarSize) & ~(BarSize-1);
  } else {
    *AllocAddr      = PchRstPcieStorageCurrentMemoryRange;
  }

  PchRstPcieStorageCurrentMemoryRange       = *AllocAddr + BarSize;
  return EFI_SUCCESS;
}

/**
  Check and detect PCIe storage device per policies, and if existed, indicate if it should be subjected for remapping
  Populate the  mRstPcieStorageDetection structure along with it for later usages

  @param[in, out]   RemappingRequired           Boolean value with TRUE indicates that RST PCIe Storage Remapping is required
  @param[in]        TempPciBusMin               The temporary minimum Bus number for root port initialization
  @param[in]        SataRegBase                 SATA Register Base

  @retval None
**/
VOID
DetectPcieStorageDevices (
  IN OUT BOOLEAN                        *RemappingRequired,
  IN UINT8                              TempPciBusMin,
  IN UINTN                              SataRegBase
  )
{
  EFI_STATUS              Status;
  PCH_SERIES              PchSeries;
  UINT8                   DeviceInterface;
  UINT32                  PortNum;
  UINTN                   PciRootPortRegBase;
  UINTN                   RpDev;
  UINTN                   RpFunc;
  UINT8                   RootPortLane;
  PCIE_STORAGE_INFO_HOB   *PcieStorageInfoHob;
  VOID                    *Hob;
  UINT32                  CycleRouterNum;

  DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: DetectPcieStorageDevices Started\n"));

  Hob = NULL;
  Hob = GetFirstGuidHob (&gPchPcieStorageDetectHobGuid);
  if (Hob != NULL) {
    PcieStorageInfoHob = (PCIE_STORAGE_INFO_HOB *) GET_GUID_HOB_DATA (Hob);
  } else {
    return;
  }

  PchSeries = GetPchSeries ();

  for (PortNum = 0; PortNum < GetPchMaxPciePortNum (); PortNum++) {

    //
    // Check Info hob for Cycle Router avaliable
    // skip to next root port if Cr is not avaliable
    //
    CycleRouterNum = PcieStorageInfoHob->RstCycleRouterMap[PortNum / 4];
    if (CycleRouterNum == RST_PCIE_STORAGE_CR_INVALID) {
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: cycle router not avaliable on Rp = %d\n", PortNum));
      continue;
    }

    Status = GetPchPcieRpDevFun (PortNum, &RpDev, &RpFunc);
    ASSERT_EFI_ERROR (Status);

    PciRootPortRegBase  = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDev, (UINT32) RpFunc);
    RootPortLane        = RstGetRpLaneOccupyMask (PortNum);


    if (RootPortLane == 0) {
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: root port lane is not occupied\n"));
      continue;
    }

    ///
    /// Check if the RST PCIe Storage Cycle Router is enabled, continue to the next root port in the next x4 lane if it is disabled
    ///
    if (mPchConfigHob->Sata.RstPcieStorageRemap[CycleRouterNum].Enable == 0) {
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: RST PCIe Storage Cycle Router %d is disabled\n", CycleRouterNum));
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: Proceed to root port in the next x4 lane (@ port %d)\n", PortNum + 1));
      PortNum += 3 - (PortNum % 4);
      continue;
    }

    ///
    /// Check if the current root port is matching selected root port from policy, continue to next port if they are not matched
    ///
    if ((mPchConfigHob->Sata.RstPcieStorageRemap[CycleRouterNum].RstPcieStoragePort != 0) &&
        (mPchConfigHob->Sata.RstPcieStorageRemap[CycleRouterNum].RstPcieStoragePort != PortNum + 1)) {
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: root port %d not matched with selected root port, proceed to next port\n", PortNum + 1));
      continue;
    }


    // Read device programming interface from info hob
    // if no device is present continue
    if (PcieStorageInfoHob->PcieStorageLinkWidth[PortNum]) {
      DeviceInterface = PcieStorageInfoHob->PcieStorageProgrammingInterface[PortNum];
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: Device present on RP# %d, PiInterface = %d\n", PortNum + 1, DeviceInterface));
    } else {
      DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: RP# %d no device present\n", PortNum + 1));
      continue;
    }


    ///
    /// Update the remapping detail for detected PCIe storage device, and move to the root port in the next x4 lane
    ///
    if ((DeviceInterface != RST_PCIE_STORAGE_INTERFACE_NONE)) {
      mRstPcieStorageDetection[CycleRouterNum].SupportRstPcieStoragRemapping     = TRUE;
      mRstPcieStorageDetection[CycleRouterNum].RootPortNum                       = (UINT8) PortNum;
      mRstPcieStorageDetection[CycleRouterNum].RootPortLane                      = RootPortLane;
      mRstPcieStorageDetection[CycleRouterNum].DeviceInterface                   = DeviceInterface;
      PortNum                       += 3 - (PortNum % 4);
      *RemappingRequired      = TRUE;
    } else {
      continue;
    }
  }

  DEBUG ((DEBUG_INFO, "DetectPcieStorageDevices: DetectPcieStorageDevices() Ended\n"));
}


