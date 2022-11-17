/** @file

  Copyright (c) 2014 - 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/ArmLib.h>
#include <Guid/ArmMpCoreInfo.h>

#include <Ppi/IscpPpi.h>
#include <Iscp.h>

/*----------------------------------------------------------------------------------------
 *                                  G L O B A L S
 *----------------------------------------------------------------------------------------
 */
//
// CoreInfo table
//
STATIC ARM_CORE_INFO mAmdMpCoreInfoTable[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,
  },
  {
    // Cluster 0, Core 1
    0x0, 0x1,
  },
  {
    // Cluster 1, Core 0
    0x1, 0x0,
  },
  {
    // Cluster 1, Core 1
    0x1, 0x1,
  },
  {
    // Cluster 2, Core 0
    0x2, 0x0,
  },
  {
    // Cluster 2, Core 1
    0x2, 0x1,
  },
  {
    // Cluster 3, Core 0
    0x3, 0x0,
  },
  {
    // Cluster 3, Core 1
    0x3, 0x1,
  }
};

//
// Core count
//
STATIC UINTN mAmdCoreCount = sizeof (mAmdMpCoreInfoTable) / sizeof (ARM_CORE_INFO);


/*----------------------------------------------------------------------------------------
 *                                   P P I   L I S T
 *----------------------------------------------------------------------------------------
 */
STATIC EFI_PEI_ISCP_PPI            *PeiIscpPpi;


/*----------------------------------------------------------------------------------------
 *                            P P I   D E S C R I P T O R
 *----------------------------------------------------------------------------------------
 */
STATIC EFI_PEI_PPI_DESCRIPTOR mPlatInitPpiDescriptor =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdStyxPlatInitPpiGuid,
  NULL
};


