/** @file
 *
 *  Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef IO_ACCESS_LIB_H_
#define IO_ACCESS_LIB_H_

#include <Base.h>

///
///  Structure to have pointer to R/W
///  Mmio operations for 16 bits.
///
typedef struct _MMIO_OPERATIONS {
  UINT16 (*Read16) (UINTN Address);
  UINT16 (*Write16) (UINTN Address, UINT16 Value);
  UINT16 (*Or16) (UINTN Address, UINT16 OrData);
  UINT16 (*And16) (UINTN Address, UINT16 AndData);
  UINT16 (*AndThenOr16) (UINTN Address, UINT16 AndData, UINT16 OrData);
  UINT32 (*Read32) (UINTN Address);
  UINT32 (*Write32) (UINTN Address, UINT32 Value);
  UINT32 (*Or32) (UINTN Address, UINT32 OrData);
  UINT32 (*And32) (UINTN Address, UINT32 AndData);
  UINT32 (*AndThenOr32) (UINTN Address, UINT32 AndData, UINT32 OrData);
  UINT64 (*Read64) (UINTN Address);
  UINT64 (*Write64) (UINTN Address, UINT64 Value);
  UINT64 (*Or64) (UINTN Address, UINT64 OrData);
  UINT64 (*And64) (UINTN Address, UINT64 AndData);
  UINT64 (*AndThenOr64) (UINTN Address, UINT64 AndData, UINT64 OrData);
} MMIO_OPERATIONS;

/**
  Function to return pointer to Mmio operations.

  @param  Swap  Flag to tell if Swap is needed or not
                on Mmio Operations.

  @return       Pointer to Mmio Operations.

**/
MMIO_OPERATIONS *
GetMmioOperations  (
  IN  BOOLEAN  Swap
  );

#endif /* IO_ACCESS_LIB_H_ */
