/** @file
  This is the driver that initializes the Intel PCH.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "PchInit.h"
#include <Protocol/PchPolicy.h>
#include <GpioDevConfig.h>
#include <ScsConfig.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/GbeLib.h>
#include <PchRstHob.h>
#include <Library/PchPcieRpLib.h>
#include <TraceHubConfig.h>
#include <PchReservedResources.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <Library/BaseLib.h>
#include <Register/UsbRegs.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/Pci22.h>
#include <PcieRegs.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPciBdfLib.h>
#include <PchHybridStorageHob.h>
#include <Library/SataLib.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

/**
  Retrieve interrupt information about a PCH device from policy

  @param[in] UartNumber                 Uart number

  @retval PCH_DEVICE_INTERRUPT_CONFIG   structure with device's interrupt information
**/
PCH_DEVICE_INTERRUPT_CONFIG
GetUartInterrupt (
  IN UINT8  UartNumber
  )
{
  PCH_DEVICE_INTERRUPT_CONFIG EmptyRecord;
  UINT8                       DevNum;
  UINT8                       FuncNum;
  UINT8                       Index;

  ZeroMem (&EmptyRecord, sizeof (PCH_DEVICE_INTERRUPT_CONFIG));
  DevNum  = SerialIoUartDevNumber (UartNumber);
  FuncNum = SerialIoUartFuncNumber (UartNumber);

  for (Index = 0; Index < mPchConfigHob->Interrupt.NumOfDevIntConfig; Index++) {
    if ((mPchConfigHob->Interrupt.DevIntConfig[Index].Device == DevNum) &&
        (mPchConfigHob->Interrupt.DevIntConfig[Index].Function == FuncNum)) {
      return mPchConfigHob->Interrupt.DevIntConfig[Index];
    }
  }
  return EmptyRecord;
}

/**
  Update ASL definitions for SerialIo devices.

**/
VOID
UpdateSerialIoAcpiData (
  VOID
  )
{
  UINT8        Index;

  for (Index = 0; Index < GetPchMaxSerialIoSpiControllersNum (); Index++) {
    mPchNvsAreaProtocol.Area->SM0[Index] = mPchConfigHob->SerialIo.SpiDeviceConfig[Index].Mode;
    mPchNvsAreaProtocol.Area->SC0[Index] = GetSerialIoSpiPciCfg (Index);
  }
  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    mPchNvsAreaProtocol.Area->IM0[Index] = mPchConfigHob->SerialIo.I2cDeviceConfig[Index].Mode;
    mPchNvsAreaProtocol.Area->IC0[Index] = GetSerialIoI2cPciCfg (Index);
  }
  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    mPchNvsAreaProtocol.Area->UM0[Index] = mPchConfigHob->SerialIo.UartDeviceConfig[Index].Mode;
    mPchNvsAreaProtocol.Area->UC0[Index] = GetSerialIoUartPciCfg (Index);
    mPchNvsAreaProtocol.Area->UD0[Index] = mPchConfigHob->SerialIo.UartDeviceConfig[Index].DmaEnable;
    mPchNvsAreaProtocol.Area->UP0[Index] = mPchConfigHob->SerialIo.UartDeviceConfig[Index].PowerGating;
    mPchNvsAreaProtocol.Area->UI0[Index] = (GetUartInterrupt (Index)).Irq;
  }
}

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
/**
  Update NVS Area for Timed GPIO devices.
**/
VOID
UpdateTimedGpioSetup (
  VOID
  )
{
  mPchNvsAreaProtocol.Area->EnableTimedGpio0 = (UINT8)mPchConfigHob->Pm.EnableTimedGpio0;
  mPchNvsAreaProtocol.Area->EnableTimedGpio1 = (UINT8)mPchConfigHob->Pm.EnableTimedGpio1;
}
#endif

