/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __CSR_TO_PCIE_ADDRESS_H__
#define __CSR_TO_PCIE_ADDRESS_H__


#include <UsraAccessApi.h>

//////////////////////////////////////////////////////////////////////////
//
// Common Silicon Address Library
// This Lib provide the way use platform Library instance
//
//////////////////////////////////////////////////////////////////////////


/**
  This Lib Convert the logical address (CSR type, e.g. CPU ID, Boxtype, Box instance etc.) into physical address

  @param[in] Global               Global pointer
  @param[in] Virtual              Virtual address
  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

  @retval NULL                    The function completed successfully.
  @retval <>NULL                  Return Error
**/
UINTN
EFIAPI
CsrGetPcieAlignAddress (
  IN VOID                     *Global,
  IN BOOLEAN                  Virtual,
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

#endif
