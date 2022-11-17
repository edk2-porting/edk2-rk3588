/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/Vtd.h>
#include <Ppi/VtdInfo.h>
#include <Ppi/VtdNullRootEntryTable.h>
#include <Ppi/IoMmu.h>
#include "IntelVTdDmarPei.h"

/**
  Flush VTD page table and context table memory.

  This action is to make sure the IOMMU engine can get final data in memory.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  Base              The base address of memory to be flushed.
  @param[in]  Size              The size of memory in bytes to be flushed.
**/
VOID
FlushPageTableMemory (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN UINTN                      Base,
  IN UINTN                      Size
  )
{
  if (VTdUnitInfo->ECapReg.Bits.C == 0) {
    WriteBackDataCacheRange ((VOID *) Base, Size);
  }
}

/**
  Flush VTd engine write buffer.

  @param[in]  VtdUnitBaseAddress        The base address of the VTd engine.
**/
VOID
FlushWriteBuffer (
  IN UINTN                      VtdUnitBaseAddress
  )
{
  UINT32                        Reg32;
  VTD_CAP_REG                   CapReg;

  CapReg.Uint64 = MmioRead64 (VtdUnitBaseAddress + R_CAP_REG);

  if (CapReg.Bits.RWBF != 0) {
    Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
    MmioWrite32 (VtdUnitBaseAddress + R_GCMD_REG, Reg32 | B_GMCD_REG_WBF);
    do {
      Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
    } while ((Reg32 & B_GSTS_REG_WBF) != 0);
  }
}

/**
  Perpare cache invalidation interface.

  @param[in]  VTdUnitInfo       The VTd engine unit information.

  @retval EFI_SUCCESS           The operation was successful.
  @retval EFI_UNSUPPORTED       Invalidation method is not supported.
  @retval EFI_OUT_OF_RESOURCES  A memory allocation failed.
**/
EFI_STATUS
PerpareCacheInvalidationInterface (
  IN VTD_UNIT_INFO *VTdUnitInfo
  )
{
  UINT16         QueueSize;
  UINT64         Reg64;
  UINT32         Reg32;
  VTD_ECAP_REG   ECapReg;


  if (VTdUnitInfo->VerReg.Bits.Major <= 6) {
    VTdUnitInfo->EnableQueuedInvalidation = 0;
    DEBUG ((DEBUG_INFO, "Use Register-based Invalidation Interface for engine [0x%x]\n", VTdUnitInfo->VtdUnitBaseAddress));
    return EFI_SUCCESS;
  }

  ECapReg.Uint64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_ECAP_REG);
  if (ECapReg.Bits.QI == 0) {
    DEBUG ((DEBUG_ERROR, "Hardware does not support queued invalidations interface for engine [0x%x]\n", VTdUnitInfo->VtdUnitBaseAddress));
    return EFI_UNSUPPORTED;
  }

  VTdUnitInfo->EnableQueuedInvalidation = 1;
  DEBUG ((DEBUG_INFO, "Use Queued Invalidation Interface for engine [0x%x]\n", VTdUnitInfo->VtdUnitBaseAddress));

  Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  if ((Reg32 & B_GSTS_REG_QIES) != 0) {
    DEBUG ((DEBUG_INFO,"Queued Invalidation Interface was enabled.\n"));
    Reg32 &= (~B_GSTS_REG_QIES);
    MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GCMD_REG, Reg32);
    do {
      Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
    } while ((Reg32 & B_GSTS_REG_QIES) != 0);
    MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_IQA_REG, 0);

    if (VTdUnitInfo->QiDesc != NULL) {
      FreePages(VTdUnitInfo->QiDesc, EFI_SIZE_TO_PAGES(sizeof(QI_DESC) * VTdUnitInfo->QiDescLength));
      VTdUnitInfo->QiDesc = NULL;
      VTdUnitInfo->QiDescLength = 0;
    }
  }

  //
  // Initialize the Invalidation Queue Tail Register to zero.
  //
  MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_IQT_REG, 0);

  //
  // Setup the IQ address, size and descriptor width through the Invalidation Queue Address Register
  //
  QueueSize = 0;
  VTdUnitInfo->QiDescLength = 1 << (QueueSize + 8);
  VTdUnitInfo->QiDesc = (QI_DESC *) AllocatePages (EFI_SIZE_TO_PAGES(sizeof(QI_DESC) * VTdUnitInfo->QiDescLength));

  if (VTdUnitInfo->QiDesc == NULL) {
    VTdUnitInfo->QiDescLength = 0;
    DEBUG ((DEBUG_ERROR,"Could not Alloc Invalidation Queue Buffer.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "Invalidation Queue Length : %d\n", VTdUnitInfo->QiDescLength));
  Reg64 = (UINT64)(UINTN)VTdUnitInfo->QiDesc;
  Reg64 |= QueueSize;
  MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_IQA_REG, Reg64);

  //
  // Enable the queued invalidation interface through the Global Command Register.
  // When enabled, hardware sets the QIES field in the Global Status Register.
  //
  Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  Reg32 |= B_GMCD_REG_QIE;
  MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GCMD_REG, Reg32);
  DEBUG ((DEBUG_INFO, "Enable Queued Invalidation Interface. GCMD_REG = 0x%x\n", Reg32));
  do {
    Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  } while ((Reg32 & B_GSTS_REG_QIES) == 0);

  VTdUnitInfo->QiFreeHead = 0;

  return EFI_SUCCESS;
}

