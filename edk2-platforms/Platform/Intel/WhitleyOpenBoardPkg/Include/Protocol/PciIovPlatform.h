/** @file
  This file declares PCI IOV platform protocols.

  @copyright
  Copyright 2005 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCI_IOV_PLATFORM_H_
#define _PCI_IOV_PLATFORM_H_


//
// Protocol for GUID.
//

typedef struct _EFI_PCI_IOV_PLATFORM_PROTOCOL EFI_PCI_IOV_PLATFORM_PROTOCOL;

typedef    UINT32   EFI_PCI_IOV_PLATFORM_POLICY;

#define     EFI_PCI_IOV_POLICY_ARI           0x0001
#define     EFI_PCI_IOV_POLICY_SRIOV         0x0002
#define     EFI_PCI_IOV_POLICY_MRIOV         0x0004

typedef
EFI_STATUS
(EFIAPI * EFI_PCI_IOV_PLATFORM_GET_SYSTEM_LOWEST_PAGE_SIZE) (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT UINT32                                  *SystemLowestPageSize
)
/**

    The GetSystemLowestPageSize() function retrieves the system lowest page size.

    @param This                 - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param SystemLowestPageSize - The system lowest page size. (This system supports a
                                  page size of 2^(n+12) if bit n is set.)

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - SystemLowestPageSize is NULL.

**/
;

typedef
EFI_STATUS
(EFIAPI * EFI_PCI_IOV_PLATFORM_GET_PLATFORM_POLICY) (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT EFI_PCI_IOV_PLATFORM_POLICY             *PciIovPolicy
)
/**

    The GetPlatformPolicy() function retrieves the platform policy regarding PCI IOV.

    @param This         - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param PciIovPolicy - The platform policy for PCI IOV configuration.

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - PciPolicy is NULL.

**/
;

typedef struct _EFI_PCI_IOV_PLATFORM_PROTOCOL {
  EFI_PCI_IOV_PLATFORM_GET_SYSTEM_LOWEST_PAGE_SIZE          GetSystemLowestPageSize;
  EFI_PCI_IOV_PLATFORM_GET_PLATFORM_POLICY                  GetPlatformPolicy;
} EFI_PCI_IOV_PLATFORM_PROTOCOL;

extern EFI_GUID   gEfiPciIovPlatformProtocolGuid;

#endif
