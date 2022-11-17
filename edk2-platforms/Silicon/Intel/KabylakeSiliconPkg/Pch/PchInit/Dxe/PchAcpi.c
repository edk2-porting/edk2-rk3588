/** @file
  This is the driver that initializes the Intel PCH.

Copyright (c) 2017 - 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchInit.h"
#include <Library/PchSerialIoLib.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

extern PCH_RST_PCIE_STORAGE_DETECTION mRstPcieStorageDetection[];

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

  for (Index = 0; Index < PCH_SERIALIO_MAX_CONTROLLERS; Index++) {
    mPchNvsAreaProtocol.Area->SMD[Index] = mPchConfigHob->SerialIo.DevMode[Index];
    mPchNvsAreaProtocol.Area->SIR[Index] = (GetInterruptPolicy (Index)).Irq;
    mPchNvsAreaProtocol.Area->SB0[Index] = FindSerialIoBar (Index, 0);
    mPchNvsAreaProtocol.Area->SB1[Index] = FindSerialIoBar (Index, 1);
  }
  if (GetPchSeries () == PchH) {
    mPchNvsAreaProtocol.Area->SMD[PchSerialIoIndexI2C4] = PchSerialIoDisabled;
    mPchNvsAreaProtocol.Area->SMD[PchSerialIoIndexI2C5] = PchSerialIoDisabled;
  }

  //
  // Update GPIO device ACPI variables
  //
  mPchNvsAreaProtocol.Area->GPEN = mPchConfigHob->SerialIo.Gpio;
  mPchNvsAreaProtocol.Area->SGIR = mPchConfigHob->Interrupt.GpioIrqRoute;


  DEBUG ((DEBUG_INFO, "UpdateSerialIoAcpiData() End\n"));

  return EFI_SUCCESS;
}

/**
  Update NVS Area after RST PCIe Storage Remapping and before Boot

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
PchUpdateNvsAreaAfterRemapping (
  VOID
  )
{
  UINTN                 Index;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    mPchNvsAreaProtocol.Area->RstPcieStorageInterfaceType[Index]        = mRstPcieStorageDetection[Index].DeviceInterface;
    mPchNvsAreaProtocol.Area->RstPcieStoragePmCapPtr[Index]             = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.PmCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStoragePcieCapPtr[Index]           = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.PcieCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageL1ssCapPtr[Index]           = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.L1ssCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpL1ssControl2[Index]       = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointL1ssControl2;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpL1ssControl1[Index]       = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointL1ssControl1;
    mPchNvsAreaProtocol.Area->RstPcieStorageLtrCapPtr[Index]            = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.LtrCapPtr;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpLtrData[Index]            = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointLtrData;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpLctlData16[Index]         = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointLctlData16;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpDctlData16[Index]         = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointDctlData16;
    mPchNvsAreaProtocol.Area->RstPcieStorageEpDctl2Data16[Index]        = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.EndpointDctl2Data16;
    mPchNvsAreaProtocol.Area->RstPcieStorageRpDctl2Data16[Index]        = mRstPcieStorageDetection[Index].PchRstPcieStorageSaveRestore.RootPortDctl2Data16;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniqueTableBar[Index]       = mRstPcieStorageDetection[Index].EndPointUniqueMsixTableBar;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniqueTableBarValue[Index]  = mRstPcieStorageDetection[Index].EndPointUniqueMsixTableBarValue;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniquePbaBar[Index]         = mRstPcieStorageDetection[Index].EndPointUniqueMsixPbaBar;
    mPchNvsAreaProtocol.Area->RstPcieStorageUniquePbaBarValue[Index]    = mRstPcieStorageDetection[Index].EndPointUniqueMsixPbaBarValue;
    mPchNvsAreaProtocol.Area->RstPcieStorageRootPortNum[Index]          = mRstPcieStorageDetection[Index].RootPortNum;
  }
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

  PCH_SERIES      PchSeries;

  DEBUG ((DEBUG_INFO, "PchAcpiOnEndOfDxe() Start\n"));

  PchSeries = GetPchSeries ();

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
  //
  // Define and update ASL definitions for Cio2 device (only on PCH-LP).
  //
  if (PchSeries >= PchLp) {
    UpdateCio2AcpiData ();
  }

  //
  // Update Pch Nvs Area
  //
  PchUpdateNvsArea ();

  //
  // Patch PchNvsArea Address
  //
  PatchPchNvsAreaAddress ();

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
  PCH Update NvsArea ExitBootServicesFlag on ExitBootService. This event is used if only ExitBootService is used
  and not in legacy boot

  @retval None
**/
VOID
EFIAPI
PchUpdateNvsOnExitBootServices (
  VOID
  )
{
  mPchNvsAreaProtocol.Area->ExitBootServicesFlag = 1;

  return;
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
  EFI_STATUS                            Status;
  PCH_SERIES                            PchSeries;
  UINTN                                 Index;
  UINT32                                HpetBaseAdress;
  GPIO_GROUP                            GroupToGpeDw0;
  GPIO_GROUP                            GroupToGpeDw1;
  GPIO_GROUP                            GroupToGpeDw2;
  GPIO_GROUP                            Group;
  UINT32                                PadNumber;
  GPIO_PAD                              GpioPad;
  GPIO_PAD_OWN                          PadOwnVal;
  GPIO_CONFIG                           GpioData;
  UINTN                                 RpDev;
  UINTN                                 RpFun;
  UINT32                                Data32;
  UINT16                                Data16;

  Status = EFI_SUCCESS;
  PchSeries = GetPchSeries ();

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
  mPchNvsAreaProtocol.Area->PchSeries     = (UINT16) PchSeries;
  //
  // Update PCHG.
  //
  mPchNvsAreaProtocol.Area->PchGeneration = (UINT16) GetPchGeneration ();
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
  PchAcpiBaseGet (&Data16);
  mPchNvsAreaProtocol.Area->PMBS = Data16;

  PchPwrmBaseGet (&Data32);
  mPchNvsAreaProtocol.Area->PWRM = Data32;

  //
  // Update GPP_X to GPE_DWX mapping.
  //
  GpioGetGroupToGpeDwX (&GroupToGpeDw0, &GroupToGpeDw1, &GroupToGpeDw2);

  //
  // GPEM is an object for informing how GPIO groups are mapped to GPE.
  // Mapping for GPP_x is evaluated from (GPEM >> (GroupNumber*2)) & 0x3
  // Here GroupNumber does not match xxx_GPIO_GROUP type and is always
  // 0 based (GPP_A = 0, both for LP and H)
  // Possible values for each group:
  // 00b - 2-tier
  // 01b - 1-tier, GPE_DW0
  // 10b - 1-tier, GPE_DW1
  // 11b - 1-tier, GPE_DW2
  //
  mPchNvsAreaProtocol.Area->GPEM = (0x1 << (GpioGetGroupIndexFromGroup (GroupToGpeDw0) * 2)) |
    (0x2 << (GpioGetGroupIndexFromGroup (GroupToGpeDw1) * 2)) |
    (0x3 << (GpioGetGroupIndexFromGroup (GroupToGpeDw2) * 2));

  //
  // GP2T[GroupIndex] is an object for storing information about GPIO pads which are
  // enabled for 2-tier GPE event and their interrupt is configured for level
  // 0b - Gpio Pad which is not 2-tier event or hasn't got level interrupt
  // 1b - Gpio Pad enabled for 2-tier event and having level interrupt
  //
  for (Group = GpioGetLowestGroup (); Group <= GpioGetHighestGroup (); Group++) {
    Index = GpioGetGroupIndexFromGroup (Group);
    if (Index >= sizeof (mPchNvsAreaProtocol.Area->GP2T) / sizeof (mPchNvsAreaProtocol.Area->GP2T[0])) {
      DEBUG ((DEBUG_ERROR, "GpioGetGroupIndexFromGroup (%x) = %x out of GP2T table range\n", Group, GpioGetGroupIndexFromGroup (Group)));
      continue;
    }
    mPchNvsAreaProtocol.Area->GP2T[Index] = 0x0;

    if ((Group == GroupToGpeDw0) ||
        (Group == GroupToGpeDw1) ||
        (Group == GroupToGpeDw2)) {
      //
      // not 2-tier GPE
      //
      continue;
    }

    for (PadNumber = 0; PadNumber < GpioGetPadPerGroup (Group); PadNumber++) {

      GpioPad = GpioGetGpioPadFromGroupAndPadNumber (Group, PadNumber);

      GpioGetPadOwnership (GpioPad, &PadOwnVal);
      if (PadOwnVal != GpioPadOwnHost) {
        continue;
      }

      GpioGetPadConfig (GpioPad, &GpioData);

      if (((GpioData.InterruptConfig & B_GPIO_INT_CONFIG_INT_SOURCE_MASK) == GpioIntSci) &&
          ((GpioData.InterruptConfig & B_GPIO_INT_CONFIG_INT_TYPE_MASK) == GpioIntLevel)) {
        //
        // This pad is enabled for GPE and has level interrupt
        //
        mPchNvsAreaProtocol.Area->GP2T[Index] |= 1 << PadNumber;
      }
    }
  }

  //
  // Thermal device in ACPI mode
  //
  mPchNvsAreaProtocol.Area->ThermalDeviceAcpiEnabled = mPchConfigHob->Thermal.ThermalDeviceEnable == 2 ? 1 : 0;
  //
  // Get Thermal Device interrupt line number
  //
  for (Index = 0; Index < mPchConfigHob->Interrupt.NumOfDevIntConfig; Index++) {
    if ((mPchConfigHob->Interrupt.DevIntConfig[Index].Device == PCI_DEVICE_NUMBER_PCH_THERMAL) &&
        (mPchConfigHob->Interrupt.DevIntConfig[Index].Function == PCI_FUNCTION_NUMBER_PCH_THERMAL)) {
      mPchNvsAreaProtocol.Area->ThermalDeviceInterruptLine = mPchConfigHob->Interrupt.DevIntConfig[Index].Irq;
    }
  }

  //
  // SCS Configuration
  //
  // Update eMMC HS400 mode enablement
  //
  mPchNvsAreaProtocol.Area->EMH4 = (UINT8) mPchConfigHob->Scs.ScsEmmcHs400Enabled;

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

  //
  // CPU SKU
  //
  mPchNvsAreaProtocol.Area->CpuSku = GetCpuSku ();

  return Status;
}

/**
  Initialize PCH Nvs Area opeartion region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
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

  return EFI_SUCCESS;
}