/**
  Disable queued invalidation interface.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
**/
VOID
DisableQueuedInvalidationInterface (
  IN VTD_UNIT_INFO *VTdUnitInfo
  )
{
  UINT32  Reg32;

  if (VTdUnitInfo->EnableQueuedInvalidation != 0) {
    Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
    Reg32 &= (~B_GMCD_REG_QIE);
    MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GCMD_REG, Reg32);
    DEBUG ((DEBUG_INFO, "Disable Queued Invalidation Interface. GCMD_REG = 0x%x\n", Reg32));
    do {
      Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
    } while ((Reg32 & B_GSTS_REG_QIES) != 0);

    if (VTdUnitInfo->QiDesc != NULL) {
      FreePages(VTdUnitInfo->QiDesc, EFI_SIZE_TO_PAGES(sizeof(QI_DESC) * VTdUnitInfo->QiDescLength));
      VTdUnitInfo->QiDesc = NULL;
      VTdUnitInfo->QiDescLength = 0;
    }

    VTdUnitInfo->EnableQueuedInvalidation = 0;
  }
}

/**
  Check Queued Invalidation Fault.

  @param[in]  VTdUnitInfo       The VTd engine unit information.

  @retval EFI_SUCCESS           The operation was successful.
  @retval RETURN_DEVICE_ERROR   A fault is detected.
**/
EFI_STATUS
QueuedInvalidationCheckFault (
  IN VTD_UNIT_INFO *VTdUnitInfo
  )
{
  UINT32     FaultReg;

  FaultReg = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_FSTS_REG);

  if (FaultReg & B_FSTS_REG_IQE) {
    DEBUG((DEBUG_ERROR, "Detect Invalidation Queue Error [0x%08x]\n", FaultReg));
    FaultReg |= B_FSTS_REG_IQE;
    MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_FSTS_REG, FaultReg);
    return RETURN_DEVICE_ERROR;
  }

  if (FaultReg & B_FSTS_REG_ITE) {
    DEBUG((DEBUG_ERROR, "Detect Invalidation Time-out Error [0x%08x]\n", FaultReg));
    FaultReg |= B_FSTS_REG_ITE;
    MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_FSTS_REG, FaultReg);
    return RETURN_DEVICE_ERROR;
  }

  if (FaultReg & B_FSTS_REG_ICE) {
    DEBUG((DEBUG_ERROR, "Detect Invalidation Completion Error [0x%08x]\n", FaultReg));
    FaultReg |= B_FSTS_REG_ICE;
    MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_FSTS_REG, FaultReg);
    return RETURN_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Submit the queued invalidation descriptor to the remapping
   hardware unit and wait for its completion.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  Desc              The invalidate descriptor

  @retval EFI_SUCCESS           The operation was successful.
  @retval RETURN_DEVICE_ERROR   A fault is detected.
  @retval EFI_INVALID_PARAMETER Parameter is invalid.
