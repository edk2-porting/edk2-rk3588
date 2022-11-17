/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MM_PCIE_BASE_LIB_H_
#define _MM_PCIE_BASE_LIB_H_

#include <Library/UsraAccessApi.h>

/**
  This procedure will get PCIE address

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number

  @retval PCIE address
**/
UINTN
MmPciBase (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
);

/**
  This procedure will get PCIE address

  @param[in] Seg                  Pcie Segment Number
  @param[in] Bus                  Pcie Bus Number
  @param[in] Device               Pcie Device Number
  @param[in] Function             Pcie Function Number

  @retval PCIE address
**/
UINTN
MmPciAddress(
IN UINT32                       Seg,
IN UINT32                       Bus,
IN UINT32                       Device,
IN UINT32                       Function,
IN UINT32                       Register
);

#endif // _MM_PCIE_BASE_LIB_H_
