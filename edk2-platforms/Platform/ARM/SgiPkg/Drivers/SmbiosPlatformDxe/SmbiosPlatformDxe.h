/** @file
  Declarations required for SMBIOS DXE driver.

  Functions declarations and data type declarations required for SMBIOS DXE
  driver of the Arm Reference Design platforms.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef SMBIOS_PLATFORM_DXE_H_
#define SMBIOS_PLATFORM_DXE_H_

/**
  Install SMBIOS BIOS information Table.

  Install the SMBIOS BIOS information (type 0) table for Arm's reference design
  platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType0BiosInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS System information Table.

  Install the SMBIOS system information (type 1) table for Arm's reference
  design platforms.

  @param[in]  Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_NOT_FOUND         Unknown product id.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType1SystemInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS System Enclosure Table

  Install the SMBIOS System Enclosure (type 3) table for Arm's Reference Design
  platforms.

  @param[in]  Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType3SystemEnclosure (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS Processor information Table

  Install the SMBIOS Processor information (type 4) table for Arm's Reference
  Design platforms.

  @param[in]  Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_NOT_FOUND         Unknown product id.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType4ProcessorInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS Cache information Table

  Install the SMBIOS Cache information (type 7) table for Arm's Reference
  Design platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_NOT_FOUND         Unknown product id.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType7CacheInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS physical memory array table.

  Install the SMBIOS physical memory array (type 16) table for Arm's Reference
  Design platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType16PhysicalMemoryArray (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );


/**
  Install SMBIOS memory device table.

  Install the SMBIOS memory device (type 17) table for RD platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType17MemoryDevice (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS memory array mapped address table

  Install the SMBIOS memory array mapped address (type 19) table for RD
  platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType19MemoryArrayMappedAddress (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

/**
  Install SMBIOS system boot information

  Install the SMBIOS system boot information (type 32) table for RD platforms.

  @param[in] Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
EFIAPI
InstallType32SystemBootInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  );

typedef enum {
  SMBIOS_HANDLE_ENCLOSURE = 0x1000,
  SMBIOS_HANDLE_CLUSTER1,
  SMBIOS_HANDLE_L1I_CACHE,
  SMBIOS_HANDLE_L1D_CACHE,
  SMBIOS_HANDLE_L2_CACHE,
  SMBIOS_HANDLE_L3_CACHE,
  SMBIOS_HANDLE_L4_CACHE,
  SMBIOS_HANDLE_PHYSICAL_MEMORY,
  SMBIOS_HANDLE_MEMORY_DEVICE0000,  // Chip 0 Bank 0
  SMBIOS_HANDLE_MEMORY_DEVICE0001,  // Chip 0 Bank 1
  SMBIOS_HANDLE_MEMORY_DEVICE0100,  // Chip 1 Bank 0
  SMBIOS_HANDLE_MEMORY_DEVICE0101,  // Chip 1 Bank 1
  SMBIOS_HANDLE_MEMORY_DEVICE0200,  // Chip 2 Bank 0
  SMBIOS_HANDLE_MEMORY_DEVICE0201,  // Chip 2 Bank 1
  SMBIOS_HANDLE_MEMORY_DEVICE0300,  // Chip 3 Bank 0
  SMBIOS_HANDLE_MEMORY_DEVICE0301,  // Chip 3 Bank 1
} SMBIOS_REFRENCE_HANDLES;

#endif // SMBIOS_PLATFORM_DXE_H_
