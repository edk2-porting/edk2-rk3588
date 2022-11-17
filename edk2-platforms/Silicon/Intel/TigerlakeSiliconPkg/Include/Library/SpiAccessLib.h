/** @file
  SPI library header for abstraction of SPI HW registers accesses

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SPI_ACCESS_LIB_H_
#define _SPI_ACCESS_LIB_H_

/**
  Returns SPI PCI Config Space base address

  @retval  UINT64  SPI Config Space base address
**/
UINT64
SpiGetPciCfgAddress (
  VOID
  );

/**
  Returns SPI BAR0 value

  @retval  UINT32  PCH SPI BAR0 value
**/
UINT32
SpiGetBar0 (
  VOID
  );

/**
  Returns SPI Device number

  @retval UINT8   PCH SPI Device number
**/
UINT8
SpiDeviceNumber (
  VOID
  );

/**
  Returns SPI Function number

  @retval UINT8   PCH SPI Function number
**/
UINT8
SpiFunctionNumber (
  VOID
  );

/**
  Returns descriptor signature

  @retval UINT32    Descriptor signature
**/
UINT32
SpiGetDescriptorSignature (
  VOID
  );

/**
  Returns supported features and R/W frequencies of Flash Component

  @retval UINT32    Flash Component features descriptor
**/
UINT32
SpiGetFlashComponentDescription (
  VOID
  );

/**
  Returns number of Flash Components

  @retval UINT32    Flash components number
**/
UINT32
SpiGetFlashComponentsNumber (
  VOID
  );

/**
  Returns total Flash size with regards to number of flash components

  @retval UINT32    Total Flash Memory size
**/
UINT32
SpiGetTotalFlashSize (
  VOID
  );

/**
  Checks if PCH SPI Controler is present and available

  @retval TRUE    PCH SPI controller is avaialable
  @retval FALSE   PCH SPI controller is not available
**/
BOOLEAN
SpiIsControllerAvailable (
  VOID
  );

/**
  Checks BIOS lock bits for proper value and checks if write protection is enabled
  Expected vales are: LE bit set, EISS bit set and WPD bit cleared

  @retval  TRUE    All protection bits are set correctly
  @retval  FALSE   Not all protection bits had exepcted values
**/
BOOLEAN
SpiIsWriteProtectionEnabled (
  VOID
  );

/**
  Returns Flash Descriptor Override Pin Strap status

  @retval TRUE     Flash Descriptor override is enabled
  @retval FALSE    Flash Descriptor override is disabled
**/
BOOLEAN
SpiIsFlashDescriptorOverrideEnabled (
  VOID
  );

/**
  Returns Flash Configuration Lock Down bit status

  @retval TRUE    Flash Configuration Lock Down bit is set
  @retval FALSE   Flash Configuration Lock Down bit is not set
**/
BOOLEAN
SpiIsFlashConfigurationLockDownEnabled (
  VOID
  );

/**
  Returns Top Swap functionality enable state

  @retval TRUE    Top Swap is enabled
  @retval FALSE   Top Swap is disabled
**/
BOOLEAN
SpiIsTopSwapEnabled (
  VOID
  );

/**
  Return Component Property Parameter Table for a given component number

  @param[in]  ComponentNumber   SPI Component number
  @param[out] CppTable          Component Poperty Parameter Table value

  @retval TRUE  Vendor Specific Component Capabilities Register value was read
  @reval  FALSE Vendor Specific Component Capabilities Register value was not present
**/
BOOLEAN
SpiGetComponentPropertyParameterTable (
  IN  UINT8  ComponentNumber,
  OUT UINT32 *CppTable
  );

/**
  Returns valid bit status in given Component Property Parameter Table

  @param[in] CppTable          Component Poperty Parameter Table value

  @retval TRUE    Valid bit is set
  @reval  FALSE   Valid bit is not set
**/
BOOLEAN
SpiIsCppValidBitSet (
  IN UINT32  CppTable
  );

/**
  Checks if Flash Descriptor is valid

  @retval TRUE  Flash Descriptor is valid
  @retval FALSE Flash Descriptor is invalid
**/
BOOLEAN
SpiIsFlashDescriptorValid (
  VOID
  );

/**
  Returns masked BIOS Master Read Access

  @retval UINT32    Already masked BIOS Master Read Access
**/
UINT32
SpiGetMasterReadAccess (
  VOID
  );

/**
  Returns masked BIOS Master Write Access

  @retval UINT32    Already masked BIOS Master Write Access
**/
UINT32
SpiGetMasterWriteAccess (
  VOID
  );

/**
  Returns GbE Region Access rights

  @retval UINT32    GbE Region access rights
**/
UINT32
SpiGetGbeRegionAccess (
  VOID
  );

/**
  Returns CSME region access rights

  @retval UINT32    CSME Region Access rights
**/
UINT32
SpiGetCsmeRegionAccess (
  VOID
  );

/**
  Returns EC region access right

  @retval UINT32     EC Region access rights
**/
UINT32
SpiGetEcRegionAccess (
  VOID
  );

/**
  Checks if Slave Attached Flash (SAF) mode is active

  @retval TRUE    SAF mode is active
  @retval FALSE   SAF mode is not active
**/
BOOLEAN
SpiIsSafModeActive (
  VOID
  );

/**
  Checks validity of GbE region

  @retval TRUE    GbE region is valid
  @retval FALSE   GbE regios in invalid
**/
BOOLEAN
SpiIsGbeRegionValid (
  VOID
  );

/**
  Returns status of BIOS Interface Lockdown

  @retval TRUE  BIOS Interface Lockdown is enabled
  @retval FALSE BIOS Interface Lockdown is disabled
**/
BOOLEAN
SpiIsBiosInterfaceLockdownEnabled (
  VOID
  );

/**
  Returns TRUE if BIOS Boot Strap is set to SPI

  @retval TRUE    BIOS Boot strap is set to SPI
  @retval FALSE   BIOS Boot strap is set to LPC/eSPI
**/
BOOLEAN
SpiIsBiosBootFromSpi (
  VOID
  );

/**
  Check SPI write status disable is set

  @retval TRUE    Write status disable is set
  @retval FALSE   Write status disable is not set
**/
BOOLEAN
SpiIsWriteStatusDisable (
  VOID
  );

#endif // _SPI_ACCESS_LIB_H_
