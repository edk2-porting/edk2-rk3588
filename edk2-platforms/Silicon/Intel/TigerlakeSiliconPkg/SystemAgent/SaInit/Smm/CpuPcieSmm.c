/** @file
  CPU PCIe SMM Driver Entry

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "SaLateInitSmm.h"
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/CpuPcieRegs.h>
#include <Library/PcieHelperLib.h>
#include <CpuPcieInfo.h>
#include <Library/TimerLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <CpuPcieConfig.h>
#include <Library/CpuPcieRpLib.h>
#include <Protocol/PchPcieSmiDispatch.h>
#include <Protocol/PchSmiDispatch.h>
#include <Protocol/SaIotrapSmi.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/BaseMemoryLib.h>
#include <CpuPcieHob.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mSaBusNumber;
//
// @note:
// These temp bus numbers cannot be used in runtime (hot-plug).
// These can be used only during boot.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mTempRootPortBusNumMin;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       mTempRootPortBusNumMax;
GLOBAL_REMOVE_IF_UNREFERENCED CPU_PCIE_ROOT_PORT_CONFIG   mCpuPcieRootPortConfig[CPU_PCIE_MAX_ROOT_PORTS];
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                     mCpuPciePmTrapExecuted = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_DEVICE_OVERRIDE    *mDevAspmOverride;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                      mNumOfDevAspmOverride;

/**
  An IoTrap callback to config PCIE power management settings
**/
VOID
CpuPciePmIoTrapSmiCallback (
  VOID
  )
{
  UINT32                                    PortIndex;
  UINT64                                    RpBase;
  UINT8                                     MaxPciePortNum;
  UINTN                                     RpDevice;
  UINTN                                     RpFunction;

  MaxPciePortNum = GetMaxCpuPciePortNum ();

  for (PortIndex = 0; PortIndex < MaxPciePortNum; PortIndex++) {
    GetCpuPcieRpDevFun (PortIndex, &RpDevice, &RpFunction);
    RpBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, (UINT32) RpDevice, (UINT32) RpFunction, 0);

    if (PciSegmentRead16 (RpBase) != 0xFFFF) {
      mDevAspmOverride                  = NULL;
      mNumOfDevAspmOverride             = 0;
      RootportDownstreamPmConfiguration (
        SA_SEG_NUM,
        SA_MC_BUS,
        (UINT8)RpDevice,
        (UINT8)RpFunction,
        mTempRootPortBusNumMin,
        mTempRootPortBusNumMax,
        &mCpuPcieRootPortConfig[PortIndex].PcieRpCommonConfig,
        mNumOfDevAspmOverride,
        mDevAspmOverride
      );

    }
  }
}

