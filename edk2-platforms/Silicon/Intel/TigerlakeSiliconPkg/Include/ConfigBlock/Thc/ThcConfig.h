/** @file
  Touch Host Controller policy.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _THC_CONFIG_H_
#define _THC_CONFIG_H_

#define THC_CONFIG_REVISION 1
extern EFI_GUID gThcConfigGuid;

#pragma pack (push,1)

/**
  Available Port Assignments

**/
typedef enum {
  ThcAssignmentNone, ///< None of the avaialbe controllers assigned
  ThcAssignmentThc0, ///< Port assigned to THC0
  ThcAssignmentThc1  ///< Port assigned to THC1
} THC_PORT_ASSIGNMENT;


/**
  Port Configuration structure required for each Port that THC might use.

**/
typedef struct {
  UINT32 Assignment;         ///< Sets THCx assignment see THC_PORT_ASSIGNMENT
  UINT32 InterruptPinMuxing; ///< Each GPIO PORTx/SPIx INTB Pin has different muxing options refer to GPIO_*_MUXING_THC_SPIx_*
} THC_PORT;

/**
  THC_CONFIG block provides the configurations forTouch Host Controllers

  Assignment field in each THC port controlls the THC behavior.

  Available scenarios:
  1: Single Port 0 used by THC0
      - THC0 Enabled
      - Port0 assigned to THC0
      - Port1 unassigned
      - THC1 will be automatically Disabled.
  2: Both ports used by THC0
      - THC0 Enabled
      - Port0 assigned to THC0
      - Port1 assigned to THC0
      - THC1 will be automatically Disabled.
  3: Port 0 used by THC0 and Port 1 used by THC1
      - THC0 Enabled
      - Port0 assigned to THC0
      - THC1 Enabled
      - Port1 assigned to THC1.
<b>4: Both Ports unassigned.</b>
      Both THC Controllers will be disabled in that case.

  @note
  Invalid scenario that will cause ASSERT.
  1. Same port Number assigned to THC0 or THC1.
  2. Two Ports assigned to THC1.

**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  THC_PORT             ThcPort[2];       ///< Port Configuration
} THC_CONFIG;

#pragma pack (pop)

#endif // _THC_CONFIG_H_
