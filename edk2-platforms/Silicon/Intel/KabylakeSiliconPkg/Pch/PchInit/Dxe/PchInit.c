/** @file
  This is the Common driver that initializes the Intel PCH.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchInit.h"

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_CONFIG_HOB           *mPchConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED SI_CONFIG_HOB            *mSiConfigHob;

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
  mSiConfigHob = (SI_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  DEBUG ((DEBUG_INFO, "PchInitEntryPointCommon() End\n"));

  return;
}

/**
  Lock USB registers before boot

  @param[in] PchConfigHob      The PCH Config HOB
**/
VOID
LockXhciConfiguration (
  IN  CONST PCH_CONFIG_HOB    *PchConfigHob
  )
{
  UINTN                       XhciPciMmBase;
  UINT32                      XhccCfg;

  DEBUG ((DEBUG_INFO, "LockXhciConfiguration () - Start\n"));

  XhciPciMmBase = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_XHCI,
                    PCI_FUNCTION_NUMBER_PCH_XHCI
                    );

  ///
  /// PCH BIOS Spec Section 13.2.4 Locking xHCI Register Settings
  /// PCH BIOS Spec Locking xHCI Register settings
  /// After xHCI is initialized, BIOS should lock the xHCI configuration registers to RO.
  /// This prevent any unintended changes. There is also a lockdown feature for OverCurrent
  /// registers. BIOS should set these bits to lock down the settings prior to end of POST.
  /// 1. Set Access Control bit at XHCI PCI offset 40h[31] to 1b to lock xHCI register settings.
  /// 2. Set OC Configuration Done bit at XHCI PCI offset 44h[31] to lock overcurrent mappings from
  ///    further changes.
  ///
  MmioOr32 (XhciPciMmBase + R_PCH_XHCI_XHCC2, (UINT32) (BIT31));
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (XhciPciMmBase + R_PCH_XHCI_XHCC2),
    1,
    (VOID *) (UINTN) (XhciPciMmBase + R_PCH_XHCI_XHCC2)
    );

  ///
  /// PCH BIOS Spec xHCI controller setup
  /// Note:
  /// XHCI PCI offset 40h is write once register.
  /// Unsupported Request Detected bit is write clear
  ///
  XhccCfg = MmioRead32 (XhciPciMmBase + R_PCH_XHCI_XHCC1);
  XhccCfg &= (UINT32) ~(B_PCH_XHCI_XHCC1_URD);
  XhccCfg |= (UINT32) (B_PCH_XHCI_XHCC1_ACCTRL);
  MmioWrite32 (XhciPciMmBase + R_PCH_XHCI_XHCC1, XhccCfg);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (XhciPciMmBase + R_PCH_XHCI_XHCC1),
    1,
    (VOID *) (UINTN) (XhciPciMmBase + R_PCH_XHCI_XHCC1)
    );

  DEBUG ((DEBUG_INFO, "LockXhciConfiguration () - End\n"));
}

