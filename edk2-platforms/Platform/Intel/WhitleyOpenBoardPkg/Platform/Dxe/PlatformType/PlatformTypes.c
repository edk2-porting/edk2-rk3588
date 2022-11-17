/** @file
  Platform type driver to identify different platform.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include "PlatformTypes.h"
#include <Library/SerialPortLib/Ns16550.h>
#include <Library/IoLib.h>
#include <Protocol/SmbusHc.h>
#include <Protocol/MpService.h>
#include <Library/SetupLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioLib.h>
#include <Protocol/DynamicSiLibraryProtocol.h>

#define STRING_WIDTH_40    40

CHAR16                              PchName[STRING_WIDTH_40];
CHAR16                              IioName[STRING_WIDTH_40];
CHAR16                              *PlatformName;

//
// Instantiation of Driver's private data.
//
EFI_PLATFORM_DATA_DRIVER_PRIVATE    mPrivatePlatformData;
EFI_IIO_UDS_DRIVER_PRIVATE          mIioUdsPrivateData;
IIO_UDS                             *IioUdsData;          // Pointer to UDS in Allocated Memory Pool

EFI_GUID gEfiSmbusHcProtocolGuid = EFI_SMBUS_HC_PROTOCOL_GUID;
EFI_GUID gEfiMpServiceProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;
EFI_GUID mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;


/**
  Get the PCH name.

  Concatenate the series, stepping, and SKU strings to initialize the module
  global variable "PchName".

  @param[in]  PlatformInfoHobData   Pointer to the platform info HOB.

  @retval EFI_INVALID_PARAMETER     Pointer parameter was null on entry.
  @retval EFI_SUCCESS               PchName was initialized successfully.
**/
EFI_STATUS
EFIAPI
GetPchName (
  IN EFI_PLATFORM_INFO *PlatformInfoHobData
  )
{
  CHAR8                             AsciiBuffer[STRING_WIDTH_40];
  CHAR16                            UnicodeBuffer[STRING_WIDTH_40];
  UINT32                            BufferAsciiSize;
  DYNAMIC_SI_LIBARY_PROTOCOL        *DynamicSiLibraryProtocol = NULL;
  EFI_STATUS                        Status;

  ASSERT (PlatformInfoHobData != NULL);
  if (PlatformInfoHobData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  BufferAsciiSize = sizeof (AsciiBuffer);

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  AsciiStrCpyS (AsciiBuffer, BufferAsciiSize, DynamicSiLibraryProtocol->PchGetSeriesStr ());
  AsciiStrToUnicodeStrS (AsciiBuffer, UnicodeBuffer, STRING_WIDTH_40);
  ZeroMem (AsciiBuffer, BufferAsciiSize);
  StrCpyS (PchName, STRING_WIDTH_40, UnicodeBuffer);

  StrCatS (PchName, STRING_WIDTH_40, L" ");

  DynamicSiLibraryProtocol->PchGetSteppingStr (AsciiBuffer, BufferAsciiSize);
  AsciiStrToUnicodeStrS (AsciiBuffer, UnicodeBuffer, STRING_WIDTH_40);
  ZeroMem (AsciiBuffer, BufferAsciiSize);
  StrCatS (PchName, STRING_WIDTH_40, UnicodeBuffer);

  StrCatS (PchName, STRING_WIDTH_40, L" - ");

  AsciiStrCpyS (AsciiBuffer, BufferAsciiSize, DynamicSiLibraryProtocol->PchGetSkuStr ());
  AsciiStrToUnicodeStrS (AsciiBuffer, UnicodeBuffer, STRING_WIDTH_40);
  ZeroMem (AsciiBuffer, BufferAsciiSize);
  StrCatS (PchName, STRING_WIDTH_40, UnicodeBuffer);

  mPrivatePlatformData.PlatformType.PchStringPtr = (UINT64)PchName;

  return EFI_SUCCESS;
}


/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
EFIAPI
GetIioName (
  VOID
  )
{

  StrCpyS(IioName, STRING_WIDTH_40, L"Unknown");

  mPrivatePlatformData.PlatformType.IioStringPtr = (UINT64)IioName;
  return ;
}


/**

  Assert the POST complete GPIO

  @param Event         Pointer to the event that triggered this Ccllback Function
  @param Context       VOID Pointer required for Ccllback functio

  @retval EFI_SUCCESS         - Assertion successfully


**/
VOID
EFIAPI
AssertPostGpio (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  UINT32                      GPIO_B20;
  UINT32                      Data32;
  DYNAMIC_SI_LIBARY_PROTOCOL  *DynamicSiLibraryProtocol = NULL;
  EFI_STATUS                  Status;

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  GPIO_B20 = PcdGet32 (PcdOemSkuAssertPostGPIO);
  Data32 = PcdGet32(PcdOemSkuAssertPostGPIOValue);
  if (GPIO_B20 == 0xFFFFFFFF) {
    DEBUG ((EFI_D_ERROR, "GPIO Pcd is invalid, so abort the GPIO Set and just return! \n"));
    return;
  }
  DynamicSiLibraryProtocol->GpioSetOutputValue (GPIO_B20, Data32);
  DEBUG ((EFI_D_INFO, "System Post Complete GPIO has been set ! \n"));
}

/**

  Gets the CpuId and fills in the pointer with the value.
  Needed for executing CpuId on other APs.

  @param RegEax - Pointer to be used to pass the CpuId value

  @retval None

**/
VOID
EFIAPI
PlatformGetProcessorID (
  IN OUT UINT32  *RegEax
  )
{

  AsmCpuid (CPUID_VERSION_INFO, RegEax, NULL, NULL, NULL);
}

//
// Define platform type check
//
/**

  Entry point for the driver.

  This routine reads the PlatformType GPI on FWH and produces a protocol
  to be consumed by the chipset driver to effect those settings.

  @param ImageHandle  -  Image Handle.
  @param SystemTable  -  EFI System Table.

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
PlatformTypeInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfoHobData = NULL;
  IIO_UDS                         *UdsHobPtr;
  EFI_GUID                        UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_EVENT                       ReadyToBootEvent;
  CHAR16                          *PcdPlatformName = NULL;
  UINT32                          PcdPlatformNameSize = 0;
  UINT32                          PlatformNameSize;

  //
  // Initialize driver private data.
  // Only one instance exists
  //
  ZeroMem (&mPrivatePlatformData, sizeof (mPrivatePlatformData));
  mPrivatePlatformData.Signature            = EFI_PLATFORM_TYPE_DRIVER_PRIVATE_SIGNATURE;

  PlatformNameSize = (STRING_WIDTH_40) * sizeof (CHAR16);
  PlatformName = AllocateZeroPool (PlatformNameSize);
  ASSERT (PlatformName != NULL);
  if (PlatformName == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Search for the Platform Info PEIM GUID HOB.
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "gEfiPlatformInfoGuid not found\n"));
    return EFI_NOT_FOUND;
  }
  PlatformInfoHobData  = GET_GUID_HOB_DATA(GuidHob);

  CopyMem(&(mPrivatePlatformData.PlatformType.SystemUuid[0]), &PlatformInfoHobData->SystemUuid[0], 16);
  mPrivatePlatformData.PlatformType.Signature = PlatformInfoHobData->Signature;
  mPrivatePlatformData.PlatformType.Size = PlatformInfoHobData->Size;
  mPrivatePlatformData.PlatformType.Revision = PlatformInfoHobData->Revision;
  mPrivatePlatformData.PlatformType.TypeRevisionId = PlatformInfoHobData->TypeRevisionId;
  mPrivatePlatformData.PlatformType.ExtendedInfoValid = PlatformInfoHobData->ExtendedInfoValid;
  mPrivatePlatformData.PlatformType.Checksum = PlatformInfoHobData->Checksum;

  CopyMem(&(mPrivatePlatformData.PlatformType.PciData), &PlatformInfoHobData->PciData, sizeof(EFI_PLATFORM_PCI_DATA));
  CopyMem(&(mPrivatePlatformData.PlatformType.CpuData), &PlatformInfoHobData->CpuData, sizeof(EFI_PLATFORM_CPU_DATA));
  CopyMem(&(mPrivatePlatformData.PlatformType.MemData), &PlatformInfoHobData->MemData, sizeof(EFI_PLATFORM_MEM_DATA));
  CopyMem(&(mPrivatePlatformData.PlatformType.SysData), &PlatformInfoHobData->SysData, sizeof(EFI_PLATFORM_SYS_DATA));
  CopyMem(&(mPrivatePlatformData.PlatformType.PchData), &PlatformInfoHobData->PchData, sizeof(EFI_PLATFORM_PCH_DATA));

  mPrivatePlatformData.PlatformType.BoardId     = PlatformInfoHobData->BoardId;
  mPrivatePlatformData.PlatformType.Type        = mPrivatePlatformData.PlatformType.BoardId;
  mPrivatePlatformData.PlatformType.IioSku      = PlatformInfoHobData->IioSku;
  mPrivatePlatformData.PlatformType.IioRevision = PlatformInfoHobData->IioRevision;
  mPrivatePlatformData.PlatformType.PchSku      = PlatformInfoHobData->PchSku;
  mPrivatePlatformData.PlatformType.PchRevision = PlatformInfoHobData->PchRevision;
  mPrivatePlatformData.PlatformType.PchType     = PlatformInfoHobData->PchType;  //Include PCH SKU type
  mPrivatePlatformData.PlatformType.CpuType     = PlatformInfoHobData->CpuType;
  mPrivatePlatformData.PlatformType.CpuStepping = PlatformInfoHobData->CpuStepping;

  mPrivatePlatformData.PlatformType.IioRiserId = PlatformInfoHobData->IioRiserId;
  mPrivatePlatformData.PlatformType.PcieRiser1Type = PlatformInfoHobData->PcieRiser1Type;
  mPrivatePlatformData.PlatformType.PcieRiser2Type = PlatformInfoHobData->PcieRiser2Type;
  mPrivatePlatformData.PlatformType.Emulation = 0x4;                 // default is Simics

  PcdPlatformNameSize = PcdGet32(PcdOemSkuPlatformNameSize);
  PcdPlatformName = PcdGetPtr (PcdOemSkuPlatformName);
  ASSERT(PlatformNameSize >= PcdPlatformNameSize);
  if (PlatformNameSize < PcdPlatformNameSize) {
    DEBUG ((EFI_D_ERROR, "Invalid buffer size\n"));
    return EFI_BUFFER_TOO_SMALL;
  }
  ASSERT(PcdPlatformName != NULL);
  if (PcdPlatformName == NULL) {
    DEBUG ((EFI_D_ERROR, "Invalid PCD detected\n"));
    return EFI_NOT_FOUND;
  }
  CopyMem (PlatformName, PcdPlatformName, PcdPlatformNameSize);

  mPrivatePlatformData.PlatformType.TypeStringPtr = (UINT64)PlatformName;

  Status = GetPchName (PlatformInfoHobData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get PCH name: %r\n", Status));
    return Status;
  }

  GetIioName();

  //
  // Install the PlatformType policy.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivatePlatformData.Handle,
                  &gEfiPlatformTypeProtocolGuid,
                  &mPrivatePlatformData.PlatformType,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%s platform is detected!\n", PlatformName));

  //
  // Time to get the IIO_UDS HOB data stored in the PEI driver
  //
  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "UniversalDataGuid not found\n"));
    return EFI_NOT_FOUND;
  }
  UdsHobPtr = GET_GUID_HOB_DATA(GuidHob);

  //
  // Allocate Memory Pool for Universal Data Storage so that protocol can expose it
  //
  Status = gBS->AllocatePool ( EfiReservedMemoryType, sizeof (IIO_UDS), (VOID **) &IioUdsData );
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize the Pool Memory with the data from the Hand-Off-Block
  //
  CopyMem(IioUdsData, UdsHobPtr, sizeof(IIO_UDS));

  //
  // Build the IIO_UDS driver instance for protocol publishing
  //
  ZeroMem (&mIioUdsPrivateData, sizeof (mIioUdsPrivateData));

  mIioUdsPrivateData.Signature            = EFI_IIO_UDS_DRIVER_PRIVATE_SIGNATURE;
  mIioUdsPrivateData.IioUds.IioUdsPtr     = IioUdsData;
  mIioUdsPrivateData.IioUds.EnableVc      = NULL;

  //
  // Install the IioUds Protocol.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mIioUdsPrivateData.Handle,
                  &gEfiIioUdsProtocolGuid,
                  &mIioUdsPrivateData.IioUds,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);


  //
  // Set up callback to assert the POST Complete GPIO to the iBMC
  //
  Status = EfiCreateEventReadyToBootEx(
             TPL_CALLBACK,
             AssertPostGpio,
             NULL,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

