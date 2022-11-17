/** @file
  This code provides a initialization of intel VT-d (Virtualization Technology for Directed I/O).

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SaInitDxe.h"
#include "SaInit.h"
#include "VTd.h"
#include <CpuRegs.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <PchInfoHob.h>
#include <PchAccess.h>


extern SA_CONFIG_HOB                                          *mSaConfigHob;

/**
  For device that specified by Device Num and Function Num,
  mDevEnMap is used to check device presence.
  0x80 means use Device ID to detemine presence
  0x8F means force to update

  The structure is used to check if device scope is valid when update DMAR table
**/
UINT16  mDevEnMap[][2] = {{0x0200, 0x80}, {0x1400, 0x80}, {0x1401, 0x80}, {0x1607, 0x8F}};

BOOLEAN mInterruptRemappingSupport;

/**
  Get the corresponding device Enable/Disable bit according DevNum and FunNum

  @param[in] DevNum  - Device Number
  @param[in] FunNum  - Function Number

  @retval If the device is found, return disable/Enable bit in FD/Deven reigster
  @retval If not found return 0xFF
**/
UINT16
GetFunDisableBit (
  UINT8 DevNum,
  UINT8 FunNum
  )
{
  UINTN Index;

  for (Index = 0; Index < sizeof (mDevEnMap) / 4; Index++) {
    if (mDevEnMap[Index][0] == ((DevNum << 0x08) | FunNum)) {
      return mDevEnMap[Index][1];
    }
  }

  return 0xFF;
}

/**
  Update the DRHD structure

  @param[in, out] DrhdEnginePtr       - A pointer to DRHD structure
**/
VOID
UpdateDrhd (
  IN OUT VOID *DrhdEnginePtr
  )
{
  UINT16                        Length;
  UINT16                        DisableBit;
  BOOLEAN                       NeedRemove;
  EFI_ACPI_DRHD_ENGINE1_STRUCT  *DrhdEngine;

  //
  // Convert DrhdEnginePtr to EFI_ACPI_DRHD_ENGINE1_STRUCT Pointer
  //
  DrhdEngine      = (EFI_ACPI_DRHD_ENGINE1_STRUCT *) DrhdEnginePtr;
  Length          = DrhdEngine->DrhdHeader.Header.Length;
  DisableBit = GetFunDisableBit (
                 DrhdEngine->DeviceScope[0].PciPath.Device,
                 DrhdEngine->DeviceScope[0].PciPath.Function
                 );
  NeedRemove = FALSE;

  if ((DisableBit == 0xFF) ||
      (DrhdEngine->DrhdHeader.RegisterBaseAddress == 0) ||
      ((DisableBit == 0x80) &&
       (PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, DrhdEngine->DeviceScope[0].PciPath.Device, DrhdEngine->DeviceScope[0].PciPath.Function, 0x00)) == 0xFFFFFFFF))
      ) {
    NeedRemove = TRUE;
  }
  if (NeedRemove) {
    Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  }
  ///
  /// If no devicescope is left, we set the structure length as 0x00
  ///
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->DrhdHeader.Flags == 0x01)) {
    DrhdEngine->DrhdHeader.Header.Length = Length;
  } else {
    DrhdEngine->DrhdHeader.Header.Length = 0;
  }
}

/**
  Get IOAPIC ID from LPC

  @retval APIC ID
**/
UINT8
GetIoApicId (
  VOID
  )
{
  UINT32                IoApicAddress;
  UINT32                IoApicId;

  IoApicAddress = PcdGet32 (PcdIoApicBaseAddress);
  ///
  /// Get current IO APIC ID
  ///
  MmioWrite8 ((UINTN) (IoApicAddress + R_IO_APIC_INDEX_OFFSET), 0);
  IoApicId = MmioRead32 ((UINTN) (IoApicAddress + R_IO_APIC_DATA_OFFSET)) >> 24;

  return (UINT8) IoApicId;
}