/**
  Process all the lock downs
**/
VOID
ProcessAllLocks (
  VOID
  )
{
  UINTN         Index;
  UINT8         Data8;
  UINT16        Data16And;
  UINT16        Data16Or;
  UINT32        Data32;
  UINT32        Data32And;
  UINT32        Data32Or;
  UINT32        DlockValue;
  UINTN         PciLpcRegBase;
  UINTN         PciSpiRegBase;
  UINTN         PciPmcRegBase;
  UINT16        TcoBase;
  UINT32        PchPwrmBase;
  UINTN         PchSpiBar0;

  PciLpcRegBase   = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC
                      );
  PciPmcRegBase   = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_PMC,
                      PCI_FUNCTION_NUMBER_PCH_PMC
                      );
  PciSpiRegBase   = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI
                      );
  PchSpiBar0      = MmioRead32 (PciSpiRegBase + R_PCH_SPI_BAR0) &~(B_PCH_SPI_BAR0_MASK);

  PchTcoBaseGet (&TcoBase);

  //
  // Lock XHCI configuration
  //
  LockXhciConfiguration (mPchConfigHob);

  ///
  /// Set PWRMBASE + 620h [31] to lock the ST and NST PG register fields.
  ///
  ConfigurePmcStaticFunctionDisableLock ();

  ///
  /// SKL PCH BWG 7.2.4 Additional PCH DMI and OP-DMI Programming Steps
  /// Step 9.2
  /// Set PCR[DMI] + 2234h [31] = 1b.
  /// Leave this in DXE since setting it in PEI would break the ActiveBIOS module.
  ///
  Data32And = 0xFFFFFFFF;
  Data32Or = B_PCH_PCR_DMI_DMIC_SRL;
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_PCR_DMI_DMIC,
    Data32And,
    Data32Or
    );
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_DMI, R_PCH_PCR_DMI_DMIC,
    &Data32Or,
    &Data32And
    );


  ///
  /// Program the Flash Protection Range Register based on policy
  ///
  DlockValue = MmioRead32 (PchSpiBar0 + R_PCH_SPI_DLOCK);
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; ++Index) {
    if ((mPchConfigHob->ProtectRange[Index].WriteProtectionEnable ||
         mPchConfigHob->ProtectRange[Index].ReadProtectionEnable) != TRUE) {
      continue;
    }

    ///
    /// Proceed to program the register after ensure it is enabled
    ///
    Data32 = 0;
    Data32 |= (mPchConfigHob->ProtectRange[Index].WriteProtectionEnable == TRUE) ? B_PCH_SPI_PRX_WPE : 0;
    Data32 |= (mPchConfigHob->ProtectRange[Index].ReadProtectionEnable == TRUE) ? B_PCH_SPI_PRX_RPE : 0;
    Data32 |= ((UINT32) mPchConfigHob->ProtectRange[Index].ProtectedRangeLimit << N_PCH_SPI_PRX_PRL) & B_PCH_SPI_PRX_PRL_MASK;
    Data32 |= ((UINT32) mPchConfigHob->ProtectRange[Index].ProtectedRangeBase << N_PCH_SPI_PRX_PRB) & B_PCH_SPI_PRX_PRB_MASK;
    DEBUG ((DEBUG_INFO, "Protected range %d: 0x%08x \n", Index, Data32));

    DlockValue |= (UINT32) (B_PCH_SPI_DLOCK_PR0LOCKDN << Index);
    MmioWrite32 ((UINTN) (PchSpiBar0 + (R_PCH_SPI_PR0 + (Index * S_PCH_SPI_PRX))), Data32);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PchSpiBar0 + (R_PCH_SPI_PR0 + (Index * S_PCH_SPI_PRX))),
      1,
      (VOID *) (UINTN) (PchSpiBar0 + (R_PCH_SPI_PR0 + (Index * S_PCH_SPI_PRX)))
      );
  }
  //
  // Program DLOCK register
  //
  MmioWrite32 ((UINTN) (PchSpiBar0 + R_PCH_SPI_DLOCK), DlockValue);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PchSpiBar0 + R_PCH_SPI_DLOCK),
    1,
    (VOID *) (UINTN) (PchSpiBar0 + R_PCH_SPI_DLOCK)
    );

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// In PCH SPI controller the BIOS should set the Flash Configuration Lock-Down bit
  /// (SPI_BAR0 + 04[15]) at end of post.  When set to 1, those Flash Program Registers
  /// that are locked down by this FLOCKDN bit cannot be written.
  /// Please refer to the EDS for which program registers are impacted.
  ///
  MmioOr32 ((UINTN) (PchSpiBar0 + R_PCH_SPI_HSFSC), (UINT32) (B_PCH_SPI_HSFSC_FLOCKDN));
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PchSpiBar0 + R_PCH_SPI_HSFSC),
    1,
    (VOID *) (UINTN) (PchSpiBar0 + R_PCH_SPI_HSFSC)
    );

  ///
  /// SPI Flash Programming Guide Section 5.5.2 Vendor Component Lock
  /// It is strongly recommended that BIOS sets the Vendor Component Lock (VCL) bits. VCL applies
  /// the lock to both VSCC0 and VSCC1 even if VSCC0 is not used. Without the VCL bits set, it is
  /// possible to make Host/GbE VSCC register(s) changes in that can cause undesired host and
  /// integrated GbE Serial Flash functionality.
  ///
  MmioOr32 ((UINTN) (PchSpiBar0 + R_PCH_SPI_SFDP0_VSCC0), B_PCH_SPI_SFDP0_VSCC0_VCL);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PchSpiBar0 + R_PCH_SPI_SFDP0_VSCC0),
    1,
    (VOID *) (UINTN) (PchSpiBar0 + R_PCH_SPI_SFDP0_VSCC0)
    );

  ///
  /// Additional Power Management Programming
  /// Step 3
  /// Set GEN_PMCON_LOCK register, PMC PCI offset A6h = 06h, after stretch and ACPI base programming completed.
  ///
  MmioOr8 (
    (UINTN) (PciPmcRegBase + R_PCH_PMC_GEN_PMCON_B + 2),
    (UINT8) ((B_PCH_PMC_GEN_PMCON_B_SLPSX_STR_POL_LOCK | B_PCH_PMC_GEN_PMCON_B_ACPI_BASE_LOCK) >> 16)
    );
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    (UINTN) (PciPmcRegBase + R_PCH_PMC_GEN_PMCON_B + 2),
    1,
    (VOID *) (UINTN) (PciPmcRegBase + R_PCH_PMC_GEN_PMCON_B + 2)
    );

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// BIOS needs to enable the BIOS Lock Enable (BLE) feature of the PCH by setting
  /// SPI/eSPI/LPC PCI offset DCh[1] = 1b.
  /// When this bit is set, attempts to write the BIOS Write Enable (BIOSWE) bit
  /// in PCH will cause a SMI which will allow the BIOS to verify that the write is
  /// from a valid source.
  /// Remember that BIOS needs to set SPI/LPC/eSPI PCI Offset DC [0] = 0b to enable
  /// BIOS region protection before exiting the SMI handler.
  /// Also, TCO_EN bit needs to be set (SMI_EN Register, ABASE + 30h[13] = 1b) to keep
  /// BLE feature enabled after booting to the OS.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to
  /// ensure SMM protection of flash.
  /// Left to platform code to register a callback function to handle BiosWp SMI
  ///
  if (mPchConfigHob->LockDown.BiosLock == TRUE) {
    //
    // eSPI/LPC
    //
    if (! (MmioRead8 (PciLpcRegBase + R_PCH_LPC_BC) & B_PCH_LPC_BC_LE)) {
      DEBUG ((DEBUG_INFO, "Set LPC bios lock\n"));
      MmioOr8 ((UINTN) (PciLpcRegBase + R_PCH_LPC_BC), B_PCH_LPC_BC_LE);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (PciLpcRegBase + R_PCH_LPC_BC),
        1,
        (VOID *) (UINTN) (PciLpcRegBase + R_PCH_LPC_BC)
        );
    }
    //
    // SPI
    //
    if (! (MmioRead8 (PciSpiRegBase + R_PCH_SPI_BC) & B_PCH_SPI_BC_LE)) {
      DEBUG ((DEBUG_INFO, "Set SPI bios lock\n"));
      MmioOr8 ((UINTN) (PciSpiRegBase + R_PCH_SPI_BC), (UINT8) B_PCH_SPI_BC_LE);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (PciSpiRegBase + R_PCH_SPI_BC),
        1,
        (VOID *) (UINTN) (PciSpiRegBase + R_PCH_SPI_BC)
        );
    }
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
    ///
    /// LPC
    ///
    MmioOr8 ((UINTN) (PciLpcRegBase + R_PCH_LPC_BC), (UINT32) B_PCH_LPC_BC_BILD);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciLpcRegBase + R_PCH_LPC_BC),
      1,
      (VOID *) (UINTN) (PciLpcRegBase + R_PCH_LPC_BC)
      );

    ///
    /// Reads back for posted write to take effect
    ///
    Data8 = MmioRead8 ((UINTN) (PciLpcRegBase + R_PCH_LPC_BC));
    S3BootScriptSaveMemPoll  (
      S3BootScriptWidthUint8,
      (UINTN) (PciLpcRegBase + R_PCH_LPC_BC),
      &Data8,  // BitMask
      &Data8,  // BitValue
      1,          // Duration
      1           // LoopTimes
      );

    ///
    /// SPI
    ///
    MmioOr8 ((UINTN) (PciSpiRegBase + R_PCH_SPI_BC), (UINT32) B_PCH_SPI_BC_BILD);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciSpiRegBase + R_PCH_SPI_BC),
      1,
      (VOID *) (UINTN) (PciSpiRegBase + R_PCH_SPI_BC)
      );

    ///
    /// Reads back for posted write to take effect
    ///
    Data8 = MmioRead8 ((UINTN) (PciSpiRegBase + R_PCH_SPI_BC));
    S3BootScriptSaveMemPoll  (
      S3BootScriptWidthUint8,
      (UINTN) (PciSpiRegBase + R_PCH_SPI_BC),
      &Data8,     // BitMask
      &Data8,     // BitValue
      1,          // Duration
      1           // LoopTimes
      );

    ///
    /// Set PCR[DMI] + 274C[0] = 1b
    ///
    Data32And = 0xFFFFFFFF;
    Data32Or = B_PCH_PCR_DMI_BILD;
    PchPcrAndThenOr32 (PID_DMI, R_PCH_PCR_DMI_GCS, Data32And, Data32Or);
    PCH_PCR_BOOT_SCRIPT_READ_WRITE (
      S3BootScriptWidthUint32,
      PID_DMI, R_PCH_PCR_DMI_GCS,
      &Data32Or,
      &Data32And
      );
  }

  ///
  /// PCH BIOS Spec Section 5.13 BIOS guide on using RTC RAM
  /// For Data integrity protection, set RTC Memory locks (Upper 128 Byte Lock and
  /// Lower 128 Byte Lock) at SBCR[RTC] + 3400h[4] and SBCR[RTC] + 3400h[3]. Note once locked
  /// bytes 0x38 - 0x3F in each of the Upper and Lower Byte blocks, respectively,
  /// cannot be unlocked until next reset.
  ///
  if (mPchConfigHob->LockDown.RtcLock == TRUE) {
    Data32And = 0xFFFFFFFF;
    Data32Or  = (B_PCH_PCR_RTC_CONF_UCMOS_LOCK | B_PCH_PCR_RTC_CONF_LCMOS_LOCK | B_PCH_PCR_RTC_CONF_RESERVED);
    PchPcrAndThenOr32 (
      PID_RTC, R_PCH_PCR_RTC_CONF,
      Data32And,
      Data32Or
      );
    PCH_PCR_BOOT_SCRIPT_READ_WRITE (
      S3BootScriptWidthUint32,
      PID_RTC, R_PCH_PCR_RTC_CONF,
      &Data32Or,
      &Data32And
      );
  }

  ///
  /// Lock Down TCO
  ///
  Data16And = 0xFFFF;
  Data16Or  = B_PCH_TCO_CNT_LOCK;
  IoOr16 (TcoBase + R_PCH_TCO1_CNT, Data16Or);
  S3BootScriptSaveIoReadWrite (
    S3BootScriptWidthUint16,
    (UINTN) (TcoBase + R_PCH_TCO1_CNT),
    &Data16Or,  // Data to be ORed
    &Data16And  // Data to be ANDed
    );

  ///
  /// PCH BIOS Spec Section 5.15.1 Additional Chipset Initialization
  /// Step 1
  /// Set SPIBAR0 + F0h [0] to 1b
  ///
  MmioOr8 ((UINTN) (PchSpiBar0 + R_PCH_SPI_SSML), B_PCH_SPI_SSML_SSL);

  //
  // Lock Down PMC
  // Set PWRM + 0x18 [27, 22] prior to OS.
  //
  PchPwrmBaseGet (&PchPwrmBase);
  Data32Or = BIT27;
  if (mPchConfigHob->Pm.PmcReadDisable) {
    Data32Or |= BIT22;
  }
  MmioOr32 (PchPwrmBase + R_PCH_PWRM_CFG, Data32Or);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PchPwrmBase + R_PCH_PWRM_CFG),
    1,
    (VOID *) (UINTN) (PchPwrmBase + R_PCH_PWRM_CFG)
    );
}

