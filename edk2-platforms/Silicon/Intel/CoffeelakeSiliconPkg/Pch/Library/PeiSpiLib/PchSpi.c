/** @file
  PCH SPI PEI Library implements the SPI Host Controller Compatibility Interface.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/Spi.h>
#include <Private/Library/PchSpiCommonLib.h>
#include <PchReservedResources.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>

typedef struct {
  EFI_PEI_PPI_DESCRIPTOR  PpiDescriptor;
  SPI_INSTANCE            SpiInstance;
} PEI_SPI_INSTANCE;

/**
  PCI Enumeratuion is not done till later in DXE
  Initlialize SPI BAR0 to a default value till enumeration is done
  also enable memory space decoding for SPI

**/
VOID
InitSpiBar0 (
  VOID
  )
{
  UINT64       PchSpiBase;
  PchSpiBase = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI,
                 0
                 );
  PciSegmentAnd8 (PchSpiBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (PchSpiBase + R_SPI_CFG_BAR0, PCH_SPI_BASE_ADDRESS);
  PciSegmentOr8 (PchSpiBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  This function Initial SPI services

  @retval EFI_STATUS  Results of the installation of the SPI services
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

  Status = PeiServicesLocatePpi (
             &gPchSpiPpiGuid,
             0,
             NULL,
             (VOID **)&SpiPpi
             );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "SpiServiceInit() Start\n"));

    //
    // PCI Enumeratuion is not done till later in DXE
    // Initlialize SPI BAR0 to a default value till enumeration is done
    // also enable memory space decoding for SPI
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

    ///
    /// Install the SPI PPI
    ///
    DEBUG ((DEBUG_INFO, "SPI PPI Installed\n"));
    Status = PeiServicesInstallPpi (&PeiSpiInstance->PpiDescriptor);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((DEBUG_INFO, "SpiServiceInit() End\n"));
  }
  else {
    DEBUG ((DEBUG_INFO, "SPI PPI already installed\n"));
  }
  return Status;
}

/**
  Acquire pch spi mmio address.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval PchSpiBar0              return SPI MMIO address
**/
UINTN
AcquireSpiBar0 (
  IN  SPI_INSTANCE                *SpiInstance
  )
{
  return PciSegmentRead32 (SpiInstance->PchSpiBase + R_SPI_CFG_BAR0) & ~(B_SPI_CFG_BAR0_MASK);
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
  UINT64           SpiBaseAddress;

  SpiBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI,
                     0
                     );
  if ((PciSegmentRead8 (SpiBaseAddress + R_SPI_CFG_BC) & B_SPI_CFG_BC_EISS) != 0) {
    return EFI_ACCESS_DENIED;
  }
  ///
  /// Enable the access to the BIOS space for both read and write cycles
  ///
  PciSegmentOr8 (
    SpiBaseAddress + R_SPI_CFG_BC,
    B_SPI_CFG_BC_WPD
    );

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
  UINT64           SpiBaseAddress;

  SpiBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI,
                     0
                     );
  ///
  /// Disable the access to the BIOS space for write cycles
  ///
  PciSegmentAnd8 (
    SpiBaseAddress + R_SPI_CFG_BC,
    (UINT8) (~B_SPI_CFG_BC_WPD)
    );
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
  return TRUE;
}