/**
  Update the second DRHD structure

  @param[in, out] DrhdEnginePtr       - A pointer to DRHD structure
**/
VOID
UpdateDrhd2 (
  IN OUT VOID *DrhdEnginePtr
  )
{
  UINT16                        Length;
  UINTN                         DeviceScopeNum;
  UINTN                         ValidDeviceScopeNum;
  UINT16                        Index;
  UINT8                         Bus;
  UINT8                         Path[2];
  BOOLEAN                       NeedRemove;
  EFI_ACPI_DRHD_ENGINE3_STRUCT  *DrhdEngine;
  VOID                          *HobPtr;
  PCH_INFO_HOB                  *PchInfoHob;

  ///
  /// Convert DrhdEnginePtr to EFI_ACPI_DRHD_ENGINE3_STRUCT Pointer
  ///
  DrhdEngine      = (EFI_ACPI_DRHD_ENGINE3_STRUCT *) DrhdEnginePtr;

  Length          = DrhdEngine->DrhdHeader.Header.Length;
  DeviceScopeNum  = (DrhdEngine->DrhdHeader.Header.Length - EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  Bus             = 0;
  ValidDeviceScopeNum = 0;
  Path[0]         = 0;
  Path[1]         = 0;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB *) GET_GUID_HOB_DATA (HobPtr);
  ASSERT (PchInfoHob != NULL);
  if (PchInfoHob == NULL) {
    return;
  }

  for (Index = 0; Index < DeviceScopeNum; Index++) {
    NeedRemove = FALSE;
    /**
      For HPET and APIC, update device scope if Interrupt remapping is supported. remove device scope
      if interrupt remapping is not supported.
      - Index = 0 - IOAPIC
      - Index = 1 - HPET
    **/
    if (mInterruptRemappingSupport) {
      if (Index == 0) {
        ///
        /// Update source id for IoApic's device scope entry
        ///
        Bus = (UINT8) PchInfoHob->IoApicBusNum;
        Path[0] = (UINT8) PchInfoHob->IoApicDevNum;
        Path[1] = (UINT8) PchInfoHob->IoApicFuncNum;
        DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.StartBusNumber = Bus;
        DrhdEngine->DeviceScope[Index].PciPath.Device = Path[0];
        DrhdEngine->DeviceScope[Index].PciPath.Function = Path[1];
        //
        // Update APIC ID
        //
        DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.EnumerationId = GetIoApicId ();
      }
      if (Index == 1) {
        ///
        /// Update source id for HPET's device scope entry
        ///
        Bus     = (UINT8) PchInfoHob->HpetBusNum;
        Path[0] = (UINT8) PchInfoHob->HpetDevNum;
        Path[1] = (UINT8) PchInfoHob->HpetFuncNum;
        DrhdEngine->DeviceScope[Index].DeviceScopeStructureHeader.StartBusNumber = Bus;
        DrhdEngine->DeviceScope[Index].PciPath.Device = Path[0];
        DrhdEngine->DeviceScope[Index].PciPath.Function = Path[1];
      }
    } else {
      if ((Index == 0) || (Index == 1)) {
        NeedRemove = TRUE;
      }
    }

    CopyMem (
      &DrhdEngine->DeviceScope[ValidDeviceScopeNum],
      &DrhdEngine->DeviceScope[Index],
      sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
      );
    if (NeedRemove) {
      Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
    } else {
      ValidDeviceScopeNum++;
    }
  }
  ///
  /// If no devicescope is left, we set the structure length as 0x00
  ///
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->DrhdHeader.Flags == 0x01)) {
    DrhdEngine->DrhdHeader.Header.Length = Length;
  } else {
    DrhdEngine->DrhdHeader.Header.Length = 0;
  }
}

