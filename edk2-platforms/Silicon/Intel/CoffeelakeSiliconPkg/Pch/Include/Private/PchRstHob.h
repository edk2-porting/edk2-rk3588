/** @file
  Definitions required to create RstHob

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_RST_HOB_
#define _PCH_RST_HOB_

extern EFI_GUID  gPchRstHobGuid;

//
// This struct is used to record the fields that should be restored during device wake up
//
typedef struct {
  UINT8     PmCapPtr;
  UINT8     PcieCapPtr;
  UINT16    L1ssCapPtr;
  UINT8     EndpointL1ssControl2;
  UINT32    EndpointL1ssControl1;
  UINT16    LtrCapPtr;
  UINT32    EndpointLtrData;
  UINT16    EndpointLctlData16;
  UINT16    EndpointDctlData16;
  UINT16    EndpointDctl2Data16;
  UINT16    RootPortDctl2Data16;
} SAVED_DEVICE_CONFIG_SPACE;

//
// This structure is used to record the result of PCIe storageremapping for each cycle router
//
typedef struct {
  UINT8                                 RootPortNum;                      // Indicates the root port number with RST PCIe Storage Remapping remapping supported and PCIe storage device plugged on, numbering is 0-based
  UINT8                                 DeviceInterface;                  // Indicates the interface of the PCIe storage device (AHCI or NVMe)
  UINT32                                EndPointUniqueMsixTableBar;       // Records the PCIe storage device's MSI-X Table BAR if it supports unique MSI-X Table BAR
  UINT32                                EndPointUniqueMsixTableBarValue;  // Records the PCIe storage device's MSI-X Table BAR value if it supports unique MSI-X Table BAR
  UINT32                                EndPointUniqueMsixPbaBar;         // Records the PCIe storage device's MSI-X PBA BAR if it supports unique MSI-X PBA BAR
  UINT32                                EndPointUniqueMsixPbaBarValue;    // Records the PCIe storage device's MSI-X PBA BAR value if it supports unique MSI-X PBA BAR
} RST_CR_CONFIGURATION;

//
//  Passes to DXE results of PCIe storage remapping
//
typedef struct {
  //
  // Stores configuration information about cycle router
  //
  RST_CR_CONFIGURATION  RstCrConfiguration[PCH_MAX_RST_PCIE_STORAGE_CR];

  //
  // Saved fields from hidden device config space to be used later by RST driver
  //
  SAVED_DEVICE_CONFIG_SPACE  SavedRemapedDeviceConfigSpace[PCH_MAX_RST_PCIE_STORAGE_CR];
} PCH_RST_HOB;

#endif
