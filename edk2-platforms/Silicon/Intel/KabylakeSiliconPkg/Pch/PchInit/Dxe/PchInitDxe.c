/** @file
  This is the Uefi driver that initializes the Intel PCH.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchInit.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE               mImageHandle;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                   mPcieIoTrapAddress;


VOID
EFIAPI
PchOnBootToOs (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


VOID
EFIAPI
PchOnExitBootServices (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


VOID
EFIAPI
PchOnReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


/**
  BIOS W/A for Si bug on PCH stepping LpA0, LpA1, LpA2, LpB0:
  allocates 1MB memory space for ISH BAR,
  overrides 4KB memory allocation done by standard PCI enumarator.

  @retval EFI_STATUS
  @retval EFI_SUCCESS             Allocation successful or stepping other that A0/A1/A2 (W/A not required)
  @retval EFI_OUT_OF_RESOURCES    Memory could not be allocated.
**/
EFI_STATUS
AllocateIshMemorySpace (
  VOID
  )
{
  EFI_STATUS              Status;
  UINTN                   PciIshRegBase;
  EFI_PHYSICAL_ADDRESS    IshMemBaseAddress;

  PciIshRegBase = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_ISH,
                    PCI_FUNCTION_NUMBER_PCH_ISH
                    );

  if ((MmioRead16 (PciIshRegBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) ||
      (GetPchSeries () == PchH) ||
      (PchStepping () > PchLpB1)) {
    return EFI_SUCCESS;
  }

  if ((PcdGet8 (PcdEfiGcdAllocateType) == EfiGcdAllocateMaxAddressSearchBottomUp) || (PcdGet8 (PcdEfiGcdAllocateType) == EfiGcdAllocateMaxAddressSearchTopDown)) {
    IshMemBaseAddress = 0xffffffff;
  }
  Status = gDS->AllocateMemorySpace (
                  PcdGet8 (PcdEfiGcdAllocateType),
                  EfiGcdMemoryTypeMemoryMappedIo,
                  N_PCH_ISH_BAR0_ALIGNMENT,
                  V_PCH_ISH_BAR0_SIZE,
                  &IshMemBaseAddress,
                  mImageHandle,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  MmioAnd16 (PciIshRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  MmioWrite32 (PciIshRegBase + R_PCH_ISH_BAR0_LOW, (UINT32) IshMemBaseAddress);
  MmioWrite32 (PciIshRegBase + R_PCH_ISH_BAR0_HIGH, 0);
  MmioOr16 (PciIshRegBase + PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);

  DEBUG ((DEBUG_INFO, "ISH W/A: Allocated 1MB MMIO, BAR0 = 0x%08x\n", (UINT32) IshMemBaseAddress));
  return Status;
}


/**
  Process all the lock downs
**/
VOID
ProcessSmiLocks (
  VOID
  )
{
  UINT32        Data32And;
  UINT32        Data32Or;
  UINT16        ABase;
  UINTN         PciPmcRegBase;

  PciPmcRegBase   = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_PMC,
                      PCI_FUNCTION_NUMBER_PCH_PMC
                      );

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// BIOS needs to enables SMI_LOCK (PMC PCI offset A0h[4] = 1b) which prevent writes
  /// to the Global SMI Enable bit (GLB_SMI_EN ABASE + 30h[0]). Enabling this bit will
  /// mitigate malicious software attempts to gain system management mode privileges.
  ///
  if (mPchConfigHob->LockDown.GlobalSmi == TRUE) {
    PchAcpiBaseGet (&ABase);
    ///
    /// Save Global SMI Enable bit setting before BIOS enables SMI_LOCK during S3 resume
    ///
    Data32Or = IoRead32 ((UINTN) (ABase + R_PCH_SMI_EN));
    if ((Data32Or & B_PCH_SMI_EN_GBL_SMI) != 0) {
      Data32And = 0xFFFFFFFF;
      Data32Or |= B_PCH_SMI_EN_GBL_SMI;
      S3BootScriptSaveIoReadWrite (
        S3BootScriptWidthUint32,
        (UINTN) (ABase + R_PCH_SMI_EN),
        &Data32Or,  // Data to be ORed
        &Data32And  // Data to be ANDed
        );
    }
    MmioOr8 ((UINTN) (PciPmcRegBase + R_PCH_PMC_GEN_PMCON_A), B_PCH_PMC_GEN_PMCON_A_SMI_LOCK);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciPmcRegBase + R_PCH_PMC_GEN_PMCON_A),
      1,
      (VOID *) (UINTN) (PciPmcRegBase + R_PCH_PMC_GEN_PMCON_A)
      );
  }
}