/**
  Update the RMRR structure

  @param[in, out] RmrrPtr             - A pointer to RMRR structure
**/
VOID
UpdateRmrr (
  IN OUT VOID *RmrrPtr
  )
{
  UINT16                  Length;
  UINT16                  DisableBit;
  UINTN                   DeviceScopeNum;
  UINTN                   ValidDeviceScopeNum;
  UINTN                   Index;
  BOOLEAN                 NeedRemove;
  EFI_ACPI_RMRR_USB_STRUC *Rmrr;

  ///
  /// To make sure all devicescope can be checked,
  /// we convert the RmrrPtr to EFI_ACPI_RMRR_USB_STRUC pointer
  ///
  Rmrr                = (EFI_ACPI_RMRR_USB_STRUC *) RmrrPtr;

  Length              = Rmrr->RmrrHeader.Header.Length;
  ValidDeviceScopeNum = 0;
  DeviceScopeNum      = (Rmrr->RmrrHeader.Header.Length - EFI_ACPI_RMRR_HEADER_LENGTH) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  for (Index = 0; Index < DeviceScopeNum; Index++) {
    DisableBit = GetFunDisableBit (
                   Rmrr->DeviceScope[Index].PciPath.Device,
                   Rmrr->DeviceScope[Index].PciPath.Function
                   );
    NeedRemove = FALSE;
    if ((DisableBit == 0xFF) ||
        ((DisableBit == 0x80) &&
         (PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, Rmrr->DeviceScope[Index].PciPath.Device, Rmrr->DeviceScope[Index].PciPath.Function, 0x00)) == 0xFFFFFFFF))
        ) {
      NeedRemove = TRUE;
    } else if (DisableBit == 0x8F) {
      NeedRemove = FALSE;
    }
    CopyMem (
      &Rmrr->DeviceScope[ValidDeviceScopeNum],
      &Rmrr->DeviceScope[Index],
      sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
      );

    if (Rmrr->RmrrHeader.ReservedMemoryRegionLimitAddress == 0x0) {
      NeedRemove = TRUE;
    }

    if (NeedRemove) {
      Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
    } else {
      ValidDeviceScopeNum++;
    }
  }
  ///
  /// If No deviceScope is left, set length as 0x00
  ///
  if (Length > EFI_ACPI_RMRR_HEADER_LENGTH) {
    Rmrr->RmrrHeader.Header.Length = Length;
  } else {
    Rmrr->RmrrHeader.Header.Length = 0;
  }
}

