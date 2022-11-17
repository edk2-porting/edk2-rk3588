/** @file
  This code provides a initialization of intel VT-d (Virtualization Technology for Directed I/O).
  
Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "SaInitDxe.h"
#include "SaInit.h"
#include "VTd.h"
#include <CpuRegs.h>
#include <SaConfigHob.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchP2sbLib.h>
#include <Library/PchInfoLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED SA_POLICY_PROTOCOL              *mSaPolicy;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_CONFIG_HOB                  *mPchConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SERIES                      mPchSeries;
GLOBAL_REMOVE_IF_UNREFERENCED SA_CONFIG_HOB                   *SaConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddI2C0Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddI2C1Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddI2C2Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddI2C3Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddI2C4Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddI2C5Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddSpi0Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddSpi1Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddUa00Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddUa01Needed;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                         mAnddUa02Needed;

/**
  For device that specified by Device Num and Function Num,
  mDevEnMap is used to check device presence.
  0x80 means use Device ID to detemine presence

  The structure is used to check if device scope is valid when update DMAR table
**/
UINT16  mDevEnMap[][2] = {{0x0200, 0x80}, {0x1400, 0x80}, {0x1401, 0x80}};

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
  UINTN                         DeviceScopeNum;
  BOOLEAN                       NeedRemove;
  EFI_ACPI_DRHD_ENGINE1_STRUCT  *DrhdEngine;

  //
  // Convert DrhdEnginePtr to EFI_ACPI_DRHD_ENGINE1_STRUCT Pointer
  //
  DrhdEngine      = (EFI_ACPI_DRHD_ENGINE1_STRUCT *) DrhdEnginePtr;
  Length          = DrhdEngine->Length;
  DeviceScopeNum  = (DrhdEngine->Length - EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  DisableBit = GetFunDisableBit (
                 DrhdEngine->DeviceScope[0].PciPath[0],
                 DrhdEngine->DeviceScope[0].PciPath[1]
                 );
  NeedRemove = FALSE;
  if ((DisableBit == 0xFF) ||
      (DrhdEngine->RegisterBaseAddress == 0) ||
      ((DisableBit == 0x80) &&
       (MmioRead32 (MmPciBase (0, DrhdEngine->DeviceScope[0].PciPath[0], DrhdEngine->DeviceScope[0].PciPath[1]) + 0x00) == 0xFFFFFFFF))
      ) {
    NeedRemove = TRUE;
  }
  if (NeedRemove) {
    Length -= sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  }
  ///
  /// If no devicescope is left, we set the structure length as 0x00
  ///
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->Flags == 0x01)) {
    DrhdEngine->Length = Length;
  } else {
    DrhdEngine->Length = 0;
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
  UINT8 volatile  *IoapicIndex;
  UINT32 volatile *IoapicData;
  UINT32          IoApicIndexAddr;
  UINT32          IoApicDataAddr;
  UINT32          Data32;

  ///
  /// Get IOAPIC base
  ///
  PchIoApicBaseGet (&IoApicIndexAddr, &IoApicDataAddr);
  IoapicIndex = (UINT8 *) (UINTN) IoApicIndexAddr;
  IoapicData  = (UINT32 *) (UINTN) IoApicDataAddr;

  ///
  /// Get APIC ID from Identification Register (Index 0)
  ///
  *IoapicIndex  = 0;
  Data32        = (*IoapicData & 0x0F000000) >> 24;

  return (UINT8) Data32;
}

