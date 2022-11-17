/** @file
  Protocol used for specifying platform related CPU information and policy setting.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_POLICY_PROTOCOL_H_
#define _CPU_POLICY_PROTOCOL_H_

//
// DXE_CPU_POLICY_PROTOCOL revisions
//
#define DXE_CPU_POLICY_PROTOCOL_REVISION 1

extern EFI_GUID gDxeCpuPolicyProtocolGuid;

#pragma pack (push,1)

/**
  The protocol allows the platform code to publish a set of configuration information that the
  CPU drivers will use to configure the processor in the DXE phase.
  This Policy Protocol needs to be initialized for CPU configuration.
  @note The Protocol has to be published before processor DXE drivers are dispatched.
**/
typedef struct {
  /**
  This member specifies the revision of the Cpu Policy protocol. This field is used to indicate backward
  compatible changes to the protocol. Any such changes to this protocol will result in an update in the revision number.

  <b>Revision 1</b>:
   - Initial version
  **/
  /**
  Policies to obtain CPU temperature.
   - <b>0: ACPI thermal management uses EC reported temperature values</b>.
   - 1: ACPI thermal management uses DTS SMM mechanism to obtain CPU temperature values.
   - 2: ACPI Thermal Management uses EC reported temperature values and DTS SMM is used to handle Out of Spec condition.
  **/
  UINT32                         EnableDts           : 2;
  UINT32                         RsvdBit             : 30;  ///< Reserved bits, align to multiple 32;

  UINT8                          Revision;                  ///< Current revision of policy.
  UINT8                          ReservedByte[3];           ///< Reserved bytes, align to multiple 8.
} DXE_CPU_POLICY_PROTOCOL;

#pragma pack (pop)

#endif
