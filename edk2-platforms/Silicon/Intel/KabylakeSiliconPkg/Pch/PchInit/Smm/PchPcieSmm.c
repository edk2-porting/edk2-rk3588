/** @file
  PCH Pcie SMM Driver Entry

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchInitSmm.h"

GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_DEVICE_OVERRIDE    *mDevAspmOverride;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                      mNumOfDevAspmOverride;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mPolicyRevision;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mPchBusNumber;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mTempRootPortBusNumMin;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mTempRootPortBusNumMax;

GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_ROOT_PORT_CONFIG   mPcieRootPortConfig[PCH_MAX_PCIE_ROOT_PORTS];
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                      mLtrNonSupportRpBitMap;

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                     mPciePmTrapExecuted = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                     mAllowNoLtrIccPllShutdownPolicy = FALSE;

extern EFI_GUID gPchDeviceTableHobGuid;


/**
  Program Common Clock and ASPM of Downstream Devices

  @param[in] PortIndex                  Pcie Root Port Number
  @param[in] RpDevice                   Pcie Root Pci Device Number
  @param[in] RpFunction                 Pcie Root Pci Function Number

  @retval EFI_SUCCESS                   Root port complete successfully
  @retval EFI_UNSUPPORTED               PMC has invalid vendor ID
**/
EFI_STATUS
PchPcieSmi (
  IN  UINT8                             PortIndex,
  IN  UINT8                             RpDevice,
  IN  UINT8                             RpFunction
  )
{
  UINT8                 SecBus;
  UINT8                 SubBus;
  EFI_HANDLE            Handle;
  UINT32                PwrmBase;
  UINTN                 RpBase;
  BOOLEAN               LtrSupported;
  BOOLEAN               DownstreamDevicePresent;

  Handle = NULL;
  LtrSupported = TRUE;

  RpBase   = MmPciBase (
               mPchBusNumber,
               (UINT32) RpDevice,
               (UINT32) RpFunction
               );

  if (MmioRead16 (RpBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_SUCCESS;
  }

  //
  // Check for presence detect state
  //
  DownstreamDevicePresent = !!(MmioRead16 (RpBase + R_PCH_PCIE_SLSTS) & B_PCIE_SLSTS_PDS);

  mLtrNonSupportRpBitMap &= ~(1 << PortIndex);

  if (DownstreamDevicePresent) {
    SecBus  = MmioRead8 (RpBase + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    SubBus  = MmioRead8 (RpBase + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    ASSERT (SecBus != 0 && SubBus != 0);
    PchPcieInitRootPortDownstreamDevices (
      DEFAULT_PCI_BUS_NUMBER_PCH,
      RpDevice,
      RpFunction,
      mTempRootPortBusNumMin,
      mTempRootPortBusNumMax,
      (BOOLEAN)mPcieRootPortConfig[PortIndex].EnableCpm
      );
    if (!LtrSupported) {
      mLtrNonSupportRpBitMap |= 1 << PortIndex;
    }

    //
    // Perform Equalization
    //
    MmioOr32 (RpBase + R_PCH_PCIE_EX_LCTL3, B_PCIE_EX_LCTL3_PE);
    MmioOr32 (RpBase + R_PCH_PCIE_LCTL, B_PCIE_LCTL_RL);
  }

  //
  // If not all devices support LTR, set PWRMBASE + 0x3EC [24,16] = 0, 1
  // This will disable ICC PLL shutdown
  //
  if (!mAllowNoLtrIccPllShutdownPolicy) {
    PchPwrmBaseGet (&PwrmBase);
    if (mLtrNonSupportRpBitMap != 0) {
      MmioAndThenOr32 (PwrmBase + R_PCH_PWRM_CS_SD_CTL2, (UINT32) ~BIT24, BIT16);
    } else {
      MmioAnd32 (PwrmBase + R_PCH_PWRM_CS_SD_CTL2, (UINT32) ~(BIT24 | BIT16));
    }
  }

  return EFI_SUCCESS;
}

/**
  PCIE Hotplug SMI call back function for each Root port

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieSmiRpHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  PchPcieSmi (RpContext->RpIndex, RpContext->DevNum, RpContext->FuncNum);
}

/**
  PCIE Link Active State Change Hotplug SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieLinkActiveStateChange (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  return;
}

/**
  PCIE Link Equalization Request SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
PchPcieLinkEqHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  ///
  /// From PCI Express specification, the PCIe device can request for Link Equalization. When the
  /// Link Equalization is requested by the device, an SMI will be generated  by PCIe RP when
  /// enabled and the SMI subroutine would invoke the Software Preset/Coefficient Search
  /// software to re-equalize the link.
  ///

  return;

}


/**
  An IoTrap callback to config PCIE power management settings
**/
VOID
PchPciePmIoTrapSmiCallback (
  VOID
  )
{
  UINT32                                    PwrmBase;
  UINT32                                    PortIndex;
  UINTN                                     RpBase;
  BOOLEAN                                   L1SubstatesSupportedPerPort;
  UINT8                                     MaxPciePortNum;
  UINTN                                     RpDevice;
  UINTN                                     RpFunction;
  PCH_GENERATION                            PchGen;
  PCH_SERIES                                PchSeries;
  PCH_STEPPING                              PchStep;

  PchGen                           = GetPchGeneration ();
  PchSeries                        = GetPchSeries ();
  PchStep                          = PchStepping ();
  L1SubstatesSupportedPerPort      = FALSE;
  MaxPciePortNum                   = GetPchMaxPciePortNum ();

  //
  // Step 4a
  // In case not all devices support LTR, disable ICC PLL shutdown
  // set PWRMBASE + 0x3EC [24,16] = 0, 1
  // (Note this register should be saved and restored during S3 transitions)
  //
  if (!mAllowNoLtrIccPllShutdownPolicy) {
    PchPwrmBaseGet (&PwrmBase);
    MmioAndThenOr32 (PwrmBase + R_PCH_PWRM_CS_SD_CTL2, (UINT32) ~BIT24, BIT16);
  }

  for (PortIndex = 0; PortIndex < MaxPciePortNum; PortIndex++) {
    GetPchPcieRpDevFun (PortIndex, &RpDevice, &RpFunction);
    RpBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDevice, (UINT32) RpFunction);
  }
  //
  // Step 4b
  // By this time, LTR support map is known.
  // If all devices support LTR, allow ICC PLL shutdown
  // (Note this register should be saved and restored during S3 transitions)
  //
  if (!mAllowNoLtrIccPllShutdownPolicy) {
    if (mLtrNonSupportRpBitMap == 0) {
      PchPwrmBaseGet (&PwrmBase);
      MmioAnd32 (PwrmBase + R_PCH_PWRM_CS_SD_CTL2, (UINT32) ~(BIT24 | BIT16));
    }
  }

  //
  // Program the remapped root port's ASPM based on endpoint's ASPM
  //
  if ((PchGen == SklPch) &&
      (((PchSeries == PchLp) && (PchStep < PchLpC0)) ||
       ((PchSeries == PchH) && (PchStep < PchHD0)))) {
    return;
  }
}

/**
  An IoTrap callback to config PCIE power management settings

  @param[in] DispatchHandle  - The handle of this callback, obtained when registering
  @param[in] DispatchContext - Pointer to the EFI_SMM_IO_TRAP_DISPATCH_CALLBACK_CONTEXT

**/
VOID
EFIAPI
PchPcieIoTrapSmiCallback (
  IN  EFI_HANDLE                            DispatchHandle,
  IN  EFI_SMM_IO_TRAP_CONTEXT                *CallbackContext,
  IN OUT VOID                               *CommBuffer,
  IN OUT UINTN                              *CommBufferSize
  )
{
  if (CallbackContext->WriteData == PciePmTrap) {
    if (mPciePmTrapExecuted == FALSE) {
      PchPciePmIoTrapSmiCallback ();
      mPciePmTrapExecuted = TRUE;
    }
  } else {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
  }
}

/**
  This function clear the Io trap executed flag before enter S3

  @param[in] Handle    Handle of the callback
  @param[in] Context   The dispatch context

  @retval EFI_SUCCESS  PCH register saved
**/
EFI_STATUS
EFIAPI
PchPcieS3EntryCallBack (
  IN  EFI_HANDLE                        Handle,
  IN CONST VOID                    *Context OPTIONAL,
  IN OUT VOID                      *CommBuffer OPTIONAL,
  IN OUT UINTN                     *CommBufferSize OPTIONAL
  )
{
  mPciePmTrapExecuted = FALSE;
  return EFI_SUCCESS;
}
/**
  Register PCIE Hotplug SMI dispatch function to handle Hotplug enabling

  @param[in] ImageHandle          The image handle of this module
  @param[in] SystemTable          The EFI System Table

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
InitializePchPcieSmm (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                            Status;
  UINT8                                 PortIndex;
  UINT8                                 Data8;
  UINT32                                Data32Or;
  UINT32                                Data32And;
  UINTN                                 RpBase;
  UINTN                                 RpDevice;
  UINTN                                 RpFunction;
  EFI_HANDLE                            PcieHandle;
  PCH_PCIE_SMI_DISPATCH_PROTOCOL        *PchPcieSmiDispatchProtocol;
  EFI_HOB_GUID_TYPE*                    Hob;
  UINT32                                DevTableSize;
  EFI_HANDLE                            PchIoTrapHandle;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT      PchIoTrapContext;
  EFI_SMM_SX_REGISTER_CONTEXT           SxDispatchContext;
  PCH_PCIE_IOTRAP_PROTOCOL              *PchPcieIoTrapProtocol;
  EFI_HANDLE                            SxDispatchHandle;
  UINT8                                 MaxPciePortNum;

  DEBUG ((DEBUG_INFO, "InitializePchPcieSmm () Start\n"));

  MaxPciePortNum    = GetPchMaxPciePortNum ();

  //
  // Locate Pch Pcie Smi Dispatch Protocol
  //
  Status = gSmst->SmmLocateProtocol (&gPchPcieSmiDispatchProtocolGuid, NULL, (VOID**)&PchPcieSmiDispatchProtocol);
  ASSERT_EFI_ERROR (Status);

  mPolicyRevision = mSiConfigHob->Header.Revision;
  mPchBusNumber = DEFAULT_PCI_BUS_NUMBER_PCH;
  mTempRootPortBusNumMin = mSiConfigHob->TempPciBusMin;
  mTempRootPortBusNumMax = mSiConfigHob->TempPciBusMax;
  mAllowNoLtrIccPllShutdownPolicy = (BOOLEAN) mPchConfigHob->PcieRp.AllowNoLtrIccPllShutdown;

  ASSERT (sizeof mPcieRootPortConfig == sizeof mPchConfigHob->PcieRp.RootPort);
  CopyMem (
    mPcieRootPortConfig,
    &(mPchConfigHob->PcieRp.RootPort),
    sizeof (mPcieRootPortConfig)
    );

  mDevAspmOverride                  = NULL;
  mNumOfDevAspmOverride             = 0;
  mLtrNonSupportRpBitMap            = 0;

  Hob = GetFirstGuidHob (&gPchDeviceTableHobGuid);
  if (Hob != NULL) {
    DevTableSize = GET_GUID_HOB_DATA_SIZE (Hob);
    ASSERT ((DevTableSize % sizeof (PCH_PCIE_DEVICE_OVERRIDE)) == 0);
    mNumOfDevAspmOverride = DevTableSize / sizeof (PCH_PCIE_DEVICE_OVERRIDE);
    DEBUG ((DEBUG_INFO, "Found PcieDeviceTable HOB (%d entries)\n", mNumOfDevAspmOverride));
    Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      DevTableSize,
                      (VOID **) &mDevAspmOverride
                      );
    CopyMem (mDevAspmOverride, GET_GUID_HOB_DATA (Hob), DevTableSize);
  }

  //
  // Throught all PCIE root port function and register the SMI Handler for enabled ports.
  //
  for (PortIndex = 0; PortIndex < MaxPciePortNum; PortIndex++) {
    GetPchPcieRpDevFun (PortIndex, &RpDevice, &RpFunction);
    RpBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDevice, (UINT32) RpFunction);
    //
    // Skip the root port function which is not enabled
    //
    if (MmioRead32 (RpBase) == 0xFFFFFFFF) {
      continue;
    }

    if (!mPcieRootPortConfig[PortIndex].EnableHotplugSmi) {
      continue;
    }
    //
    // Register SMI Handlers for Hot Plug and Link Active State Change
    //
    Data8 = MmioRead8 (RpBase + R_PCH_PCIE_SLCAP);
    if (Data8 & B_PCIE_SLCAP_HPC) {
      PcieHandle = NULL;
      Status = PchPcieSmiDispatchProtocol->HotPlugRegister (
                                             PchPcieSmiDispatchProtocol,
                                             PchPcieSmiRpHandlerFunction,
                                             PortIndex,
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);

      Status = PchPcieSmiDispatchProtocol->LinkActiveRegister (
                                             PchPcieSmiDispatchProtocol,
                                             PchPcieLinkActiveStateChange,
                                             PortIndex,
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);

      Data32Or  = B_PCH_PCIE_MPC_HPME;
      Data32And = (UINT32) ~B_PCH_PCIE_MPC_HPME;
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RpBase + R_PCH_PCIE_MPC),
        &Data32Or,  /// Data to be ORed
        &Data32And  /// Data to be ANDed
        );
    }

    //
    // Register SMI Handler for Link Equalization Request from Gen 3 Devices.
    //
    Data8 = MmioRead8 (RpBase + R_PCH_PCIE_LCAP);
    if ((Data8 & B_PCIE_LCAP_MLS) == V_PCIE_LCAP_MLS_GEN3) {
      Status = PchPcieSmiDispatchProtocol->LinkEqRegister (
                                             PchPcieSmiDispatchProtocol,
                                             PchPcieLinkEqHandlerFunction,
                                             PortIndex,
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);
    }
  }

  ASSERT_EFI_ERROR (Status);

  PchIoTrapContext.Type     = WriteTrap;
  PchIoTrapContext.Length   = 4;
  PchIoTrapContext.Address  = 0;
  Status = mPchIoTrap->Register (
                         mPchIoTrap,
                         (EFI_SMM_HANDLER_ENTRY_POINT2) PchPcieIoTrapSmiCallback,
                         &PchIoTrapContext,
                         &PchIoTrapHandle
                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Install the PCH Pcie IoTrap protocol
  //
  (gBS->AllocatePool) (EfiBootServicesData, sizeof (PCH_PCIE_IOTRAP_PROTOCOL), (VOID **)&PchPcieIoTrapProtocol);
  PchPcieIoTrapProtocol->PcieTrapAddress = PchIoTrapContext.Address;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchPcieIoTrapProtocolGuid,
                  PchPcieIoTrapProtocol,
                  NULL
                  );

  //
  // Register the callback for S3 entry
  //
  SxDispatchContext.Type  = SxS3;
  SxDispatchContext.Phase = SxEntry;
  Status = mSxDispatch->Register (
                          mSxDispatch,
                          PchPcieS3EntryCallBack,
                          &SxDispatchContext,
                          &SxDispatchHandle
                          );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InitializePchPcieSmm () End\n"));

  return EFI_SUCCESS;
}