/**
  Update the DMAR table

  @param[in, out] TableHeader         - The table to be set
  @param[in, out] Version             - Version to publish
**/
VOID
DmarTableUpdate (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER       *TableHeader,
  IN OUT   EFI_ACPI_TABLE_VERSION            *Version
  )
{
  EFI_ACPI_DMAR_TABLE *DmarTable;
  EFI_ACPI_DMAR_TABLE TempDmarTable;
  UINTN               Offset;
  UINTN               StructureLen;
  UINT64              McD0BaseAddress;
  UINTN               MchBar;
  UINT16              IgdMode;
  UINT16              GttMode;
  UINT32              IgdMemSize;
  UINT32              GttMemSize;
  EFI_STATUS          Status;
  MISC_DXE_CONFIG     *MiscDxeConfig;

  IgdMemSize  = 0;
  GttMemSize  = 0;
  DmarTable   = (EFI_ACPI_DMAR_TABLE *) TableHeader;

  Status = GetConfigBlock ((VOID *) mSaPolicy, &gMiscDxeConfigGuid, (VOID *)&MiscDxeConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set INTR_REMAP bit (BIT 0) if interrupt remapping is supported
  ///
  if (mInterruptRemappingSupport) {
    DmarTable->DmarHeader.Flags |= BIT0;
  }

  if (mSaConfigHob->VtdData.X2ApicOptOut == 1) {
    DmarTable->DmarHeader.Flags |= BIT1;
  } else {
    DmarTable->DmarHeader.Flags &= 0xFD;
  }

  ///
  /// Get OemId
  ///
  CopyMem (DmarTable->DmarHeader.Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (DmarTable->DmarHeader.Header.OemId));
  DmarTable->DmarHeader.Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  DmarTable->DmarHeader.Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  DmarTable->DmarHeader.Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  DmarTable->DmarHeader.Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  ///
  /// Calculate IGD memsize
  ///
  McD0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  MchBar          = PciSegmentRead32 (McD0BaseAddress + R_SA_MCHBAR) & ~BIT0;
  IgdMode = ((PciSegmentRead16 (McD0BaseAddress + R_SA_GGC) & B_SA_GGC_GMS_MASK) >> N_SA_GGC_GMS_OFFSET) & 0xFF;
  if (IgdMode < 0xF0) {
    IgdMemSize = IgdMode * 32 * (1024) * (1024);
  } else {
    IgdMemSize = 4 * (IgdMode - 0xF0 + 1) * (1024) * (1024);
  }
  ///
  /// Calculate GTT mem size
  ///
  GttMemSize = 0;
  GttMode = (PciSegmentRead16 (McD0BaseAddress + R_SA_GGC) & B_SA_GGC_GGMS_MASK) >> N_SA_GGC_GGMS_OFFSET;
  if (GttMode <= V_SA_GGC_GGMS_8MB) {
    GttMemSize = (1 << GttMode) * (1024) * (1024);
  }

  DmarTable->RmrrIgd.RmrrHeader.ReservedMemoryRegionBaseAddress   = (PciSegmentRead32 (McD0BaseAddress + R_SA_TOLUD) & ~(0x01)) - IgdMemSize - GttMemSize;
  DmarTable->RmrrIgd.RmrrHeader.ReservedMemoryRegionLimitAddress  = DmarTable->RmrrIgd.RmrrHeader.ReservedMemoryRegionBaseAddress + IgdMemSize + GttMemSize - 1;
  DEBUG ((DEBUG_INFO, "RMRR Base  address IGD %016lX\n", DmarTable->RmrrIgd.RmrrHeader.ReservedMemoryRegionBaseAddress));
  DEBUG ((DEBUG_INFO, "RMRR Limit address IGD %016lX\n", DmarTable->RmrrIgd.RmrrHeader.ReservedMemoryRegionLimitAddress));

  DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionBaseAddress   = MiscDxeConfig->RmrrUsbBaseAddress[0];
  DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionLimitAddress  = MiscDxeConfig->RmrrUsbBaseAddress[1];

  ///
  /// Convert to 4KB alignment.
  ///
  if (DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionLimitAddress != 0x0) {
    DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionBaseAddress  &= (EFI_PHYSICAL_ADDRESS) ~0xFFF;
    DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionLimitAddress &= (EFI_PHYSICAL_ADDRESS) ~0xFFF;
    DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionLimitAddress += 0x1000-1;
  }

  DEBUG ((DEBUG_INFO, "RMRR Base  address USB %016lX\n", DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionBaseAddress));
  DEBUG ((DEBUG_INFO, "RMRR Limit address USB %016lX\n", DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionLimitAddress));

  if (DmarTable->RmrrUsb.RmrrHeader.ReservedMemoryRegionBaseAddress == 0) {
    DEBUG ((DEBUG_WARN, "WARNING:  RmrrUsb.RmrrHeader.ReservedMemoryRegionBaseAddress is 0.\n"));
  }

  DmarTable->RmrrCsme.RmrrHeader.ReservedMemoryRegionBaseAddress   = MiscDxeConfig->RmrrCsmeBaseAddress[0];
  DmarTable->RmrrCsme.RmrrHeader.ReservedMemoryRegionLimitAddress  = MiscDxeConfig->RmrrCsmeBaseAddress[1];
  DEBUG ((DEBUG_INFO, "RMRR Base  address CSME %016lX\n", DmarTable->RmrrCsme.RmrrHeader.ReservedMemoryRegionBaseAddress));
  DEBUG ((DEBUG_INFO, "RMRR Limit address CSME %016lX\n", DmarTable->RmrrCsme.RmrrHeader.ReservedMemoryRegionLimitAddress));
  ///
  /// Update DRHD structures of DmarTable
  ///
  DmarTable->DrhdEngine1.DrhdHeader.RegisterBaseAddress = (MmioRead32 (MchBar + R_SA_MCHBAR_VTD1_OFFSET) &~1);
  DmarTable->DrhdEngine3.DrhdHeader.RegisterBaseAddress = (MmioRead32 (MchBar + R_SA_MCHBAR_VTD3_OFFSET) &~1);

  DEBUG ((DEBUG_INFO, "VTD base address1 %x\n", DmarTable->DrhdEngine1.DrhdHeader.RegisterBaseAddress));
  DEBUG ((DEBUG_INFO, "VTD base address3 %x\n", DmarTable->DrhdEngine3.DrhdHeader.RegisterBaseAddress));
  ///
  /// copy DmarTable to TempDmarTable to be processed
  ///
  CopyMem (&TempDmarTable, DmarTable, sizeof (EFI_ACPI_DMAR_TABLE));

  ///
  /// Update DRHD structures of temp DMAR table
  ///
  UpdateDrhd (&TempDmarTable.DrhdEngine1);
  UpdateDrhd2 (&TempDmarTable.DrhdEngine3);

  ///
  /// Update RMRR structures of temp DMAR table
  ///
  UpdateRmrr ((VOID *) &TempDmarTable.RmrrUsb);
  UpdateRmrr ((VOID *) &TempDmarTable.RmrrIgd);
  UpdateRmrr ((VOID *) &TempDmarTable.RmrrCsme);

  ///
  /// Remove unused device scope or entire DRHD structures
  ///
  Offset = (UINTN) (&TempDmarTable.DrhdEngine1);
  if (TempDmarTable.DrhdEngine1.DrhdHeader.Header.Length != 0) {
    Offset += TempDmarTable.DrhdEngine1.DrhdHeader.Header.Length;
  }
  if (TempDmarTable.DrhdEngine3.DrhdHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.DrhdEngine3.DrhdHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.DrhdEngine3, TempDmarTable.DrhdEngine3.DrhdHeader.Header.Length);
    Offset += StructureLen;
  }
  ///
  /// Remove unused device scope or entire RMRR structures
  ///
  if (TempDmarTable.RmrrUsb.RmrrHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.RmrrUsb.RmrrHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.RmrrUsb, TempDmarTable.RmrrUsb.RmrrHeader.Header.Length);
    Offset += StructureLen;
  }
  if (TempDmarTable.RmrrIgd.RmrrHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.RmrrIgd.RmrrHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.RmrrIgd, TempDmarTable.RmrrIgd.RmrrHeader.Header.Length);
    Offset += StructureLen;
  }
  if (TempDmarTable.RmrrCsme.RmrrHeader.Header.Length != 0) {
    StructureLen = TempDmarTable.RmrrCsme.RmrrHeader.Header.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.RmrrCsme, TempDmarTable.RmrrCsme.RmrrHeader.Header.Length);
    Offset += StructureLen;
  }

  Offset = Offset - (UINTN) &TempDmarTable;
  ///
  /// Re-calculate DMAR table check sum
  ///
  TempDmarTable.DmarHeader.Header.Checksum = (UINT8) (TempDmarTable.DmarHeader.Header.Checksum + TempDmarTable.DmarHeader.Header.Length - Offset);
  ///
  /// Set DMAR table length
  ///
  TempDmarTable.DmarHeader.Header.Length = (UINT32) Offset;
  ///
  /// Replace DMAR table with rebuilt table TempDmarTable
  ///
  CopyMem ((VOID *) DmarTable, (VOID *) &TempDmarTable, TempDmarTable.DmarHeader.Header.Length);
}

