/** @file
*
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>

#include <Guid/ArmMpCoreInfo.h>

#include <libfdt.h>
#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/AmdMpCoreInfo.h>

#include <SocVersion.h>

#define PMU_INT_FLAG_SPI        0
#define PMU_INT_TYPE_HIGH_LEVEL 4

#define NUM_CORES               FixedPcdGet32 (PcdCoreCount)

//
// PMU interrupts per core
//
#pragma pack(push, 1)
typedef struct {
  UINT32 Flag;          // 0 == SPI
  UINT32 IntId;         // GSIV == IntId+32
  UINT32 Type;          // 4 == Level-Sensitive, Active-High
} PMU_INTERRUPT;
#pragma pack(pop)

STATIC
BOOLEAN
ClusterInRange (
  IN ARM_CORE_INFO  *ArmCoreInfoTable,
  IN UINTN          ClusterId,
  IN UINTN          LowIndex,
  IN UINTN          HighIndex
  )
{
  do {
    if (ClusterId == ArmCoreInfoTable[LowIndex].ClusterId)
      return TRUE;
  } while (++LowIndex <= HighIndex);

  return FALSE;
}


STATIC
UINTN
NumberOfCoresInCluster (
  IN ARM_CORE_INFO  *ArmCoreInfoTable,
  IN UINTN          NumberOfEntries,
  IN UINTN          ClusterId
  )
{
  UINTN Index, Cores;

  Cores = 0;
  for (Index = 0; Index < NumberOfEntries; ++Index) {
    if (ClusterId == ArmCoreInfoTable[Index].ClusterId)
      ++Cores;
  }

  return Cores;
}


STATIC
UINTN
NumberOfClustersInTable (
  IN ARM_CORE_INFO  *ArmCoreInfoTable,
  IN UINTN          NumberOfEntries
  )
{
  UINTN Index, Cores, Clusters, ClusterId;

  Index = 0;
  Clusters = 0;
  Cores = NumberOfEntries;
  while (Cores) {
     ++Clusters;
     ClusterId = ArmCoreInfoTable[Index].ClusterId;
     Cores -= NumberOfCoresInCluster (ArmCoreInfoTable,
                                      NumberOfEntries,
                                      ClusterId);
     if (Cores) {
       do {
         ++Index;
       } while (ClusterInRange (ArmCoreInfoTable,
                                ArmCoreInfoTable[Index].ClusterId,
                                0, Index-1));
     }
  }

  return Clusters;
}


STATIC
INT32
fdt_alloc_phandle (
  IN VOID     *Fdt
  )
{
  INT32    Offset;
  INT32    Phandle;

  Phandle = 0;

  for (Offset = fdt_next_node (Fdt, -1, NULL); Offset >= 0;
       Offset = fdt_next_node (Fdt, Offset, NULL)) {
       Phandle = MAX (Phandle, fdt_get_phandle (Fdt, Offset));
  }

  return Phandle + 1;
}

STATIC
VOID
SetDeviceStatus (
  IN VOID           *Fdt,
  IN CONST CHAR8    *Device,
  IN BOOLEAN        Enable
  )
{
  INT32     Node;
  INT32     SubNode;
  INT32     Rc;

  Node = fdt_subnode_offset (Fdt, 0, "smb");
  if (Node >= 0) {
    SubNode = fdt_subnode_offset (Fdt, Node, Device);
    if (SubNode >= 0) {
      Rc = fdt_setprop_string (Fdt, SubNode, "status",
             Enable ? "okay" : "disabled");
      if (Rc) {
        DEBUG ((DEBUG_ERROR,
          "%a: Could not set 'status' property for '%a' node\n",
          __FUNCTION__, Device));
      }
    }
  }
}

#define MAC_ADDRESS_BYTES       6

STATIC
VOID
SetMacAddress (
  IN VOID           *Fdt,
  IN CONST CHAR8    *Device,
  IN UINT8          *MacAddress
  )
{
  INT32     Node;
  INT32     SubNode;
  INT32     Rc;

  Node = fdt_subnode_offset (Fdt, 0, "smb");
  if (Node >= 0) {
    SubNode = fdt_subnode_offset (Fdt, Node, Device);
    if (SubNode >= 0) {
      Rc = fdt_setprop (Fdt, SubNode, "mac-address", MacAddress,
             MAC_ADDRESS_BYTES);
      if (Rc) {
        DEBUG ((DEBUG_ERROR,
          "%a: Could not set 'mac-address' property for '%a' node\n",
          __FUNCTION__, Device));
      }
    }
  }
}

STATIC
VOID
DisableSmmu (
  IN  VOID          *Fdt,
  IN  CONST CHAR8   *IommuPropName,
  IN  CONST CHAR8   *SmmuNodeName,
  IN  CONST CHAR8   *DeviceNodeName
  )
{
  INT32   Node;
  INT32   Error;

  Node = fdt_path_offset (Fdt, DeviceNodeName);
  if (Node <= 0) {
    DEBUG ((DEBUG_WARN, "%a: Failed to find path %s: %a\n",
      __FUNCTION__, DeviceNodeName, fdt_strerror (Node)));
    return;
  }

  Error = fdt_delprop (Fdt, Node, IommuPropName);
  if (Error != 0) {
    DEBUG ((DEBUG_WARN, "%a: Failed to delete property %a: %a\n",
      __FUNCTION__, IommuPropName, fdt_strerror (Error)));
    return;
  }

  Node = fdt_path_offset (Fdt, SmmuNodeName);
  if (Node <= 0) {
    return;
  }

  Error = fdt_del_node (Fdt, Node);
  if (Error != 0) {
    DEBUG ((DEBUG_WARN, "%a: Failed to delete node %a: %a\n",
      __FUNCTION__, SmmuNodeName, fdt_strerror (Error)));
  }
}

STATIC
VOID
SetSocIdStatus (
  IN VOID       *Fdt
  )
{
  UINT32        SocId;
  BOOLEAN       IsRevB1;

  SocId = PcdGet32 (PcdSocCpuId);
  IsRevB1 = (SocId & STYX_SOC_VERSION_MASK) >= STYX_SOC_VERSION_B1;

  SetDeviceStatus (Fdt, "sata@e0d00000",
    IsRevB1 && FixedPcdGet8 (PcdSata1PortCount) > 0);
  SetDeviceStatus (Fdt, "gpio@e0020000", IsRevB1);
  SetDeviceStatus (Fdt, "gpio@e0030000", IsRevB1);
  SetDeviceStatus (Fdt, "gwdt@e0bb0000", IsRevB1);

  if (FixedPcdGetBool (PcdEnableKcs)) {
    SetDeviceStatus (Fdt, "kcs@e0010000", IsRevB1);
  } else {
    SetDeviceStatus (Fdt, "kcs@e0010000", FALSE);
  }

  if (!PcdGetBool (PcdEnableSmmus)) {
    DisableSmmu (Fdt, "iommu-map", "/smb/smmu@e0a00000", "/smb/pcie@f0000000");
    DisableSmmu (Fdt, "iommus", "/smb/smmu@e0200000", "/smb/sata@e0300000");
    DisableSmmu (Fdt, "iommus", "/smb/smmu@e0c00000", "/smb/ccp@e0100000");
    DisableSmmu (Fdt, "iommus", "/smb/smmu@e0c00000", "/smb/sata@e0d00000");
  }

  if (!FixedPcdGetBool (PcdXgbeEnable) || !PcdGetBool (PcdEnableSmmus)) {
    DisableSmmu (Fdt, "iommus", "/smb/smmu@e0600000", "/smb/xgmac@e0700000");
    DisableSmmu (Fdt, "iommus", "/smb/smmu@e0800000", "/smb/xgmac@e0900000");
  }
}

STATIC
VOID
SetXgbeStatus (
  IN VOID       *Fdt
  )
{
  if (FixedPcdGetBool (PcdXgbeEnable)) {
    SetDeviceStatus (Fdt, "xgmac@e0700000", TRUE);
    SetDeviceStatus (Fdt, "phy@e1240800", TRUE);
    SetDeviceStatus (Fdt, "xgmac@e0900000", TRUE);
    SetDeviceStatus (Fdt, "phy@e1240c00", TRUE);

    SetMacAddress (Fdt, "xgmac@e0700000", PcdGetPtr (PcdEthMacA));
    SetMacAddress (Fdt, "xgmac@e0900000", PcdGetPtr (PcdEthMacB));
  } else {
    SetDeviceStatus (Fdt, "xgmac@e0700000", FALSE);
    SetDeviceStatus (Fdt, "phy@e1240800", FALSE);
    SetDeviceStatus (Fdt, "xgmac@e0900000", FALSE);
    SetDeviceStatus (Fdt, "phy@e1240c00", FALSE);
  }
}

STATIC CONST CHAR8 mCpuCompatible[] = "arm,cortex-a57\0arm,armv8";
STATIC CONST CHAR8 mPmuCompatible[] = "arm,cortex-a57-pmu\0arm,armv8-pmuv3";

STATIC
EFI_STATUS
PrepareFdt (
  IN OUT VOID                 *Fdt,
  IN     UINTN                FdtSize
  )
{
  EFI_STATUS                  Status;
  INT32                       Node;
  INT32                       CpuNode;
  UINTN                       Index;
  ARM_CORE_INFO               *ArmCoreInfoTable;
  UINTN                       ArmCoreCount;
  INT32                       MapNode;
  INT32                       ClusterNode;
  INT32                       PmuNode;
  PMU_INTERRUPT               PmuInt;
  INT32                       Phandle[NUM_CORES];
  UINT32                      ClusterIndex;
  UINT32                      CoreIndex;
  UINT32                      ClusterCount;
  UINT32                      CoresInCluster;
  UINT32                      ClusterId;
  INT32                       L2Node;
  INT32                       L3Node;
  INT32                       L2Phandle[NUM_CORES / 2];
  INT32                       L3Phandle;
  UINTN                       MpId;
  CHAR8                       Name[10];
  AMD_MP_CORE_INFO_PROTOCOL   *AmdMpCoreInfoProtocol;

  //
  // Setup Arm Mpcore Info if it is a multi-core or multi-cluster platforms.
  //
  // For 'cpus' and 'cpu' device tree nodes bindings, refer to this file
  // in the kernel documentation:
  // Documentation/devicetree/bindings/arm/cpus.txt
  //
  Status = gBS->LocateProtocol (
                  &gAmdMpCoreInfoProtocolGuid,
                  NULL,
                  (VOID **)&AmdMpCoreInfoProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  // Get pointer to ARM core info table
  ArmCoreInfoTable = AmdMpCoreInfoProtocol->GetArmCoreInfoTable (&ArmCoreCount);
  ASSERT (ArmCoreInfoTable != NULL);
  ASSERT (ArmCoreCount <= NUM_CORES);

  // Create the L3 cache node
  L3Node = fdt_add_subnode (Fdt, 0, "l3cache");
  if (L3Node < 0) {
    DEBUG ((DEBUG_ERROR, "FDT: Error creating 'l3cache' node\n"));
    return EFI_INVALID_PARAMETER;
  }

  L3Phandle = fdt_alloc_phandle (Fdt);
  fdt_setprop_cell (Fdt, L3Node, "cache-level", 3);
  fdt_setprop_cell (Fdt, L3Node, "cache-size", SIZE_8MB);
  fdt_setprop_cell (Fdt, L3Node, "cache-line-size", 64);
  fdt_setprop_cell (Fdt, L3Node, "cache-sets", 8192);
  fdt_setprop_empty (Fdt, L3Node, "cache-unified");
  fdt_setprop_cell (Fdt, L3Node, "phandle", L3Phandle);

  ClusterCount = NumberOfClustersInTable (ArmCoreInfoTable, ArmCoreCount);
  ASSERT (ClusterCount <= ARRAY_SIZE (L2Phandle));

  for (Index = 0; Index < ClusterCount; Index++) {
    AsciiSPrint (Name, sizeof (Name), "l2cache%d", Index);

    L2Node = fdt_add_subnode (Fdt, 0, Name);
    if (L2Node < 0) {
      DEBUG ((DEBUG_ERROR, "FDT: Error creating '%a' node\n", Name));
      return EFI_INVALID_PARAMETER;
    }

    L2Phandle[Index] = fdt_alloc_phandle (Fdt);
    fdt_setprop_cell (Fdt, L2Node, "cache-size", SIZE_1MB);
    fdt_setprop_cell (Fdt, L2Node, "cache-line-size", 64);
    fdt_setprop_cell (Fdt, L2Node, "cache-sets", 1024);
    fdt_setprop_empty (Fdt, L2Node, "cache-unified");
    fdt_setprop_cell (Fdt, L2Node, "next-level-cache", L3Phandle);
    fdt_setprop_cell (Fdt, L2Node, "phandle", L2Phandle[Index]);
  }

  // Get Id from primary CPU
  MpId = (UINTN)ArmReadMpidr ();

  // Create /cpus noide
  Node = fdt_add_subnode (Fdt, 0, "cpus");
  if (Node >= 0) {
    // Configure the 'cpus' node
    fdt_setprop_string (Fdt, Node, "name", "cpus");
    fdt_setprop_cell (Fdt, Node, "#address-cells", sizeof (UINTN) / 4);
    fdt_setprop_cell (Fdt, Node, "#size-cells", 0);
  } else {
    DEBUG ((DEBUG_ERROR, "FDT: Error creating 'cpus' node\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Walk the processor table in reverse order for proper listing in FDT
  //
  Index = ArmCoreCount;
  while (Index--) {
    // Create 'cpu' node
    AsciiSPrint (Name, sizeof (Name), "CPU%d", Index);
    CpuNode = fdt_add_subnode (Fdt, Node, Name);
    if (CpuNode < 0) {
      DEBUG ((DEBUG_ERROR, "FDT: Error on creating '%a' node\n", Name));
      return EFI_INVALID_PARAMETER;
    }
    Phandle[Index] = fdt_alloc_phandle (Fdt);
    fdt_setprop_cell (Fdt, CpuNode, "phandle", Phandle[Index]);

    fdt_setprop_string (Fdt, CpuNode, "enable-method", "psci");

    MpId = (UINTN)GET_MPID (ArmCoreInfoTable[Index].ClusterId,
                            ArmCoreInfoTable[Index].CoreId);
    MpId = cpu_to_fdt64 (MpId);
    fdt_setprop (Fdt, CpuNode, "reg", &MpId, sizeof (MpId));
    fdt_setprop (Fdt, CpuNode, "compatible", mCpuCompatible,
      sizeof (mCpuCompatible));
    fdt_setprop_string (Fdt, CpuNode, "device_type", "cpu");

    fdt_setprop_cell (Fdt, CpuNode, "i-cache-size", 3 * SIZE_16KB);
    fdt_setprop_cell (Fdt, CpuNode, "i-cache-line-size", 64);
    fdt_setprop_cell (Fdt, CpuNode, "i-cache-sets", 256);
    fdt_setprop_cell (Fdt, CpuNode, "d-cache-size", 2 * SIZE_16KB);
    fdt_setprop_cell (Fdt, CpuNode, "d-cache-line-size", 64);
    fdt_setprop_cell (Fdt, CpuNode, "d-cache-sets", 256);
    fdt_setprop_cell (Fdt, CpuNode, "l2-cache",
      L2Phandle[ArmCoreInfoTable[Index].ClusterId]);
  }

  // Create /cpu-map node
  MapNode = fdt_add_subnode (Fdt, Node, "cpu-map");
  if (MapNode >= 0) {
    ClusterIndex = ArmCoreCount - 1;
    ClusterCount = NumberOfClustersInTable (ArmCoreInfoTable,
                                            ArmCoreCount);
    while (ClusterCount--) {
      // Create 'cluster' node
      AsciiSPrint (Name, sizeof (Name), "cluster%d", ClusterCount);
      ClusterNode = fdt_add_subnode (Fdt, MapNode, Name);
      if (ClusterNode < 0) {
        DEBUG ((DEBUG_ERROR, "FDT: Error creating '%a' node\n", Name));
        return EFI_INVALID_PARAMETER;
      }

      ClusterId = ArmCoreInfoTable[ClusterIndex].ClusterId;
      CoreIndex = ClusterIndex;
      CoresInCluster = NumberOfCoresInCluster (ArmCoreInfoTable,
                                               ArmCoreCount,
                                               ClusterId);
      while (CoresInCluster--) {
        // Create 'core' node
        AsciiSPrint (Name, sizeof (Name), "core%d", CoresInCluster);
        CpuNode = fdt_add_subnode (Fdt, ClusterNode, Name);
        if (CpuNode < 0) {
          DEBUG ((DEBUG_ERROR, "FDT: Error creating '%a' node\n", Name));
          return EFI_INVALID_PARAMETER;
        }
        fdt_setprop_cell (Fdt, CpuNode, "cpu", Phandle[CoreIndex]);

        // iterate to next core in cluster
        if (CoresInCluster) {
          do {
             --CoreIndex;
          } while (ClusterId != ArmCoreInfoTable[CoreIndex].ClusterId);
        }
      }

      // iterate to next cluster
      if (ClusterCount) {
        do {
           --ClusterIndex;
        } while (ClusterInRange (ArmCoreInfoTable,
                                 ArmCoreInfoTable[ClusterIndex].ClusterId,
                                 ClusterIndex + 1,
                                 ArmCoreCount - 1));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR,"FDT: Error creating 'cpu-map' node\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Create /pmu node
  PmuNode = fdt_add_subnode(Fdt, 0, "pmu");
  if (PmuNode >= 0) {
    fdt_setprop (Fdt, PmuNode, "compatible", mPmuCompatible,
      sizeof (mPmuCompatible));

    // append PMU interrupts
    for (Index = 0; Index < ArmCoreCount; Index++) {
      MpId = (UINTN)GET_MPID (ArmCoreInfoTable[Index].ClusterId,
                              ArmCoreInfoTable[Index].CoreId);

      Status = AmdMpCoreInfoProtocol->GetPmuSpiFromMpId (MpId, &PmuInt.IntId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR,
          "FDT: Error getting PMU interrupt for MpId '0x%x'\n", MpId));
        return Status;
      }

      PmuInt.Flag = cpu_to_fdt32 (PMU_INT_FLAG_SPI);
      PmuInt.IntId = cpu_to_fdt32 (PmuInt.IntId);
      PmuInt.Type = cpu_to_fdt32 (PMU_INT_TYPE_HIGH_LEVEL);
      fdt_appendprop (Fdt, PmuNode, "interrupts", &PmuInt, sizeof(PmuInt));
      fdt_appendprop_cell (Fdt, PmuNode, "interrupt-affinity", Phandle[Index]);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "FDT: Error creating 'pmu' node\n"));
    return EFI_INVALID_PARAMETER;
  }

  SetSocIdStatus (Fdt);
  SetXgbeStatus (Fdt);

  // Update the real size of the Device Tree
  fdt_pack (Fdt);

  return EFI_SUCCESS;
}


/**
  Return a pool allocated copy of the DTB image that is appropriate for
  booting the current platform via DT.

  @param[out]   Dtb                   Pointer to the DTB copy
  @param[out]   DtbSize               Size of the DTB copy

  @retval       EFI_SUCCESS           Operation completed successfully
  @retval       EFI_NOT_FOUND         No suitable DTB image could be located
  @retval       EFI_OUT_OF_RESOURCES  No pool memory available

**/
EFI_STATUS
EFIAPI
DtPlatformLoadDtb (
  OUT   VOID        **Dtb,
  OUT   UINTN       *DtbSize
  )
{
  EFI_STATUS      Status;
  VOID            *OrigDtb;
  VOID            *CopyDtb;
  UINTN           OrigDtbSize;
  UINTN           CopyDtbSize;
  INT32           Error;

  Status = GetSectionFromAnyFv (&gDtPlatformDefaultDtbFileGuid,
             EFI_SECTION_RAW, 0, &OrigDtb, &OrigDtbSize);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Allocate space for the DTB: add a page of slack space to make some room
  // for our modifications.
  //
  CopyDtbSize = OrigDtbSize + EFI_PAGE_SIZE;
  CopyDtb = AllocatePool (CopyDtbSize);
  if (CopyDtb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Error = fdt_open_into (OrigDtb, CopyDtb, CopyDtbSize);
  if (Error != 0) {
    //
    // fdt_open_into() validates the DTB header, so if it fails, the template
    // is most likely invalid.
    //
    return EFI_NOT_FOUND;
  }

  Status = PrepareFdt (CopyDtb, CopyDtbSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Dtb = CopyDtb;
  *DtbSize = CopyDtbSize;

  return EFI_SUCCESS;
}
