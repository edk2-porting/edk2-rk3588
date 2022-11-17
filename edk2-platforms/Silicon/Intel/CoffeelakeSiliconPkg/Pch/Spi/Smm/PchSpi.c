/** @file
  PCH SPI SMM Driver implements the SPI Host Controller Compatibility Interface.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/Spi.h>
#include <Protocol/SmmCpu.h>
#include <Private/Library/PchSpiCommonLib.h>
#include <Private/Library/SmmPchPrivateLib.h>
#include <PchReservedResources.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsSpi.h>

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED SPI_INSTANCE          *mSpiInstance;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_CPU_PROTOCOL  *mSmmCpuProtocol;
//
// mPchSpiResvMmioAddr keeps the reserved MMIO range assiged to SPI.
// In SMM it always set back the reserved MMIO address to SPI BAR0 to ensure the MMIO range
// won't overlap with SMRAM range, and trusted.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                mSpiResvMmioAddr;

/**
  <b>SPI Runtime SMM Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SPI SMM module provide a standard way for other modules to use the PCH SPI Interface in SMM.

  - @pre
    - EFI_SMM_BASE2_PROTOCOL
      - Documented in System Management Mode Core Interface Specification .

  - @result
    The SPI SMM driver produces @link _PCH_SPI_PROTOCOL PCH_SPI_PROTOCOL @endlink with GUID
    gPchSmmSpiProtocolGuid which is different from SPI RUNTIME driver.

  - <b>Integration Check List</b>\n
    - This driver supports Descriptor Mode only.
    - This driver supports Hardware Sequence only.
    - When using SMM SPI Protocol to perform flash access in an SMI handler,
      and the SMI occurrence is asynchronous to normal mode code execution,
      proper synchronization mechanism must be applied, e.g. disable SMI before
      the normal mode SendSpiCmd() starts and re-enable SMI after
      the normal mode SendSpiCmd() completes.
      @note The implementation of SendSpiCmd() uses GBL_SMI_EN in
      SMI_EN register (ABase + 30h) to disable and enable SMIs. But this may
      not be effective as platform may well set the SMI_LOCK bit (i.e., PMC PCI Offset A0h [4]).
      So the synchronization at caller level is likely needed.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
InstallPchSpi (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS  Status;

  //
  // Init PCH spi reserved MMIO address.
  //
  mSpiResvMmioAddr = PCH_SPI_BASE_ADDRESS;

  ///
  /// Allocate pool for SPI protocol instance
  ///
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData, /// MemoryType don't care
                    sizeof (SPI_INSTANCE),
                    (VOID **) &mSpiInstance
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (mSpiInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((VOID *) mSpiInstance, sizeof (SPI_INSTANCE));
  ///
  /// Initialize the SPI protocol instance
  ///
  Status = SpiProtocolConstructor (mSpiInstance);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// Install the SMM PCH_SPI_PROTOCOL interface
  ///
  Status = gSmst->SmmInstallProtocolInterface (
                    &(mSpiInstance->Handle),
                    &gPchSmmSpiProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &(mSpiInstance->SpiProtocol)
                    );
  if (EFI_ERROR (Status)) {
    gSmst->SmmFreePool (mSpiInstance);
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Acquire PCH spi mmio address.
  If it is ever different from the preallocated address, reassign it back.
  In SMM, it always override the BAR0 and returns the reserved MMIO range for SPI.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval PchSpiBar0              return SPI MMIO address
**/
UINTN
AcquireSpiBar0 (
  IN  SPI_INSTANCE                *SpiInstance
  )
{
  UINT32                          SpiBar0;
  //
  // Save original SPI physical MMIO address
  //
  SpiBar0 = PciSegmentRead32 (SpiInstance->PchSpiBase + R_SPI_CFG_BAR0) & ~(B_SPI_CFG_BAR0_MASK);

  if (SpiBar0 != mSpiResvMmioAddr) {
    //
    // Temporary disable MSE, and override with SPI reserved MMIO address, then enable MSE.
    //
    PciSegmentAnd8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
    PciSegmentWrite32 (SpiInstance->PchSpiBase + R_SPI_CFG_BAR0, mSpiResvMmioAddr);
    PciSegmentOr8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }
  //
  // SPIBAR0 will be different before and after PCI enum so need to get it from SPI BAR0 reg.
  //
  return mSpiResvMmioAddr;
}