**/
EFI_STATUS
SubmitQueuedInvalidationDescriptor (
  IN VTD_UNIT_INFO *VTdUnitInfo,
  IN QI_DESC       *Desc
  )
{
  EFI_STATUS Status;
  UINT16     QiDescLength;
  QI_DESC    *BaseDesc;
  UINT64     Reg64Iqt;
  UINT64     Reg64Iqh;

  if (Desc == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  QiDescLength = VTdUnitInfo->QiDescLength;
  BaseDesc = VTdUnitInfo->QiDesc;

  DEBUG((DEBUG_INFO, "[0x%x] Submit QI Descriptor [0x%08x, 0x%08x]\n", VTdUnitInfo->VtdUnitBaseAddress, Desc->Low, Desc->High));

  BaseDesc[VTdUnitInfo->QiFreeHead].Low = Desc->Low;
  BaseDesc[VTdUnitInfo->QiFreeHead].High = Desc->High;
  FlushPageTableMemory(VTdUnitInfo, (UINTN) &BaseDesc[VTdUnitInfo->QiFreeHead], sizeof(QI_DESC));

  DEBUG((DEBUG_INFO,"QI Free Head=0x%x\n", VTdUnitInfo->QiFreeHead));
  VTdUnitInfo->QiFreeHead = (VTdUnitInfo->QiFreeHead + 1) % QiDescLength;

  Reg64Iqh = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_IQH_REG);
  //
  // Update the HW tail register indicating the presence of new descriptors.
  //
  Reg64Iqt = VTdUnitInfo->QiFreeHead << DMAR_IQ_SHIFT;
  MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_IQT_REG, Reg64Iqt);

  Status = EFI_SUCCESS;
  do {
    Status = QueuedInvalidationCheckFault(VTdUnitInfo);
    if (Status != EFI_SUCCESS) {
      DEBUG((DEBUG_ERROR,"Detect Queued Invalidation Fault.\n"));
      break;
    }

    Reg64Iqh = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_IQH_REG);
  } while (Reg64Iqt != Reg64Iqh);

  DEBUG((DEBUG_ERROR,"SubmitQueuedInvalidationDescriptor end\n"));
  return Status;
}

/**
  Invalidate VTd context cache.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
**/
EFI_STATUS
InvalidateContextCache (
  IN VTD_UNIT_INFO              *VTdUnitInfo
  )
{
  UINT64                        Reg64;
  QI_DESC                       QiDesc;

  if (VTdUnitInfo->EnableQueuedInvalidation == 0) {
    //
    // Register-based Invalidation
    //
    Reg64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_CCMD_REG);
    if ((Reg64 & B_CCMD_REG_ICC) != 0) {
      DEBUG ((DEBUG_ERROR,"ERROR: InvalidateContextCache: B_CCMD_REG_ICC is set for VTD(%x)\n", (UINTN)VTdUnitInfo->VtdUnitBaseAddress));
      return EFI_DEVICE_ERROR;
    }

    Reg64 &= ((~B_CCMD_REG_ICC) & (~B_CCMD_REG_CIRG_MASK));
    Reg64 |= (B_CCMD_REG_ICC | V_CCMD_REG_CIRG_GLOBAL);
    MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_CCMD_REG, Reg64);

    do {
      Reg64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_CCMD_REG);
    } while ((Reg64 & B_CCMD_REG_ICC) != 0);
  } else {
    //
    // Queued Invalidation
    //
    QiDesc.Low = QI_CC_FM(0) | QI_CC_SID(0) | QI_CC_DID(0) | QI_CC_GRAN(1) | QI_CC_TYPE;
    QiDesc.High = 0;

    return SubmitQueuedInvalidationDescriptor(VTdUnitInfo, &QiDesc);
  }

  return EFI_SUCCESS;
}