/**
  PciEnumerationComplete routine for update DMAR
**/
VOID
UpdateDmarPciEnumCompleteCallback (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      *HandleBuffer;
  UINTN                           NumberOfHandles;
  EFI_FV_FILETYPE                 FileType;
  UINT32                          FvStatus;
  EFI_FV_FILE_ATTRIBUTES          Attributes;
  UINTN                           Size;
  UINTN                           i;
  INTN                            Instance;
  EFI_ACPI_TABLE_VERSION          Version;
  EFI_ACPI_COMMON_HEADER          *CurrentTable;
  UINTN                           AcpiTableHandle;
  EFI_FIRMWARE_VOLUME2_PROTOCOL   *FwVol;
  EFI_ACPI_TABLE_PROTOCOL         *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER     *VtdAcpiTable;
  STATIC BOOLEAN                  Triggered = FALSE;


  if (Triggered) {
    return;
  }

  Triggered     = TRUE;

  FwVol         = NULL;
  AcpiTable     = NULL;
  VtdAcpiTable  = NULL;

  DEBUG ((DEBUG_INFO, "UpdateDmarPciEnumCompleteCallback \n"));


  ///
  /// Fix DMAR Table always created, skip install when disabled
  ///
  if ((mSaConfigHob->VtdData.VtdDisable == TRUE) || (PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, R_SA_MC_CAPID0_A_OFFSET)) & BIT23)) {
    DEBUG ((DEBUG_INFO, "Vtd Disabled, skip DMAR Table install\n"));
    return;
  }


  ///
  /// Locate ACPI support protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Locate protocol.
  /// There is little chance we can't find an FV protocol
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Looking for FV with ACPI storage file
  ///
  for (i = 0; i < NumberOfHandles; i++) {
    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[i],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);

    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gSaAcpiTableStorageGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    ///
    /// If we found it, then we are done
    ///
    if (Status == EFI_SUCCESS) {
      break;
    }
  }
  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);
  if (FwVol == NULL) {
    return;
  }
  ///
  /// By default, a table belongs in all ACPI table versions published.
  ///
  Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  ///
  /// Read tables from the storage file.
  ///
  Instance      = 0;
  CurrentTable  = NULL;

  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gSaAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the Signature ID to modify the table
      ///
      switch (((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->Signature) {

        case EFI_ACPI_VTD_DMAR_TABLE_SIGNATURE:
          VtdAcpiTable = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;
          DmarTableUpdate (VtdAcpiTable, &Version);
          break;

        default:
          break;
      }
      ///
      /// Increment the instance
      ///
      Instance++;
      CurrentTable = NULL;
    }
  }
  ///
  /// Update the VTD table in the ACPI tables.
  ///
  AcpiTableHandle = 0;
  if (VtdAcpiTable != NULL) {
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          VtdAcpiTable,
                          VtdAcpiTable->Length,
                          &AcpiTableHandle
                          );
    FreePool (VtdAcpiTable);
  }
}

