/** @file
  PCH SPI PEI Library implements the SPI Host Controller Interface.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PchReservedResources.h>
#include <IndustryStandard/Pci30.h>
#include <Ppi/Spi.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchSpiCommonLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>

typedef struct {
  EFI_PEI_PPI_DESCRIPTOR  PpiDescriptor;
  SPI_INSTANCE            SpiInstance;
} PEI_SPI_INSTANCE;

/**
  Initializes the SPI BAR0 value to a default value and enables memory space decoding.

  The SPI BAR0 will be assigned later in PCI enumeration.

**/
VOID
InitSpiBar0 (
  VOID
  )
{
  UINT64       PchSpiBase;
  PchSpiBase = PCI_SEGMENT_LIB_ADDRESS (
                 0,
                 0,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI,
                 0
                 );
  PciSegmentWrite32 (PchSpiBase + R_PCH_SPI_BAR0, PCH_SPI_BASE_ADDRESS);
  PciSegmentOr32 (PchSpiBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Initializes SPI for access from future services.

  @retval EFI_SUCCESS         The SPI service was initialized successfully.
  @retval EFI_OUT_OF_RESOUCES Insufficient memory available to allocate structures required for initialization.
  @retval Others              An error occurred initializing SPI services.

**/
EFI_STATUS
EFIAPI
SpiServiceInit (
  VOID
  )
{
  EFI_STATUS        Status;
  PEI_SPI_INSTANCE  *PeiSpiInstance;
  SPI_INSTANCE      *SpiInstance;
  PCH_SPI_PPI       *SpiPpi;
  UINT16            AcpiBase;

  AcpiBase = 0;

  Status = PeiServicesLocatePpi (
             &gPchSpiPpiGuid,
             0,
             NULL,
             (VOID **) &SpiPpi
             );

  if (Status != EFI_SUCCESS) {
    //
    // Initialize the ACPI base address so the ACPI PM timer can be used to wait for
    // SPI cycle completion
    //
    PchAcpiBaseGet (&AcpiBase);
    if (AcpiBase == 0) {
      PchAcpiBaseSet (PcdGet16 (PcdAcpiBaseAddress));
    }

    //
    // Prior to PCI enumeration, initialize SPI BAR0 to a default value
    // and also enable memory space decoding for SPI
    //
    InitSpiBar0 ();

    PeiSpiInstance = (PEI_SPI_INSTANCE *) AllocateZeroPool (sizeof (PEI_SPI_INSTANCE));
    if (NULL == PeiSpiInstance) {
      return EFI_OUT_OF_RESOURCES;
    }

    SpiInstance = &(PeiSpiInstance->SpiInstance);
    SpiProtocolConstructor (SpiInstance);

    PeiSpiInstance->PpiDescriptor.Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    PeiSpiInstance->PpiDescriptor.Guid = &gPchSpiPpiGuid;
    PeiSpiInstance->PpiDescriptor.Ppi = &(SpiInstance->SpiProtocol);

    Status = PeiServicesInstallPpi (&PeiSpiInstance->PpiDescriptor);
  }
  return Status;
}

/**
  Acquires the PCH SPI BAR0 MMIO address.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval    UINTN                The SPIO BAR0 MMIO address

**/
UINTN
AcquireSpiBar0 (
  IN  SPI_INSTANCE                *SpiInstance
  )
{
  return MmioRead32 (SpiInstance->PchSpiBase + R_PCH_SPI_BAR0) & ~(B_PCH_SPI_BAR0_MASK);
}

/**
  Release the PCH SPI BAR0 MMIO address.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval None
**/
VOID
ReleaseSpiBar0 (
  IN  SPI_INSTANCE                *SpiInstance
  )
{
  return;
}

/**
  Disables BIOS Write Protect

  @retval EFI_SUCCESS             BIOS Write Protect was disabled successfully

**/
EFI_STATUS
EFIAPI
DisableBiosWriteProtect (
  VOID
  )
{
  UINT64  SpiBaseAddress;

  SpiBaseAddress =  PCI_SEGMENT_LIB_ADDRESS (
                      0,
                      0,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI,
                      0
                      );
  //
  // Clear EISS bit to allow for SPI use
  //
  PciSegmentAnd8 (SpiBaseAddress + R_PCH_SPI_BC, (UINT8) ~B_PCH_SPI_BC_EISS);

  //
  // Write clear BC_SYNC_SS prior to change WPD from 0 to 1.
  //
  PciSegmentOr8 (
    SpiBaseAddress + R_PCH_SPI_BC + 1,
    (B_PCH_SPI_BC_SYNC_SS >> 8)
    );

  //
  // Set BIOSWE bit (SPI PCI Offset DCh [0]) = 1b
  // Enable the access to the BIOS space for both read and write cycles
  //
  PciSegmentOr8 (
    SpiBaseAddress + R_PCH_SPI_BC,
    B_PCH_SPI_BC_WPD
    );

  ASSERT ((PciSegmentRead8 (SpiBaseAddress + R_PCH_SPI_BC) & B_PCH_SPI_BC_EISS) == 0);

  return EFI_SUCCESS;
}

/**
  Enables BIOS Write Protect

**/
VOID
EFIAPI
EnableBiosWriteProtect (
  VOID
  )
{
  UINT64  SpiBaseAddress;

  SpiBaseAddress =  PCI_SEGMENT_LIB_ADDRESS (
                      0,
                      0,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI,
                      0
                      );

  //
  // Disable the access to the BIOS space for write cycles
  //
  PciSegmentAnd8 (
    SpiBaseAddress + R_PCH_SPI_BC,
    (UINT8) (~B_PCH_SPI_BC_WPD)
    );
}
