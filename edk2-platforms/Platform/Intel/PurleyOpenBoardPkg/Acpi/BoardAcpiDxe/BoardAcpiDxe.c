/** @file

Copyright (c) 2018 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BoardAcpiDxe.h"

EFI_STATUS
PatchDsdtTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  );

#pragma optimize("",off)

BIOS_ACPI_PARAM                *mAcpiParameter;

EFI_IIO_UDS_PROTOCOL        *mIioUds;

UINT32                      mNumOfBitShift;
BOOLEAN                     mForceX2ApicId;
BOOLEAN                     mX2ApicEnabled;

struct SystemMemoryMapHob   *mSystemMemoryMap;

SOCKET_MP_LINK_CONFIGURATION  mSocketMpLinkConfiguration;
SOCKET_IIO_CONFIGURATION      mSocketIioConfiguration;
SOCKET_POWERMANAGEMENT_CONFIGURATION mSocketPowermanagementConfiguration;

BOOLEAN               mFirstNotify;
PCH_RC_CONFIGURATION  mPchRcConfiguration;

UINT8                 mKBPresent = 0;
UINT8                 mMousePresent = 0;

/**

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param Protocol  -  The protocol to find.
  Instance  -  Return pointer to the first instance of the protocol.
  Type      -  The type of protocol to locate.

  @retval EFI_SUCCESS           -  The function completed successfully.
  @retval EFI_NOT_FOUND         -  The protocol could not be located.
  @retval EFI_OUT_OF_RESOURCES  -  There are not enough resources to find the protocol.

**/
EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  IN   EFI_GUID       *gEfiAcpiMultiTableStorageGuid,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  Protocol,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }
  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    Protocol,
                    Instance
                    );
    ASSERT (!EFI_ERROR (Status));

    if (!Type) {
      //
      // Not looking for the FV protocol, so find the first instance of the
      // protocol.  There should not be any errors because our handle buffer
      // should always contain at least one or LocateHandleBuffer would have
      // returned not found.
      //
      break;
    }
    //
    // See if it has the ACPI storage file
    //
    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL *) (*Instance))->ReadFile (
                                                              *Instance,
                                                              gEfiAcpiMultiTableStorageGuid,
                                                              NULL,
                                                              &Size,
                                                              &FileType,
                                                              &Attributes,
                                                              &FvStatus
                                                              );

    //
    // If we found it, then we are done
    //
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}

