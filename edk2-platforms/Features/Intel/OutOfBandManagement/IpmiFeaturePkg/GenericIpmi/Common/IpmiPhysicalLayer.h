/** @file
  IPMI Common physical layer functions.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_PHYSICAL_LAYER_H
#define _IPMI_PHYSICAL_LAYER_H

//
// KCS physical interface layer
//
EFI_STATUS
InitializeIpmiKcsPhysicalLayer (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );


EFI_STATUS
SmmInitializeIpmiKcsPhysicalLayer (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );

#endif

