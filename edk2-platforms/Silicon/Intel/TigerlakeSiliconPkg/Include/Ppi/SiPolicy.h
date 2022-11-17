/** @file
  Silicon Policy PPI is used for specifying platform
  related Intel silicon information and policy setting.
  This PPI is consumed by the silicon PEI modules and carried
  over to silicon DXE modules.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SI_POLICY_PPI_H_
#define _SI_POLICY_PPI_H_

#include <SiPolicyStruct.h>
#include <PchPolicyCommon.h>
#include <PchPreMemPolicyCommon.h>
#include <MePolicyCommon.h>
#include <CpuPolicyCommon.h>
#include <Uefi.h>
#include <Library/ConfigBlockLib.h>

#ifndef DISABLED
#define DISABLED  0
#endif
#ifndef ENABLED
#define ENABLED   1
#endif

extern EFI_GUID gSiPreMemPolicyPpiGuid;
extern EFI_GUID gSiPolicyPpiGuid;


#include <GraphicsConfig.h>
extern EFI_GUID gGraphicsPeiPreMemConfigGuid;
extern EFI_GUID gGraphicsPeiConfigGuid;

#include <VtdConfig.h>
extern EFI_GUID gVtdConfigGuid;

#include <GnaConfig.h>
extern EFI_GUID gGnaConfigGuid;

#include <CpuPcieConfig.h>
extern EFI_GUID gCpuPciePeiPreMemConfigGuid;
extern EFI_GUID gCpuPcieRpConfigGuid;

#include <HybridGraphicsConfig.h>
extern EFI_GUID gHybridGraphicsConfigGuid;

#include <ConfigBlock/PramPreMemConfig.h>
#include <MemoryConfig.h>
extern EFI_GUID gMemoryConfigGuid;
extern EFI_GUID gMemoryConfigNoCrcGuid;

#include <ConfigBlock/SaMiscPeiPreMemConfig.h>
extern EFI_GUID gSaMiscPeiPreMemConfigGuid;

#include <ConfigBlock/SaMiscPeiConfig.h>
extern EFI_GUID gSaMiscPeiConfigGuid;


#include <TraceHubConfig.h>
extern EFI_GUID gCpuTraceHubConfigGuid;

#include <HostBridgeConfig.h>
extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

#include <CpuDmiPreMemConfig.h>
extern EFI_GUID gCpuDmiPreMemConfigGuid;

typedef struct _SI_PREMEM_POLICY_STRUCT SI_PREMEM_POLICY_PPI;
typedef struct _SI_POLICY_STRUCT SI_POLICY_PPI;

#endif // _SI_POLICY_PPI_H_