/**
  Set eSPI BME bit
**/
VOID
ConfigureEspiBme (
  VOID
  )
{
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

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteCommon() Start\n"));

  ProcessAllLocks ();

  ///
  /// Perform remaining configuration for PCH SATA on End of DXE
  ///
  ConfigurePchSataOnEndOfDxe ();

  ///
  /// PSTHCTL (0xD00h[2]) = 1, PSTH IOSF Primary Trunk Clock Gating Enable (PSTHIOSFPTCGE)
  ///
  Data32And = 0xFFFFFFFF;
  Data32Or =  B_PCH_PCR_PSTH_PSTHIOSFPTCGE;
  PchPcrAndThenOr32 (PID_PSTH, R_PCH_PCR_PSTH_PSTHCTL, Data32And, Data32Or);
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    PID_PSTH, R_PCH_PCR_PSTH_PSTHCTL,
    &Data32Or,
    &Data32And
    );

  //
  // Set eSPI BME after PCI enumeration
  //
  ConfigureEspiBme ();

  //
  // This function process P2SB configuration
  // This MUST be executed in very end of End Of Dxe
  // All PSF access after this function will not acceptable
  //
  ConfigureP2sbSbiLock ((BOOLEAN) mPchConfigHob->P2sb.SbiUnlock);

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteCommon() End\n"));
}
