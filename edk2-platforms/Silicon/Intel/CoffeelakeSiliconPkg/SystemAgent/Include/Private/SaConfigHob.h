/** @file
  The GUID definition for SaConfigHob

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_CONFIG_HOB_H_
#define _SA_CONFIG_HOB_H_

#include <SaAccess.h>
#include <Base.h>

extern EFI_GUID gSaConfigHobGuid;

#pragma pack (push,1)
///
#define DPR_DIRECTORY_MAX               2         ///< DPR Maximum Size
/// DPR directory entry definition
///
typedef struct {
  UINT8   Type;          ///< DPR Directory Type
  UINT8   Size;          ///< DPR Size in MB
  UINT32  PhysBase;      ///< Must be 4K aligned (bits 11..0 must be clear)
  UINT16  Reserved;      ///< Must be 0
} DPR_DIRECTORY_ENTRY;

///
/// The data elements should be initialized by a Platform Module.
/// The data structure is for VT-d driver initialization
///
typedef struct {
  BOOLEAN               VtdDisable;                        ///< 1 = Avoids programming Vtd bars, Vtd overrides and DMAR table
  UINT32                BaseAddress[SA_VTD_ENGINE_NUMBER]; ///< This field is used to describe the base addresses for VT-d function
  BOOLEAN               X2ApicOptOut;                      ///< This field is used to enable the X2APIC_OPT_OUT bit in the DMAR table. <b>1=Enable/Set</b> and 0=Disable/Clear
  BOOLEAN               InterruptRemappingSupport;         ///< This field is used to indicate Interrupt Remapping supported or not
} SA_VTD_CONFIGURATION_HOB;

///
/// SA GPIO Data Structure
///
typedef struct {
  UINT8   ExpanderNo; ///< =Expander No For I2C based GPIO
  UINT32  GpioNo;     ///< GPIO pad
  BOOLEAN Active;     ///< 0=Active Low; 1=Active High
} SA_GPIO;

///
/// SA PCIE RTD3 GPIO Data Structure
///
typedef struct {
  UINT8              GpioSupport;      ///< 0=Not Supported; 1=PCH based; 2=I2C Based
  SA_GPIO            HoldRst;          ///< Offset 8 This field contain PCIe HLD RESET GPIO value and level information
  SA_GPIO            PwrEnable;        ///< This field contain PCIe PWR Enable GPIO value and level information
  UINT32             WakeGpioNo;       ///< This field contain PCIe RTD3 Device Wake GPIO number
} PCIE_RTD3_GPIO;

///
/// SG Info HOB
///
typedef struct {
  SG_MODE           SgMode;
  UINT8             RootPortIndex;
  PCIE_RTD3_GPIO    Rtd3Pcie0Gpio;
  PCIE_RTD3_GPIO    Rtd3Pcie1Gpio;
  PCIE_RTD3_GPIO    Rtd3Pcie2Gpio;
  UINT16            DelayAfterPwrEn;
  UINT16            DelayAfterHoldReset;
} SA_RTD3;

///
/// System Agent Config Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                           ///< GUID Hob type structure for gSaConfigHobGuid
  DPR_DIRECTORY_ENTRY      DprDirectory[DPR_DIRECTORY_MAX];          ///< DPR directory entry definition
  BOOLEAN                  InitPcieAspmAfterOprom;                   ///< 1=initialize PCIe ASPM after Oprom; 0=before (This will be set basing on policy)
  SA_RTD3                  SaRtd3;                                   ///< SG Info HOB
  UINT8                    ApertureSize;                             ///< Aperture size value
  UINT8                    IpuAcpiMode;                              ///< IPU ACPI mode: 0=Disabled, 1=IGFX Child device, 2=ACPI device
  SA_VTD_CONFIGURATION_HOB VtdData;                                  ///< VT-d Data HOB
  BOOLEAN                  CridEnable;                               ///< This field inidicates if CRID is enabled or disabled (to support Intel(R) SIPP)
  BOOLEAN                  SkipPamLock;                              ///< 0=All PAM registers will be locked in System Agent code, 1=Do not lock PAM registers in System Agent code.
  UINT8                    PowerDownUnusedBundles[SA_PEG_MAX_FUN];   ///< PCIe power down unused bundles support
  UINT8                    PegMaxPayload[SA_PEG_MAX_FUN];            ///< PEG Max Pay Load Size (0xFF: Auto, 0:128B, 1:256B)
} SA_CONFIG_HOB;
#pragma pack (pop)
#endif