/**
  Locate the VT-d ACPI tables data file and read ACPI SSDT tables.
  Publish the appropriate SSDT based on current configuration and capabilities.

  @param[in] SaPolicy     -  SA DXE Policy protocol

  @retval EFI_SUCCESS     - Vtd initialization complete
  @exception EFI_UNSUPPORTED - Vtd is not enabled by policy
**/
EFI_STATUS
VtdInit (
  IN  SA_POLICY_PROTOCOL    *SaPolicy
  )
{
  EFI_STATUS                      Status;
  UINT64                          McD0BaseAddress;
  UINT64                          McD2BaseAddress;
  UINTN                           MchBar;
  SYSTEM_AGENT_NVS_AREA_PROTOCOL  *SaNvsAreaProtocol;

  mInterruptRemappingSupport  = FALSE;
  mSaConfigHob       = NULL;
  mSaConfigHob = GetFirstGuidHob (&gSaConfigHobGuid);
  if (mSaConfigHob != NULL) {
    mInterruptRemappingSupport  = mSaConfigHob->VtdData.InterruptRemappingSupport;
  }

  ///
  ///  Locate the SA Global NVS Protocol.
  ///
  Status = gBS->LocateProtocol (
                  &gSaNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &SaNvsAreaProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  McD0BaseAddress  = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  McD2BaseAddress  = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_IGD_BUS, SA_IGD_DEV, SA_IGD_FUN_0, 0);
  mSaPolicy        = SaPolicy;
  MchBar           = PciSegmentRead32(McD0BaseAddress + R_SA_MCHBAR) & ~BIT0;

  if (mSaConfigHob != NULL) {
    SaNvsAreaProtocol->Area->VtdDisable = mSaConfigHob->VtdData.VtdDisable;
  }
  SaNvsAreaProtocol->Area->VtdBaseAddress1 = (MmioRead32(MchBar + R_SA_MCHBAR_VTD1_OFFSET) &~1);
  SaNvsAreaProtocol->Area->VtdBaseAddress3 = (MmioRead32(MchBar + R_SA_MCHBAR_VTD3_OFFSET) &~1);
  SaNvsAreaProtocol->Area->VtdEngine1Vid = PciSegmentRead16(McD2BaseAddress + PCI_VENDOR_ID_OFFSET);

  if (mSaConfigHob != NULL) {
    if ((mSaConfigHob->VtdData.VtdDisable) || (PciSegmentRead32 (McD0BaseAddress + R_SA_MC_CAPID0_A_OFFSET) & BIT23)) {
      DEBUG ((DEBUG_WARN, "VTd disabled or no capability!\n"));
      return EFI_UNSUPPORTED;
    }
  }
  ///
  /// Check SA supports VTD and VTD is enabled in setup menu
  ///
  DEBUG ((DEBUG_INFO, "VTd enabled\n"));

  return EFI_SUCCESS;
}
