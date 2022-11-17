/** @file
  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PlatformArch.h>
#include <IndustryStandard/Acpi.h>
#include <Library/AcpiNextLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/HwMemInitLib.h>
#include <Library/OemConfigData.h>
#include <Library/OemMiscLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>

#define CORECOUNT(X) ((X) * CORE_NUM_PER_SOCKET)

#define FIELD_IORT_NODE_OFFSET     40

typedef enum {
  NodeTypeIts = 0,
  NodeTypeNameComponent,
  NodeTypePciRC,
  NodeTypeSmmuV1,
  NodeTypeSmmuV3,
  NodeTypePMCG
} IORT_NODE_TYPE;

#pragma pack(1)
typedef struct {
  UINT8   Type;
  UINT16  Length;
  UINT8   Revision;
  UINT32  Reserved;
  UINT32  IdMapNumber;
  UINT32  IdArrayOffset;
} IORT_NODE_HEAD;
#pragma pack()

BOOLEAN
IsIortWithSmmu (
  IN EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
  )
{
  UINT32           *NodeOffset;
  UINT32           NextOffset;
  IORT_NODE_HEAD   *Node;

  NodeOffset = (UINT32 *)((UINT8 *)TableHeader + FIELD_IORT_NODE_OFFSET);
  NextOffset = *NodeOffset;

  while (NextOffset < TableHeader->Length) {
    Node = (IORT_NODE_HEAD *)((UINT8 *)TableHeader + NextOffset);
    NextOffset += Node->Length;

    if ((Node->Type == NodeTypeSmmuV1) || (Node->Type == NodeTypeSmmuV3)) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
SelectIort (
  IN EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
  )
{
  EFI_STATUS          Status;
  UINTN               Size;
  OEM_CONFIG_DATA     Configuration;

  Configuration.EnableSmmu = 0;
  Size = sizeof (OEM_CONFIG_DATA);
  Status = gRT->GetVariable (
                  OEM_CONFIG_NAME,
                  &gOemConfigGuid,
                  NULL,
                  &Size,
                  &Configuration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get OemConfig variable (%r).\n", Status));
  }

  Status =  EFI_SUCCESS;
  if (IsIortWithSmmu (TableHeader)) {
    if (!Configuration.EnableSmmu) {
      Status = EFI_ABORTED;
    }
  } else {
    if (Configuration.EnableSmmu) {
      Status = EFI_ABORTED;
    }
  }
  DEBUG ((DEBUG_INFO, "SmmuEnable=%x, return %r for Iort table.\n",
          Configuration.EnableSmmu, Status));

  return Status;
}

STATIC
VOID
RemoveUnusedMemoryNode (
  IN OUT EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE  *Table,
  IN     UINTN                        MemoryNodeNum
)
{
  UINTN                   CurrPtr, NewPtr;

  if (MemoryNodeNum >= EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT) {
    return;
  }

  CurrPtr = (UINTN) &(Table->Memory[EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT]);
  NewPtr = (UINTN) &(Table->Memory[MemoryNodeNum]);

  CopyMem ((VOID *)NewPtr, (VOID *)CurrPtr, (UINTN)Table + Table->Header.Header.Length - CurrPtr);

  Table->Header.Header.Length -= CurrPtr - NewPtr;

  return;
}

STATIC
EFI_STATUS
UpdateSrat (
  IN OUT EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *Table
  )
{
  UINT8               Skt = 0;
  UINTN               Index = 0;
  VOID                *HobList;
  GBL_INTERFACE       *Gbl_Data;
  UINTN               Base;
  UINTN               Size;
  UINT8               NodeId;
  UINT32              ScclInterleaveEn;
  UINTN               MemoryNode = 0;

  DEBUG((DEBUG_INFO, "SRAT: Updating SRAT memory information.\n"));

  HobList = GetHobList();
  if (HobList == NULL) {
    return EFI_UNSUPPORTED;
  }
  Gbl_Data = (GBL_INTERFACE*)GetNextGuidHob(&gHisiEfiMemoryMapGuid, HobList);
  if (Gbl_Data == NULL) {
    DEBUG((DEBUG_ERROR, "Get next Guid HOb fail.\n"));
    return EFI_NOT_FOUND;
  }
  Gbl_Data = GET_GUID_HOB_DATA(Gbl_Data);
  for(Skt = 0; Skt < MAX_SOCKET; Skt++) {
    for(Index = 0; Index < MAX_NUM_PER_TYPE; Index++) {
      NodeId = Gbl_Data->NumaInfo[Skt][Index].NodeId;
      Base = Gbl_Data->NumaInfo[Skt][Index].Base;
      Size = Gbl_Data->NumaInfo[Skt][Index].Length;
      DEBUG((DEBUG_INFO, "Skt %d Index %d: NodeId = %d, Base = 0x%lx, Size = 0x%lx\n", Skt, Index, NodeId, Base, Size));
      if (Size > 0) {
        Table->Memory[MemoryNode].ProximityDomain = NodeId;
        Table->Memory[MemoryNode].AddressBaseLow = Base;
        Table->Memory[MemoryNode].AddressBaseHigh = Base >> 32;
        Table->Memory[MemoryNode].LengthLow = Size;
        Table->Memory[MemoryNode].LengthHigh = Size >> 32;
        MemoryNode = MemoryNode + 1;
      }
    }
    ScclInterleaveEn = Gbl_Data->NumaInfo[Skt][0].ScclInterleaveEn;
    DEBUG((DEBUG_INFO, "ScclInterleaveEn = %d\n", ScclInterleaveEn));
    //update gicc structure
    if (ScclInterleaveEn != 0) {
      DEBUG((DEBUG_INFO, "SRAT: Updating SRAT Gicc information.\n"));
      for (Index = CORECOUNT (Skt); Index < CORECOUNT (Skt + 1); Index++) {
        Table->Gicc[Index].ProximityDomain = Skt * NODE_IN_SOCKET;
      }
    }
  }

  //remove invalid memory node
  RemoveUnusedMemoryNode (Table, MemoryNode);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateSlit (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  return  EFI_SUCCESS;
}

EFI_STATUS
UpdateAcpiTable (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  switch (TableHeader->Signature) {

  case EFI_ACPI_6_0_SYSTEM_RESOURCE_AFFINITY_TABLE_SIGNATURE:
    Status = UpdateSrat ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) TableHeader);
    break;

  case EFI_ACPI_6_0_SYSTEM_LOCALITY_INFORMATION_TABLE_SIGNATURE:
    Status = UpdateSlit (TableHeader);
    break;
  case EFI_ACPI_6_2_IO_REMAPPING_TABLE_SIGNATURE:
    Status = SelectIort (TableHeader);
    break;
  }
  return Status;
}
