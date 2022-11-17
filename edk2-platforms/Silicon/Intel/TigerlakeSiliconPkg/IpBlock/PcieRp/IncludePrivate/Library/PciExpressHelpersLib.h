/** @file
  Header file for PCI Express helpers library

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCI_EXPRESS_HELPERS_LIB_H_
#define _PCI_EXPRESS_HELPERS_LIB_H_

#include <PchPolicyCommon.h>
#include <Library/PcieRpLib.h>

typedef enum {
  TpoScale2us,
  TpoScale10us,
  TpoScale100us,
  TpoScaleMax
} T_PO_SCALE;
//
// This structure keeps segment:bus:device:function coordinates of a PCIe device
// in a single variable. PcieCap is offset to PCI Express capabilities.
//
typedef struct {
  UINT32 Seg : 8;
  UINT32 Bus : 8;
  UINT32 Dev : 5;
  UINT32 Func : 3;
  UINT32 PcieCap : 8;
} SBDF;

/*
  Converts Tpower_on from value:scale notation to microseconds

  @param[in] TpoScale   T power on scale
  @param[in] TpoValue   T power on value

  @retval    number of microseconds
*/
UINT32
TpoToUs (
  UINT32 TpoScale,
  UINT32 TpoValue
  );

/**
PCIe controller Sku.
**/

typedef enum {
  EnumPchPcie = 0,
  EnumiTbtPcie = 1,
  EnumCpuPcie = 2,
  EnumPciSkuMax = 3
} PCI_SKU;

/*
  Initializes the following features in rootport and devices behind it:
  Maximum Payload Size (generic)
  Rootport packet split (proprietary)
  EonOfInterrupt forwarding (proprietary)
  Common Clock Configuration (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  CCC requires link retrain, which takes a while. CCC must happen before L0s/L1 programming.
  If there was guarantee no code would access PCI while links retrain, it would be possible to skip this waiting

  @param[in] RpSegment  address of rootport on PCIe
  @param[in] RpBus      address of rootport on PCIe
  @param[in] RpDevice   address of rootport on PCIe
  @param[in] RpFunction address of rootport on PCIe
  @param[in] BusMin     minimum Bus number that can be assigned below this rootport
  @param[in] BusMax     maximum Bus number that can be assigned below this rootport
*/
VOID
RootportDownstreamConfiguration (
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  UINT8                     BusMin,
  UINT8                     BusMax,
  PCI_SKU                   PciSku
  );

/*
  Configures the following power-management related features in rootport and devices behind it:
  LTR limit (generic)
  LTR override (proprietary)
  Clock Power Management (generic)
  L1 substates (generic except for the override table)
  L1.LOW substate (proprietary)
  L0s and L1 (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  @param[in] RpSegment                address of rootport on PCIe
  @param[in] RpBus                    address of rootport on PCIe
  @param[in] RpDevice                 address of rootport on PCIe
  @param[in] RpFunction               address of rootport on PCIe
  @param[in] BusMin                   minimal Bus number that can be assigned below this rootport
  @param[in] BusMax                   maximum Bus number that can be assigned below this rootport
  @param[in] PcieRpCommonConfig       a pointer to Pcie Root Port Common Config
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration
*/
VOID
RootportDownstreamPmConfiguration (
  UINT8                               RpSegment,
  UINT8                               RpBus,
  UINT8                               RpDevice,
  UINT8                               RpFunction,
  UINT8                               BusMin,
  UINT8                               BusMax,
  PCIE_ROOT_PORT_COMMON_CONFIG        *PcieRpCommonConfig,
  UINT32                              AspmOverrideTableSize,
  PCH_PCIE_DEVICE_OVERRIDE            *AspmOverrideTable
  );

typedef struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 Reserved                     : 6;
} LTR_LIMIT;

/**
  Checks if given PCI device is capable of Latency Tolerance Reporting

  @param[in] Sbdf            device's segment:bus:device:function coordinates

  @retval TRUE if yes
**/
BOOLEAN
IsLtrCapable (
  SBDF Sbdf
  );

/**
  Returns combination of two LTR override values
  The resulting LTR override separately chooses stricter limits for snoop and nosnoop

  @param[in] LtrA      LTR override values to be combined
  @param[in] LtrB      LTR override values to be combined

  @retval LTR override value
**/
LTR_OVERRIDE
CombineLtr (
  LTR_OVERRIDE LtrA,
  LTR_OVERRIDE LtrB
  );

/**
  Extended Virtual Channel Configuration
**/
typedef struct {
  UINT16 CapOffset;
  UINT8  ExtVcCount;
} MULTI_VC_SUPPORT;

#endif // _PCI_EXPRESS_HELPERS_LIB_H_