/**
  Do PCIE power management while resume from S3
**/
VOID
ReconfigurePciePowerManagementForS3 (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Data32;
  PCH_PCIE_IOTRAP_PROTOCOL              *PchPcieIoTrapProtocol;

  Status = gBS->LocateProtocol (&gPchPcieIoTrapProtocolGuid, NULL, (VOID **) &PchPcieIoTrapProtocol);
  ASSERT_EFI_ERROR (Status);
  mPcieIoTrapAddress = PchPcieIoTrapProtocol->PcieTrapAddress;
  DEBUG ((DEBUG_INFO, "PcieIoTrapAddress: %0x\n", mPcieIoTrapAddress));

  if (mPcieIoTrapAddress != 0) {
    //
    // Save PCH PCIE IoTrap address to re-config PCIE power management setting after resume from S3
    //
    Data32 = PciePmTrap;
    S3BootScriptSaveIoWrite (
      S3BootScriptWidthUint32,
      (UINTN) (mPcieIoTrapAddress),
      1,
      &Data32
      );
  } else {
    ASSERT (FALSE);
  }
}

/**
  This is the callback function for PCI ENUMERATION COMPLETE.
**/
VOID
EFIAPI
PchOnPciEnumComplete (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  VOID                *ProtocolPointer;
  UINTN               ThermalPciBase;
  UINT32              PchPwrmBase;

  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }
  gBS->CloseEvent (Event);


  PchPwrmBaseGet (&PchPwrmBase);

  //
  // Allocate BAR for ISH - workaround for Si bug on stepping LpA0/A1/A2
  //
  AllocateIshMemorySpace ();

  //
  // Enable Thermal MSE
  //
  ThermalPciBase = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_THERMAL,
                     PCI_FUNCTION_NUMBER_PCH_THERMAL
                     );
  if (MmioRead16 (ThermalPciBase + PCI_VENDOR_ID_OFFSET) != 0xFFFF) {
    if (((MmioRead32 (ThermalPciBase + R_PCH_THERMAL_TBAR) & B_PCH_THERMAL_TBAR_MASK) != 0) ||
        ((MmioRead32 (ThermalPciBase + R_PCH_THERMAL_TBARH) != 0))) {
      MmioOr8 (ThermalPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
    }
  }

  ReconfigurePciePowerManagementForS3 ();
  ProcessSmiLocks ();
#ifndef FSP_WRAPPER_FLAG
  PchOnPciEnumCompleteCommon ();
#endif
}

/**
  Register callback functions for PCH DXE.
**/
VOID
PchRegisterNotifications (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBoot;
  EFI_EVENT   LegacyBootEvent;
  EFI_EVENT   ExitBootServicesEvent;
  VOID        *Registration;

  ///
  /// Create PCI Enumeration Completed callback for PCH
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    PchOnPciEnumComplete,
    NULL,
    &Registration
    );

  //
  // Register a Ready to boot event to config PCIE power management setting after OPROM executed
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             PchOnReadyToBoot,
             NULL,
             &ReadyToBoot
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Create events for PCH to do the task before ExitBootServices/LegacyBoot.
  // It is guaranteed that only one of two events below will be signalled
  //
  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  PchOnExitBootServices,
                  NULL,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventLegacyBootEx (
             TPL_CALLBACK,
             PchOnBootToOs,
             NULL,
             &LegacyBootEvent
             );
  ASSERT_EFI_ERROR (Status);
}

