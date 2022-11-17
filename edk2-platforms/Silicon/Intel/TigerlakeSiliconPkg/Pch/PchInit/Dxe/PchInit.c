/** @file
  This is the Common driver that initializes the Intel PCH.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/HobLib.h>

#include "PchInit.h"
#include <PchPolicyCommon.h>
#include <Library/SpiCommonLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PchDmiLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/SpiAccessPrivateLib.h>
#include <Register/PchRegs.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/SpiRegs.h>
#include <Register/PchRegsPsth.h>
#include <Register/PmcRegs.h>
#include <Register/HdaRegs.h>
#include <Library/GpioCheckConflictLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_CONFIG_HOB           *mPchConfigHob;

//
// EFI_EVENT
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT mHeciEvent;

/**
  Common PchInit Module Entry Point
**/
VOID
PchInitEntryPointCommon (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS  HobPtr;

  DEBUG ((DEBUG_INFO, "PchInitEntryPointCommon() Start\n"));

  //
  // Get PCH Config HOB.
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchConfigHob = (PCH_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  DEBUG ((DEBUG_INFO, "PchInitEntryPointCommon() End\n"));

  return;
}

/**
  Lock SPI register before boot
**/
VOID
LockSpiConfiguration (
  VOID
  )
{
  UINTN         Index;
  UINT16        Data16;
  UINT16        Data16And;
  UINT16        Data16Or;
  UINT32        Data32;
  UINT32        DlockValue;
  UINT64        PciSpiRegBase;
  UINT32        PchSpiBar0;
  UINT32        Timer;

  PciSpiRegBase = SpiPciCfgBase ();

  //
  // Check for SPI controller presence before programming
  //
  if (PciSegmentRead16 (PciSpiRegBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }

  //
  // Make sure SPI BAR0 has fixed address before writing to boot script.
  // The same base address is set in PEI and will be used during resume.
  //
  PchSpiBar0 = PCH_SPI_BASE_ADDRESS;

  PciSegmentAnd8    (PciSpiRegBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (PciSpiRegBase + R_SPI_CFG_BAR0, PchSpiBar0);
  PciSegmentOr8     (PciSpiRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Locking for security reasons only if Extended BIOS Range Decode is supported
  //
  if (IsExtendedBiosRangeDecodeSupported ()) {
    //
    // Before setting FLOCKDN lock Extended BIOS Range configuration
    // All configuration of this feature shall be done already at this moment
    //
    PciSegmentOr32 (PciSpiRegBase + R_SPI_CFG_BC, BIT28);
    S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      (UINTN) PciSpiRegBase + R_SPI_CFG_BC,
      1,
      (VOID *) (UINTN) (PciSpiRegBase + R_SPI_CFG_BC)
      );
  }

  //
  // Program the Flash Protection Range Register based on policy
  //
  DlockValue = MmioRead32 (PchSpiBar0 + R_SPI_MEM_DLOCK);
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; ++Index) {
    if ((mPchConfigHob->ProtectRange[Index].WriteProtectionEnable ||
         mPchConfigHob->ProtectRange[Index].ReadProtectionEnable) != TRUE) {
      continue;
    }

    //
    // Proceed to program the register after ensure it is enabled
    //
    Data32 = 0;
    Data32 |= (mPchConfigHob->ProtectRange[Index].WriteProtectionEnable == TRUE) ? B_SPI_MEM_PRX_WPE : 0;
    Data32 |= (mPchConfigHob->ProtectRange[Index].ReadProtectionEnable == TRUE) ? B_SPI_MEM_PRX_RPE : 0;
    Data32 |= ((UINT32) mPchConfigHob->ProtectRange[Index].ProtectedRangeLimit << N_SPI_MEM_PRX_PRL) & B_SPI_MEM_PRX_PRL_MASK;
    Data32 |= ((UINT32) mPchConfigHob->ProtectRange[Index].ProtectedRangeBase << N_SPI_MEM_PRX_PRB) & B_SPI_MEM_PRX_PRB_MASK;
    DEBUG ((DEBUG_INFO, "Protected range %d: 0x%08x \n", Index, Data32));

    DlockValue |= (UINT32) (B_SPI_MEM_DLOCK_PR0LOCKDN << Index);
    MmioWrite32 ((UINTN) (PchSpiBar0 + (R_SPI_MEM_PR0 + (Index * S_SPI_MEM_PRX))), Data32);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PchSpiBar0 + (R_SPI_MEM_PR0 + (Index * S_SPI_MEM_PRX))),
      1,
      (VOID *) (UINTN) (PchSpiBar0 + (R_SPI_MEM_PR0 + (Index * S_SPI_MEM_PRX)))
      );
  }
  //
  // Program DLOCK register
  //
  MmioWrite32 ((UINTN) (PchSpiBar0 + R_SPI_MEM_DLOCK), DlockValue);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PchSpiBar0 + R_SPI_MEM_DLOCK),
    1,
    (VOID *) (UINTN) (PchSpiBar0 + R_SPI_MEM_DLOCK)
    );

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// In PCH SPI controller the BIOS should set the Flash Configuration Lock-Down bit
  /// (SPI_BAR0 + 04[15]) at end of post.  When set to 1, those Flash Program Registers
  /// that are locked down by this FLOCKDN bit cannot be written.
  /// Please refer to the EDS for which program registers are impacted.
  /// Additionally BIOS must program SPI_BAR0 + 0x04 BIT11 (WRSDIS) to disable Write Status in HW sequencing
  ///

  //
  // Ensure there is no pending SPI trasaction before setting lock bits
  //
  Timer = 0;
  while (MmioRead16 (PchSpiBar0 + R_SPI_MEM_HSFSC) & B_SPI_MEM_HSFSC_SCIP) {
    if (Timer > SPI_WAIT_TIME) {
      //
      // SPI transaction is pending too long at this point, exit with error.
      //
      DEBUG ((DEBUG_ERROR, "SPI Cycle timeout\n"));
      ASSERT (FALSE);
      break;
    }
    MicroSecondDelay (SPI_WAIT_PERIOD);
    Timer += SPI_WAIT_PERIOD;
  }

  Data16And = B_SPI_MEM_HSFSC_SCIP;
  Data16    = 0;
  S3BootScriptSaveMemPoll (
    S3BootScriptWidthUint16,
    PchSpiBar0 + R_SPI_MEM_HSFSC,
    &Data16And,
    &Data16,
    SPI_WAIT_PERIOD,
    SPI_WAIT_TIME / SPI_WAIT_PERIOD
    );

  //
  // Clear any outstanding status
  //
  Data16Or  = B_SPI_MEM_HSFSC_SAF_DLE
            | B_SPI_MEM_HSFSC_SAF_ERROR
            | B_SPI_MEM_HSFSC_AEL
            | B_SPI_MEM_HSFSC_FCERR
            | B_SPI_MEM_HSFSC_FDONE;
  Data16And = 0xFFFF;
  MmioAndThenOr16 (PchSpiBar0 + R_SPI_MEM_HSFSC, Data16And, Data16Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint16,
    PchSpiBar0 + R_SPI_MEM_HSFSC,
    &Data16Or,
    &Data16And
    );

  //
  // Set WRSDIS
  //
  Data16Or  = B_SPI_MEM_HSFSC_WRSDIS;
  Data16And = 0xFFFF;
  MmioAndThenOr16 (PchSpiBar0 + R_SPI_MEM_HSFSC, Data16And, Data16Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint16,
    PchSpiBar0 + R_SPI_MEM_HSFSC,
    &Data16Or,
    &Data16And
    );

  //
  // Set FLOCKDN
  //
  Data16Or  = B_SPI_MEM_HSFSC_FLOCKDN;
  Data16And = 0xFFFF;
  MmioAndThenOr16 (PchSpiBar0 + R_SPI_MEM_HSFSC, Data16And, Data16Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint16,
    PchSpiBar0 + R_SPI_MEM_HSFSC,
    &Data16Or,
    &Data16And
    );

  ///
  /// SPI Flash Programming Guide Section 5.5.2 Vendor Component Lock
  /// It is strongly recommended that BIOS sets the Vendor Component Lock (VCL) bits. VCL applies
  /// the lock to both VSCC0 and VSCC1 even if VSCC0 is not used. Without the VCL bits set, it is
  /// possible to make Host/GbE VSCC register(s) changes in that can cause undesired host and
  /// integrated GbE Serial Flash functionality.
  ///
  MmioOr32 ((UINTN) (PchSpiBar0 + R_SPI_MEM_SFDP0_VSCC0), B_SPI_MEM_SFDP0_VSCC0_VCL);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PchSpiBar0 + R_SPI_MEM_SFDP0_VSCC0),
    1,
    (VOID *) (UINTN) (PchSpiBar0 + R_SPI_MEM_SFDP0_VSCC0)
    );
}

