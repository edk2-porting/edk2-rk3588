/** @file
  Dci policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DCI_CONFIG_H_
#define _DCI_CONFIG_H_

#define DCI_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gDciPreMemConfigGuid;

#pragma pack (push,1)

typedef enum {
  ProbeTypeDisabled   = 0x0,
  ProbeTypeDciOobDbc  = 0x1,
  ProbeTypeDciOob     = 0x2,
  ProbeTypeUsb3Dbc    = 0x3,
  ProbeTypeXdp3       = 0x4,
  ProbeTypeUsb2Dbc    = 0x5,
  ProbeTypeMax
} PLATFORM_DEBUG_CONSENT_PROBE_TYPE;

/**
  The PCH_DCI_PREMEM_CONFIG block describes policies related to Direct Connection Interface (DCI)

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    Platform Debug Consent
    As a master switch to enable platform debug capability and relevant settings with specified probe type.
    Note: DCI OOB (aka BSSB) uses CCA probe; [DCI OOB+DbC] and [USB2 DbC] have the same setting.
    Refer to definition of PLATFORM_DEBUG_CONSENT_PROBE_TYPE
    <b>0:Disabled</b>; 1:DCI OOB+DbC; 2:DCI OOB; 3:USB3 DbC; 4:XDP3/MIPI60 5:USB2 DbC;
  **/
  UINT32    PlatformDebugConsent  :  3;
  /**
    USB3 Type-C UFP2DFP kenel / platform debug support. No change will do nothing to UFP2DFP configuration.
    When enabled, USB3 Type C UFP (upstream-facing port) may switch to DFP (downstream-facing port) for first connection.
    It must be enabled for USB3 kernel(kernel mode debug) and platform debug(DFx, DMA, Trace) over UFP Type-C receptacle.
    Refer to definition of DCI_USB_TYPE_C_DEBUG_MODE for supported settings.
    0:Disabled; 1:Enabled; <b>2:No Change</b>
  **/
  UINT32    DciUsb3TypecUfpDbg    :  2;
  UINT32    RsvdBits              : 27;       ///< Reserved bits
} PCH_DCI_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _DCI_CONFIG_H_