/**
  Release pch spi mmio address. Do nothing.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval None
**/
VOID
ReleaseSpiBar0 (
  IN  SPI_INSTANCE                *SpiInstance
  )
{
}

/**
  This function is a hook for Spi to disable BIOS Write Protect

  @retval EFI_SUCCESS             The protocol instance was properly initialized
  @retval EFI_ACCESS_DENIED       The BIOS Region can only be updated in SMM phase

**/
EFI_STATUS
EFIAPI
DisableBiosWriteProtect (
  VOID
  )
{
  UINT64     SpiBaseAddress;

  SpiBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI,
                     0
                     );
  // Write clear BC_SYNC_SS prior to change WPD from 0 to 1.
  //
  PciSegmentOr8 (
    SpiBaseAddress + R_SPI_CFG_BC + 1,
    (B_SPI_CFG_BC_SYNC_SS >> 8)
    );
  ///
  /// Set BIOSWE bit (SPI PCI Offset DCh [0]) = 1b
  /// Enable the access to the BIOS space for both read and write cycles
  ///
  PciSegmentOr8 (
    SpiBaseAddress + R_SPI_CFG_BC,
    B_SPI_CFG_BC_WPD
    );

  ///
  /// PCH BIOS Spec Section 3.7 BIOS Region SMM Protection Enabling
  /// If the following steps are implemented:
  ///  - Set the EISS bit (SPI PCI Offset DCh [5]) = 1b
  ///  - Follow the 1st recommendation in section 3.6
  /// the BIOS Region can only be updated by following the steps bellow:
  ///  - Once all threads enter SMM
  ///  - Read memory location FED30880h OR with 00000001h, place the result in EAX,
  ///    and write data to lower 32 bits of MSR 1FEh (sample code available)
  ///  - Set BIOSWE bit (SPI PCI Offset DCh [0]) = 1b
  ///  - Modify BIOS Region
  ///  - Clear BIOSWE bit (SPI PCI Offset DCh [0]) = 0b
  ///
  if ((PciSegmentRead8 (SpiBaseAddress + R_SPI_CFG_BC) & B_SPI_CFG_BC_EISS) != 0) {
    PchSetInSmmSts ();
  }

  return EFI_SUCCESS;
}

/**
  This function is a hook for Spi to enable BIOS Write Protect


**/
VOID
EFIAPI
EnableBiosWriteProtect (
  VOID
  )
{
  UINT64     SpiBaseAddress;

  SpiBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI,
                     0
                     );
  ///
  /// Clear BIOSWE bit (SPI PCI Offset DCh [0]) = 0b
  /// Disable the access to the BIOS space for write cycles
  ///
  PciSegmentAnd8 (
    SpiBaseAddress + R_SPI_CFG_BC,
    (UINT8) (~B_SPI_CFG_BC_WPD)
    );

  ///
  /// Check if EISS bit is set
  ///
  if (((PciSegmentRead8 (SpiBaseAddress + R_SPI_CFG_BC)) & B_SPI_CFG_BC_EISS) == B_SPI_CFG_BC_EISS) {
    PchClearInSmmSts ();
  }
}

/**
  Check if it's granted to do flash write.

  @retval TRUE    It's secure to do flash write.
  @retval FALSE   It's not secure to do flash write.
**/
BOOLEAN
IsSpiFlashWriteGranted (
  VOID
  )
{
  EFI_STATUS    Status;
  UINT32        CpuIndex;
  UINT64        ProcessorId;

  if (mSmmCpuProtocol == NULL) {
    Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuProtocolGuid, NULL, (VOID **)&mSmmCpuProtocol);
    ASSERT_EFI_ERROR (Status);
    if (mSmmCpuProtocol == NULL) {
      return TRUE;
    }
  }

  for (CpuIndex = 0; CpuIndex < gSmst->NumberOfCpus; CpuIndex++) {
    Status = mSmmCpuProtocol->ReadSaveState (
                                mSmmCpuProtocol,
                                sizeof (ProcessorId),
                                EFI_SMM_SAVE_STATE_REGISTER_PROCESSOR_ID,
                                CpuIndex,
                                &ProcessorId
                                );
    //
    // If the processor is in SMM at the time the SMI occurred,
    // it will return success. Otherwise, EFI_NOT_FOUND is returned.
    //
    if (EFI_ERROR (Status)) {
      return FALSE;
    }
  }

  return TRUE;
}
