/** @file
  Header file for SMM Access Driver.

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SMM_ACCESS_DRIVER_H_
#define _SMM_ACCESS_DRIVER_H_

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Uefi/UefiBaseType.h>

#include <Guid/SmramMemoryReserve.h>
#include <Protocol/SmmAccess2.h>
#include <IndustryStandard/Pci22.h>

#define SMM_ACCESS_PRIVATE_DATA_SIGNATURE SIGNATURE_32 ('4', '5', 's', 'a')

///
/// Private data
///
typedef struct {
  UINTN                           Signature;
  EFI_HANDLE                      Handle;
  EFI_SMM_ACCESS2_PROTOCOL        SmmAccess;

  ///
  /// Local Data for SMM Access interface goes here
  ///
  UINTN                           NumberRegions;
  EFI_SMRAM_DESCRIPTOR            *SmramDesc;
} SMM_ACCESS_PRIVATE_DATA;

#define SMM_ACCESS_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, \
      SMM_ACCESS_PRIVATE_DATA, \
      SmmAccess, \
      SMM_ACCESS_PRIVATE_DATA_SIGNATURE \
      )

//
// Prototypes
// Driver model protocol interface
//
/**
  <b>SMM Access Driver Entry Point</b>
  This driver installs an SMM Access Protocol
  - <b>Introduction</b> \n
    This module publishes the SMM access protocol.  The protocol is used by the SMM Base driver to access the SMRAM region when the processor is not in SMM.
    The SMM Base driver uses the services provided by the SMM access protocol to open SMRAM during POST and copy the SMM handler.
    SMM access protocol is also used to close the SMRAM region once the copying is done.
    Finally, the SMM access protocol provides services to "Lock" the SMRAM region.
    Please refer the SMM Protocols section in the attached SMM CIS Specification version 0.9 for further details.
    This driver is required if SMM is supported. Proper configuration of SMM registers is recommended even if SMM is not supported.

  - <b>Porting Recommendations</b> \n
    No modification of this module is recommended.  Any modification should be done in compliance with the _EFI_SMM_ACCESS_PROTOCOL protocol definition.

  @param[in] ImageHandle        - Handle for the image of this driver
  @param[in] SystemTable        - Pointer to the EFI System Table

  @retval EFI_SUCCESS           - Protocol was installed successfully
  @retval EFI_UNSUPPORTED       - Protocol was not installed
  @retval EFI_NOT_FOUND         - Protocol can't be found.
  @retval EFI_OUT_OF_RESOURCES  - Protocol does not have enough resources to initialize the driver.
**/
EFI_STATUS
EFIAPI
SmmAccessDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  This routine accepts a request to "open" a region of SMRAM.  The
  region could be legacy ABSEG, HSEG, or TSEG near top of physical memory.
  The use of "open" means that the memory is visible from all boot-service
  and SMM agents.

  @param[in] This                  - Pointer to the SMM Access Interface.

  @retval EFI_SUCCESS           - The region was successfully opened.
  @retval EFI_DEVICE_ERROR      - The region could not be opened because locked by chipset.
  @retval EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
**/
EFI_STATUS
EFIAPI
Open (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  );

/**
  This routine accepts a request to "close" a region of SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "close" means that the memory is only visible from SMM agents,
  not from BS or RT code.

  @param[in] This               - Pointer to the SMM Access Interface.

  @retval EFI_SUCCESS           - The region was successfully closed.
  @retval EFI_DEVICE_ERROR      - The region could not be closed because locked by chipset.
  @retval EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
**/
EFI_STATUS
EFIAPI
Close (
  IN EFI_SMM_ACCESS2_PROTOCOL  *This
  );

/**
  This routine accepts a request to "lock" SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "lock" means that the memory can no longer be opened
  to BS state..

  @param[in] This               - Pointer to the SMM Access Interface.

  @retval EFI_SUCCESS           - The region was successfully locked.
  @retval EFI_DEVICE_ERROR      - The region could not be locked because at least
                                  one range is still open.
  @retval EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
**/
EFI_STATUS
EFIAPI
Lock (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  );

/**
  This routine services a user request to discover the SMRAM
  capabilities of this platform.  This will report the possible
  ranges that are possible for SMRAM access, based upon the
  memory controller capabilities.

  @param[in] This                  - Pointer to the SMRAM Access Interface.
  @param[in, out] SmramMapSize     - Pointer to the variable containing size of the
                                     buffer to contain the description information.
  @param[in, out] SmramMap         - Buffer containing the data describing the Smram
                                     region descriptors.

  @retval EFI_BUFFER_TOO_SMALL     - The user did not provide a sufficient buffer.
  @retval EFI_SUCCESS              - The user provided a sufficiently-sized buffer.
**/
EFI_STATUS
EFIAPI
GetCapabilities (
  IN CONST EFI_SMM_ACCESS2_PROTOCOL  *This,
  IN OUT UINTN                   *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR    *SmramMap
  );
#endif