/**
  Invalidate VTd IOTLB.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
**/
EFI_STATUS
InvalidateIOTLB (
  IN VTD_UNIT_INFO              *VTdUnitInfo
  )
{
  UINT64                        Reg64;
  VTD_ECAP_REG                  ECapReg;
  QI_DESC                       QiDesc;

  if (VTdUnitInfo->EnableQueuedInvalidation == 0) {
    //
    // Register-based Invalidation
    //
    ECapReg.Uint64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_ECAP_REG);

    Reg64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + (ECapReg.Bits.IRO * 16) + R_IOTLB_REG);
     if ((Reg64 & B_IOTLB_REG_IVT) != 0) {
       DEBUG ((DEBUG_ERROR, "ERROR: InvalidateIOTLB: B_IOTLB_REG_IVT is set for VTD(%x)\n", (UINTN)VTdUnitInfo->VtdUnitBaseAddress));
       return EFI_DEVICE_ERROR;
    }

    Reg64 &= ((~B_IOTLB_REG_IVT) & (~B_IOTLB_REG_IIRG_MASK));
    Reg64 |= (B_IOTLB_REG_IVT | V_IOTLB_REG_IIRG_GLOBAL);
    MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + (ECapReg.Bits.IRO * 16) + R_IOTLB_REG, Reg64);

    do {
      Reg64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + (ECapReg.Bits.IRO * 16) + R_IOTLB_REG);
    } while ((Reg64 & B_IOTLB_REG_IVT) != 0);
  } else {
    //
    // Queued Invalidation
    //
    ECapReg.Uint64 = MmioRead64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_ECAP_REG);
    QiDesc.Low = QI_IOTLB_DID(0) | QI_IOTLB_DR(CAP_READ_DRAIN(ECapReg.Uint64)) | QI_IOTLB_DW(CAP_WRITE_DRAIN(ECapReg.Uint64)) | QI_IOTLB_GRAN(1) | QI_IOTLB_TYPE;
    QiDesc.High = QI_IOTLB_ADDR(0) | QI_IOTLB_IH(0) | QI_IOTLB_AM(0);

    return SubmitQueuedInvalidationDescriptor(VTdUnitInfo, &QiDesc);
  }

  return EFI_SUCCESS;
}

/**
  Enable DMAR translation inpre-mem phase.

  @param[in]  VtdUnitBaseAddress  The base address of the VTd engine.
  @param[in]  RootEntryTable      The address of the VTd RootEntryTable.

  @retval EFI_SUCCESS             DMAR translation is enabled.
  @retval EFI_DEVICE_ERROR        DMAR translation is not enabled.
**/
EFI_STATUS
EnableDmarPreMem (
  IN UINTN                        VtdUnitBaseAddress,
  IN UINTN                        RootEntryTable
  )
{
  UINT32                          Reg32;

  DEBUG ((DEBUG_INFO, ">>>>>>EnableDmarPreMem() for engine [%x] \n", VtdUnitBaseAddress));

  DEBUG ((DEBUG_INFO, "RootEntryTable 0x%x \n", RootEntryTable));
  MmioWrite64 (VtdUnitBaseAddress + R_RTADDR_REG, (UINT64) (UINTN) RootEntryTable);

  Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  MmioWrite32 (VtdUnitBaseAddress + R_GCMD_REG, Reg32 | B_GMCD_REG_SRTP);

  DEBUG ((DEBUG_INFO, "EnableDmarPreMem: waiting for RTPS bit to be set... \n"));
  do {
    Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  } while((Reg32 & B_GSTS_REG_RTPS) == 0);
  DEBUG ((DEBUG_INFO, "EnableDmarPreMem: R_GSTS_REG = 0x%x \n", Reg32));

  //
  // Init DMAr Fault Event and Data registers
  //
  Reg32 = MmioRead32 (VtdUnitBaseAddress + R_FEDATA_REG);

  //
  // Write Buffer Flush before invalidation
  //
  FlushWriteBuffer (VtdUnitBaseAddress);

  //
  // Enable VTd
  //
  Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  MmioWrite32 (VtdUnitBaseAddress + R_GCMD_REG, Reg32 | B_GMCD_REG_TE);
  DEBUG ((DEBUG_INFO, "EnableDmarPreMem: Waiting B_GSTS_REG_TE ...\n"));
  do {
    Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  } while ((Reg32 & B_GSTS_REG_TE) == 0);

  DEBUG ((DEBUG_INFO, "VTD () enabled!<<<<<<\n"));

  return EFI_SUCCESS;
}