/**

    GC_TODO: add routine description

    @param None

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
PlatformHookInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_PHYSICAL_ADDRESS          AcpiParameterAddr;
  UINT32                        RegEax;
  UINT32                        RegEbx;
  UINT32                        RegEcx;
  UINT32                        RegEdx;

  CopyMem (&mSocketMpLinkConfiguration, PcdGetPtr(PcdSocketMpLinkConfigData), sizeof(SOCKET_MP_LINK_CONFIGURATION));
  CopyMem (&mSocketPowermanagementConfiguration, PcdGetPtr(PcdSocketPowerManagementConfigData), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&mSocketIioConfiguration, PcdGetPtr(PcdSocketIioConfigData), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&mPchRcConfiguration, PcdGetPtr(PcdPchRcConfigurationData), sizeof(PCH_RC_CONFIGURATION));

  DEBUG ((DEBUG_INFO, "mX2ApicEnabled - 0x%x\n", mX2ApicEnabled));
  DEBUG ((DEBUG_INFO, "mForceX2ApicId - 0x%x\n", mForceX2ApicId));

  {
    UINT32  Index;

    for (Index = 0; Index < 4; Index++) {
      AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, Index, &RegEax, &RegEbx, &RegEcx, &RegEdx);
      DEBUG ((DEBUG_INFO, "CPUID(0xB - %d) - 0x%08x.0x%08x.0x%08x.0x%08x\n", Index, RegEax, RegEbx, RegEcx, RegEdx));
    }
  }

  //
  // Allocate 256 runtime memory to pass ACPI parameter
  // This Address must be < 4G because we only have 32bit in the dsdt
  //
  AcpiParameterAddr = 0xffffffff;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (sizeof(BIOS_ACPI_PARAM)),
                  &AcpiParameterAddr
                  );
  ASSERT_EFI_ERROR (Status);
  mAcpiParameter = (BIOS_ACPI_PARAM *)AcpiParameterAddr;

  DEBUG ((EFI_D_ERROR, "ACPI Parameter Block Address: 0x%X\n", mAcpiParameter));
  Status = PcdSet64S (PcdAcpiGnvsAddress, (UINT64)(UINTN)mAcpiParameter);
  ASSERT_EFI_ERROR (Status);

  ZeroMem (mAcpiParameter, sizeof (BIOS_ACPI_PARAM));
  mAcpiParameter->PlatformId    = 0;
#if MAX_SOCKET > 4
  mAcpiParameter->IoApicEnable  = PcdGet32 (PcdPcIoApicEnable);
#else
  mAcpiParameter->IoApicEnable  = (PcdGet32 (PcdPcIoApicEnable) << 1) | 1;
#endif
  DEBUG((EFI_D_ERROR, "io apic settings:%d\n", mAcpiParameter->IoApicEnable));

  AsmCpuid (CPUID_VERSION_INFO,  &RegEax, &RegEbx, &RegEcx, &RegEdx);
  mAcpiParameter->ProcessorId = (RegEax & 0xFFFF0);

  // support up to 64 threads/socket
  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;

  // Set the bit shift value for CPU SKU
  mAcpiParameter->CpuSkuNumOfBitShift = (UINT8) mNumOfBitShift;

  mAcpiParameter->ProcessorApicIdBase[0] = (UINT32) (0 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[1] = (UINT32) (1 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[2] = (UINT32) (2 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[3] = (UINT32) (3 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[4] = (UINT32) (4 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[5] = (UINT32) (5 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[6] = (UINT32) (6 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[7] = (UINT32) (7 << mNumOfBitShift);

  if(mForceX2ApicId) {
    mAcpiParameter->ProcessorApicIdBase[0] = 0x7F00;
    mAcpiParameter->ProcessorApicIdBase[1] = 0x7F20;
    mAcpiParameter->ProcessorApicIdBase[2] = 0x7F40;
    mAcpiParameter->ProcessorApicIdBase[3] = 0x7F60;
    mAcpiParameter->ProcessorApicIdBase[4] = 0x7F80;
    mAcpiParameter->ProcessorApicIdBase[5] = 0x7Fa0;
    mAcpiParameter->ProcessorApicIdBase[6] = 0x7Fc0;
    mAcpiParameter->ProcessorApicIdBase[7] = 0x7Fe0;

     if (mNumOfBitShift == 4) {
      mAcpiParameter->ProcessorApicIdBase[0] = 0x7F00;
      mAcpiParameter->ProcessorApicIdBase[1] = 0x7F10;
      mAcpiParameter->ProcessorApicIdBase[2] = 0x7F20;
      mAcpiParameter->ProcessorApicIdBase[3] = 0x7F30;
      mAcpiParameter->ProcessorApicIdBase[4] = 0x7F40;
      mAcpiParameter->ProcessorApicIdBase[5] = 0x7F50;
      mAcpiParameter->ProcessorApicIdBase[6] = 0x7F60;
      mAcpiParameter->ProcessorApicIdBase[7] = 0x7F70;
    } else if(mNumOfBitShift == 6) {
      mAcpiParameter->ProcessorApicIdBase[0] = 0x7E00;
      mAcpiParameter->ProcessorApicIdBase[1] = 0x7E20;
      mAcpiParameter->ProcessorApicIdBase[2] = 0x7E40;
      mAcpiParameter->ProcessorApicIdBase[3] = 0x7E60;
      mAcpiParameter->ProcessorApicIdBase[4] = 0x7E80;
      mAcpiParameter->ProcessorApicIdBase[5] = 0x7Ea0;
      mAcpiParameter->ProcessorApicIdBase[6] = 0x7Ec0;
      mAcpiParameter->ProcessorApicIdBase[7] = 0x7Ee0;
    }
  }

  //
  // If SNC is enabled, and NumOfCluster is 2, set the ACPI variable for PXM value
  //
  if(mIioUds->IioUdsPtr->SystemStatus.OutSncEn && (mIioUds->IioUdsPtr->SystemStatus.OutNumOfCluster == 2)){
    mAcpiParameter->SncAnd2Cluster = 1;
  } else {
    mAcpiParameter->SncAnd2Cluster = 0;
  }

   mAcpiParameter->MmCfg = (UINT32)mIioUds->IioUdsPtr->PlatformData.PciExpressBase;
   mAcpiParameter->TsegSize = (UINT32)(mIioUds->IioUdsPtr->PlatformData.MemTsegSize >> 20);

  return EFI_SUCCESS;
}

/**

  This function will update any runtime platform specific information.
  This currently includes:
    Setting OEM table values, ID, table ID, creator ID and creator revision.
    Enabling the proper processor entries in the APIC tables.

  @param Table  -  The table to update

  @retval EFI_SUCCESS  -  The function completed successfully.

**/
EFI_STATUS
PlatformUpdateTables (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table,
  IN OUT EFI_ACPI_TABLE_VERSION     *Version
  )
{
  EFI_STATUS                                Status;

  Status = EFI_SUCCESS;

  //
  // By default, a table belongs in all ACPI table versions published.
  // Some tables will override this because they have different versions of the table.
  //
  *Version = EFI_ACPI_TABLE_VERSION_2_0;
  //
  // Update the processors in the APIC table
  //
  switch (Table->Signature) {

  case EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    //
    // Patch the memory resource
    //
    Status = PatchDsdtTable(Table);
    break;

  default:
    ASSERT(FALSE);
    break;
  }
  //
  //
  // Update the hardware signature in the FACS structure
  //
  //
  //
  return Status;
}



