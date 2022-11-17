/** @file
  This is the Common driver that initializes the Intel PCH.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

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
#include <Private/Library/PchSpiCommonLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Private/Library/PchDmiLib.h>
#include <Private/Library/SiScheduleResetLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/BiosLockLib.h>
#include <Library/PchPcrLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>
#include <Register/PchRegsPsth.h>
#include <Register/PchRegsPmc.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_CONFIG_HOB           *mPchConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED SI_CONFIG_HOB_DATA       *mSiConfigHobData;

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

  //
  // Get Silicon Config data HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gSiConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mSiConfigHobData = (SI_CONFIG_HOB_DATA *) GET_GUID_HOB_DATA (HobPtr.Guid);

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

  PciSpiRegBase   = PCI_SEGMENT_LIB_ADDRESS (
                      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI,
                      0
                      );

  //
  // Check for SPI controller presence before programming
  //
  if (PciSegmentRead32 (PciSpiRegBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
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
  Process all the lock downs
**/
VOID
ProcessAllLocks (
  VOID
  )
{
  UINT8         Data8;
  UINT16        Data16And;
  UINT16        Data16Or;
  UINT32        Data32And;
  UINT32        Data32Or;
  UINT64        PciLpcRegBase;
  UINT16        TcoBase;
  UINT64        PciSpiRegBase;

  PciLpcRegBase   = PCI_SEGMENT_LIB_ADDRESS (
                      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC,
                      0
                      );
  PciSpiRegBase   = PCI_SEGMENT_LIB_ADDRESS (
                      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI,
                      0
                      );

  PchTcoBaseGet (&TcoBase);

  //
  // Lock function disable (ST and NST PG) register fields.
  //
  PmcLockFunctionDisableConfigWithS3BootScript ();

  ///
  /// PCH BWG Additional PCH DMI and OP-DMI Programming Steps
  /// Lock DMI.
  ///
    PchDmiSetLockWithS3BootScript ();

  //
  // Lock SPI register before boot.
  //
  LockSpiConfiguration ();

  ///
  /// Additional Power Management Programming
  /// Step 3
  /// Lock configuration after stretch and ACPI base programming completed.
  ///
  PmcLockSlpSxStretchingPolicyWithS3BootScript ();

  //
  // Set BiosLock.
  //
  if (mPchConfigHob->LockDown.BiosLock == TRUE) {
    BiosLockEnable ();
  }

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// BIOS also needs to set the BIOS Interface Lock Down bit in multiple locations
  /// (PCR[DMI] + 274Ch[0], LPC/eSPI PCI offset DCh[7] and SPI PCI offset DCh[7]).
  /// Setting these bits will prevent writes to the Top Swap bit (under their respective locations)
  /// and the Boot BIOS Straps. Enabling this bit will mitigate malicious software
  /// attempts to replace the system BIOS option ROM with its own code.
  ///
  if (mPchConfigHob->LockDown.BiosInterface == TRUE) {
    //
    // LPC
    //
    PciSegmentOr8 ((UINT64) (PciLpcRegBase + R_LPC_CFG_BC), (UINT32) B_LPC_CFG_BC_BILD);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      PcdGet64 (PcdPciExpressBaseAddress) + PciLpcRegBase + R_LPC_CFG_BC,
      1,
      (VOID *) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + PciLpcRegBase + R_LPC_CFG_BC)
      );

    //
    // Reads back for posted write to take effect
    //
    Data8 = PciSegmentRead8 ((UINTN) (PciLpcRegBase + R_LPC_CFG_BC));
    S3BootScriptSaveMemPoll  (
      S3BootScriptWidthUint8,
      PcdGet64 (PcdPciExpressBaseAddress) + PciLpcRegBase + R_LPC_CFG_BC,
      &Data8,  // BitMask
      &Data8,  // BitValue
      1,       // Duration
      1        // LoopTimes
      );

    //
    // SPI
    //
    PciSegmentOr8 ((UINT64) (PciSpiRegBase + R_SPI_CFG_BC), (UINT32) B_SPI_CFG_BC_BILD);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      PcdGet64 (PcdPciExpressBaseAddress) + PciSpiRegBase + R_SPI_CFG_BC,
      1,
      (VOID *) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + PciSpiRegBase + R_SPI_CFG_BC)
      );

    //
    // Reads back for posted write to take effect
    //
    Data8 = PciSegmentRead8 ((UINT64) (PciSpiRegBase + R_SPI_CFG_BC));
    S3BootScriptSaveMemPoll  (
      S3BootScriptWidthUint8,
      PcdGet64 (PcdPciExpressBaseAddress) + PciSpiRegBase + R_SPI_CFG_BC,
      &Data8,     // BitMask
      &Data8,     // BitValue
      1,          // Duration
      1           // LoopTimes
      );

    ///
    /// Set BIOS interface Lock-Down
    ///
    PchDmiSetBiosLockDownWithS3BootScript ();
  }

  ///
  /// PCH BIOS Spec on using RTC RAM
  /// Regardless of BUC.TS being updated or not, BIOS must set RC.BILD bit PCR[RTC] + 3400h[31] before exit
  /// For Data integrity protection, set RTC Memory locks (Upper 128 Byte Lock and
  /// Lower 128 Byte Lock) at PCR[RTC] + 3400h[4] and PCR[RTC] + 3400h[3].
  /// Note once locked bytes 0x38 - 0x3F in each of the Upper and Lower Byte blocks, respectively,
  /// cannot be unlocked until next reset.
  ///
  Data32And = 0xFFFFFFFF;
  Data32Or = 0x0;

  if (mPchConfigHob->LockDown.BiosInterface == TRUE) {
    Data32Or  = B_RTC_PCR_CONF_BILD;
  }
  if (mPchConfigHob->LockDown.RtcMemoryLock == TRUE) {
    Data32Or |= (B_RTC_PCR_CONF_UCMOS_LOCK | B_RTC_PCR_CONF_LCMOS_LOCK);
  }
  PchPcrAndThenOr32 (
    PID_RTC_HOST, R_RTC_PCR_CONF,
    Data32And,
    Data32Or
    );
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_RTC_HOST, R_RTC_PCR_CONF,
    &Data32Or,
    &Data32And
    );

  ///
  ///  Remove access to RTC PCRs
  ///
  Data32And = (UINT32)~(BIT0);
  Data32Or  = 0;
  PchPcrAndThenOr32 (
    PID_RTC_HOST, R_RTC_PCR_PG1_AC_LO,
    Data32And,
    Data32Or
    );
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_RTC_HOST, R_RTC_PCR_PG1_AC_LO,
    &Data32Or,
    &Data32And
    );
  PchPcrAndThenOr32 (
    PID_RTC_HOST, R_RTC_PCR_PG1_CP_LO,
    Data32And,
    Data32Or
    );
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_RTC_HOST, R_RTC_PCR_PG1_CP_LO,
    &Data32Or,
    &Data32And
    );

  //
  // Lock Down TCO
  //
  Data16And = 0xFFFF;
  Data16Or  = B_TCO_IO_TCO1_CNT_LOCK;
  IoOr16 (TcoBase + R_TCO_IO_TCO1_CNT, Data16Or);
  S3BootScriptSaveIoReadWrite (
    S3BootScriptWidthUint16,
    (UINTN) (TcoBase + R_TCO_IO_TCO1_CNT),
    &Data16Or,  // Data to be ORed
    &Data16And  // Data to be ANDed
    );

  ///
  /// PCH BIOS Spec Section 5.15.1 Additional Chipset Initialization
  /// Step 1
  /// Lock PMC Set Strap Message Interface
  ///
  PmcLockSetStrapMsgInterfaceWithS3BootScript ();
  //
  // Lock Down PMC
  //
  PmcLockWithS3BootScript ();
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

  EspiPciBase = PCI_SEGMENT_LIB_ADDRESS (
                  DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_LPC,
                  PCI_FUNCTION_NUMBER_PCH_LPC,
                  0
                  );

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