/**
  Enable DMAR translation.

  @param[in]  VTdUnitInfo       The VTd engine unit information.
  @param[in]  RootEntryTable    The address of the VTd RootEntryTable.

  @retval EFI_SUCCESS           DMAR translation is enabled.
  @retval EFI_DEVICE_ERROR      DMAR translation is not enabled.
**/
EFI_STATUS
EnableDmar (
  IN VTD_UNIT_INFO              *VTdUnitInfo,
  IN UINTN                      RootEntryTable
  )
{
  UINT32                        Reg32;

  DEBUG ((DEBUG_INFO, ">>>>>>EnableDmar() for engine [%x] \n", VTdUnitInfo->VtdUnitBaseAddress));

  DEBUG ((DEBUG_INFO, "RootEntryTable 0x%x \n", RootEntryTable));
  MmioWrite64 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_RTADDR_REG, (UINT64) (UINTN) RootEntryTable);

  Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GCMD_REG, Reg32 | B_GMCD_REG_SRTP);

  DEBUG ((DEBUG_INFO, "EnableDmar: waiting for RTPS bit to be set... \n"));
  do {
    Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  } while((Reg32 & B_GSTS_REG_RTPS) == 0);
  DEBUG ((DEBUG_INFO, "EnableDmar: R_GSTS_REG = 0x%x \n", Reg32));

  //
  // Init DMAr Fault Event and Data registers
  //
  Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_FEDATA_REG);

  //
  // Write Buffer Flush before invalidation
  //
  FlushWriteBuffer ((UINTN)VTdUnitInfo->VtdUnitBaseAddress);

  //
  // Invalidate the context cache
  //
  InvalidateContextCache (VTdUnitInfo);

  //
  // Invalidate the IOTLB cache
  //
  InvalidateIOTLB (VTdUnitInfo);

  //
  // Enable VTd
  //
  Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  MmioWrite32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GCMD_REG, Reg32 | B_GMCD_REG_TE);
  DEBUG ((DEBUG_INFO, "EnableDmar: Waiting B_GSTS_REG_TE ...\n"));
  do {
    Reg32 = MmioRead32 ((UINTN)VTdUnitInfo->VtdUnitBaseAddress + R_GSTS_REG);
  } while ((Reg32 & B_GSTS_REG_TE) == 0);

  DEBUG ((DEBUG_INFO, "VTD () enabled!<<<<<<\n"));

  return EFI_SUCCESS;
}

/**
  Disable DMAR translation.

  @param[in] VtdUnitBaseAddress         The base address of the VTd engine.

  @retval EFI_SUCCESS           DMAR translation is disabled.
  @retval EFI_DEVICE_ERROR      DMAR translation is not disabled.
**/
EFI_STATUS
DisableDmar (
  IN UINTN                      VtdUnitBaseAddress
  )
{
  UINT32                        Reg32;
  UINT32                        Status;
  UINT32                        Command;

  DEBUG ((DEBUG_INFO, ">>>>>>DisableDmar() for engine [%x] \n", VtdUnitBaseAddress));

  //
  // Write Buffer Flush before invalidation
  //
  FlushWriteBuffer (VtdUnitBaseAddress);

  //
  // Disable Dmar
  //
  //
  // Set TE (Translation Enable: BIT31) of Global command register to zero
  //
  Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  Status = (Reg32 & 0x96FFFFFF);       // Reset the one-shot bits
  Command = (Status & ~B_GMCD_REG_TE);
  MmioWrite32 (VtdUnitBaseAddress + R_GCMD_REG, Command);

   //
   // Poll on TE Status bit of Global status register to become zero
   //
   do {
     Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
   } while ((Reg32 & B_GSTS_REG_TE) == B_GSTS_REG_TE);

  //
  // Set SRTP (Set Root Table Pointer: BIT30) of Global command register in order to update the root table pointerDisable VTd
  //
  Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  Status = (Reg32 & 0x96FFFFFF);       // Reset the one-shot bits
  Command = (Status | B_GMCD_REG_SRTP);
  MmioWrite32 (VtdUnitBaseAddress + R_GCMD_REG, Command);
  do {
    Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  } while((Reg32 & B_GSTS_REG_RTPS) == 0);

  Reg32 = MmioRead32 (VtdUnitBaseAddress + R_GSTS_REG);
  DEBUG((DEBUG_INFO, "DisableDmar: GSTS_REG - 0x%08x\n", Reg32));

  MmioWrite64 (VtdUnitBaseAddress + R_RTADDR_REG, 0);

  DEBUG ((DEBUG_INFO,"VTD () Disabled!<<<<<<\n"));

  return EFI_SUCCESS;
}

/**
  Dump VTd version registers.

  @param[in]  VerReg            The version register.
**/
VOID
DumpVtdVerRegs (
  IN VTD_VER_REG                *VerReg
  )
{
  DEBUG ((DEBUG_INFO, "  VerReg:\n", VerReg->Uint32));
  DEBUG ((DEBUG_INFO, "    Major    - 0x%x\n", VerReg->Bits.Major));
  DEBUG ((DEBUG_INFO, "    Minor    - 0x%x\n", VerReg->Bits.Minor));
}

