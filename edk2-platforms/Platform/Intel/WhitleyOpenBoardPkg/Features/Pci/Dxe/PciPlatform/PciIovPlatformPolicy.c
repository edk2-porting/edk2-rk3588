/** @file

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include "PciPlatform.h"
#include <Guid/SetupVariable.h>

#ifdef EFI_PCI_IOV_SUPPORT

/**

    The GetSystemLowestPageSize() function retrieves the system lowest page size.

    @param This                 - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param SystemLowestPageSize - The system lowest page size. (This system supports a
                                  page size of 2^(n+12) if bit n is set.)

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - SystemLowestPageSize is NULL.

**/
EFI_STATUS
EFIAPI
GetSystemLowestPageSize (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT UINT32                                  *SystemLowestPageSize
)
{
  UINT8                                 SystemPageSizeOption;

  CopyMem (&SystemPageSizeOption, (((UINT8*) PcdGetPtr (PcdSetup)) + OFFSET_OF (SYSTEM_CONFIGURATION, SystemPageSize)), sizeof (UINT8));

  if (SystemLowestPageSize != NULL) {
    //
    // Convert page size option to page size
    // Option is n in 2^n
    // Page size is number of 4KiB pages
    //

    *SystemLowestPageSize = (UINT32) (1 << SystemPageSizeOption);
  }
  return EFI_SUCCESS;
}

/**

    The GetIovPlatformPolicy() function retrieves the platform policy regarding PCI IOV.

    @param This         - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param PciIovPolicy - The platform policy for PCI IOV configuration.

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - PciPolicy is NULL.

**/
EFI_STATUS
EFIAPI
GetIovPlatformPolicy (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT EFI_PCI_IOV_PLATFORM_POLICY             *PciIovPolicy
)
{
  UINT8                                 PolicyEnable;
  UINT8                                 ARIEnable;
  UINT8                                 SRIOVEnable;
  UINT8                                 MRIOVEnable;

  PolicyEnable = 0;

  CopyMem (&ARIEnable,   (((UINT8*) PcdGetPtr (PcdSetup)) + OFFSET_OF (SYSTEM_CONFIGURATION, ARIEnable)),   sizeof (UINT8));
  CopyMem (&SRIOVEnable, (((UINT8*) PcdGetPtr (PcdSetup)) + OFFSET_OF (SYSTEM_CONFIGURATION, SRIOVEnable)), sizeof (UINT8));
  CopyMem (&MRIOVEnable, (((UINT8*) PcdGetPtr (PcdSetup)) + OFFSET_OF (SYSTEM_CONFIGURATION, MRIOVEnable)), sizeof (UINT8));

  if (ARIEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_ARI;
  }

  if (SRIOVEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_SRIOV;
  }

  if (MRIOVEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_MRIOV;
  }

  if (PciIovPolicy != NULL) {
    //*PciIovPolicy = EFI_PCI_IOV_POLICY_ARI | EFI_PCI_IOV_POLICY_SRIOV;
    *PciIovPolicy = PolicyEnable;
  }
  return EFI_SUCCESS;
}

#endif

