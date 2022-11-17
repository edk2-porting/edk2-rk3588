/** @file

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PchLimits.h>
#include <PiDxe.h>
#include <Protocol/IioUds.h>
#include <PciPlatform.h>
#include <PciPlatformHooks.h>
#include <PciSupportLib.h>
#include <IoApic.h>
#include <Library/S3BootScriptLib.h>
#include <Library/IoLib.h>
#include <Protocol/DynamicSiLibraryProtocol.h>
#include "../PciHostBridge/PciRootBridge.h"
#include "../PciHostBridge/PciHostBridge.h"


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
/**
  Uncomment the PCIDEBUG macro to enable tracing the library activity in a test build.
 **/
#define PCIDEBUG(...) // { DEBUG((DEBUG_INFO, "[PCI] " __VA_ARGS__)); }


/******************************************************************************
 * Variables.
 ******************************************************************************/
SYSTEM_CONFIGURATION  mSystemConfiguration;
EFI_IIO_UDS_PROTOCOL *mIioUds = NULL;


/******************************************************************************
 * Functions.
 ******************************************************************************/
VOID
ChipsetCallback (
  IN  EFI_HANDLE                                    RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS   PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                     Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                      *ContextPtr
  )
{
  EFI_LIST_ENTRY    *NodePtr;
  PCI_CALLBACK_DATA *CallbackDataPtr;

  //
  // Check if the node has been added
  //
  for (NodePtr = GetFirstNode (&mPciPrivateData.PciCallbackList);
       !IsNull (&mPciPrivateData.PciCallbackList, NodePtr);
       NodePtr = GetNextNode (&mPciPrivateData.PciCallbackList, NodePtr)) {
    CallbackDataPtr = PCI_CALLBACK_DATA_FROM_LINK (NodePtr);
    if (CallbackDataPtr->Phase & Phase) {
      (CallbackDataPtr->Function) (RootBridgeHandle, PciAddress, Phase, ContextPtr);
    }
  }
}

/**

    GC_TODO: add routine description

    @param StartBus - GC_TODO: add arg description

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
PciTreeTraverse (
  IN  UINT8       Socket,
  IN  UINT8       Stack,
  IN  UINT8       StartBus
  )
{
  UINT64                          PciAddress;
  UINT8                           Device;
  UINT8                           Func;
  UINT8                           SecondaryBus;
  BOOLEAN                         MultiFunc;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;

  if (Socket >= NELEMENTS (mPciPrivateData.PciRootBridgeIo) ||
      Stack >= NELEMENTS (mPciPrivateData.PciRootBridgeIo[Socket]) ||
      mPciPrivateData.PciRootBridgeIo[Socket][Stack] == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PciRootBridgeIo = mPciPrivateData.PciRootBridgeIo[Socket][Stack];

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    MultiFunc = FALSE;
    for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
      if (IsPciDevicePresent (
            PciRootBridgeIo,
            &mPciPrivateData.Context.PciHeader,
            StartBus,
            Device,
            Func
            )) {
        if ((Func == 0) && IS_PCI_MULTI_FUNC(&mPciPrivateData.Context.PciHeader)) {
          MultiFunc = TRUE;
        }
        PciAddress = EFI_PCI_ADDRESS (StartBus, Device, Func, 0);
        mPciPrivateData.Context.PciRootBridgeIo = PciRootBridgeIo;
        ChipsetCallback (
          mPciPrivateData.RootBridgeHandle,
          *(EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &PciAddress,
          mPciPrivateData.PciEnumerationPhase,
          &(mPciPrivateData.Context)
          );
        if (IS_PCI_BRIDGE (&(mPciPrivateData.Context.PciHeader))) {
          PciAddress = EFI_PCI_ADDRESS (StartBus, Device, Func, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
          PciRootBridgeIo->Pci.Read (
                                PciRootBridgeIo,
                                EfiPciWidthUint8,
                                *(UINT64 *) &PciAddress,
                                1,
                                &SecondaryBus
                                );
          if ((SecondaryBus > 0) && (SecondaryBus < 0xFF)) {
            //
            // Recursive call for next bus in this stack
            //
            PciTreeTraverse (Socket, Stack, SecondaryBus);
          }
        }
      }

      if (!MultiFunc) {
        //
        // Skip sub functions, this is not a multi function device
        //
        Func = PCI_MAX_FUNC;
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Program Io Apic Id

  @param IoApicAddress and IoApicId

  @retval None

**/
VOID
ProgramIoApicId (
  IN UINT32                            IoApicAddress,
  IN UINT8                             IoApicId
  )
{

  UINT32  Data;

  mPciPrivateData.CpuIo->Mem.Read (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_INDEX_OFFSET,
                              1,
                              &Data
                              );

  //
  // IOAPIC is not there
  //
  if (Data == (UINT32) -1) {
    return ;
  }
  //
  // Set up IO APIC ID and enable FSB delivery
  // Use CPU IO protocol since the IO APIC ranges
  // are not included in PCI apertures
  //
  Data = EFI_IO_APIC_ID_REGISTER;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_INDEX_OFFSET,
                              1,
                              &Data
                              );

  Data = IoApicId << EFI_IO_APIC_ID_BITSHIFT;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_DATA_OFFSET,
                              1,
                              &Data
                              );

  Data = EFI_IO_APIC_BOOT_CONFIG_REGISTER;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_INDEX_OFFSET,
                              1,
                              &Data
                              );

  Data = EFI_IO_APIC_FSB_INT_DELIVERY;
  mPciPrivateData.CpuIo->Mem.Write (
                              mPciPrivateData.CpuIo,
                              EfiCpuIoWidthUint32,
                              IoApicAddress + EFI_IO_APIC_DATA_OFFSET,
                              1,
                              &Data
                              );
}