/**
  Dump VTd capability registers.

  @param[in]  CapReg            The capability register.
**/
VOID
DumpVtdCapRegs (
  IN VTD_CAP_REG                *CapReg
  )
{
  DEBUG ((DEBUG_INFO, "  CapReg:\n", CapReg->Uint64));
  DEBUG ((DEBUG_INFO, "    ND     - 0x%x\n", CapReg->Bits.ND));
  DEBUG ((DEBUG_INFO, "    AFL    - 0x%x\n", CapReg->Bits.AFL));
  DEBUG ((DEBUG_INFO, "    RWBF   - 0x%x\n", CapReg->Bits.RWBF));
  DEBUG ((DEBUG_INFO, "    PLMR   - 0x%x\n", CapReg->Bits.PLMR));
  DEBUG ((DEBUG_INFO, "    PHMR   - 0x%x\n", CapReg->Bits.PHMR));
  DEBUG ((DEBUG_INFO, "    CM     - 0x%x\n", CapReg->Bits.CM));
  DEBUG ((DEBUG_INFO, "    SAGAW  - 0x%x\n", CapReg->Bits.SAGAW));
  DEBUG ((DEBUG_INFO, "    MGAW   - 0x%x\n", CapReg->Bits.MGAW));
  DEBUG ((DEBUG_INFO, "    ZLR    - 0x%x\n", CapReg->Bits.ZLR));
  DEBUG ((DEBUG_INFO, "    FRO    - 0x%x\n", CapReg->Bits.FRO));
  DEBUG ((DEBUG_INFO, "    SLLPS  - 0x%x\n", CapReg->Bits.SLLPS));
  DEBUG ((DEBUG_INFO, "    PSI    - 0x%x\n", CapReg->Bits.PSI));
  DEBUG ((DEBUG_INFO, "    NFR    - 0x%x\n", CapReg->Bits.NFR));
  DEBUG ((DEBUG_INFO, "    MAMV   - 0x%x\n", CapReg->Bits.MAMV));
  DEBUG ((DEBUG_INFO, "    DWD    - 0x%x\n", CapReg->Bits.DWD));
  DEBUG ((DEBUG_INFO, "    DRD    - 0x%x\n", CapReg->Bits.DRD));
  DEBUG ((DEBUG_INFO, "    FL1GP  - 0x%x\n", CapReg->Bits.FL1GP));
  DEBUG ((DEBUG_INFO, "    PI     - 0x%x\n", CapReg->Bits.PI));
}

/**
  Dump VTd extended capability registers.

  @param[in]  ECapReg           The extended capability register.
**/
VOID
DumpVtdECapRegs (
  IN VTD_ECAP_REG               *ECapReg
  )
{
  DEBUG ((DEBUG_INFO, "  ECapReg:\n", ECapReg->Uint64));
  DEBUG ((DEBUG_INFO, "    C      - 0x%x\n", ECapReg->Bits.C));
  DEBUG ((DEBUG_INFO, "    QI     - 0x%x\n", ECapReg->Bits.QI));
  DEBUG ((DEBUG_INFO, "    DT     - 0x%x\n", ECapReg->Bits.DT));
  DEBUG ((DEBUG_INFO, "    IR     - 0x%x\n", ECapReg->Bits.IR));
  DEBUG ((DEBUG_INFO, "    EIM    - 0x%x\n", ECapReg->Bits.EIM));
  DEBUG ((DEBUG_INFO, "    PT     - 0x%x\n", ECapReg->Bits.PT));
  DEBUG ((DEBUG_INFO, "    SC     - 0x%x\n", ECapReg->Bits.SC));
  DEBUG ((DEBUG_INFO, "    IRO    - 0x%x\n", ECapReg->Bits.IRO));
  DEBUG ((DEBUG_INFO, "    MHMV   - 0x%x\n", ECapReg->Bits.MHMV));
  DEBUG ((DEBUG_INFO, "    ECS    - 0x%x\n", ECapReg->Bits.ECS));
  DEBUG ((DEBUG_INFO, "    MTS    - 0x%x\n", ECapReg->Bits.MTS));
  DEBUG ((DEBUG_INFO, "    NEST   - 0x%x\n", ECapReg->Bits.NEST));
  DEBUG ((DEBUG_INFO, "    DIS    - 0x%x\n", ECapReg->Bits.DIS));
  DEBUG ((DEBUG_INFO, "    PASID  - 0x%x\n", ECapReg->Bits.PASID));
  DEBUG ((DEBUG_INFO, "    PRS    - 0x%x\n", ECapReg->Bits.PRS));
  DEBUG ((DEBUG_INFO, "    ERS    - 0x%x\n", ECapReg->Bits.ERS));
  DEBUG ((DEBUG_INFO, "    SRS    - 0x%x\n", ECapReg->Bits.SRS));
  DEBUG ((DEBUG_INFO, "    NWFS   - 0x%x\n", ECapReg->Bits.NWFS));
  DEBUG ((DEBUG_INFO, "    EAFS   - 0x%x\n", ECapReg->Bits.EAFS));
  DEBUG ((DEBUG_INFO, "    PSS    - 0x%x\n", ECapReg->Bits.PSS));
}


