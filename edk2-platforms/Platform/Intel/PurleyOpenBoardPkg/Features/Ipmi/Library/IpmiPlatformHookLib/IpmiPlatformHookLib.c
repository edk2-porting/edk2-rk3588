/** @file
    IPMI platform hook.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/IpmiPlatformHookLib.h>

#include <Library/PchCycleDecodingLib.h>
#include <Register/PchRegsLpc.h>
//
// Prototype definitions for IPMI Platform Update Library
//

EFI_STATUS
EFIAPI
PlatformIpmiIoRangeSet(
  UINT16 IpmiIoBase
)
/*++

  Routine Description:

  This function sets IPMI Io range

  Arguments:

   IpmiIoBase

  Returns:

    Status

--*/
{
  return PchLpcGenIoRangeSet((IpmiIoBase & 0xFF0), 0x10, LPC_ESPI_FIRST_SLAVE);
}