/**
  Updates PCIe configuration in PCH Info Protocol.

  @param[in,out] PchInfo      Pointer to PCH info protocol instance to be updated
**/
VOID
UpdatePcieConfigInfo (
  IN OUT PCH_INFO_PROTOCOL   *PchInfo
  )
{
  EFI_STATUS  Status;
  PCH_SERIES  PchSeries;
  UINT32      StrapFuseCfg1;
  UINT32      StrapFuseCfg5;
  UINT32      StrapFuseCfg9;
  UINT32      StrapFuseCfg13;
  UINT32      StrapFuseCfg17;
  UINT32      StrapFuseCfg21;

  PchSeries = GetPchSeries ();

  Status = PchSbiRpPciRead32 (
             0,
             R_PCH_PCIE_STRPFUSECFG,
             &StrapFuseCfg1
             );
  ASSERT_EFI_ERROR (Status);

  Status = PchSbiRpPciRead32 (
             4,
             R_PCH_PCIE_STRPFUSECFG,
             &StrapFuseCfg5
             );
  ASSERT_EFI_ERROR (Status);

  Status = PchSbiRpPciRead32 (
             8,
             R_PCH_PCIE_STRPFUSECFG,
             &StrapFuseCfg9
             );
  ASSERT_EFI_ERROR (Status);

  PchInfo->PcieControllerCfg1 = (UINT8) ((StrapFuseCfg1 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
  PchInfo->PcieControllerCfg2 = (UINT8) ((StrapFuseCfg5 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
  PchInfo->PcieControllerCfg3 = (UINT8) ((StrapFuseCfg9 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);

  if (PchSeries == PchH) {
    Status = PchSbiRpPciRead32 (
               12,
               R_PCH_PCIE_STRPFUSECFG,
               &StrapFuseCfg13
               );
    ASSERT_EFI_ERROR (Status);
    Status = PchSbiRpPciRead32 (
               16,
               R_PCH_PCIE_STRPFUSECFG,
               &StrapFuseCfg17
               );
    ASSERT_EFI_ERROR (Status);

    PchInfo->PcieControllerCfg4 = (UINT8) ((StrapFuseCfg13 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
    PchInfo->PcieControllerCfg5 = (UINT8) ((StrapFuseCfg17 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);

    if (GetPchGeneration () == KblPch) {
      Status = PchSbiRpPciRead32 (
                 20,
                 R_PCH_PCIE_STRPFUSECFG,
                 &StrapFuseCfg21
                 );
      ASSERT_EFI_ERROR (Status);

      PchInfo->PcieControllerCfg6 = (UINT8) ((StrapFuseCfg21 & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
    }

  }
}

/**
  Stores PCIE Gen3 software equalization settings in bootscript to accelerate S3 resume
**/
VOID
SavePcieGen3Eq (
  VOID
  )
{
  UINT8      RpIndex;
  UINTN      RpBase;
  UINT8      EqMethod;
  UINT32     Data32;
  UINT32     Data32Or;
  UINT32     Data32And;

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); ++RpIndex) {
    RpBase = PchPcieBase (RpIndex);
    //
    // Before trying to save it, make sure software gen3 equalization did actually happen
    //
    if (MmioRead32 (RpBase) == 0xFFFFFFFF) {
      continue;
    }
    if (GetMaxLinkSpeed (RpBase) < 3) {
      continue;
    }
    EqMethod = mPchConfigHob->PcieRp.RootPort[RpIndex].Gen3EqPh3Method;
    if (EqMethod != PchPcieEqDefault && EqMethod != PchPcieEqSoftware) {
      continue;
    }
    //
    // Can't check current link speed due to some nonconformant devices
    // Instead we verify B_PCH_PCIE_EQCFG1_RTLEPCEB bit which together with policy check above
    // guarantees that software gen3 eq was performed
    //
    if (!(MmioRead32 (RpBase + R_PCH_PCIE_EQCFG1) & B_PCH_PCIE_EQCFG1_RTLEPCEB)) {
      continue;
    }

    //
    // Restore PCIE Gen3 equalization coefficients
    //
    Data32 = MmioRead32 (RpBase + R_PCH_PCIE_RTPCL1);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_RTPCL1, 1, &Data32);
    Data32 = MmioRead32 (RpBase + R_PCH_PCIE_RTPCL2);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_RTPCL2, 1, &Data32);
    Data32And = 0xFFFFFFFF;
    Data32Or = B_PCH_PCIE_EQCFG1_RTLEPCEB | B_PCH_PCIE_EQCFG1_RTPCOE;
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_EQCFG1, &Data32Or, &Data32And);

    //
    // Perform link retrain.
    //
    Data32And = (UINT32) (~B_PCIE_LCTL2_TLS);
    Data32Or = V_PCIE_LCTL2_TLS_GEN3;
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_LCTL2, &Data32Or, &Data32And);
    Data32And = 0xFFFFFFFF;
    Data32Or = B_PCIE_EX_LCTL3_PE;
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_EX_LCTL3, &Data32Or, &Data32And);
    Data32Or = B_PCIE_LCTL_RL;
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_LCTL, &Data32Or, &Data32And);
    /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
    /// Set "Link Speed Training Policy", Dxx:Fn + D4h[6] to 1.
    /// Make sure this is after mod-PHY related programming is completed.
    Data32Or = B_PCH_PCIE_MPC2_LSTP;
    DEBUG ((DEBUG_WARN, "Gen3Eq S3MmioOr32 0x%08x\n", RpBase + R_PCH_PCIE_MPC2));
    S3BootScriptSaveMemReadWrite (S3BootScriptWidthUint32, RpBase + R_PCH_PCIE_MPC2, &Data32Or, &Data32And);
  }
}

/**
  Initialize the PCH device according to the PCH Policy HOB
  and install PCH info instance.
**/
VOID
InitializePchDevice (
  VOID
  )
{
  EFI_HANDLE              Handle;
  PCH_INFO_PROTOCOL       *PchInfo;
  EFI_STATUS              Status;
  UINT32                  PchPwrmBase;

  Handle = NULL;

  DEBUG ((DEBUG_INFO, "InitializePchDevice() Start\n"));

  PchPwrmBaseGet (&PchPwrmBase);

  //
  // Create the PCH Info protocol instance
  //
  PchInfo = AllocateZeroPool (sizeof (PCH_INFO_PROTOCOL));
  if (PchInfo == NULL) {
    ASSERT (FALSE);
    return;
  }

  PchInfo->Revision = PCH_INFO_PROTOCOL_REVISION;

  UpdatePcieConfigInfo (PchInfo);

  //
  // Store PCIE Gen3 software equalization settings in bootscripts to save time on S3 resume
  // This must happen before some rootports get hidden by RST remapping
  //
  SavePcieGen3Eq ();


  //
  // Configure root port function number mapping
  // For UEFI bios, execute RPFN mapping after RST configuraion and before PCI enumeration.
  //
  Status = PchConfigureRpfnMapping ();
  ASSERT_EFI_ERROR (Status);

  //
  // Install Pch Emmc Tuning Protocol
  //
  if ((mPchConfigHob->Scs.ScsEmmcEnabled == TRUE) && (mPchConfigHob->Scs.ScsEmmcHs400Enabled == TRUE)) {
    InstallPchEmmcTuningProtocol ();
  }

  //
  // Install PCH info protocol on new handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gPchInfoProtocolGuid,
                  PchInfo,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "InitializePchDevice() End\n"));
}
/**
  <b>PchInit DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
      The PchInit module is a DXE driver that initializes the Intel Platform Controller Hub
      following the PCH BIOS specification and EDS requirements and recommendations. It consumes
      the PCH_POLICY_HOB SI_POLICY_HOB for expected configurations per policy.
      This is the standard EFI driver point that detects whether there is an supported PCH in
      the system and if so, initializes the chipset.

  - <b>Details</b>\n
    This module is required for initializing the Intel Platform Controller Hub to
    follow the PCH BIOS specification and EDS.
    This includes some initialization sequences, enabling and disabling PCH devices,
    configuring clock gating, RST PCIe Storage Remapping, SATA controller, ASPM of PCIE devices. Right before end of DXE,
    it's responsible to lock down registers for security requirement.

  - @pre
    - PCH PCR base address configured
    - EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
      - This is to ensure that PCI MMIO and IO resource has been prepared and available for this driver to allocate.

  - @result
    - Publishes the @link _PCH_INFO_PROTOCOL PCH_INFO_PROTOCOL @endlink
    - Publishes the @link _PCH_EMMC_TUNING_PROTOCOL PCH_EMMC_TUNING_PROTOCOL @endlink

  - <b>References</b>\n
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink.
    - @link _SI_POLICY_STRUCT SI_POLICY_HOB @endlink.

  - <b>Integration Checklists</b>\n
    - Verify prerequisites are met. Porting Recommendations.
    - No modification of this module should be necessary
    - Any modification of this module should follow the PCH BIOS Specification and EDS

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchInitEntryPointDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;

  DEBUG ((DEBUG_INFO, "PchInitEntryPointDxe() Start\n"));

  mImageHandle = ImageHandle;

  PchInitEntryPointCommon ();

  InitializePchDevice ();

  Status = PchAcpiInit (ImageHandle);

  CreateSerialIoHandles ();

  PchRegisterNotifications ();

  DEBUG ((DEBUG_INFO, "PchInitEntryPointDxe() End\n"));

  return Status;
}

/**
  PCH initialization before ExitBootServices / LegacyBoot events
  Useful for operations which must happen later than at EndOfPost event

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PchOnBootToOs (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  ///
  /// Closed the event to avoid call twice
  ///
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  ConfigureSerialIoAtBoot ();

  return;
}

/**
  PCH initialization on ExitBootService. This event is used if only ExitBootService is used
  and not in legacy boot

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

  @retval None
**/
VOID
EFIAPI
PchOnExitBootServices (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  PchOnBootToOs (NULL, NULL);

  PchUpdateNvsOnExitBootServices ();

  return;
}