/**
  Update NVS Area after RST PCIe Storage Remapping and before Boot

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Couldn't fetch RstHob
**/
EFI_STATUS
PchUpdateNvsAreaAfterRemapping (
  VOID
  )
{
  UINTN                 Index;
  VOID                  *Hob;
  PCH_RST_HOB           *RstHob;

  Hob = GetFirstGuidHob (&gPchRstHobGuid);
  if (Hob == NULL) {
    return EFI_NOT_FOUND;
  }

  RstHob = (PCH_RST_HOB *) GET_GUID_HOB_DATA (Hob);

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    mPchNvsAreaProtocol.Area->RstPcieStorageInterfaceType[Index]        = RstHob->RstCrConfiguration[Index].DeviceInterface;
    mPchNvsAreaProtocol.Area->RstPcieStoragePmCapPtr[Index]             = RstHob->SavedRemapedDeviceConfigSpace[Index].PmCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStoragePcieCapPtr[Index]           = RstHob->SavedRemapedDeviceConfigSpace[Index].PcieCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageL1ssCapPtr[Index]           = RstHob->SavedRemapedDeviceConfigSpace[Index].L1ssCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpL1ssControl2[Index]       = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointL1ssControl2;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpL1ssControl1[Index]       = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointL1ssControl1;
    mPchNvsAreaProtocol.Area->RstPcieStorageLtrCapPtr[Index]            = RstHob->SavedRemapedDeviceConfigSpace[Index].LtrCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpLtrData[Index]            = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointLtrData;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpLctlData16[Index]         = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointLctlData16;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpDctlData16[Index]         = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointDctlData16;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpDctl2Data16[Index]        = RstHob->SavedRemapedDeviceConfigSpace[Index].EndpointDctl2Data16;
    mPchNvsAreaProtocol.Area->RstPcieStorageRpDctl2Data16[Index]        = RstHob->SavedRemapedDeviceConfigSpace[Index].RootPortDctl2Data16;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniqueTableBar[Index]       = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixTableBar;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniqueTableBarValue[Index]  = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixTableBarValue;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniquePbaBar[Index]         = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixPbaBar;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniquePbaBarValue[Index]    = RstHob->RstCrConfiguration[Index].EndPointUniqueMsixPbaBarValue;
    mPchNvsAreaProtocol.Area->RstPcieStorageRootPortNum[Index]          = RstHob->RstCrConfiguration[Index].RootPortNum;
  }
  return EFI_SUCCESS;
}

/**
  Update the Hybrid storage location NVS Area if Hybrid Storage device is present
**/
EFI_STATUS
UpdateHybridStorageLocation (
  VOID
  )
{
  VOID                               *Hob;
  PCH_HYBRIDSTORAGE_HOB              *HybridStorageHob;

  Hob = GetFirstGuidHob (&gHybridStorageHobGuid);
  if (Hob == NULL) {
    return EFI_NOT_FOUND;
  }

  HybridStorageHob = (PCH_HYBRIDSTORAGE_HOB *) GET_GUID_HOB_DATA (Hob);
  mPchNvsAreaProtocol.Area->HybridStorageLocation = HybridStorageHob->HybridStorageLocation;
  return EFI_SUCCESS;
}

/**
  PCH ACPI initialization before Boot Sript Table is closed
  It update ACPI table and ACPI NVS area.

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PchAcpiOnEndOfDxe (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  DEBUG ((DEBUG_INFO, "PchAcpiOnEndOfDxe() Start\n"));

  ///
  /// Closed the event to avoid call twice when launch shell
  ///
  gBS->CloseEvent (Event);

  //
  // Init HDA Audio ACPI tables
  //
  PchHdAudioAcpiInit ();
  //
  // Update ASL definitions for SerialIo devices.
  //
  UpdateSerialIoAcpiData ();
  UpdateCnviAcpiData ();
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UpdateTimedGpioSetup();
#endif

  //
  // Update Pch Nvs Area
  //
  PchUpdateNvsArea ();

  DEBUG ((DEBUG_INFO, "PchAcpiOnEndOfDxe() End\n"));

  return;
}

/**
  Initialize Pch acpi
  @param[in] ImageHandle          Handle for the image of this driver

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchAcpiInit (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "Install PCH NVS protocol\n"));

  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (PCH_NVS_AREA), (VOID **) &mPchNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) mPchNvsAreaProtocol.Area, sizeof (PCH_NVS_AREA));
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchNvsAreaProtocolGuid,
                  &mPchNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update the NVS Area after RST PCIe Storage Remapping
  ///
  PchUpdateNvsAreaAfterRemapping ();

  UpdateHybridStorageLocation ();
  //
  // Register an end of DXE event for PCH ACPI to do tasks before invoking any UEFI drivers,
  // applications, or connecting consoles,...
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PchAcpiOnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


/**
  Update NVS area for PCIe root ports.
**/
STATIC
VOID
PcieRpUpdateNvsArea (
  VOID
  )
{
  UINT32  Index;

  for (Index = 0; Index < PCH_MAX_PCIE_CLOCKS; Index++) {
    mPchNvsAreaProtocol.Area->ClockToRootPortMap[Index] = mPchConfigHob->PcieRp.PcieClock[Index].Usage;
  }
}

