/** @file
  Protocol used for specifying platform related Silicon information and policy setting.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SI_POLICY_PROTOCOL_H_
#define _SI_POLICY_PROTOCOL_H_

#include <IndustryStandard/Hsti.h>

//
// DXE_SI_POLICY_PROTOCOL revisions
//
#define DXE_SI_POLICY_PROTOCOL_REVISION 2

extern EFI_GUID gDxeSiPolicyProtocolGuid;

#pragma pack (push,1)

/**
  The protocol allows the platform code to publish a set of configuration information that the
  Silicon drivers will use to configure the processor in the DXE phase.
  This Policy Protocol needs to be initialized for Silicon configuration.
  @note The Protocol has to be published before processor DXE drivers are dispatched.
**/
typedef struct {
  /**
  This member specifies the revision of the Si Policy protocol. This field is used to indicate backward
  compatible changes to the protocol. Any such changes to this protocol will result in an update in the revision number.

  <b>Revision 1</b>:
   - Initial version
  <b>Revision 2</b>:
   - Added SmbiosOemTypeFirmwareVersionInfo to determines the SMBIOS OEM type
  **/
  UINT8                          Revision;
  /**
    SmbiosOemTypeFirmwareVersionInfo determines the SMBIOS OEM type (0x80 to 0xFF) defined in SMBIOS,
    values 0-0x7F will be treated as disable FVI reporting.
    FVI structure uses it as SMBIOS OEM type to provide version information.
  **/
  UINT8                          SmbiosOemTypeFirmwareVersionInfo;
  UINT8                          ReservedByte[6];  ///< Reserved bytes, align to multiple 8.
  /**
    This member describes a pointer to Hsti results from previous boot. In order to mitigate the large performance cost
    of performing all of the platform security tests on each boot, we can save the results across boots and retrieve
    and point this policy to them prior to the launch of HstiSiliconDxe. Logic should be implemented to not populate this
    upon major platform changes (i.e changes to setup option or platform hw)to ensure that results accurately reflect the
    configuration of the platform.
  **/
  ADAPTER_INFO_PLATFORM_SECURITY *Hsti;    ///< This is a pointer to Hsti results from previous boot
  UINTN                          HstiSize; ///< Size of results, if setting Hsti policy to point to previous results
} DXE_SI_POLICY_PROTOCOL;

#pragma pack (pop)

#endif