/**
  Initialize some data required for ANDD table generation.

**/
VOID
AnddTablePreInit (
  VOID
  )
{
  /*
  For Serial IO devices, they do not appear in PCI space, use platform policy or SPT SKU to determine existence
    - I2C0  (D21: F0)
    - I2C1  (D21: F1)
    - I2C2  (D21: F2) (LP Only)
    - I2C3  (D21: F3) (LP Only)
    - I2C4  (D25: F2) (LP Only)
    - I2C5  (D25: F1) (LP Only)
    - SPI0  (D30: F2)
    - SPI1  (D30: F3)
    - UART0 (D30: F0)
    - UART1 (D30: F1)
    - UART2 (D25: F0)
    - PEMC  (D30: F4) (LP Only) (PCI mode only)
    - PSDO  (D30: F5) (LP Only) (Deprecated)
    - PSDC  (D30: F6) (LP Only) (PCI mode only)
  */
  mAnddI2C0Needed = TRUE;
  mAnddI2C1Needed = TRUE;
  mAnddI2C2Needed = TRUE;
  mAnddI2C3Needed = TRUE;
  mAnddI2C4Needed = TRUE;
  mAnddI2C5Needed = TRUE;
  mAnddSpi0Needed = TRUE;
  mAnddSpi1Needed = TRUE;
  mAnddUa00Needed = TRUE;
  mAnddUa01Needed = TRUE;
  mAnddUa02Needed = TRUE;

  if (mPchConfigHob->SerialIo.DevMode[0] != 2) {
    mAnddI2C0Needed = FALSE;
    if (mPchConfigHob->SerialIo.DevMode[0] == 0) {
      //
      // Function 0 disabled so no sub-functions
      //
      mAnddI2C1Needed = FALSE;
      mAnddI2C2Needed = FALSE;
      mAnddI2C3Needed = FALSE;
    }
  }
  if (mPchConfigHob->SerialIo.DevMode[1] != 2) {
    mAnddI2C1Needed = FALSE;
  }
  if ((mPchSeries != PchLp) || (mPchConfigHob->SerialIo.DevMode[2] != 2)) {
    mAnddI2C2Needed = FALSE;
  }
  if ((mPchSeries != PchLp) || (mPchConfigHob->SerialIo.DevMode[3] != 2)) {
    mAnddI2C3Needed = FALSE;
  }
  if ((mPchSeries != PchLp) || (mPchConfigHob->SerialIo.DevMode[4] != 2)) {
    mAnddI2C4Needed = FALSE;
  }
  if ((mPchSeries != PchLp) || (mPchConfigHob->SerialIo.DevMode[5] != 2)) {
    mAnddI2C5Needed = FALSE;
  }
  if (mPchConfigHob->SerialIo.DevMode[6] != 2) {
    mAnddSpi0Needed = FALSE;
  }
  if (mPchConfigHob->SerialIo.DevMode[7] != 2) {
    mAnddSpi1Needed = FALSE;
  }
  if (mPchConfigHob->SerialIo.DevMode[8] != 2) {
    mAnddUa00Needed = FALSE;
    if (mPchConfigHob->SerialIo.DevMode[8] == 0) {
      //
      // Function 0 disabled so no sub-functions
      //
      mAnddUa01Needed = FALSE;
      mAnddSpi0Needed = FALSE;
      mAnddSpi1Needed = FALSE;
    }
  }
  if (mPchConfigHob->SerialIo.DevMode[9] != 2) {
    mAnddUa01Needed = FALSE;
  }
  if (mPchConfigHob->SerialIo.DevMode[10] != 2) {
    mAnddUa02Needed = FALSE;
    if (mPchConfigHob->SerialIo.DevMode[10] == 0) {
      //
      // Function 0 disabled so no sub-functions
      //
      mAnddI2C4Needed = FALSE;
      mAnddI2C5Needed = FALSE;
    }
  }
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
  UINT16                        Data16;
  UINT16                        Index;
  UINT8                         Bus;
  UINT8                         Path[2];
  BOOLEAN                       NeedRemove;
  EFI_ACPI_DRHD_ENGINE2_STRUCT  *DrhdEngine;

  ///
  /// Convert DrhdEnginePtr to EFI_ACPI_DRHD_ENGINE2_STRUCT Pointer
  ///
  DrhdEngine      = (EFI_ACPI_DRHD_ENGINE2_STRUCT *) DrhdEnginePtr;

  Length          = DrhdEngine->Length;
  DeviceScopeNum  = (DrhdEngine->Length - EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  Data16          = 0;
  Bus             = 0;
  ValidDeviceScopeNum = 0;
  Path[0]         = 0;
  Path[1]         = 0;

  AnddTablePreInit ();

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
        PchP2sbCfgGet16 (R_PCH_P2SB_IBDF, &Data16);
        Bus     = (UINT8) (Data16 >> 8);
        if (Bus != 0x00) {
          Path[0] = (UINT8) ((Data16 & 0xff) >> 3);
          Path[1] = (UINT8) (Data16 & 0x7);
        } else {
          DEBUG ((DEBUG_WARN, "P2SB APIC Bus, Device and Function numbers were not programmed yet or invalid, use hardcoding values instead!\n"));
          Bus     = 0xF0;
          Path[0] = 0x1F;
          Path[1] = 0x0;
        }
        DrhdEngine->DeviceScope[Index].StartBusNumber = Bus;
        DrhdEngine->DeviceScope[Index].PciPath[0] = Path[0];
        DrhdEngine->DeviceScope[Index].PciPath[1] = Path[1];
        //
        // Update APIC ID
        //
        DrhdEngine->DeviceScope[Index].EnumId = GetIoApicId ();
      }
      if (Index == 1) {
        ///
        /// Update source id for HPET's device scope entry
        ///
        PchP2sbCfgGet16 (R_PCH_P2SB_HBDF, &Data16);
        Bus     = (UINT8) (Data16 >> 8);
        Path[0] = (UINT8) ((Data16 & 0xFF) >> 3);
        Path[1] = (UINT8) (Data16 & 0x7);
        DrhdEngine->DeviceScope[Index].StartBusNumber = Bus;
        DrhdEngine->DeviceScope[Index].PciPath[0] = Path[0];
        DrhdEngine->DeviceScope[Index].PciPath[1] = Path[1];
      }
    } else {
      if ((Index == 0) || (Index == 1)) {
        NeedRemove = TRUE;
      }
    }

    switch (Index) {
      case 0:
      case 1:
        break;
      case 2:
        if (mAnddI2C0Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 3:
        if (mAnddI2C1Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 4:
        if (mAnddI2C2Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 5:
        if (mAnddI2C3Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 6:
        if (mAnddI2C4Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 7:
        if (mAnddI2C5Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 8:
        if (mAnddSpi0Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 9:
        if (mAnddSpi1Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 10:
        if (mAnddUa00Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 11:
        if (mAnddUa01Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      case 12:
        if (mAnddUa02Needed == FALSE) {
          NeedRemove = TRUE;
        }
        break;
      default:
        NeedRemove = TRUE;
        break;
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
  if ((Length > EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH) || (DrhdEngine->Flags == 0x01)) {
    DrhdEngine->Length = Length;
  } else {
    DrhdEngine->Length = 0;
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

  Length              = Rmrr->Length;
  ValidDeviceScopeNum = 0;
  DeviceScopeNum      = (Rmrr->Length - EFI_ACPI_RMRR_HEADER_LENGTH) / sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE);
  for (Index = 0; Index < DeviceScopeNum; Index++) {
    DisableBit = GetFunDisableBit (
                   Rmrr->DeviceScope[Index].PciPath[0],
                   Rmrr->DeviceScope[Index].PciPath[1]
                   );
    NeedRemove = FALSE;
    if ((DisableBit == 0xFF) ||
        ((DisableBit == 0x80) &&
         (MmioRead32 (MmPciBase (0, Rmrr->DeviceScope[Index].PciPath[0], Rmrr->DeviceScope[Index].PciPath[1]) + 0x00) == 0xFFFFFFFF))
        ) {
      NeedRemove = TRUE;
    }
    CopyMem (
      &Rmrr->DeviceScope[ValidDeviceScopeNum],
      &Rmrr->DeviceScope[Index],
      sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE)
      );

    if (Rmrr->RmrLimitAddress == 0x0) {
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
    Rmrr->Length = Length;
  } else {
    Rmrr->Length = 0;
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
  UINTN               McD0BaseAddress;
  UINT32              MchBar;
  UINT16              IgdMode;
  UINT16              GttMode;
  UINT32              IgdMemSize;
  UINT32              GttMemSize;
  EFI_STATUS          Status;
  MISC_DXE_CONFIG     *MiscDxeConfig;
  CPU_GENERATION      CpuGeneration;
  UINT64              OemTableId;

  IgdMemSize  = 0;
  GttMemSize  = 0;
  DmarTable   = (EFI_ACPI_DMAR_TABLE *) TableHeader;
  CpuGeneration = GetCpuGeneration ();
  //
  // The Default OEM Table ID is "KBL "
  //
  OemTableId  = KBL_OEM_TABLE_ID;

  Status = GetConfigBlock ((VOID *) mSaPolicy, &gMiscDxeConfigGuid, (VOID *)&MiscDxeConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set INTR_REMAP bit (BIT 0) if interrupt remapping is supported
  ///
  if (mInterruptRemappingSupport) {
    DmarTable->Flags |= BIT0;
  }

  if (SaConfigHob->VtdData.X2ApicOptOut == 1) {
    DmarTable->Flags |= BIT1;
  } else {
    DmarTable->Flags &= 0xFD;
  }

  ///
  /// Get OemId
  ///
  CopyMem (DmarTable->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (DmarTable->Header.OemId));

  if (CpuGeneration == EnumSklCpu) {
    //
    // Use "SKL " as OEM Table ID if the CpuGeneration is SKL
    //
    OemTableId = SKL_OEM_TABLE_ID;
  }
  //
  // Update OEM Table ID
  //
  DmarTable->Header.OemTableId = OemTableId;

  ///
  /// Calculate IGD memsize
  ///
  McD0BaseAddress = MmPciBase (SA_MC_BUS, 0, 0);
  MchBar          = (UINT32) MmioRead64 (McD0BaseAddress + R_SA_MCHBAR) & ~BIT0;
  IgdMode = ((MmioRead16 (McD0BaseAddress + R_SA_GGC) & B_SKL_SA_GGC_GMS_MASK) >> N_SKL_SA_GGC_GMS_OFFSET) & 0xFF;
  if (IgdMode < 0xF0) {
    IgdMemSize = IgdMode * 32 * (1024) * (1024);
  } else {
    IgdMemSize = 4 * (IgdMode - 0xF0 + 1) * (1024) * (1024);
  }
  ///
  /// Calculate GTT mem size
  ///
  GttMemSize = 0;
  GttMode = (MmioRead16 (McD0BaseAddress + R_SA_GGC) & B_SKL_SA_GGC_GGMS_MASK) >> N_SKL_SA_GGC_GGMS_OFFSET;
  if (GttMode <= V_SKL_SA_GGC_GGMS_8MB) {
    GttMemSize = (1 << GttMode) * (1024) * (1024);
  }

  DmarTable->RmrrIgd.RmrBaseAddress   = (MmioRead32 (McD0BaseAddress + R_SA_TOLUD) & ~(0x01)) - IgdMemSize - GttMemSize;
  DmarTable->RmrrIgd.RmrLimitAddress  = DmarTable->RmrrIgd.RmrBaseAddress + IgdMemSize + GttMemSize - 1;
  DEBUG ((DEBUG_INFO, "RMRR Base  address IGD %016lX\n", DmarTable->RmrrIgd.RmrBaseAddress));
  DEBUG ((DEBUG_INFO, "RMRR Limit address IGD %016lX\n", DmarTable->RmrrIgd.RmrLimitAddress));

  DmarTable->RmrrUsb.RmrBaseAddress   = MiscDxeConfig->RmrrUsbBaseAddress[0];
  DmarTable->RmrrUsb.RmrLimitAddress  = MiscDxeConfig->RmrrUsbBaseAddress[1];

  ///
  /// Convert to 4KB alignment.
  ///
  if (DmarTable->RmrrUsb.RmrLimitAddress != 0x0) {
    DmarTable->RmrrUsb.RmrBaseAddress  &= (EFI_PHYSICAL_ADDRESS) ~0xFFF;
    DmarTable->RmrrUsb.RmrLimitAddress &= (EFI_PHYSICAL_ADDRESS) ~0xFFF;
    DmarTable->RmrrUsb.RmrLimitAddress += 0x1000-1;
  }

  DEBUG ((DEBUG_INFO, "RMRR Base  address USB %016lX\n", DmarTable->RmrrUsb.RmrBaseAddress));
  DEBUG ((DEBUG_INFO, "RMRR Limit address USB %016lX\n", DmarTable->RmrrUsb.RmrLimitAddress));

  if (DmarTable->RmrrUsb.RmrBaseAddress == 0) {
    DEBUG ((DEBUG_WARN, "WARNING:  RmrrUsb.RmrBaseAddress is 0.\n"));
  }
  ///
  /// Update DRHD structures of DmarTable
  ///
  DmarTable->DrhdEngine1.RegisterBaseAddress = (MmioRead32 (MchBar + R_SA_MCHBAR_VTD1_OFFSET) &~1);
  DmarTable->DrhdEngine2.RegisterBaseAddress = (MmioRead32 (MchBar + R_SA_MCHBAR_VTD2_OFFSET) &~1);

  DEBUG ((DEBUG_INFO, "VTD base address1 %x\n", DmarTable->DrhdEngine1.RegisterBaseAddress));
  DEBUG ((DEBUG_INFO, "VTD base address2 %x\n", DmarTable->DrhdEngine2.RegisterBaseAddress));
  ///
  /// copy DmarTable to TempDmarTable to be processed
  ///
  CopyMem (&TempDmarTable, DmarTable, sizeof (EFI_ACPI_DMAR_TABLE));

  UpdateDrhd (&TempDmarTable.DrhdEngine1);
  UpdateDrhd2 (&TempDmarTable.DrhdEngine2);

  ///
  /// Update RMRR structures of temp DMAR table
  ///
  UpdateRmrr ((VOID *) &TempDmarTable.RmrrUsb);
  UpdateRmrr ((VOID *) &TempDmarTable.RmrrIgd);

  ///
  /// Remove unused device scope or entire DRHD structures
  ///
  Offset = (UINTN) (&TempDmarTable.DrhdEngine1);
  if (TempDmarTable.DrhdEngine1.Length != 0) {
    Offset += TempDmarTable.DrhdEngine1.Length;
  }
  if (TempDmarTable.DrhdEngine2.Length != 0) {
    StructureLen = TempDmarTable.DrhdEngine2.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.DrhdEngine2, TempDmarTable.DrhdEngine2.Length);
    Offset += StructureLen;
  }
  ///
  /// Remove unused device scope or entire RMRR structures
  ///
  if (TempDmarTable.RmrrUsb.Length != 0) {
    StructureLen = TempDmarTable.RmrrUsb.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.RmrrUsb, TempDmarTable.RmrrUsb.Length);
    Offset += StructureLen;
  }
  if (TempDmarTable.RmrrIgd.Length != 0) {
    StructureLen = TempDmarTable.RmrrIgd.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.RmrrIgd, TempDmarTable.RmrrIgd.Length);
    Offset += StructureLen;
  }

  ///
  /// Include necessary ANDD structures for SPT
  ///
  if (mAnddI2C0Needed == TRUE) {
    StructureLen = TempDmarTable.AnddI2C0.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddI2C0, TempDmarTable.AnddI2C0.Length);
    Offset += StructureLen;
  }
  if (mAnddI2C1Needed == TRUE) {
    StructureLen = TempDmarTable.AnddI2C1.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddI2C1, TempDmarTable.AnddI2C1.Length);
    Offset += StructureLen;
  }
  if (mAnddI2C2Needed == TRUE) {
    StructureLen = TempDmarTable.AnddI2C2.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddI2C2, TempDmarTable.AnddI2C2.Length);
    Offset += StructureLen;
  }
  if (mAnddI2C3Needed == TRUE) {
    StructureLen = TempDmarTable.AnddI2C3.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddI2C3, TempDmarTable.AnddI2C3.Length);
    Offset += StructureLen;
  }
  if (mAnddI2C4Needed == TRUE) {
    StructureLen = TempDmarTable.AnddI2C4.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddI2C4, TempDmarTable.AnddI2C4.Length);
    Offset += StructureLen;
  }
  if (mAnddI2C5Needed == TRUE) {
    StructureLen = TempDmarTable.AnddI2C5.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddI2C5, TempDmarTable.AnddI2C5.Length);
    Offset += StructureLen;
  }
  if (mAnddSpi0Needed == TRUE) {
    StructureLen = TempDmarTable.AnddSpi0.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddSpi0, TempDmarTable.AnddSpi0.Length);
    Offset += StructureLen;
  }
  if (mAnddSpi1Needed == TRUE) {
    StructureLen = TempDmarTable.AnddSpi1.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddSpi1, TempDmarTable.AnddSpi1.Length);
    Offset += StructureLen;
  }
  if (mAnddUa00Needed == TRUE) {
    StructureLen = TempDmarTable.AnddUa00.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddUa00, TempDmarTable.AnddUa00.Length);
    Offset += StructureLen;
  }
  if (mAnddUa01Needed == TRUE) {
    StructureLen = TempDmarTable.AnddUa01.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddUa01, TempDmarTable.AnddUa01.Length);
    Offset += StructureLen;
  }
  if (mAnddUa02Needed == TRUE) {
    StructureLen = TempDmarTable.AnddUa02.Length;
    CopyMem ((VOID *) Offset, (VOID *) &TempDmarTable.AnddUa02, TempDmarTable.AnddUa02.Length);
    Offset += StructureLen;
  }

  Offset = Offset - (UINTN) &TempDmarTable;
  ///
  /// Re-calculate DMAR table check sum
  ///
  TempDmarTable.Header.Checksum = (UINT8) (TempDmarTable.Header.Checksum + TempDmarTable.Header.Length - Offset);
  ///
  /// Set DMAR table length
  ///
  TempDmarTable.Header.Length = (UINT32) Offset;
  ///
  /// Replace DMAR table with rebuilt table TempDmarTable
  ///
  CopyMem ((VOID *) DmarTable, (VOID *) &TempDmarTable, TempDmarTable.Header.Length);
}

