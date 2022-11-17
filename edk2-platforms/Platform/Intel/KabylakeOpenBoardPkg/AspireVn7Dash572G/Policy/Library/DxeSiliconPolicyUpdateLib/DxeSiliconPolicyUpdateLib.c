/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/ConfigBlockLib.h>
#include <Library/SiliconPolicyUpdateLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Protocol/GopPolicy.h>
#include <Protocol/SaPolicy.h>

#include "DxeSaPolicyInit.h"
#include "DxeGopPolicyInit.h"

/**
  Performs silicon late policy update.

  The meaning of Policy is defined by silicon code.
  It could be the raw data, a handle, a Protocol, etc.

  The input Policy must be returned by SiliconPolicyDoneLate().

  In FSP or non-FSP path, the board may use additional way to get
  the silicon policy data field based upon the input Policy.

  @param[in, out] Policy       Pointer to policy.

  @return the updated policy.
**/
VOID *
EFIAPI
SiliconPolicyUpdateLate (
  IN VOID *Policy
  )
{
  SA_POLICY_PROTOCOL       *SaPolicy = Policy;
  EFI_STATUS               Status;
  GRAPHICS_DXE_CONFIG      *GraphicsDxeConfig;
  GOP_POLICY_PROTOCOL      *GopPolicy;
  EFI_PHYSICAL_ADDRESS     VbtAddress;
  UINT32                   VbtSize;

  Status = GetConfigBlock ((VOID *)SaPolicy, &gGraphicsDxeConfigGuid, (VOID *) &GraphicsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  UpdateDxeSaPolicy (SaPolicy);

  if (PcdGetBool (PcdIntelGopEnable)) {
    //
    // GOP Dxe Policy Initialization
    //
    Status = GopPolicyInitDxe (gImageHandle);
    DEBUG ((DEBUG_INFO, "GOP Dxe Policy Initialization done\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Copy VBT address to Policy
  Status = gBS->LocateProtocol (&gGopPolicyProtocolGuid, NULL, (VOID **) &GopPolicy);
  if (!EFI_ERROR (Status)) {
    Status = GopPolicy->GetVbtData (&VbtAddress, &VbtSize);
    if (!EFI_ERROR (Status) && GraphicsDxeConfig != NULL) {
      GraphicsDxeConfig->VbtAddress = VbtAddress;
      GraphicsDxeConfig->Size = VbtSize;
      DEBUG ((DEBUG_INFO, "Located VBT at 0x%x with size 0x%x\n", VbtAddress, VbtSize));
    } else {
      DEBUG ((DEBUG_ERROR, "No VBT found, or Policy == NULL; Status - %r\n", Status));
    }
  }

  return Policy;
}