/**
  Set HD Audio PME bit
**/
VOID
ConfigureHdAudioPme (
  VOID
  )
{
  UINT64 HdaPciBase;

  HdaPciBase = HdaPciCfgBase ();

  if (PciSegmentRead16 (HdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }

  ///
  /// PME Enable for Audio controller
  ///
  if (mPchConfigHob->HdAudio.Pme == TRUE) {
    PciSegmentOr32 (HdaPciBase + R_HDA_CFG_PCS, (UINT32) B_HDA_CFG_PCS_PMEE);
  }
}

/**
  Set eSPI BME bit
**/
VOID
ConfigureEspiBme (
  VOID
  )
{
  UINT64 EspiPciBase;

  EspiPciBase = EspiPciCfgBase ();

  if (PciSegmentRead16 (EspiPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }
  if ((PciSegmentRead32 (EspiPciBase + R_ESPI_CFG_PCBC) & B_ESPI_CFG_PCBC_ESPI_EN) == 0) {
    return;
  }

  //
  // Refer to PCH BWG.
  // To enable eSPI bus mastering BIOS must enable BME in eSPI controller
  // and also set BME bit in the respective slave devices through Configuration
  // and Capabilities register of each slave using Get_Configuration and Set_Configuration functionality.
  //
  // NOTE: The setting is also done in PEI, but might be cleared by PCI bus during PCI enumeration.
  //       Therefore, reeable it after PCI enumeration done.
  //
  if (mPchConfigHob->Espi.BmeMasterSlaveEnabled == TRUE) {
    PciSegmentOr8 (EspiPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_BUS_MASTER);
  }
}
