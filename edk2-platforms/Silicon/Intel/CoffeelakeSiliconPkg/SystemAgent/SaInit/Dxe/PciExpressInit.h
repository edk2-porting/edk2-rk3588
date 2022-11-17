/** @file
  Header file for PciExpress Initialization Driver.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCIEXPRESS_INITIALIZATION_DRIVER_H_
#define _PCIEXPRESS_INITIALIZATION_DRIVER_H_

#include <Library/HobLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/Msr.h>
#include <SaAccess.h>
#include <PchAccess.h>
#include <Private/SaConfigHob.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/SaPolicy.h>
#include <Private/Protocol/SaNvsArea.h>

#define GEN1      1
#define GEN2      2

///
/// Function prototypes
///
/**
  PCI Express Dxe Initialization.
  Run before PCI Bus Init, where assignment of Bus, Memory,
    and I/O Resources are assigned.

  @param[in] SaPolicy    -     SA DXE Policy protocol

  @retval EFI_SUCCESS        - Pci Express successfully started and ready to be used
  @exception EFI_UNSUPPORTED - Pci Express can't be initialized
**/
EFI_STATUS
PciExpressInit (
  IN SA_POLICY_PROTOCOL *SaPolicy
  );

/**
  Find the Offset to a given Capabilities ID

  @param[in] Segment   -   Pci Segment Number
  @param[in] Bus       -   Pci Bus Number
  @param[in] Device    -   Pci Device Number
  @param[in] Function  -   Pci Function Number
  @param[in] CapId     -   CAPID to search fo

  @retval 0       - CAPID not found
  @retval Other   - CAPID found, Offset of desired CAPID
**/
UINT32
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );

/**
  Search and return the offset of desired Pci Express Capability ID

  @param[in] Segment   -   Pci Segment Number
  @param[in] Bus       -   Pci Bus Number
  @param[in] Device    -   Pci Device Number
  @param[in] Function  -   Pci Function Number
  @param[in] CapId     -   Extended CAPID to search for

  @retval 0       - CAPID not found
  @retval Other   - CAPID found, Offset of desired CAPID
**/
UINT32
PcieFindExtendedCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  );
#endif