/**
  Program Common Clock and ASPM of Downstream Devices

  @param[in] PortIndex                  Pcie Root Port Number
  @param[in] RpDevice                   Pcie Root Pci Device Number
  @param[in] RpFunction                 Pcie Root Pci Function Number

  @retval EFI_SUCCESS                   Root port complete successfully
  @retval EFI_UNSUPPORTED               PMC has invalid vendor ID
**/
EFI_STATUS
CpuPcieSmi (
  IN  UINT8                             PortIndex,
  IN  UINT8                             RpDevice,
  IN  UINT8                             RpFunction
  )
{
  UINT8                 SecBus;
  UINT8                 SubBus;
  UINT64                RpBase;
  UINT64                EpBase;
  UINT8                 EpPcieCapPtr;
  UINT8                 EpMaxSpeed;
  BOOLEAN               DownstreamDevicePresent;
  UINT32                Timeout;
  UINT32                MaxLinkSpeed;

  RpBase   = PCI_SEGMENT_LIB_ADDRESS (
               SA_SEG_NUM,
               SA_MC_BUS,
               (UINT32) RpDevice,
               (UINT32) RpFunction,
               0
               );

  if (PciSegmentRead16 (RpBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG((DEBUG_INFO, "PCIe controller is disabled, return!!\n"));
    return EFI_SUCCESS;
  }
  //
  // Check presence detect state. Here the endpoint must be detected using PDS rather than
  // the usual LinkActive check, because PDS changes immediately and LA takes a few milliseconds to stabilize
  //
  DownstreamDevicePresent = !!(PciSegmentRead16 (RpBase + R_PCIE_SLSTS) & B_PCIE_SLSTS_PDS);

  if (DownstreamDevicePresent) {
    ///
    /// Make sure the link is active before trying to talk to device behind it
    /// Wait up to 100ms, according to PCIE spec chapter 6.7.3.3
    ///
    Timeout = 100 * 1000;
    while (CpuPcieIsLinkActive(RpBase) == 0) {
      MicroSecondDelay (10);
      Timeout-=10;
      if (Timeout == 0) {
        DEBUG((DEBUG_INFO, "PDS set but timeout while waiting for LA bit to get set!!!\n"));
        return EFI_NOT_FOUND;
      }
    }
    SecBus  = PciSegmentRead8 (RpBase + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    SubBus  = PciSegmentRead8 (RpBase + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    ASSERT (SecBus != 0 && SubBus != 0);
    if (SecBus == 0) {
      DEBUG((DEBUG_INFO, "Secondary Bus is 0, return!!!\n"));
      return EFI_NOT_FOUND;
    }
    RootportDownstreamConfiguration (
      SA_SEG_NUM,
      SA_MC_BUS,
      RpDevice,
      RpFunction,
      mTempRootPortBusNumMin,
      mTempRootPortBusNumMax,
      EnumCpuPcie
      );
    RootportDownstreamPmConfiguration (
      SA_SEG_NUM,
      SA_MC_BUS,
      RpDevice,
      RpFunction,
      mTempRootPortBusNumMin,
      mTempRootPortBusNumMax,
      &mCpuPcieRootPortConfig[PortIndex].PcieRpCommonConfig,
      mNumOfDevAspmOverride,
      mDevAspmOverride
    );
    //
    // Perform Equalization
    //
    EpBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SecBus, 0, 0, 0);
    EpPcieCapPtr = PcieFindCapId (SA_SEG_NUM, SecBus, 0, 0, EFI_PCI_CAPABILITY_ID_PCIEXP);
    EpMaxSpeed = PciSegmentRead8 (EpBase + EpPcieCapPtr + R_PCIE_LCAP_OFFSET) & B_PCIE_LCAP_MLS;
    MaxLinkSpeed = CpuPcieGetMaxLinkSpeed (RpBase);
    if (EpMaxSpeed < MaxLinkSpeed) {
        MaxLinkSpeed = EpMaxSpeed;
    }
    if (EpMaxSpeed >= V_PCIE_LCAP_MLS_GEN3 && EpMaxSpeed <= V_PCIE_LCAP_MLS_GEN4) {
      PciSegmentAndThenOr16 (RpBase + R_PCIE_LCTL2, (UINT16)~B_PCIE_LCTL2_TLS, (UINT16)MaxLinkSpeed);
      PciSegmentOr32 (RpBase + R_PCIE_LCTL3, B_PCIE_LCTL3_PE);
      PciSegmentOr32 (RpBase + R_PCIE_LCTL, B_PCIE_LCTL_RL);
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
CpuPcieSmiRpHandlerFunction (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  PCH_PCIE_SMI_RP_CONTEXT           *RpContext
  )
{
  CpuPcieSmi (RpContext->RpIndex, RpContext->DevNum, RpContext->FuncNum);
}

/**
  PCIE Link Active State Change Hotplug SMI call back function for all Root ports

  @param[in] DispatchHandle             Handle of this dispatch function
  @param[in] RpContext                  Rootport context, which contains RootPort Index,
                                        and RootPort PCI BDF.
**/
VOID
EFIAPI
CpuPcieLinkActiveStateChange (
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
CpuPcieLinkEqHandlerFunction (
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

  @param[in] DispatchHandle  - The handle of this callback, obtained when registering
  @param[in] DispatchContext - Pointer to the EFI_SMM_IO_TRAP_DISPATCH_CALLBACK_CONTEXT

**/
VOID
EFIAPI
CpuPcieIoTrapSmiCallback (
  IN  EFI_HANDLE                            DispatchHandle,
  IN  EFI_SMM_IO_TRAP_CONTEXT               *CallbackContext,
  IN OUT VOID                               *CommBuffer,
  IN OUT UINTN                              *CommBufferSize
  )
{
  if (CallbackContext->WriteData == CpuPciePmTrap) {
    if (mCpuPciePmTrapExecuted == FALSE) {
      CpuPciePmIoTrapSmiCallback ();
      mCpuPciePmTrapExecuted = TRUE;
    }
  } else {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
  }
}

/**
  This function clear the Io trap executed flag before enter S3

  @param[in] Handle    Handle of the callback
  @param[in] Context   The dispatch context

  @retval EFI_SUCCESS  SA register saved
**/
EFI_STATUS
EFIAPI
CpuPcieS3EntryCallBack (
  IN  EFI_HANDLE                   Handle,
  IN CONST VOID                    *Context OPTIONAL,
  IN OUT VOID                      *CommBuffer OPTIONAL,
  IN OUT UINTN                     *CommBufferSize OPTIONAL
  )
{
  mCpuPciePmTrapExecuted = FALSE;
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
InitializeCpuPcieSmm (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                            Status;
  UINT8                                 PortIndex;
  UINT8                                 Data8;
  UINT32                                Data32Or;
  UINT32                                Data32And;
  UINT64                                RpBase;
  UINTN                                 RpDevice;
  UINTN                                 RpFunction;
  EFI_HANDLE                            PcieHandle;
  EFI_HANDLE                            PchIoTrapHandle;
  PCH_PCIE_SMI_DISPATCH_PROTOCOL        *PchPcieSmiDispatchProtocol;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT      PchIoTrapContext;
  EFI_SMM_SX_REGISTER_CONTEXT           SxDispatchContext;
  SA_IOTRAP_SMI_PROTOCOL                *CpuPcieIoTrapProtocol;
  EFI_HANDLE                            SxDispatchHandle;
  UINT8                                 MaxPciePortNum;
  CPU_PCIE_HOB                          *CpuPcieHob;

  DEBUG ((DEBUG_INFO, "InitializeCpuPcieSmm () Start\n"));

  MaxPciePortNum    = GetMaxCpuPciePortNum ();

  //
  // Locate Pch Pcie Smi Dispatch Protocol
  //
  Status = gSmst->SmmLocateProtocol (&gPchPcieSmiDispatchProtocolGuid, NULL, (VOID**) &PchPcieSmiDispatchProtocol);
  ASSERT_EFI_ERROR (Status);

  mTempRootPortBusNumMin = PcdGet8 (PcdSiliconInitTempPciBusMin);
  mTempRootPortBusNumMax = PcdGet8 (PcdSiliconInitTempPciBusMax);

  ///
  /// Locate HOB for CPU PCIe
  ///
  CpuPcieHob = GetFirstGuidHob(&gCpuPcieHobGuid);
  if (CpuPcieHob != NULL) {
    ASSERT (sizeof mCpuPcieRootPortConfig == sizeof CpuPcieHob->RootPort);
    CopyMem (
      mCpuPcieRootPortConfig,
      &(CpuPcieHob->RootPort),
      sizeof (mCpuPcieRootPortConfig)
      );
  }

  //
  // Throught all PCIE root port function and register the SMI Handler for enabled ports.
  //
  for (PortIndex = 0; PortIndex < MaxPciePortNum; PortIndex++) {
    GetCpuPcieRpDevFun (PortIndex, &RpDevice, &RpFunction);
    RpBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, (UINT32) RpDevice, (UINT32) RpFunction, 0);
    //
    // Skip the root port function which is not enabled
    //
    if (PciSegmentRead32 (RpBase) == 0xFFFFFFFF) {
      continue;
    }

    //
    // Register SMI Handlers for Hot Plug and Link Active State Change
    //
    Data8 = PciSegmentRead8 (RpBase + R_PCIE_SLCAP);
    if (Data8 & B_PCIE_SLCAP_HPC) {
      PcieHandle = NULL;
      Status = PchPcieSmiDispatchProtocol->HotPlugRegister (
                                             PchPcieSmiDispatchProtocol,
                                             CpuPcieSmiRpHandlerFunction,
                                             (PortIndex + CpuRpIndex0),
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);

      Status = PchPcieSmiDispatchProtocol->LinkActiveRegister (
                                             PchPcieSmiDispatchProtocol,
                                             CpuPcieLinkActiveStateChange,
                                             (PortIndex + CpuRpIndex0),
                                             &PcieHandle
                                             );
      ASSERT_EFI_ERROR (Status);

      Data32Or  = B_PCIE_MPC_HPME;
      Data32And = (UINT32) ~B_PCIE_MPC_HPME;
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        PcdGet64 (PcdSiPciExpressBaseAddress) + RpBase + R_PCIE_MPC,
        &Data32Or,  /// Data to be ORed
        &Data32And  /// Data to be ANDed
        );
    }

    //
    // Register SMI Handler for Link Equalization Request from Gen 3 Devices.
    //
    Data8 = PciSegmentRead8 (RpBase + R_PCIE_LCAP);
    if ((Data8 & B_PCIE_LCAP_MLS) == V_PCIE_LCAP_MLS_GEN3) {
      Status = PchPcieSmiDispatchProtocol->LinkEqRegister (
                                             PchPcieSmiDispatchProtocol,
                                             CpuPcieLinkEqHandlerFunction,
                                             (PortIndex + CpuRpIndex0),
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
                         (EFI_SMM_HANDLER_ENTRY_POINT2) CpuPcieIoTrapSmiCallback,
                         &PchIoTrapContext,
                         &PchIoTrapHandle
                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Install the SA Pcie IoTrap protocol
  //
  (gBS->AllocatePool) (EfiBootServicesData, sizeof (SA_IOTRAP_SMI_PROTOCOL), (VOID **)&CpuPcieIoTrapProtocol);
  CpuPcieIoTrapProtocol->SaIotrapSmiAddress = PchIoTrapContext.Address;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gCpuPcieIoTrapProtocolGuid,
                  CpuPcieIoTrapProtocol,
                  NULL
                  );

  //
  // Register the callback for S3 entry
  //
  SxDispatchContext.Type  = SxS3;
  SxDispatchContext.Phase = SxEntry;
  Status = mSxDispatch->Register (
                          mSxDispatch,
                          CpuPcieS3EntryCallBack,
                          &SxDispatchContext,
                          &SxDispatchHandle
                          );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InitializeCpuPcieSmm, IoTrap @ %x () End\n", PchIoTrapContext.Address));

  return EFI_SUCCESS;
}