/**
  Update ASL object before Boot

  @retval EFI_STATUS
  @retval EFI_NOT_READY         The Acpi protocols are not ready.
**/
EFI_STATUS
PchUpdateNvsArea (
  VOID
  )
{
  EFI_STATUS            Status;
  UINTN                 Index;
  UINT32                HpetBaseAdress;
  GPIO_GROUP            GroupToGpeDwX[3];
  UINT32                GroupDw[3];
  UINTN                 RpDev;
  UINTN                 RpFun;
  UINT32                Data32;
  PCH_POLICY_PROTOCOL   *PchPolicy;
  GPIO_DXE_CONFIG       *GpioDxeConfig;
  UINT64                XdciPciBase;
  UINT64                XdciBar;
  UINT16                PciMemConfig;
  UINT16                TcoBase;
  UINT8                 ClearXdciBar = FALSE;

  ///
  /// Get PCH Policy Protocol
  ///
  Status = gBS->LocateProtocol (&gPchPolicyProtocolGuid, NULL, (VOID **)&PchPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Get GPIO DXE Config Block
  ///
  Status = GetConfigBlock ((VOID *)PchPolicy, &gGpioDxeConfigGuid, (VOID *)&GpioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update ASL PCIE port address according to root port device and function
  //
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    RpDev = PchPcieRpDevNumber (Index);
    RpFun = PchPcieRpFuncNumber (Index);
    Data32 = ((UINT8) RpDev << 16) | (UINT8) RpFun;
    mPchNvsAreaProtocol.Area->RpAddress[Index] = Data32;

    //
    // Update Maximum Snoop Latency and Maximum No-Snoop Latency values for PCIE
    //
    mPchNvsAreaProtocol.Area->PcieLtrMaxSnoopLatency[Index]   = mPchConfigHob->PcieRp.RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency;
    mPchNvsAreaProtocol.Area->PcieLtrMaxNoSnoopLatency[Index] = mPchConfigHob->PcieRp.RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency;
  }

  //
  // Update PCHS.
  //
  mPchNvsAreaProtocol.Area->PchSeries     = PchSeries ();
  //
  // Update PCHG.
  //
  mPchNvsAreaProtocol.Area->PchGeneration = (UINT16) PchGeneration ();
  //
  // Update PSTP.
  //
  mPchNvsAreaProtocol.Area->PchStepping = (UINT16) PchStepping ();
  //
  // Update HPET base address.
  //
  PchHpetBaseGet (&HpetBaseAdress);
  mPchNvsAreaProtocol.Area->HPTE          = TRUE;  // @todo remove the NVS, since it's always enabled.
  mPchNvsAreaProtocol.Area->HPTB          = HpetBaseAdress;
  //
  // Update SBREG_BAR.
  //
  mPchNvsAreaProtocol.Area->SBRG          = PCH_PCR_BASE_ADDRESS;

  //
  // Update base address
  //
  mPchNvsAreaProtocol.Area->PMBS = PmcGetAcpiBase ();
  mPchNvsAreaProtocol.Area->PWRM = PmcGetPwrmBase ();
  PchTcoBaseGet (&TcoBase);
  mPchNvsAreaProtocol.Area->TcoBase = TcoBase;

  //
  // Update PCH PID info
  //
  mPchNvsAreaProtocol.Area->IclkPid = PchPcrGetPid (PchIpIclk);

  //
  // Update GPIO device ACPI variables
  //
  mPchNvsAreaProtocol.Area->SGIR = mPchConfigHob->Interrupt.GpioIrqRoute;
  mPchNvsAreaProtocol.Area->GPHD = (UINT8)GpioDxeConfig->HideGpioAcpiDevice;

  //
  // Update GPP_X to GPE_DWX mapping.
  //
  GpioGetGroupDwToGpeDwX (
    &GroupToGpeDwX[0], &GroupDw[0],
    &GroupToGpeDwX[1], &GroupDw[1],
    &GroupToGpeDwX[2], &GroupDw[2]
    );

  //
  // GEI0/1/2 and GED0/1/2 are objects for informing how GPIO groups are mapped to GPE0.
  // If Group is mapped to 1-Tier GPE information is also stored on what Group DW
  // is mapped to GPE_DWx. Because GPE_DWx register is 32 bits large if groups have more than
  // 32 pads only part of it can be mapped.
  //
  //  GEIx - GroupIndex mapped to GPE0_DWx
  //  GEDx - DoubleWorld part of Group: 0 - pins 31-0, 1 - pins 63-32, ...
  //
  mPchNvsAreaProtocol.Area->GEI0 = (UINT8) GpioGetGroupIndexFromGroup (GroupToGpeDwX[0]);
  mPchNvsAreaProtocol.Area->GEI1 = (UINT8) GpioGetGroupIndexFromGroup (GroupToGpeDwX[1]);
  mPchNvsAreaProtocol.Area->GEI2 = (UINT8) GpioGetGroupIndexFromGroup (GroupToGpeDwX[2]);
  mPchNvsAreaProtocol.Area->GED0 = (UINT8) GroupDw[0];
  mPchNvsAreaProtocol.Area->GED1 = (UINT8) GroupDw[1];
  mPchNvsAreaProtocol.Area->GED2 = (UINT8) GroupDw[2];

  //
  // SCS Configuration
  //

  PcieRpUpdateNvsArea ();

  //
  // SATA configuration.
  //
  mPchNvsAreaProtocol.Area->SataPortPresence = GetSataPortPresentStatus (0);
  DEBUG ((DEBUG_INFO, "SataPortPresence: 0x%x\n", mPchNvsAreaProtocol.Area->SataPortPresence));

  //
  // CPU SKU
  //
  mPchNvsAreaProtocol.Area->CpuSku = 0;
  mPchNvsAreaProtocol.Area->PsOnEnable            = (UINT8)mPchConfigHob->Pm.PsOnEnable;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    mPchNvsAreaProtocol.Area->LtrEnable[Index]  = (UINT8)mPchConfigHob->PcieRp.RootPort[Index].PcieRpCommonConfig.LtrEnable;
  }

  mPchNvsAreaProtocol.Area->GBES = IsGbePresent ();

  //
  // Update PCH Trace Hub Mode
  //
  mPchNvsAreaProtocol.Area->PchTraceHubMode     = (UINT8) mPchConfigHob->PchTraceHub.PchTraceHubMode;

  //
  // Saving GCTL value into PCH NVS area
  //

  XdciPciBase = PchXdciPciCfgBase ();

  //
  // Determine Base address for Base address register (Offset 0x10)
  //
  if (PciSegmentRead32 (XdciPciBase) != 0xFFFFFFFF) {

    XdciBar = PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;

    if ((PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
      XdciBar += (UINT64)PciSegmentRead32 (XdciPciBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)) << 32;
    }

    if (XdciBar == 0x0) {
      ClearXdciBar = TRUE;
      PciSegmentWrite32 ((XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET), PcdGet32 (PcdSiliconInitTempMemBaseAddr));
      XdciBar = PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;

      if ((PciSegmentRead32 (XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
        XdciBar += (UINT64)PciSegmentRead32 (XdciPciBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)) << 32;
      }
    }

    //
    // Enable Pci Memconfig to read the correct value for GCTL register
    //
    PciMemConfig = PciSegmentRead16 (XdciPciBase + PCI_COMMAND_OFFSET);
    PciSegmentWrite16 (XdciPciBase + PCI_COMMAND_OFFSET, PciMemConfig | (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));

    mPchNvsAreaProtocol.Area->PchxDCIPwrDnScale = MmioRead32(XdciBar + R_XDCI_MEM_GCTL);
    DEBUG ((DEBUG_INFO, "PchxDCIPwrDnScale  0x%x\n", (UINT64)mPchNvsAreaProtocol.Area->PchxDCIPwrDnScale));
    //
    // Disable Pci Memconfig & clear Base address
    //
    PciSegmentWrite16(XdciPciBase + PCI_COMMAND_OFFSET, PciMemConfig);

    if (ClearXdciBar == TRUE) {
      PciSegmentWrite32 ((XdciPciBase + PCI_BASE_ADDRESSREG_OFFSET), 0x0);
      PciSegmentWrite32 ((XdciPciBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 0x0);
    }
  }

  return Status;
}
