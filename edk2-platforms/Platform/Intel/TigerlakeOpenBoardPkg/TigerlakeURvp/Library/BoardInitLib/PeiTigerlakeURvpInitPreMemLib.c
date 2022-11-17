/** @file

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BoardInitLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <Library/PchPcrLib.h>
#include <ConfigBlock.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchPcrLib.h>
#include <Register/PchRegsLpc.h>
#include <Ppi/Reset.h>
#include <PlatformBoardConfig.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <PeiPlatformHookLib.h>
#include <PlatformBoardId.h>

///
/// Reset Generator I/O Port
///
#define RESET_GENERATOR_PORT           0xCF9

typedef struct {
  EFI_PHYSICAL_ADDRESS    BaseAddress;
  UINT64                  Length;
} MEMORY_MAP;

GLOBAL_REMOVE_IF_UNREFERENCED MEMORY_MAP MmioMap[] = {
  { FixedPcdGet64 (PcdLocalApicAddress),  FixedPcdGet32 (PcdLocalApicMmioSize) },
  { FixedPcdGet64 (PcdMchBaseAddress),    FixedPcdGet32 (PcdMchMmioSize) },
  { FixedPcdGet64 (PcdDmiBaseAddress),    FixedPcdGet32 (PcdDmiMmioSize) },
  { FixedPcdGet64 (PcdEpBaseAddress),     FixedPcdGet32 (PcdEpMmioSize) }
};

EFI_STATUS
MrcConfigInit (
  IN UINT16 BoardId
  );

EFI_STATUS
SaGpioConfigInit (
  IN UINT16 BoardId
  );

EFI_STATUS
SaMiscConfigInit (
  IN UINT16         BoardId
  );

EFI_STATUS
RootPortClkInfoInit (
  IN UINT16 BoardId
  );

EFI_STATUS
UsbConfigInit (
  IN UINT16 BoardId
  );

EFI_STATUS
GpioGroupTierInit (
  IN UINT16 BoardId
  );

EFI_STATUS
GpioTablePreMemInit (
  IN UINT16 BoardId
  );

EFI_STATUS
PchPmConfigInit (
  IN UINT16 BoardId
  );

EFI_STATUS
SaDisplayConfigInit (
  IN UINT16 BoardId
  );

EFI_STATUS
EFIAPI
PlatformInitPreMemCallBack (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotify (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
PchReset (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

static EFI_PEI_RESET_PPI mResetPpi = {
  PchReset
};

static EFI_PEI_PPI_DESCRIPTOR mPreMemPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiResetPpiGuid,
    &mResetPpi
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR mPreMemNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReadOnlyVariable2PpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT)PlatformInitPreMemCallBack
};

static EFI_PEI_NOTIFY_DESCRIPTOR mMemDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT)MemoryDiscoveredPpiNotify
};

/**
  Board misc init function for PEI pre-memory phase.

  @param[in]  BoardId       An unsigned integer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardMiscInitPreMem (
  IN UINT16 BoardId
  )
{
  return EFI_SUCCESS;
}

/**
  Board configuration initialization in the pre-memory boot phase.
**/
VOID
BoardConfigInitPreMem (
  VOID
  )
{
  EFI_STATUS Status;
  UINT16 BoardId;

  BoardId = BoardIdTglUDdr4;

  Status = MrcConfigInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = SaGpioConfigInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = SaMiscConfigInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = RootPortClkInfoInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = UsbConfigInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = GpioGroupTierInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = GpioTablePreMemInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = PchPmConfigInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = BoardMiscInitPreMem (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = SaDisplayConfigInit (BoardId);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function handles PlatformInit task after PeiReadOnlyVariable2 PPI produced

  @param[in]  PeiServices   Pointer to PEI Services Table.
  @param[in]  NotifyDesc    Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in]  Ppi           Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others       Failure
**/
EFI_STATUS
EFIAPI
PlatformInitPreMemCallBack (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                        Status;

  DEBUG ((DEBUG_INFO, "PlatformInitPreMemCallBack Start...\n"));
  //
  // Init Board Config Pcd.
  //
  BoardConfigInitPreMem ();

  ///
  /// Configure GPIO and SIO
  ///
  Status = BoardInitPreMem ();
  ASSERT_EFI_ERROR (Status);

  ///
  /// Install Pre Memory PPIs
  ///
  Status = PeiServicesInstallPpi (&mPreMemPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PlatformInitPreMemCallBack End...\n"));

  return Status;
}

/**
  Provide hard reset PPI service.
  To generate full hard reset, write 0x0E to PCH RESET_GENERATOR_PORT (0xCF9).

  @param[in]  PeiServices       General purpose services available to every PEIM.

  @retval     Not return        System reset occured.
  @retval     EFI_DEVICE_ERROR  Device error, could not reset the system.
**/
EFI_STATUS
EFIAPI
PchReset (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  DEBUG ((DEBUG_INFO, "Perform Cold Reset\n"));
  IoWrite8 (RESET_GENERATOR_PORT, 0x0E);

  CpuDeadLoop ();

  ///
  /// System reset occured, should never reach at this line.
  ///
  ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
  return EFI_DEVICE_ERROR;
}

/**
  Install Firmware Volume Hob's once there is main memory

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotify (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                    Status;
  EFI_BOOT_MODE                 BootMode;
  UINTN                         Index;
  UINT8                         PhysicalAddressBits;
  UINT32                        RegEax;
  MEMORY_MAP                    PcieMmioMap;

 DEBUG ((DEBUG_INFO, "MemoryDiscoveredPpiNotify Start!\n"));

  Index = 0;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8)RegEax;
  }
  else {
    PhysicalAddressBits = 36;
  }

  ///
  /// Create a CPU hand-off information
  ///
  BuildCpuHob (PhysicalAddressBits, 16);

  ///
  /// Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios.
  ///
  PcieMmioMap.BaseAddress = FixedPcdGet64 (PcdPciExpressBaseAddress);
  PcieMmioMap.Length = PcdGet32 (PcdPciExpressRegionLength);

  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
     (EFI_RESOURCE_ATTRIBUTE_PRESENT      |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED  |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    PcieMmioMap.BaseAddress,
    PcieMmioMap.Length
    );
  BuildMemoryAllocationHob (
    PcieMmioMap.BaseAddress,
    PcieMmioMap.Length,
    EfiMemoryMappedIO
    );
  for (Index = 0; Index < sizeof(MmioMap) / (sizeof(MEMORY_MAP)); Index++) {
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_MAPPED_IO,
       (EFI_RESOURCE_ATTRIBUTE_PRESENT      |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED  |
        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
      MmioMap[Index].BaseAddress,
      MmioMap[Index].Length
      );
    BuildMemoryAllocationHob (
      MmioMap[Index].BaseAddress,
      MmioMap[Index].Length,
      EfiMemoryMappedIO
      );
  }

  //
  // Report resource HOB for flash FV
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
     (EFI_RESOURCE_ATTRIBUTE_PRESENT      |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED  |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    (UINTN) FixedPcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN) FixedPcdGet32 (PcdFlashAreaSize)
    );

  BuildMemoryAllocationHob (
    (UINTN) FixedPcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN) FixedPcdGet32 (PcdFlashAreaSize),
    EfiMemoryMappedIO
    );

  BuildFvHob (
    (UINTN)FixedPcdGet32 (PcdFlashAreaBaseAddress),
    (UINTN)FixedPcdGet32 (PcdFlashAreaSize)
    );

 DEBUG ((DEBUG_INFO, "MemoryDiscoveredPpiNotify End!\n"));

  return Status;
}

/**
  Board configuration init function for PEI pre-memory phase.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   The parameter is NULL.
**/
EFI_STATUS
EFIAPI
TigerlakeURvpInitPreMem (
  VOID
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "TigerlakeURvpInitPreMem Start!\n"));
  ///
  /// Performing PlatformInitPreMemCallBack after PeiReadOnlyVariable2 PPI produced
  ///
  Status = PeiServicesNotifyPpi (&mPreMemNotifyList);
  ASSERT_EFI_ERROR (Status);

  ///
  /// After code reorangized, memorycallback will run because the PPI is already
  /// installed when code run to here, it is supposed that the InstallEfiMemory is
  /// done before.
  ///
  Status = PeiServicesNotifyPpi (&mMemDiscoveredNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "TigerlakeURvpInitPreMem End!\n"));

  return EFI_SUCCESS;
}

/**
  Configure GPIO and SIO before memory ready

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
TigerlakeURvpBoardInitBeforeMemoryInit(
  VOID
  )
{

  TigerlakeURvpInitPreMem();

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
TigerlakeURvpBoardDebugInit(
  VOID
  )
{
  ///
  /// Do Early PCH init
  ///
  return EFI_SUCCESS;
}

EFI_BOOT_MODE
EFIAPI
TigerlakeURvpBoardBootModeDetect(
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}