/**
  Enable VTd translation table protection for all.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  EngineMask        The mask of the VTd engine to be accessed.
**/
VOID
EnableVTdTranslationProtectionAll (
  IN VTD_INFO                   *VTdInfo,
  IN UINT64                     EngineMask
  )
{
  EFI_STATUS                                Status;
  EDKII_VTD_NULL_ROOT_ENTRY_TABLE_PPI       *RootEntryTable;
  UINTN                                     Index;

  DEBUG ((DEBUG_INFO, "EnableVTdTranslationProtectionAll - 0x%lx\n", EngineMask));

  Status = PeiServicesLocatePpi (
                 &gEdkiiVTdNullRootEntryTableGuid,
                 0,
                 NULL,
                 (VOID **)&RootEntryTable
                 );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate Null Root Entry Table Ppi Failed : %r\n", Status));
    ASSERT (FALSE);
    return;
  }

  for (Index = 0; Index < VTdInfo->VTdEngineCount; Index++) {
    if ((EngineMask & LShiftU64(1, Index)) == 0) {
      continue;
    }

    VTdInfo->VtdUnitInfo[Index].VerReg.Uint32 = MmioRead32 (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress + R_VER_REG);
    DumpVtdVerRegs (&VTdInfo->VtdUnitInfo[Index].VerReg);
    VTdInfo->VtdUnitInfo[Index].CapReg.Uint64 = MmioRead64 (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress + R_CAP_REG);
    DumpVtdCapRegs (&VTdInfo->VtdUnitInfo[Index].CapReg);
    VTdInfo->VtdUnitInfo[Index].ECapReg.Uint64 = MmioRead64 (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress + R_ECAP_REG);
    DumpVtdECapRegs (&VTdInfo->VtdUnitInfo[Index].ECapReg);

    EnableDmarPreMem (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress, (UINTN) *RootEntryTable);
  }

  return;
}

/**
  Enable VTd translation table protection.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           DMAR translation is enabled.
  @retval EFI_DEVICE_ERROR      DMAR translation is not enabled.
**/
EFI_STATUS
EnableVTdTranslationProtection (
  IN VTD_INFO                   *VTdInfo
  )
{
  EFI_STATUS                    Status;
  UINTN                         VtdIndex;

  for (VtdIndex = 0; VtdIndex < VTdInfo->VTdEngineCount; VtdIndex++) {
    if (VTdInfo->VtdUnitInfo[VtdIndex].ExtRootEntryTable != 0) {
      DEBUG ((DEBUG_INFO, "EnableVtdDmar (%d) ExtRootEntryTable 0x%x\n", VtdIndex, VTdInfo->VtdUnitInfo[VtdIndex].ExtRootEntryTable));
      Status = EnableDmar (&VTdInfo->VtdUnitInfo[VtdIndex], VTdInfo->VtdUnitInfo[VtdIndex].ExtRootEntryTable);
    } else {
      DEBUG ((DEBUG_INFO, "EnableVtdDmar (%d) RootEntryTable 0x%x\n", VtdIndex, VTdInfo->VtdUnitInfo[VtdIndex].RootEntryTable));
      Status = EnableDmar (&VTdInfo->VtdUnitInfo[VtdIndex], VTdInfo->VtdUnitInfo[VtdIndex].RootEntryTable);
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "EnableVtdDmar (%d) Failed !\n", VtdIndex));
      return Status;
    }
  }
  return EFI_SUCCESS;
}