/**
  EndOfDxe routine for update DMAR
**/
VOID
UpdateDmarEndOfDxe (
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
  EFI_PEI_HOB_POINTERS            HobPtr;
  if (Triggered) {
    return;
  }

  Triggered     = TRUE;

  FwVol         = NULL;
  AcpiTable     = NULL;
  VtdAcpiTable  = NULL;

  ///
  /// Locate PCH Policy HOB and PCH series to support feature enabling/disabling
  ///
  //
  // Get PCH Config HOB.
  //
  HobPtr.Guid  = GetFirstGuidHob (&gPchConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchConfigHob = GET_GUID_HOB_DATA (HobPtr.Guid);

  mPchSeries    = GetPchSeries ();

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
  UINTN        McD0BaseAddress;

  mInterruptRemappingSupport  = FALSE;
  SaConfigHob       = NULL;
  SaConfigHob = GetFirstGuidHob (&gSaConfigHobGuid);
  if (SaConfigHob != NULL) {
    mInterruptRemappingSupport  = SaConfigHob->VtdData.InterruptRemappingSupport;
  }

  McD0BaseAddress             = MmPciBase (SA_MC_BUS, 0, 0);
  mSaPolicy                   = SaPolicy;

  if (((SaConfigHob != NULL) && (SaConfigHob->VtdData.VtdDisable)) || (MmioRead32 (McD0BaseAddress + R_SA_MC_CAPID0_A_OFFSET) & BIT23)) {
    DEBUG ((DEBUG_WARN, "VTd disabled or no capability!\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// Check SA supports VTD and VTD is enabled in setup menu
  ///
  DEBUG ((DEBUG_INFO, "VTd enabled\n"));

  return EFI_SUCCESS;
}
