/** @file
  This file contains definitions of PCH Info HOB.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_INFO_HOB_H_
#define _PCH_INFO_HOB_H_

extern EFI_GUID gPchInfoHobGuid;

#define PCH_INFO_HOB_REVISION  2

#pragma pack (push,1)
/**
  This structure is used to provide the information of PCH controller.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add CridSupport, CridOrgRid, and CridNewRid.
  <b>Revision 3</b>:
  - Added LaneReversal Field
**/
typedef struct {
  /**
    This member specifies the revision of the PCH Info HOB. This field is used
    to indicate backwards compatible changes to the protocol. Platform code that
    consumes this protocol must read the correct revision value to correctly interpret
    the content of the protocol fields.
  **/
  UINT8        Revision;
  UINT8        PcieControllerCfg[6];

  /**
    GbE over PCIe port number when GbE is enabled
    >0 - Root port number (1-based)
    0  - GbE over PCIe disabled
    This information needs to be passed through HOB as FIA registers
    are not accessible with POSTBOOT_SAI
  **/
  UINT8        GbePciePortNumber;
  UINT32       PciePortFuses;
  /**
    Bit map for PCIe Root Port Lane setting. If bit is set it means that
    corresponding Root Port has its lane enabled.
    BIT0 - RP0, BIT1 - RP1, ...
    This information needs to be passed through HOB as FIA registers
    are not accessible with POSTBOOT_SAI
  **/
  UINT32       PciePortLaneEnabled;
  /**
    Publish Hpet BDF and IoApic BDF information for VTD.
  **/
  UINT32       HpetBusNum    :  8;
  UINT32       HpetDevNum    :  5;
  UINT32       HpetFuncNum   :  3;
  UINT32       IoApicBusNum  :  8;
  UINT32       IoApicDevNum  :  5;
  UINT32       IoApicFuncNum :  3;
  /**
    Publish the CRID information.
  **/
  UINT32       CridOrgRid    :  8;
  UINT32       CridNewRid    :  8;
  UINT32       CridSupport   :  1;
  UINT32       Rsvdbits      : 15;
  /**
    This member specifies if lane reversal is enabled on the specific
    Pcie Root port controller.
  **/
  UINT8        PcieControllerLaneReversal[6];
} PCH_INFO_HOB;

#pragma pack (pop)

#endif // _PCH_INFO_HOB_H_