#ifdef EFI_PCI_IOV_SUPPORT
/**

  Initialize the Pci Iov Platform Data.

  @param ImageHandle  -  Handle to the image.
  @param SystemTable  -  Handle to System Table.

  @retval EFI_STATUS  -  Status of the function calling.

**/
EFI_STATUS
EFIAPI
PciPlatformInitPciIovData (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IOV_PLATFORM_POLICY           PciIovPolicy;
  UINT32                                SystemPageSize;
  EFI_PCI_IOV_PLATFORM_PROTOCOL         *gPciIovPlatformProtocol;

  Status = gBS->LocateProtocol (
             &gEfiPciIovPlatformProtocolGuid,
             NULL,
             &gPciIovPlatformProtocol
             );
  if (!EFI_ERROR (Status)) {
    Status = gPciIovPlatformProtocol->GetSystemLowestPageSize (
                                           gPciIovPlatformProtocol,
                                           &SystemPageSize
                                           );
    if (!EFI_ERROR (Status)) {
      Status = PcdSet32S (PcdSrIovSystemPageSize, SystemPageSize);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    } else {
      return Status;
    }
    Status = gPciIovPlatformProtocol->GetPlatformPolicy (
                                           gPciIovPlatformProtocol,
                                           &PciIovPolicy
                                           );
    if (!EFI_ERROR (Status)) {
      if (PciIovPolicy & EFI_PCI_IOV_POLICY_ARI) {
        Status = PcdSetBoolS (PcdAriSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      } else {
        Status = PcdSetBoolS (PcdAriSupport, FALSE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      }
      if (PciIovPolicy & EFI_PCI_IOV_POLICY_SRIOV) {
        Status = PcdSetBoolS (PcdSrIovSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      } else {
        Status = PcdSetBoolS (PcdSrIovSupport, FALSE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      }
      if (PciIovPolicy & EFI_PCI_IOV_POLICY_MRIOV) {
        Status = PcdSetBoolS (PcdMrIovSupport, TRUE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      } else {
        Status = PcdSetBoolS (PcdMrIovSupport, FALSE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      }
    } else {
      return Status;
    }
    DEBUG ((
          EFI_D_INFO,
          " Initialized SR-IOV Platform Data: PCIIovPolicy = 0x%x; SystemPageSize = 0x%x;\n",
          PciIovPolicy, SystemPageSize
          ));
  } else {
    DEBUG ((
          EFI_D_INFO,
          " Using default values for SystemPageSize;\n"
          ));
  }
  return Status;
}
#endif

/**

  Platform Pci Express init.

  @param HostBridgeInstance  -  Pointer to Host Bridge private data
                                does not support 64 bit memory addresses.

  @retval EFI_SUCCESS  -  Success.

**/
EFI_STATUS
PciPlatformEarlyInit (
  VOID
  )
{
  EFI_STATUS                        Status;

  Status = EFI_SUCCESS;

#ifdef EFI_PCI_IOV_SUPPORT
  Status = PciPlatformInitPciIovData();  // Update IOV PCD values
#endif
  return Status;
}

/**

  Attempts to set the XHCI controller's PCI CMD.MSE and CMD.BME bits to enable OS kernel debugging over XHCI.

**/
VOID
AttemptToSetXhciMse (
  )

{
  UINT32                            XhciBar;
  UINT16                            Command;
  DYNAMIC_SI_LIBARY_PROTOCOL        *DynamicSiLibraryProtocol = NULL;
  EFI_STATUS                        Status;

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Step 1. Make sure the XHCI BAR is initialized.
  //         Check if lower 32 bits of 64-bit BAR are configured.
  //
  XhciBar = MmioRead32 (DynamicSiLibraryProtocol->MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI) + R_XHCI_CFG_BAR0) & ~(0xF);
  if (XhciBar == 0xFFFFFFF0) {
    return;
  }
  if ((XhciBar & 0xFFFF0000) == 0) {
    //
    // If lower 32 bits are not configured, check upper 32 bits.
    //
    XhciBar = MmioRead32 (DynamicSiLibraryProtocol->MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI) + R_XHCI_CFG_BAR0 + 4);
    if (XhciBar == 0) {
      return;
    }
  }

  //
  // Step 2. If XHCI's MSE (Memory Space Enable) or BME (Bus Master Enable) bits are cleared, set them.
  //
  Command = MmioRead16 (DynamicSiLibraryProtocol->MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI) + PCI_COMMAND_OFFSET);
  if ((Command & (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) != (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) {
    MmioOr16 (DynamicSiLibraryProtocol->MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI) + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
  }
}


/**

  Init pci device registers after the device resources have been allocated, so
  that devices behind a bus could be accessed.

  @param HostBridgeInstance  -  PCI_HOST_BRIDGE_INSTANCE.

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
PciPlatformPostInit (
  VOID
  )
{
  DYNAMIC_SI_LIBARY_PROTOCOL        *DynamicSiLibraryProtocol = NULL;
  EFI_STATUS                        Status;

  //
  // Program all the IOAPIC in system
  //

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  UINT8 Socket, Stack, IoApicId, ApicIndex = 0;
  CPU_CSR_ACCESS_VAR   *CpuCsrAccessVarPtr = NULL;
  Stack = 0;
  IoApicId = 0;
  CpuCsrAccessVarPtr = DynamicSiLibraryProtocol->GetSysCpuCsrAccessVar ();
  DEBUG ((DEBUG_INFO, "PciPlatformPostInit: setting up IOAPIC for PCH\n"));
  ProgramIoApicId (mIioUds->IioUdsPtr->PlatformData.IIO_resource[0].StackRes[0].IoApicBase, PCH_IOAPIC_ID);
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!(CpuCsrAccessVarPtr->socketPresentBitMap & (1 << Socket))) {
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++, ApicIndex++) {
      if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
        continue;
      }
      switch (ApicIndex) {
      case 0:
        IoApicId = PC00_IOAPIC_ID;
        break;
      case 1:
        IoApicId = PC01_IOAPIC_ID;
        break;
      case 2:
        IoApicId = PC02_IOAPIC_ID;
        break;
      case 3:
        IoApicId = PC03_IOAPIC_ID;
        break;
      case 4:
        IoApicId = PC04_IOAPIC_ID;
        break;
      case 5:
        IoApicId = PC05_IOAPIC_ID;
        break;
      case 6:
        IoApicId = PC06_IOAPIC_ID;
        break;
      case 7:
        IoApicId = PC07_IOAPIC_ID;
        break;
      case 8:
        IoApicId = PC08_IOAPIC_ID;
        break;
      case 9:
        IoApicId = PC09_IOAPIC_ID;
        break;
      case 10:
        IoApicId = PC10_IOAPIC_ID;
        break;
      case 11:
        IoApicId = PC11_IOAPIC_ID;
        break;
      case 12:
        IoApicId = PC12_IOAPIC_ID;
        break;
      case 13:
        IoApicId = PC13_IOAPIC_ID;
        break;
      case 14:
        IoApicId = PC14_IOAPIC_ID;
        break;
      case 15:
        IoApicId = PC15_IOAPIC_ID;
        break;
      case 16:
        IoApicId = PC16_IOAPIC_ID;
        break;
      case 17:
        IoApicId = PC17_IOAPIC_ID;
        break;
      case 18:
        IoApicId = PC18_IOAPIC_ID;
        break;
      case 19:
        IoApicId = PC19_IOAPIC_ID;
        break;
      case 20:
        IoApicId = PC20_IOAPIC_ID;
        break;
      case 21:
        IoApicId = PC21_IOAPIC_ID;
        break;
      case 22:
        IoApicId = PC22_IOAPIC_ID;
        break;
      case 23:
        IoApicId = PC23_IOAPIC_ID;
        break;
      case 24:
        IoApicId = PC24_IOAPIC_ID;
        break;
      case 25:
        IoApicId = PC25_IOAPIC_ID;
        break;
      case 26:
        IoApicId = PC26_IOAPIC_ID;
        break;
      case 27:
        IoApicId = PC27_IOAPIC_ID;
        break;
      case 28:
        IoApicId = PC28_IOAPIC_ID;
        break;
      case 29:
        IoApicId = PC29_IOAPIC_ID;
        break;
      case 30:
        IoApicId = PC30_IOAPIC_ID;
        break;
      case 31:
        IoApicId = PC31_IOAPIC_ID;
        break;
      case 32:
        IoApicId = PC32_IOAPIC_ID;
        break;
      case 33:
        IoApicId = PC33_IOAPIC_ID;
        break;
      case 34:
        IoApicId = PC34_IOAPIC_ID;
        break;
      case 35:
        IoApicId = PC35_IOAPIC_ID;
        break;
      case 36:
        IoApicId = PC36_IOAPIC_ID;
        break;
      case 37:
        IoApicId = PC37_IOAPIC_ID;
        break;
      case 38:
        IoApicId = PC38_IOAPIC_ID;
        break;
      case 39:
        IoApicId = PC39_IOAPIC_ID;
        break;
      case 40:
        IoApicId = PC40_IOAPIC_ID;
        break;
      case 41:
        IoApicId = PC41_IOAPIC_ID;
        break;
      case 42:
        IoApicId = PC42_IOAPIC_ID;
        break;
      case 43:
        IoApicId = PC43_IOAPIC_ID;
        break;
      case 44:
        IoApicId = PC44_IOAPIC_ID;
        break;
      case 45:
        IoApicId = PC45_IOAPIC_ID;
        break;
      case 46:
        IoApicId = PC46_IOAPIC_ID;
        break;
      case 47:
        IoApicId = PC47_IOAPIC_ID;
        break;
      default:
        break;
      }
      if ((Socket == 0) && (Stack == 0)) {
        ProgramIoApicId ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase + 0x1000), IoApicId);
      } else {
        ProgramIoApicId (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].IoApicBase, IoApicId);
      }
    }
  }

  AttemptToSetXhciMse ();

  return EFI_SUCCESS;
}

/**

  The PlatformPrepController() function can be used to notify the platform driver so that
  it can perform platform-specific actions. No specific actions are required.
  Several notification points are defined at this time. More synchronization points may be
  added as required in the future. The PCI bus driver calls the platform driver twice for
  every PCI controller-once before the PCI Host Bridge Resource Allocation Protocol driver
  is notified, and once after the PCI Host Bridge Resource Allocation Protocol driver has
  been notified.
  This member function may not perform any error checking on the input parameters. It also
  does not return any error codes. If this member function detects any error condition, it
  needs to handle those errors on its own because there is no way to surface any errors to
  the caller.

  @param This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.
  @param HostBridge    -  The associated PCI Host bridge handle.
  @param RootBridge    -  The associated PCI root bridge handle.
  @param PciAddress    -  The address of the PCI device on the PCI bus.
  @param Phase         -  The phase of the PCI controller enumeration.
  @param ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

  @retval EFI_SUCCESS     -  The function completed successfully.
  @retval EFI_UNSUPPORTED -  Not supported.

**/
EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *RootBridgeIo;
  PCI_TYPE00                       Pci0;
  UINT64                           Address;
  UINT8                            SecBus;
  UINT8                            Device;
  UINT8                            Func;
  UINT64                           DummyData = 0xFFFFFFFF;
  UINT32                           DidVid;
  DYNAMIC_SI_LIBARY_PROTOCOL       *DynamicSiLibraryProtocol = NULL;

  if (mPciPrivateData.RootBridgeHandle == NULL) {
    mPciPrivateData.RootBridgeHandle = RootBridge;
  }

  Status = gBS->HandleProtocol (
                    mPciPrivateData.RootBridgeHandle,
                    &gEfiPciRootBridgeIoProtocolGuid,
                    (VOID **) &RootBridgeIo
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

// Workaround for PCI devices under Pilot IV, this video controller can only be exposed if write 0xFFFFFFFF to it and read back
  if (Phase == EfiPciBeforeChildBusEnumeration && ChipsetPhase == ChipsetExit ) {

// Read the entire config header
    Address = EFI_PCI_ADDRESS (PciAddress.Bus, PciAddress.Device, PciAddress.Function, 0);
    Status = RootBridgeIo->Pci.Read (
                                RootBridgeIo,
                                EfiPciWidthUint32,
                                Address,
                                sizeof (PCI_TYPE00) / sizeof (UINT32),
                                &Pci0
                                );

    if (!EFI_ERROR (Status) && IS_PCI_BRIDGE(&Pci0)) {

      // Read the secondary bus number
      Address = EFI_PCI_ADDRESS (PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
      Status = RootBridgeIo->Pci.Read (
                                RootBridgeIo,
                                EfiPciWidthUint8,
                                Address,
                                1,
                                &SecBus
                                );

      if (!EFI_ERROR (Status)) {

        //
        // For this bridge with existing secondary bus apply PCI Intel WAs
        //
        DidVid = ((Pci0.Hdr).DeviceId << 16) | (Pci0.Hdr).VendorId;
        DynamicSiLibraryProtocol->IioPciHookBeforeEnumeration ((UINT8)RootBridgeIo->SegmentNumber, PciAddress.Bus, PciAddress.Device, PciAddress.Function, DidVid);

        for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
          for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
            Address = EFI_PCI_ADDRESS (SecBus, Device, Func, 0);
            Status = RootBridgeIo->Pci.Read (
                                      RootBridgeIo,
                                      EfiPciWidthUint32,
                                      Address,
                                      1,
                                      &Pci0
                                      );

            if ( !EFI_ERROR (Status) && (Pci0.Hdr).VendorId == 0xffff) {

              Status = RootBridgeIo->Pci.Write(
                                        RootBridgeIo,
                                        EfiPciWidthUint32,
                                        Address,
                                        1,
                                        &DummyData
                                        );
              PCIDEBUG ("%a: For B(0x%x)-D(0x%x)-F(0x%x),Pci.Write() returns with %r\n",
                        __FUNCTION__, SecBus, Device, Func, Status);

              if (EFI_ERROR (Status)) {
                //
                // If error, go to next function
                //
                continue;
              } else {
                Func = PCI_MAX_FUNC;    // skip the remaining function
              }
            }
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Perform initialization by the phase indicated.

  @param This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.
  @param HostBridge    -  The associated PCI Host bridge handle.
  @param Phase         -  The phase of the PCI controller enumeration.
  @param ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

  @retval EFI_SUCCESS  -  Must return with success.

**/
EFI_STATUS
EFIAPI
PhaseNotify (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE  Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  EFI_STATUS                                        Status = EFI_SUCCESS;
  UINT16                                            StackBit;
  UINT8                                             Socket;
  UINT8                                             Stack;
  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL  *HostResAllocPtr;
  PCI_HOST_BRIDGE_INSTANCE                          *HostBridgePtr;
  PCI_ROOT_BRIDGE_INSTANCE                          *RootBridgePtr;
  LIST_ENTRY                                        *NodePtr;
  CPU_CSR_ACCESS_VAR                                *CpuCsrAccessVarPtr;
  DYNAMIC_SI_LIBARY_PROTOCOL                        *DynamicSiLibraryProtocol = NULL;

  static CHAR8 *NotifyPhase2Name[] = {"EfiPciHostBridgeBeginEnumeration",
                                      "EfiPciHostBridgeBeginBusAllocation",
                                      "EfiPciHostBridgeEndBusAllocation",
                                      "EfiPciHostBridgeBeginResourceAllocation",
                                      "EfiPciHostBridgeAllocateResources",
                                      "EfiPciHostBridgeSetResources",
                                      "EfiPciHostBridgeFreeResources",
                                      "EfiPciHostBridgeEndResourceAllocation",
                                      "EfiPciHostBridgeEndEnumeration"};

  if (Phase < NELEMENTS (NotifyPhase2Name)) {
    DEBUG ((DEBUG_INFO, "[PCI] %a phase notified (execution %d)\n", NotifyPhase2Name[Phase], ChipsetPhase));
  } else {
    DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Unknown phase %d notified (execution %d)\n", Phase, ChipsetPhase));
  }

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  CpuCsrAccessVarPtr = DynamicSiLibraryProtocol->GetSysCpuCsrAccessVar ();

  if (ChipsetPhase == ChipsetEntry) {
    return EFI_SUCCESS;
  }
  //
  // If for multiple Host bridges, need special consideration
  //
  switch (Phase) {

  case EfiPciHostBridgeBeginEnumeration:
    //
    // Pre-initialization before PCI bus enumeration
    // No bus number and no PCI resource
    // Locate the IIO Protocol Interface
    //
    Status = gBS->LocateProtocol (
                    &gEfiIioUdsProtocolGuid,
                    NULL,
                    &mIioUds
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->LocateProtocol (
                    &gEfiCpuIo2ProtocolGuid,
                    NULL,
                    &mPciPrivateData.CpuIo
                    );
    ASSERT_EFI_ERROR (Status);

    mPciPrivateData.Context.CpuIo = mPciPrivateData.CpuIo;
    DEBUG ((DEBUG_INFO, "[PCI] Platform Pre-Initialization (Before bus scanning)\n"));
    //
    // Locate gEfiPciRootBridgeIoProtocolGuid instance created for each IIO stack.
    // They were created by host bridge driver and linked to the
    // gEfiPciHostBridgeResourceAllocationProtocolGuid protocol.
    //
    Status = gBS->LocateProtocol (
                    &gEfiPciHostBridgeResourceAllocationProtocolGuid,
                    NULL,
                    &HostResAllocPtr
                    );
    ASSERT_EFI_ERROR (Status);

    HostBridgePtr = CR (HostResAllocPtr, PCI_HOST_BRIDGE_INSTANCE, ResAlloc, PCI_HOST_BRIDGE_SIGNATURE);
    for (NodePtr = GetFirstNode (&HostBridgePtr->RootBridges);
         !IsNull (&HostBridgePtr->RootBridges, NodePtr);
         NodePtr = GetNextNode (&HostBridgePtr->RootBridges, NodePtr)) {
      RootBridgePtr = CR (NodePtr, PCI_ROOT_BRIDGE_INSTANCE, Link, PCI_ROOT_BRIDGE_SIGNATURE);
      for (Socket = 0; Socket < NELEMENTS (mPciPrivateData.PciRootBridgeIo); Socket++) {
        if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) {
          continue;
        }
        for (StackBit = 1, Stack = 0;
             Stack < NELEMENTS (mPciPrivateData.PciRootBridgeIo[Socket]);
             StackBit <<= 1, Stack++) {
          if ((CpuCsrAccessVarPtr->stackPresentBitmap[Socket] & StackBit) &&
              CpuCsrAccessVarPtr->StackBus[Socket][Stack] == RootBridgePtr->Aperture.BusBase) {
            //
            // This is the stack handled by this instance of root bridge IO protocol. Store it for future use.
            //
            mPciPrivateData.PciRootBridgeIo[Socket][Stack] = &RootBridgePtr->RootBridgeIo;
            Socket = NELEMENTS (mPciPrivateData.PciRootBridgeIo);
            break;
          }
        }
      }
    }
    PciPlatformEarlyInit ();
    break;

  case EfiPciHostBridgeEndBusAllocation:
    //
    // There are two rounds PCI bus scanning
    // First round will initilize the PCI hotplug device
    // Second round will be the final one
    //
    if (mPciPrivateData.BusAssignedTime == 0) {
      mPciPrivateData.PciEnumerationPhase = EfiPciEnumerationDeviceScanning;
      for (Socket = 0; Socket < NELEMENTS (mPciPrivateData.PciRootBridgeIo); Socket++) {
        if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) {
          continue;
        }
        for (Stack = 0; Stack < NELEMENTS (mPciPrivateData.PciRootBridgeIo[Socket]); Stack ++) {
          if (mPciPrivateData.PciRootBridgeIo[Socket][Stack] == NULL) {
            continue;
          }
          PciTreeTraverse (Socket, Stack, CpuCsrAccessVarPtr->StackBus[Socket][Stack]);
        }
      }
      mPciPrivateData.BusAssignedTime++;
      DEBUG ((DEBUG_INFO, "[PCI] Platform bus assigned\n"));
    }
    break;

  case EfiPciHostBridgeBeginResourceAllocation:
    //
    // PCI bus number has been assigned, but resource is still empty
    //
    DEBUG ((DEBUG_INFO, "[PCI] Platform Mid-Initialization (After bus number assignment)\n"));
    mPciPrivateData.PciEnumerationPhase = EfiPciEnumerationBusNumberAssigned;
    for (Socket = 0; Socket < NELEMENTS (mPciPrivateData.PciRootBridgeIo); Socket++) {
      if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) {
        continue;
      }
      for (Stack = 0; Stack < NELEMENTS (mPciPrivateData.PciRootBridgeIo[Socket]); Stack ++) {
        if (mPciPrivateData.PciRootBridgeIo[Socket][Stack] == NULL) {
          continue;
        }
        PciTreeTraverse (Socket, Stack, CpuCsrAccessVarPtr->StackBus[Socket][Stack]);
      }
    }
    //PciPlatformMidInit ();
    break;

  case EfiPciHostBridgeEndResourceAllocation:
    //
    // Resource enumeration is done.
    // Both bus number and resource have been assigned
    // Do any post initialization.
    //
    DEBUG ((DEBUG_INFO, "[PCI] Platform Post-Initialization (After resource alloction)\n"));
    mPciPrivateData.PciEnumerationPhase = EfiPciEnumerationResourceAssigned;
    for (Socket = 0; Socket < NELEMENTS (mPciPrivateData.PciRootBridgeIo); Socket++) {
      if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) {
        continue;
      }
      for (Stack = 0; Stack < NELEMENTS (mPciPrivateData.PciRootBridgeIo[Socket]); Stack ++) {
        if (mPciPrivateData.PciRootBridgeIo[Socket][Stack] == NULL) {
          continue;
        }
        PciTreeTraverse (Socket, Stack, CpuCsrAccessVarPtr->StackBus[Socket][Stack]);
      }
    }
    PciPlatformPostInit ();
    break;

  default:
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