/**
  Disable VTd translation table protection.

  @param[in]  VTdInfo           The VTd engine context information.
  @param[in]  EngineMask        The mask of the VTd engine to be accessed.
**/
VOID
DisableVTdTranslationProtection (
  IN VTD_INFO                   *VTdInfo,
  IN UINT64                     EngineMask
  )
{
  UINTN                         Index;

  DEBUG ((DEBUG_INFO, "DisableVTdTranslationProtection - 0x%lx\n", EngineMask));

  for (Index = 0; Index < VTdInfo->VTdEngineCount; Index++) {
    if ((EngineMask & LShiftU64(1, Index)) == 0) {
      continue;
    }
    DisableDmar ((UINTN) VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress);

    DisableQueuedInvalidationInterface(&VTdInfo->VtdUnitInfo[Index]);
  }

  return;
}

/**
  Prepare VTD cache invalidation configuration.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           Prepare Vtd config success
**/
EFI_STATUS
PrepareVtdCacheInvalidationConfig (
  IN VTD_INFO                   *VTdInfo
  )
{
  UINTN                         Index;
  EFI_STATUS                    Status;

  for (Index = 0; Index < VTdInfo->VTdEngineCount; Index++) {
    Status = PerpareCacheInvalidationInterface(&VTdInfo->VtdUnitInfo[Index]);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Prepare VTD configuration.

  @param[in]  VTdInfo           The VTd engine context information.

  @retval EFI_SUCCESS           Prepare Vtd config success
**/
EFI_STATUS
PrepareVtdConfig (
  IN VTD_INFO                   *VTdInfo
  )
{
  UINTN                         Index;
  UINTN                         DomainNumber;

  for (Index = 0; Index < VTdInfo->VTdEngineCount; Index++) {
    DEBUG ((DEBUG_ERROR, "Dump VTd Capability (%d)\n", Index));
    VTdInfo->VtdUnitInfo[Index].VerReg.Uint32 = MmioRead32 (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress + R_VER_REG);
    DumpVtdVerRegs (&VTdInfo->VtdUnitInfo[Index].VerReg);
    VTdInfo->VtdUnitInfo[Index].CapReg.Uint64 = MmioRead64 (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress + R_CAP_REG);
    DumpVtdCapRegs (&VTdInfo->VtdUnitInfo[Index].CapReg);
    VTdInfo->VtdUnitInfo[Index].ECapReg.Uint64 = MmioRead64 (VTdInfo->VtdUnitInfo[Index].VtdUnitBaseAddress + R_ECAP_REG);
    DumpVtdECapRegs (&VTdInfo->VtdUnitInfo[Index].ECapReg);

    VTdInfo->VtdUnitInfo[Index].Is5LevelPaging = FALSE;
    if ((VTdInfo->VtdUnitInfo[Index].CapReg.Bits.SAGAW & BIT2) != 0) {
      DEBUG ((DEBUG_INFO, "Support 4-level page-table on VTD %d\n", Index));
    }
    if ((VTdInfo->VtdUnitInfo[Index].CapReg.Bits.SAGAW & BIT3) != 0) {
      DEBUG((DEBUG_INFO, "Support 5-level page-table on VTD %d\n", Index));
      VTdInfo->VtdUnitInfo[Index].Is5LevelPaging = TRUE;

      if ((VTdInfo->HostAddressWidth <= 48) &&
          ((VTdInfo->VtdUnitInfo[Index].CapReg.Bits.SAGAW & BIT2) != 0)) {
        DEBUG ((DEBUG_INFO, "Rollback to 4-level page-table on VTD %d\n", Index));
        VTdInfo->VtdUnitInfo[Index].Is5LevelPaging = FALSE;
      }
    }
    if ((VTdInfo->VtdUnitInfo[Index].CapReg.Bits.SAGAW & (BIT3 | BIT2)) == 0) {
      DEBUG ((DEBUG_ERROR, "!!!! Page-table type 0x%X is not supported on VTD %d !!!!\n", Index, VTdInfo->VtdUnitInfo[Index].CapReg.Bits.SAGAW));
      return EFI_UNSUPPORTED;
    }

    DomainNumber = (UINTN)1 << (UINT8) ((UINTN) VTdInfo->VtdUnitInfo[Index].CapReg.Bits.ND * 2 + 4);
    if (VTdInfo->VtdUnitInfo[Index].PciDeviceInfo.PciDeviceDataNumber >= DomainNumber) {
      DEBUG ((DEBUG_ERROR, "!!!! Pci device Number(0x%x) >= DomainNumber(0x%x) !!!!\n", VTdInfo->VtdUnitInfo[Index].PciDeviceInfo.PciDeviceDataNumber, DomainNumber));
      return EFI_UNSUPPORTED;
    }
  }
  return EFI_SUCCESS;
}

