/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PCH_RESET_H
#define _PCH_RESET_H

#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/EventGroup.h>
#include <Library/MmPciBaseLib.h>
#include <Protocol/PchReset.h>
#include <PchAccess.h>
#include <Library/PchCycleDecodingLib.h>
#include <IncludePrivate/Library/PchResetCommonLib.h>
#include <IncludePrivate/PchPolicyHob.h>
#include <Library/HobLib.h>

#define EFI_INTERNAL_POINTER 0x04

#define PCH_RESET_INSTANCE_FROM_THIS(a) \
  CR ( \
  a, \
  PCH_RESET_INSTANCE, \
  PchResetInterface.PchResetProtocol, \
  PCH_RESET_SIGNATURE \
  )

#define CAPSULE_RESET_S3         0
#define CAPSULE_RESET_WARM       1

/**
  <b>PchReset Runtime  DXE Driver Entry Point</b>\n
  - <b>Introduction</b>\n
    The PchReset Runtime DXE driver provide a standard way for other modules to
    use the PCH Reset Interface in DXE/SMM/Runtime environments. It has no longer
    hooked ResetSystem() function of the runtime service table.

  - @pre
    - If there is any driver which needs to run the callback function right before
    issuing the reset, PCH Reset Callback Protocol will need to be installed
    before PCH Reset Runtime DXE driver. If PchReset Runtime DXE driver is run
    before Status Code Runtime Protocol is installed and there is the need
    to use Status code in the driver, it will be necessary to add EFI_STATUS_CODE_RUNTIME_PROTOCOL_GUID
    to the dependency file.
    - @link _PCH_RESET_CALLBACK_PROTOCOL  PCH_RESET_CALLBACK_PROTOCOL @endlink

  - @result
    The Reset driver produces @link _PCH_RESET_PROTOCOL PCH_RESET_PROTOCOL @endlink

  @param[in] ImageHandle          Image handle of the loaded driver
  @param[in] SystemTable          Pointer to the System Table

  @retval EFI_SUCCESS             Thread can be successfully created
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR        Cannot create the timer service
**/
EFI_STATUS
EFIAPI
InstallPchReset (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Execute call back function for Pch Reset.

  @param[in] PchResetType         Pch Reset Types which includes PowerCycle, Globalreset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback protocol. Or, none of
                                  callback protocol is installed.
  @retval Others                  Do not do any reset from PCH
**/
EFI_STATUS
EFIAPI
PchResetCallback (
  IN     PCH_RESET_TYPE           PchResetType
  );

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.

**/
VOID
EFIAPI
PchResetVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );
#endif
