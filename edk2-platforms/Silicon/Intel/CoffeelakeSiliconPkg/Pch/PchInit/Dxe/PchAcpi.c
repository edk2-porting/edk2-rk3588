/** @file
  This is the driver that initializes the Intel PCH.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Pci.h>

#include "PchInit.h"
#include <Protocol/PchPolicy.h>
#include <ConfigBlock/GpioDevConfig.h>
#include <ConfigBlock/ScsConfig.h>
#include <Library/AslUpdateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchGbeLib.h>
#include <Private/PchRstHob.h>
#include <Library/PchPcieRpLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SataLib.h>
#include <Register/PchRegsSata.h>
#include <TraceHubCommonConfig.h>
#include <PchReservedResources.h>
#include <Register/PchRegsTraceHub.h>
#include <Library/ConfigBlockLib.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

/**
  Retrieve interrupt information about a PCH device from policy

  @param[in] Device                     PCI device number

  @retval PCH_DEVICE_INTERRUPT_CONFIG   structure with device's interrupt information
**/
PCH_DEVICE_INTERRUPT_CONFIG
GetInterruptPolicy (
  IN PCH_SERIAL_IO_CONTROLLER  Device
  )
{
  PCH_DEVICE_INTERRUPT_CONFIG EmptyRecord;
  UINT8                       DevNum;
  UINT8                       FuncNum;
  UINT8                       Index;

  ZeroMem (&EmptyRecord, sizeof (PCH_DEVICE_INTERRUPT_CONFIG));
  DevNum  = GetSerialIoDeviceNumber (Device);
  FuncNum = GetSerialIoFunctionNumber (Device);

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

  @retval EFI_SUCCESS                   The function completed successfully
**/
EFI_STATUS
UpdateSerialIoAcpiData (
  VOID
  )
{
  PCH_SERIAL_IO_CONTROLLER Index;

  for (Index = 0; Index < GetPchMaxSerialIoControllersNum (); Index++) {
    mPchNvsAreaProtocol.Area->SMD[Index] = mPchConfigHob->SerialIo.DevMode[Index];
    mPchNvsAreaProtocol.Area->SIR[Index] = (GetInterruptPolicy (Index)).Irq;
    mPchNvsAreaProtocol.Area->SB0[Index] = (UINT32) FindSerialIoBar (Index, 0);
    mPchNvsAreaProtocol.Area->SB1[Index] = (UINT32) FindSerialIoBar (Index, 1);
  }
  if (IsPchH ()) {
    mPchNvsAreaProtocol.Area->SMD[PchSerialIoIndexI2C4] = PchSerialIoDisabled;
    mPchNvsAreaProtocol.Area->SMD[PchSerialIoIndexI2C5] = PchSerialIoDisabled;
  }

  return EFI_SUCCESS;
}

/**
  Update NVS Area after RST PCIe Storage Remapping and before Boot
**/
VOID
PchUpdateNvsAreaAfterRemapping (
  VOID
  )
{
  UINTN                 Index;
  VOID                  *Hob;
  PCH_RST_HOB           *RstHob;

  Hob = GetFirstGuidHob (&gPchRstHobGuid);
  if (Hob == NULL) {
    DEBUG (( DEBUG_INFO , "PchUpdateNvsAreaAfterRemapping: cannot fetch RstHob" ));
    return;
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
  EFI_STATUS  Status;

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

  //
  // Update Pch Nvs Area
  //
  Status = PchUpdateNvsArea ();
  if (EFI_ERROR (Status)) {
    return;
  }

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
  PCH_GPIO_DXE_CONFIG   *GpioDxeConfig;

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
    Status = GetPchPcieRpDevFun (Index, &RpDev, &RpFun);
    ASSERT_EFI_ERROR (Status);

    Data32 = ((UINT8) RpDev << 16) | (UINT8) RpFun;
    mPchNvsAreaProtocol.Area->RpAddress[Index] = Data32;

    //
    // Update Maximum Snoop Latency and Maximum No-Snoop Latency values for PCIE
    //
    mPchNvsAreaProtocol.Area->PcieLtrMaxSnoopLatency[Index]   = mPchConfigHob->PcieRp.RootPort[Index].LtrMaxSnoopLatency;
    mPchNvsAreaProtocol.Area->PcieLtrMaxNoSnoopLatency[Index] = mPchConfigHob->PcieRp.RootPort[Index].LtrMaxNoSnoopLatency;
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
  // Update PMC ACPIBASE and PWRMBASE
  //
  mPchNvsAreaProtocol.Area->PMBS = PmcGetAcpiBase ();

  mPchNvsAreaProtocol.Area->PWRM = PmcGetPwrmBase ();

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
  // Update eMMC HS400 mode enablement
  //
  mPchNvsAreaProtocol.Area->EMH4 = (UINT8) mPchConfigHob->Scs.ScsEmmcHs400Enabled;
  mPchNvsAreaProtocol.Area->EmmcEnabled = (UINT8) mPchConfigHob->Scs.ScsEmmcEnabled;

  //
  // Update eMMC Driver Strength
  // Per eMMC 5.01 JEDEC Specification (JESD84-B50.1, Table 186)
  // Nominal Impedance - Driver Type Values:
  // 50 Ohm              0x0
  // 33 Ohm              0x1
  // 40 Ohm              0x4
  //
  switch (mPchConfigHob->Scs.ScsEmmcHs400DriverStrength) {
    case DriverStrength33Ohm:
      mPchNvsAreaProtocol.Area->EMDS = 0x1;
      break;
    case DriverStrength40Ohm:
      mPchNvsAreaProtocol.Area->EMDS = 0x4;
      break;
    case DriverStrength50Ohm:
    default:
      mPchNvsAreaProtocol.Area->EMDS = 0x0;
  }

  mPchNvsAreaProtocol.Area->SdPowerEnableActiveHigh = (UINT8) mPchConfigHob->Scs.ScsSdPowerEnableActiveHigh;
  mPchNvsAreaProtocol.Area->SdCardEnabled           = (UINT8) mPchConfigHob->Scs.ScsSdCardEnabled;

  //
  // SATA configuration.
  //
  if (PciSegmentRead16 (GetSataRegBase (SATA_1_CONTROLLER_INDEX) + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    mPchNvsAreaProtocol.Area->SataPortPresence = 0;
  } else {
    mPchNvsAreaProtocol.Area->SataPortPresence = PciSegmentRead8 (GetSataRegBase (SATA_1_CONTROLLER_INDEX) + R_SATA_CFG_PCS + 2);
  }

  //
  // CPU SKU
  //
  mPchNvsAreaProtocol.Area->CpuSku = GetCpuSku ();

  mPchNvsAreaProtocol.Area->SlpS0VmRuntimeControl = (UINT8)mPchConfigHob->Pm.SlpS0VmRuntimeControl;
  mPchNvsAreaProtocol.Area->SlpS0Vm070VSupport    = (UINT8)mPchConfigHob->Pm.SlpS0Vm070VSupport;
  mPchNvsAreaProtocol.Area->SlpS0Vm075VSupport    = (UINT8)mPchConfigHob->Pm.SlpS0Vm075VSupport;
  mPchNvsAreaProtocol.Area->PsOnEnable            = (UINT8)mPchConfigHob->Pm.PsOnEnable;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    mPchNvsAreaProtocol.Area->LtrEnable[Index]  = (UINT8)mPchConfigHob->PcieRp.RootPort[Index].LtrEnable;
  }

  mPchNvsAreaProtocol.Area->GBES = PchIsGbePresent ();

  //
  // Update PCH Trace Hub Mode
  //
  mPchNvsAreaProtocol.Area->PchTraceHubMode     = (UINT8) mPchConfigHob->PchTraceHub.PchTraceHubMode;
  //
  // if SCRPD0[24] is set, force TH to be host debugger mode.
  //
  if (MmioRead32 (PCH_TRACE_HUB_MTB_BASE_ADDRESS) != 0xFFFFFFFF) {
    if (MmioRead32 (PCH_TRACE_HUB_MTB_BASE_ADDRESS + R_TRACE_HUB_MEM_CSR_MTB_SCRATCHPAD0) & BIT24) {
      mPchNvsAreaProtocol.Area->PchTraceHubMode = TraceHubModeHostDebugger;
    }
  }

  //
  // Update TWMB, Temp memory base address
  //
  mPchNvsAreaProtocol.Area->TempRsvdMemBase = (UINT32) PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  return Status;
}

/**
  Initialize PCH Nvs Area opeartion region.

**/
VOID
PatchPchNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;

  Address = (UINT32) (UINTN) mPchNvsAreaProtocol.Area;
  Length  = (UINT16) sizeof (PCH_NVS_AREA);
  DEBUG ((DEBUG_INFO, "PatchPchNvsAreaAddress: PCH NVS Address %x Length %x\n", Address, Length));
  Status  = UpdateNameAslCode (SIGNATURE_32 ('P','N','V','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('P','N','V','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);
}

