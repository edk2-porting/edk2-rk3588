/** @file
  Smbus Policy PPI as defined in EFI 2.0

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_SMBUS_POLICY_PPI_H
#define _PEI_SMBUS_POLICY_PPI_H

#define PEI_SMBUS_POLICY_PPI_GUID \
  { \
  0x63b6e435, 0x32bc, 0x49c6, { 0x81, 0xbd, 0xb7, 0xa1, 0xa0, 0xfe, 0x1a, 0x6c } \
  }

typedef struct _PEI_SMBUS_POLICY_PPI PEI_SMBUS_POLICY_PPI;

typedef struct _PEI_SMBUS_POLICY_PPI {
  UINTN   BaseAddress;
  UINT32  PciAddress;
  UINT8   NumRsvdAddress;
  UINT8   *RsvdAddress;
} PEI_SMBUS_POLICY_PPI;

extern EFI_GUID gPeiSmbusPolicyPpiGuid;

#endif
