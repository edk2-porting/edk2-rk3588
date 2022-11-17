/** @file
  Main System Agent Policy structure definition which will contain several config blocks during runtime.

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_POLICY_COMMON_H_
#define _SA_POLICY_COMMON_H_

#include <Uefi.h>
#include <Library/SmbusLib.h>
#include <SaAccess.h>
#include <ConfigBlock.h>
#include <ConfigBlock/SwitchableGraphicsConfig.h>
#include <ConfigBlock/MemoryConfig.h>
#include <ConfigBlock/GraphicsPeiPreMemConfig.h>
#include <ConfigBlock/PciePeiPreMemConfig.h>
#include <ConfigBlock/IpuPreMemConfig.h>
#include <ConfigBlock/SaMiscPeiPreMemConfig.h>
#include <ConfigBlock/GnaConfig.h>
#include <ConfigBlock/GraphicsPeiConfig.h>
#include <ConfigBlock/SaMiscPeiConfig.h>
#include <ConfigBlock/OverClockingConfig.h>
#include <ConfigBlock/VtdConfig.h>
#include <ConfigBlock/PciePeiConfig.h>


//
// Extern the GUID for PPI users.
//
extern EFI_GUID gSiPolicyPpiGuid;
extern EFI_GUID gSaMiscPeiConfigGuid;
extern EFI_GUID gGraphicsPeiConfigGuid;
extern EFI_GUID gSaPciePeiConfigGuid;
extern EFI_GUID gGnaConfigGuid;
extern EFI_GUID gVtdConfigGuid;
extern EFI_GUID gSaOverclockingPreMemConfigGuid;
extern EFI_GUID gSiPreMemPolicyPpiGuid;
extern EFI_GUID gSaMiscPeiPreMemConfigGuid;
extern EFI_GUID gSaPciePeiPreMemConfigGuid;
extern EFI_GUID gGraphicsPeiPreMemConfigGuid;
extern EFI_GUID gIpuPreMemConfigGuid;
extern EFI_GUID gSwitchableGraphicsConfigGuid;
extern EFI_GUID gCpuTraceHubConfigGuid;
extern EFI_GUID gMemoryConfigGuid;
extern EFI_GUID gMemoryConfigNoCrcGuid;

#endif // _SA_POLICY_COMMON_H_