/**
  PCH initialization before boot to OS

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PchOnReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
#ifndef FSP_WRAPPER_FLAG
  UINTN    XhciMmioBase;
  UINTN    XhciPciMmBase;
  UINT8    XhciPciCommand;
  BOOLEAN  XhciPdoRestartNeeded;
#endif // FSP_WRAPPER_FLAG

  DEBUG ((DEBUG_INFO, "Uefi PchOnReadyToBoot() Start\n"));

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

#ifndef FSP_WRAPPER_FLAG
  if (mPchConfigHob->Usb.DelayPdoProgramming == TRUE) {
    XhciPciMmBase = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_XHCI,
                      PCI_FUNCTION_NUMBER_PCH_XHCI
                      );

    // Check if xHCI has memory access
    XhciPciCommand = MmioRead8 (XhciPciMmBase + PCI_COMMAND_OFFSET);

    if ((XhciPciCommand & EFI_PCI_COMMAND_MEMORY_SPACE) == 0x0) {
      DEBUG ((DEBUG_ERROR, "xHCI Controller isn't configured properly for PDO setting\n"));
    } else {
      // Getting base MMIO address for xHCI
      XhciMmioBase = (
        MmioRead32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE) |
        ((UINTN) MmioRead32 (XhciPciMmBase + R_PCH_XHCI_MEM_BASE_2) << N_PCH_XHCI_MEM_SHIFT)
        ) & ((UINTN)~(B_PCH_XHCI_MEM_ALIGN_MASK));

      XhciPdoRestartNeeded = UsbPdoProgramming (
                               XhciMmioBase,
                               mPchConfigHob->Usb.Usb2DisabledPorts,
                               mPchConfigHob->Usb.Usb3DisabledPorts
                               );
      // If PDO registers are locked, reset platform to unlock them
      if (XhciPdoRestartNeeded) {
        gST->RuntimeServices->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
      }
    }
  } else {
    DEBUG ((DEBUG_INFO, "PDO register already programmed\n"));
  }
#endif // FSP_WRAPPER_FLAG

  //
  // Trigger an Iotrap SMI to config PCIE power management setting after PCI enumrate is done
  //
  if (mPcieIoTrapAddress != 0) {
    IoWrite32 ((UINTN) mPcieIoTrapAddress, PciePmTrap);
  } else {
    ASSERT (FALSE);
  }

  DEBUG ((DEBUG_INFO, "Uefi PchOnReadyToBoot() End\n"));
}