/**

  GC_TODO: Add function description

  @param Event   - GC_TODO: add argument description
  @param Context - GC_TODO: add argument description

  @retval GC_TODO: add return values

**/
STATIC
VOID
EFIAPI
OnReadyToBoot (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  UINT32                        RegEax;
  UINT32                        RegEbx;
  UINT32                        RegEcx;
  UINT32                        RegEdx;

  if (mFirstNotify) {
    return ;
  }

  mFirstNotify = TRUE;

  CopyMem (&mSocketIioConfiguration, PcdGetPtr(PcdSocketIioConfigData), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&mSocketPowermanagementConfiguration,PcdGetPtr(PcdSocketPowerManagementConfigData), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&mPchRcConfiguration, PcdGetPtr(PcdPchRcConfigurationData), sizeof(PCH_RC_CONFIGURATION));

  // CpuPm.Asl: External (CSEN, FieldUnitObj)
  mAcpiParameter->CStateEnable = !mSocketPowermanagementConfiguration.ProcessorAutonomousCstateEnable;
  // CpuPm.Asl: External (C3EN, FieldUnitObj)
  mAcpiParameter->C3Enable     = mSocketPowermanagementConfiguration.C3Enable;
  // CpuPm.Asl: External (C6EN, FieldUnitObj)
  if (mSocketPowermanagementConfiguration.C6Enable == PPM_AUTO) {
    mAcpiParameter->C6Enable     = 1;  //POR Default = Enabled
  } else {
    mAcpiParameter->C6Enable     = mSocketPowermanagementConfiguration.C6Enable;
  }
  if(mAcpiParameter->C6Enable && mAcpiParameter->C3Enable) {  //C3 and C6 enable are exclusive
    mAcpiParameter->C6Enable = 1;
    mAcpiParameter->C3Enable = 0;
  }
  // CpuPm.Asl: External (C7EN, FieldUnitObj)
  mAcpiParameter->C7Enable     = 0;
  // CpuPm.Asl: External (OSCX, FieldUnitObj)
  mAcpiParameter->OSCX         = mSocketPowermanagementConfiguration.OSCx;
  // CpuPm.Asl: External (MWOS, FieldUnitObj)
  mAcpiParameter->MonitorMwaitEnable = 1;
  // CpuPm.Asl: External (PSEN, FieldUnitObj)
  mAcpiParameter->PStateEnable = mSocketPowermanagementConfiguration.ProcessorEistEnable;
  // CpuPm.Asl: External (HWAL, FieldUnitObj)
  mAcpiParameter->HWAllEnable = 0; //Update in PatchGv3SsdtTable

  mAcpiParameter->KBPresent    = mKBPresent;
  mAcpiParameter->MousePresent = mMousePresent;
  mAcpiParameter->TStateEnable = mSocketPowermanagementConfiguration.TStateEnable;
  //Fine grained T state
  AsmCpuid (CPUID_THERMAL_POWER_MANAGEMENT,  &RegEax, &RegEbx, &RegEcx, &RegEdx);
  if ((RegEax & EFI_FINE_GRAINED_CLOCK_MODULATION) && (mSocketPowermanagementConfiguration.OnDieThermalThrottling > 0)){
    mAcpiParameter->TStateFineGrained = 1;
  }
  if(RegEax & B_CPUID_POWER_MANAGEMENT_EAX_HWP_LVT_INTERRUPT_SUPPORT) {
    mAcpiParameter->HwpInterrupt = 1;
  }
  // CpuPm.Asl: External (HWEN, FieldUnitObj)
  mAcpiParameter->HWPMEnable = mSocketPowermanagementConfiguration.ProcessorHWPMEnable;
  // CpuPm.Asl: External (ACEN, FieldUnitObj)
  mAcpiParameter->AutoCstate = mSocketPowermanagementConfiguration.ProcessorAutonomousCstateEnable;

  mAcpiParameter->EmcaEn    = 0;

  mAcpiParameter->PcieAcpiHotPlugEnable = (UINT8) (BOOLEAN) (mSocketIioConfiguration.PcieAcpiHotPlugEnable != 0);
  //
  // Initialize USB3 mode from setup data
  //
  // If mode != manual control
  //  just copy mode from setup
  if (mPchRcConfiguration.PchUsbManualMode != 1) {
    mAcpiParameter->XhciMode = mPchRcConfiguration.PchUsbManualMode;
  }

}