/**
  Common PCH initialization before Boot Sript Table is closed

**/
VOID
PchOnPciEnumCompleteCommon (
  VOID
  )
{
  UINT32                                    Data32Or;
  UINT32                                    Data32And;
  BOOLEAN                                   ResetStatus;

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteCommon() Start\n"));

  if (SiScheduleResetIsRequired ()) {
    ResetStatus = SiScheduleResetPerformReset ();
    ASSERT (!ResetStatus);
  }

  ProcessAllLocks ();

  //
  // Perform remaining configuration for PCH SATA on End of DXE
  //
  ConfigurePchSataOnEndOfDxe ();
  //
  // PSTHCTL (0xD00h[2]) = 1, PSTH IOSF Primary Trunk Clock Gating Enable (PSTHIOSFPTCGE)
  //
  Data32And = 0xFFFFFFFF;
  Data32Or =  B_PSTH_PCR_PSTHIOSFPTCGE;
  PchPcrAndThenOr32 (PID_PSTH, R_PSTH_PCR_PSTHCTL, Data32And, Data32Or);
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_PSTH, R_PSTH_PCR_PSTHCTL,
    &Data32Or,
    &Data32And
    );

  //
  // Set eSPI BME after PCI enumeration
  //
  ConfigureEspiBme ();

  ///
  /// Clear Global Reset Status, Power Failure and Host Reset Status bits
  ///
  PmcClearGlobalResetStatus ();
  PmcClearPowerFailureStatus ();
  PmcClearHostResetStatus ();

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteCommon() End\n"));
}
