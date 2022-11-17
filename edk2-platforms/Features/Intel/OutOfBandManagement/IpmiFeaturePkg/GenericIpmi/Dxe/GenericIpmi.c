/** @file
  Generic IPMI Transport functions..

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <IndustryStandard/Ipmi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include "IpmiPhysicalLayer.h"

/*****************************************************************************
 @brief
  This is entry point for IPMI service for BIOS POST.

 @param[in] ImageHandle  a handle to driver image
 @param[in] SystemTable  a pointer to system table

 @retval EFI_SUCCESS      The function completed successfully.
 @retval EFI_UNSUPPORTED  IPMI is not available.
**/
EFI_STATUS
LocateIpmiInterface (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS          Status;

  Status = InitializeIpmiKcsPhysicalLayer (ImageHandle, SystemTable);

  //
  // keep this interface for other Physical Layer as new interface.
  //

  return Status;
} // LocateIpmiInterface()