/**
 *---------------------------------------------------------------------------------------
 *  PlatInitPeiEntryPoint
 *
 *  Description:
 *    Entry point of the PlatInit PEI module.
 *
 *  Control flow:
 *    Query platform parameters via ISCP.
 *
 *  Parameters:
 *    @param[in]      FfsHeader            EFI_PEI_FILE_HANDLE
 *    @param[in]      **PeiServices        Pointer to the PEI Services Table.
 *
 *    @return         EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
PlatInitPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE      FfsHeader,
  IN       CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  EFI_STATUS                  Status;
  AMD_MEMORY_RANGE_DESCRIPTOR IscpMemDescriptor = {0};
  ISCP_FUSE_INFO              IscpFuseInfo = {0};
  ISCP_CPU_RESET_INFO         CpuResetInfo = {0};
  ISCP_MAC_INFO               MacAddrInfo = {0};
  UINTN                       MacSize;
  UINTN                       CpuCoreCount, CpuMap, CpuMapSize;
  UINTN                       Index, CoreNum;
  UINT32                      *CpuIdReg = (UINT32 *)FixedPcdGet32 (PcdCpuIdRegister);

  DEBUG ((EFI_D_ERROR, "PlatInit PEIM Loaded\n"));

  // CPUID
  Status = PcdSet32S (PcdSocCpuId, *CpuIdReg);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_ERROR, "SocCpuId = 0x%X\n", PcdGet32 (PcdSocCpuId)));

  // Update core count based on PCD option
  if (mAmdCoreCount > PcdGet32 (PcdSocCoreCount)) {
    mAmdCoreCount = PcdGet32 (PcdSocCoreCount);
  }

  Status = PeiServicesLocatePpi (&gPeiIscpPpiGuid, 0, NULL, (VOID**)&PeiIscpPpi);
  ASSERT_EFI_ERROR (Status);

  // Get fuse information from ISCP
  Status = PeiIscpPpi->ExecuteFuseTransaction (PeiServices, &IscpFuseInfo);
  ASSERT_EFI_ERROR (Status);

  CpuMap = IscpFuseInfo.SocConfiguration.CpuMap;
  CpuCoreCount = IscpFuseInfo.SocConfiguration.CpuCoreCount;
  CpuMapSize = sizeof (IscpFuseInfo.SocConfiguration.CpuMap) * 8;

  ASSERT (CpuMap != 0);
  ASSERT (CpuCoreCount != 0);
  ASSERT (CpuCoreCount <= CpuMapSize);

  // Update core count based on fusing
  if (mAmdCoreCount > CpuCoreCount) {
    mAmdCoreCount = CpuCoreCount;
  }

  //
  // Update per-core information from ISCP
  // Walk CPU map to enumerate active cores
  //
  for (CoreNum = 0, Index = 0; CoreNum < CpuMapSize && Index < mAmdCoreCount; ++CoreNum) {
    if (CpuMap & 1) {
      CpuResetInfo.CoreNum = CoreNum;
      Status = PeiIscpPpi->ExecuteCpuRetrieveIdTransaction (
                 PeiServices, &CpuResetInfo );
      ASSERT_EFI_ERROR (Status);
      ASSERT (CpuResetInfo.CoreStatus.Status != CPU_CORE_DISABLED);
      ASSERT (CpuResetInfo.CoreStatus.Status != CPU_CORE_UNDEFINED);

      mAmdMpCoreInfoTable[Index].ClusterId = CpuResetInfo.CoreStatus.ClusterId;
      mAmdMpCoreInfoTable[Index].CoreId = CpuResetInfo.CoreStatus.CoreId;

      DEBUG ((EFI_D_ERROR, "Core[%d]: ClusterId = %d   CoreId = %d\n",
        Index, mAmdMpCoreInfoTable[Index].ClusterId,
        mAmdMpCoreInfoTable[Index].CoreId));

      // Next core in Table
      ++Index;
    }
    // Next core in Map
    CpuMap >>= 1;
  }

  // Update core count based on CPU map
  if (mAmdCoreCount > Index) {
    mAmdCoreCount = Index;
  }

  // Update SocCoreCount on Dynamic PCD
  if (PcdGet32 (PcdSocCoreCount) != mAmdCoreCount) {
    Status = PcdSet32S (PcdSocCoreCount, mAmdCoreCount);
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((EFI_D_ERROR, "SocCoreCount = %d\n", PcdGet32 (PcdSocCoreCount)));

  // Build AmdMpCoreInfo HOB
  BuildGuidDataHob (&gAmdStyxMpCoreInfoGuid, mAmdMpCoreInfoTable, sizeof (ARM_CORE_INFO) * mAmdCoreCount);

  // Get SystemMemorySize from ISCP
  IscpMemDescriptor.Size0 = 0;
  Status = PeiIscpPpi->ExecuteMemoryTransaction (PeiServices, &IscpMemDescriptor);
  ASSERT_EFI_ERROR (Status);

  // Update SystemMemorySize on Dynamic PCD
  if (IscpMemDescriptor.Size0) {
    Status = PcdSet64S (PcdSystemMemorySize, IscpMemDescriptor.Size0);
    ASSERT_EFI_ERROR (Status);
  }
  if (IscpMemDescriptor.Size0 == 0) {
    DEBUG ((EFI_D_ERROR, "Warning: Could not get SystemMemorySize via ISCP, using default value.\n"));
  }

  DEBUG ((EFI_D_ERROR, "SystemMemorySize = %ld\n", PcdGet64 (PcdSystemMemorySize)));


  if (FixedPcdGetBool (PcdXgbeEnable)) {
    // Get MAC Address from ISCP
    Status = PeiIscpPpi->ExecuteGetMacAddressTransaction (
               PeiServices, &MacAddrInfo );
    ASSERT_EFI_ERROR (Status);

    // Check for bogus MAC addresses that have the multicast bit set. This
    // includes FF:FF:FF:FF:FF:FF, which is what you get from the SCP on
    // some versions of the B0 Overdrive
    MacSize = sizeof(MacAddrInfo.MacAddress0);
    if ((MacAddrInfo.MacAddress0[0] & 0x1) == 0x0) {
      Status = PcdSetPtrS (PcdEthMacA, &MacSize, MacAddrInfo.MacAddress0);
      ASSERT_EFI_ERROR (Status);
    }
    if ((MacAddrInfo.MacAddress1[0] & 0x1) == 0x0) {
      Status = PcdSetPtrS (PcdEthMacB, &MacSize, MacAddrInfo.MacAddress1);
      ASSERT_EFI_ERROR (Status);
    }

    DEBUG ((EFI_D_ERROR, "EthMacA = %02x:%02x:%02x:%02x:%02x:%02x\n",
      ((UINT8 *)PcdGetPtr (PcdEthMacA))[0], ((UINT8 *)PcdGetPtr (PcdEthMacA))[1],
      ((UINT8 *)PcdGetPtr (PcdEthMacA))[2], ((UINT8 *)PcdGetPtr (PcdEthMacA))[3],
      ((UINT8 *)PcdGetPtr (PcdEthMacA))[4], ((UINT8 *)PcdGetPtr (PcdEthMacA))[5]));
    DEBUG ((EFI_D_ERROR, "EthMacB = %02x:%02x:%02x:%02x:%02x:%02x\n",
      ((UINT8 *)PcdGetPtr (PcdEthMacB))[0], ((UINT8 *)PcdGetPtr (PcdEthMacB))[1],
      ((UINT8 *)PcdGetPtr (PcdEthMacB))[2], ((UINT8 *)PcdGetPtr (PcdEthMacB))[3],
      ((UINT8 *)PcdGetPtr (PcdEthMacB))[4], ((UINT8 *)PcdGetPtr (PcdEthMacB))[5]));
  }

  // Let other PEI modules know we're done!
  Status = (*PeiServices)->InstallPpi (PeiServices, &mPlatInitPpiDescriptor);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

