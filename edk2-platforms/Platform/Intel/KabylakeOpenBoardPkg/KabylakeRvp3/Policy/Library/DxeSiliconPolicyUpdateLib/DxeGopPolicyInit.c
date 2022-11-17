/** @file
  This file initialises and Installs GopPolicy Protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DxeGopPolicyInit.h"
#include <Protocol/GopPolicy.h>

GLOBAL_REMOVE_IF_UNREFERENCED GOP_POLICY_PROTOCOL        mGOPPolicy;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                     mVbtSize = 0;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PHYSICAL_ADDRESS       mVbtAddress = 0;

//
// Function implementations
//

/**

  @param[out] CurrentLidStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
GetPlatformLidStatus (
  OUT LID_STATUS *CurrentLidStatus
  )
{
  return EFI_UNSUPPORTED;
}
/**

  @param[out] CurrentDockStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
GetPlatformDockStatus (
  OUT DOCK_STATUS  CurrentDockStatus
  )
{
    return EFI_UNSUPPORTED;
}


/**

  @param[out] VbtAddress
  @param[out] VbtSize

  @retval     EFI_SUCCESS
  @retval     EFI_NOT_FOUND
**/
EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  )
{
  EFI_STATUS                    Status;
  UINTN                         FvProtocolCount;
  EFI_HANDLE                    *FvHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINTN                         Index;
  UINT32                        AuthenticationStatus;
  UINT8                         *Buffer;
  UINTN                         VbtBufferSize;


  Status = EFI_NOT_FOUND;
  if ( mVbtAddress == 0) {
    Fv           = NULL;

    Buffer = 0;
    FvHandles       = NULL;
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiFirmwareVolume2ProtocolGuid,
                    NULL,
                    &FvProtocolCount,
                    &FvHandles
                    );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < FvProtocolCount; Index++) {
        Status = gBS->HandleProtocol (
                        FvHandles[Index],
                        &gEfiFirmwareVolume2ProtocolGuid,
                        (VOID **) &Fv
                        );
        VbtBufferSize = 0;
        Status = Fv->ReadSection (
                       Fv,
                       PcdGetPtr (PcdGraphicsVbtGuid),
                       EFI_SECTION_RAW,
                       0,
                       (VOID **) &Buffer,
                       &VbtBufferSize,
                       &AuthenticationStatus
                       );
        if (!EFI_ERROR (Status)) {
          *VbtAddress = (EFI_PHYSICAL_ADDRESS)Buffer;
          *VbtSize = (UINT32)VbtBufferSize;
          mVbtAddress = *VbtAddress;
          mVbtSize = *VbtSize;
          Status = EFI_SUCCESS;
          break;
        }
      }
    } else {
      Status = EFI_NOT_FOUND;
    }

    if (FvHandles != NULL) {
      FreePool (FvHandles);
      FvHandles = NULL;
    }
  } else {
    *VbtAddress = mVbtAddress;
    *VbtSize = mVbtSize;
    Status = EFI_SUCCESS;
  }

  return Status;
}



/**
Initialize GOP DXE Policy

@param[in] ImageHandle          Image handle of this driver.

@retval EFI_SUCCESS             Initialization complete.
@retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
@retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
@retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/

EFI_STATUS
EFIAPI
GopPolicyInitDxe (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS         Status;

  //
  // Initialize the EFI Driver Library
  //
  SetMem (&mGOPPolicy, sizeof (GOP_POLICY_PROTOCOL), 0);

  mGOPPolicy.Revision                = GOP_POLICY_PROTOCOL_REVISION_03;
  mGOPPolicy.GetPlatformLidStatus    = GetPlatformLidStatus;
  mGOPPolicy.GetVbtData              = GetVbtData;
  mGOPPolicy.GetPlatformDockStatus   = GetPlatformDockStatus;

  //
  // Install protocol to allow access to this Policy.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gGopPolicyProtocolGuid,
                  &mGOPPolicy,
                  NULL
                  );

  return Status;
}