/**

  Entry point for Acpi platform driver.

  @param ImageHandle  -  A handle for the image that is initializing this driver.
  @param SystemTable  -  A pointer to the EFI system table.

  @retval EFI_SUCCESS           -  Driver initialized successfully.
  @retval EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  @retval EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
InstallAcpiBoard (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINTN                         TableHandle;
  UINT32                        FvStatus;
  UINT32                        Size;
  EFI_EVENT                     Event;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  EFI_HOB_GUID_TYPE             *GuidHob;

  mFirstNotify      = FALSE;

  TableVersion      = EFI_ACPI_TABLE_VERSION_NONE;
  Instance          = 0;
  CurrentTable      = NULL;
  TableHandle       = 0;

  //
  // Locate the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL,&mIioUds);
  ASSERT_EFI_ERROR (Status);

  GuidHob    = GetFirstGuidHob (&gEfiMemoryMapGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  mSystemMemoryMap = GET_GUID_HOB_DATA(GuidHob);

  PlatformHookInit ();

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, &AcpiTable);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            &gEfiCallerIdGuid,
            &FwVol,
            TRUE
            );
  ASSERT_EFI_ERROR (Status);

  Status    = EFI_SUCCESS;
  Instance  = 0;

  //
  // Read tables from the storage file.
  //
  while (!EFI_ERROR (Status)) {
    CurrentTable = NULL;

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      (UINTN *) &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      //
      // Allow platform specific code to reject the table or update it
      //
      {
        //
        // Perform any table specific updates.
        //
        AcpiStatus = PlatformUpdateTables (CurrentTable, &TableVersion);
        if (!EFI_ERROR (AcpiStatus)) {
          //
          // Add the table
          //
          TableHandle = 0;
          if (TableVersion != EFI_ACPI_TABLE_VERSION_NONE) {
            AcpiStatus = AcpiTable->InstallAcpiTable (
                                      AcpiTable,
                                      CurrentTable,
                                      CurrentTable->Length,
                                      &TableHandle
                                      );
          }
          ASSERT_EFI_ERROR (AcpiStatus);
        }
      }
      //
      // Increment the instance
      //
      Instance++;
    }
  }

  Status = EfiCreateEventReadyToBootEx(
             TPL_NOTIFY,
             OnReadyToBoot,
             NULL,
             &Event
             );

  //
  // Finished
  //
  return EFI_SUCCESS;
}
