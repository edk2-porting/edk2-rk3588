/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under Platform/ARM/JunoPkg/AcpiTables/
*
**/

#include "Pptt.h"

typedef EFI_ACPI_5_1_GIC_STRUCTURE                          ACPI_GIC_STRUCTURE;
typedef EFI_ACPI_5_1_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER ACPI_MADT_TABLE_HEADER;

EFI_ACPI_TABLE_PROTOCOL       *mAcpiTableProtocol = NULL;
EFI_ACPI_SDT_PROTOCOL         *mAcpiSdtProtocol   = NULL;

EFI_ACPI_DESCRIPTION_HEADER mPpttHeader =
  ARM_ACPI_HEADER (
    EFI_ACPI_6_2_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_STRUCTURE_SIGNATURE,
    EFI_ACPI_DESCRIPTION_HEADER,
    EFI_ACPI_6_2_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_REVISION
  );

EFI_ACPI_6_2_PPTT_STRUCTURE_ID mPpttSocketType2[PPTT_SOCKET_COMPONENT_NO] =
{
  {2, sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_ID), {0, 0}, PPTT_VENDOR_ID, 0, 0, 0, 0, 0}
};

EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE mPpttCacheType1[PPTT_CACHE_NO];

STATIC UINT32 mSocketOffset[MAX_SOCKET];
STATIC UINT32 mScclOffset[MAX_SCL];
STATIC UINT32 mClusterOffset[MAX_SCL][MAX_CLUSTER_PER_SCL];

STATIC
VOID
InitCacheInfo (
  VOID
  )
{
  UINT8                                        Index;
  EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE_ATTRIBUTES Type1Attributes;
  CSSELR_DATA                                  CsselrData;
  CCSIDR_DATA                                  CcsidrData;

  for (Index = 0; Index < PPTT_CACHE_NO; Index++) {
    CsselrData.Data = 0;
    CcsidrData.Data = 0;
    SetMem (
      &Type1Attributes,
      sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE_ATTRIBUTES),
      0
      );

    if (Index == 0) { //L1I
      CsselrData.Bits.InD = 1;
      CsselrData.Bits.Level = 0;
      Type1Attributes.CacheType  = 1;
    } else if (Index == 1) {
      Type1Attributes.CacheType  = 0;
      CsselrData.Bits.Level = Index - 1;
    } else {
      Type1Attributes.CacheType  = 2;
      CsselrData.Bits.Level = Index - 1;
    }

    CcsidrData.Data = ReadCCSIDR (CsselrData.Data);

    if (CcsidrData.Bits.Wa == 1) {
      Type1Attributes.AllocationType = EFI_ACPI_6_2_CACHE_ATTRIBUTES_ALLOCATION_WRITE;
      if (CcsidrData.Bits.Ra == 1) {
        Type1Attributes.AllocationType = EFI_ACPI_6_2_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE;
      }
    }

    if (CcsidrData.Bits.Wt == 1) {
      Type1Attributes.WritePolicy = 1;
    }
    DEBUG ((DEBUG_INFO,
            "[Acpi PPTT] Level = %x!CcsidrData = %x!\n",
            CsselrData.Bits.Level,
            CcsidrData.Data));

    mPpttCacheType1[Index].Type = EFI_ACPI_6_2_PPTT_TYPE_CACHE;
    mPpttCacheType1[Index].Length = sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE);
    mPpttCacheType1[Index].Reserved[0] = 0;
    mPpttCacheType1[Index].Reserved[1] = 0;
    mPpttCacheType1[Index].Flags.SizePropertyValid = 1;
    mPpttCacheType1[Index].Flags.NumberOfSetsValid = 1;
    mPpttCacheType1[Index].Flags.AssociativityValid = 1;
    mPpttCacheType1[Index].Flags.AllocationTypeValid = 1;
    mPpttCacheType1[Index].Flags.CacheTypeValid = 1;
    mPpttCacheType1[Index].Flags.WritePolicyValid = 1;
    mPpttCacheType1[Index].Flags.LineSizeValid = 1;
    mPpttCacheType1[Index].Flags.Reserved = 0;
    mPpttCacheType1[Index].NextLevelOfCache = 0;

    if (Index != PPTT_CACHE_NO - 1) {
      mPpttCacheType1[Index].NumberOfSets = (UINT16)CcsidrData.Bits.NumSets + 1;
      mPpttCacheType1[Index].Associativity = (UINT16)CcsidrData.Bits.Associativity + 1;
      mPpttCacheType1[Index].LineSize = (UINT16)( 1 << (CcsidrData.Bits.LineSize + 4));
      mPpttCacheType1[Index].Size = mPpttCacheType1[Index].LineSize *      \
                                    mPpttCacheType1[Index].Associativity * \
                                    mPpttCacheType1[Index].NumberOfSets;
      CopyMem (
        &mPpttCacheType1[Index].Attributes,
        &Type1Attributes,
        sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE_ATTRIBUTES)
        );
    } else {
      // L3 cache
      mPpttCacheType1[Index].Size = 0x2000000;       // 32MB
      mPpttCacheType1[Index].NumberOfSets = 0x800;
      mPpttCacheType1[Index].Associativity = 0x0F;   // CacheAssociativity16Way
      SetMem (
        &mPpttCacheType1[Index].Attributes,
        sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE_ATTRIBUTES),
        0x0A
        );
      mPpttCacheType1[Index].LineSize = 0x80;        // 128byte
    }
  }
}

STATIC
EFI_STATUS
AddCoreTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *PpttTable,
  IN OUT UINT32                      *PpttTableLengthRemain,
  IN     UINT32                      Parent,
  IN     UINT32                      ResourceNo,
  IN     UINT32                      ProcessorId
  )
{
  EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *PpttType0;
  EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE     *PpttType1;
  UINT32                                *PrivateResource;
  UINT8                                 Index;
  UINT32                                NextLevelCacheOffset;

  if (*PpttTableLengthRemain <
      (sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) + ResourceNo * 4)) {
    return EFI_OUT_OF_RESOURCES;
  }
  PpttType0 = (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *)((UINT8 *)PpttTable +
                                                        PpttTable->Length);
  PpttType0->Type = 0;
  SetMem (&PpttType0->Flags, sizeof (PpttType0->Flags), 0);
  PpttType0->Flags.AcpiProcessorIdValid = EFI_ACPI_6_2_PPTT_PROCESSOR_ID_VALID;
  PpttType0->Parent= Parent;
  PpttType0->AcpiProcessorId = ProcessorId;
  PpttType0->NumberOfPrivateResources = ResourceNo;
  PpttType0->Length = sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) +
                      ResourceNo * 4;

  *PpttTableLengthRemain  -= (UINTN)PpttType0->Length;
  PpttTable->Length += PpttType0->Length;
  PrivateResource = (UINT32 *)((UINT8 *)PpttType0 +
                               sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR));

  // Add cache type structure
  for (Index = 0; Index < ResourceNo; Index++, PrivateResource++) {
    if (*PpttTableLengthRemain < sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE)) {
      return EFI_OUT_OF_RESOURCES;
    }
    *PrivateResource = PpttTable->Length;
    PpttType1 = (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE *)((UINT8 *)PpttTable +
                                                      PpttTable->Length);
    gBS->CopyMem (
           PpttType1,
           &mPpttCacheType1[Index],
           sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE)
           );
    *PpttTableLengthRemain -= PpttType1->Length;
    PpttTable->Length += PpttType1->Length;
  }

  NextLevelCacheOffset = *(PrivateResource - 1);
  PrivateResource = (UINT32 *)(PpttType0 + 1);
  // Set the next level to L2 for L1I and L1D
  PpttType1 = (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE *)((UINT8 *) PpttTable + *PrivateResource++);
  PpttType1->NextLevelOfCache = NextLevelCacheOffset;
  PpttType1 = (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE *)((UINT8 *) PpttTable + *PrivateResource++);
  PpttType1->NextLevelOfCache = NextLevelCacheOffset;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
AddClusterTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *PpttTable,
  IN OUT UINT32                      *PpttTableLengthRemain,
  IN     UINT32                      Parent,
  IN     UINT32                      ResourceNo
  )
{
  EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *PpttType0;

  if ((*PpttTableLengthRemain) <
      (sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) + ResourceNo * 4)) {
    return EFI_OUT_OF_RESOURCES;
  }
  PpttType0 = (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *)((UINT8 *)PpttTable +
                                                        PpttTable->Length);
  PpttType0->Type = 0;
  SetMem (&PpttType0->Flags, sizeof (PpttType0->Flags), 0);
  PpttType0->Parent= Parent;
  PpttType0->NumberOfPrivateResources = ResourceNo;
  PpttType0->Length = sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) +
                      ResourceNo * 4;

  *PpttTableLengthRemain -= PpttType0->Length;
  PpttTable->Length += PpttType0->Length;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
AddScclTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *PpttTable,
  IN OUT UINT32                      *PpttTableLengthRemain,
  IN     UINT32                      Parent,
  IN     UINT32                      ResourceNo
  )
{
  EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *PpttType0;
  EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE     *PpttType1;
  UINT32                                *PrivateResource;

  if (*PpttTableLengthRemain <
      (sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) + ResourceNo * 4)) {
    return EFI_OUT_OF_RESOURCES;
  }
  PpttType0 = (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *)((UINT8 *)PpttTable +
                                                        PpttTable->Length);
  PpttType0->Type = 0;
  SetMem (&PpttType0->Flags, sizeof (PpttType0->Flags), 0);
  PpttType0->Parent= Parent;
  PpttType0->NumberOfPrivateResources = ResourceNo;
  PpttType0->Length = sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) +
                      ResourceNo * 4;

  *PpttTableLengthRemain -= PpttType0->Length;
  PpttTable->Length += PpttType0->Length;
  PrivateResource = (UINT32 *)((UINT8 *)PpttType0 +
                               sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR));

  // Add cache type structure
  if (*PpttTableLengthRemain < sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE)) {
    return EFI_OUT_OF_RESOURCES;
  }
  *PrivateResource = PpttTable->Length;
  PpttType1 = (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE *)((UINT8 *)PpttTable +
                                                    PpttTable->Length);
  gBS->CopyMem (
         PpttType1,
         &mPpttCacheType1[3],
         sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_CACHE)
         );
  *PpttTableLengthRemain -= PpttType1->Length;
  PpttTable->Length += PpttType1->Length;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
AddSocketTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *PpttTable,
  IN OUT UINT32                      *PpttTableLengthRemain,
  IN     UINT32                      Parent,
  IN     UINT32                      ResourceNo
  )
{
  EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *PpttType0;
  EFI_ACPI_6_2_PPTT_STRUCTURE_ID        *PpttType2;
  UINT32                                *PrivateResource;
  UINT8                                 Index;

  if (*PpttTableLengthRemain < sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR)) {
    return EFI_OUT_OF_RESOURCES;
  }
  PpttType0 = (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR *)((UINT8 *)PpttTable +
                                                        PpttTable->Length);
  PpttType0->Type = 0;
  SetMem (&PpttType0->Flags, sizeof (PpttType0->Flags), 0);
  PpttType0->Flags.PhysicalPackage = EFI_ACPI_6_2_PPTT_PROCESSOR_ID_VALID;
  PpttType0->Parent= Parent;
  PpttType0->NumberOfPrivateResources = ResourceNo;
  PpttType0->Length = sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR) +
                      ResourceNo * 4;
  PpttTable->Length += PpttType0->Length;

  *PpttTableLengthRemain -= PpttType0->Length;
  if (*PpttTableLengthRemain < ResourceNo * 4) {
    return EFI_OUT_OF_RESOURCES;
  }
  PrivateResource = (UINT32 *)((UINT8 *)PpttType0 +
                               sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_PROCESSOR));
  DEBUG ((DEBUG_INFO,
          "[Acpi PPTT]  sizeof(EFI_ACPI_6_2_PPTT_STRUCTURE_ID) = %x!\n",
          sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_ID)));

  for (Index = 0; Index < ResourceNo; Index++, PrivateResource++) {
    if (*PpttTableLengthRemain < sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_ID)) {
      return EFI_OUT_OF_RESOURCES;
    }
    *PrivateResource = PpttTable->Length;
    PpttType2 = (EFI_ACPI_6_2_PPTT_STRUCTURE_ID *)((UINT8 *)PpttTable +
                                                   PpttTable->Length);
    gBS->CopyMem (
           PpttType2,
           &mPpttSocketType2[Index],
           sizeof (EFI_ACPI_6_2_PPTT_STRUCTURE_ID)
           );
    *PpttTableLengthRemain -= PpttType2->Length;
    PpttTable->Length += PpttType2->Length;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
GetAffLvl (
  IN     UINT64     Mpidr,
  IN OUT UINT8      *Level3,
  IN OUT UINT8      *Level2,
  IN OUT UINT8      *Level1,
  IN OUT UINT8      *Level0
  )
{
  *Level3 = BitFieldRead64 (Mpidr, 32, 39);
  *Level2 = BitFieldRead64 (Mpidr, 16, 23);
  *Level1 = BitFieldRead64 (Mpidr, 8, 15);
  *Level0 = BitFieldRead64 (Mpidr, 0, 7);
}


STATIC
VOID
GetApic (
  IN     ACPI_MADT_TABLE_HEADER                 *ApicTable,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER            *PpttTable,
  IN     UINT32                                 PpttTableLengthRemain
)
{
  UINT32                Parent = 0;
  UINT32                ResourceNo = 0;
  ACPI_GIC_STRUCTURE    *Ptr;
  UINT8                 AffLvl3 = 0;
  UINT8                 AffLvl2 = 0;
  UINT8                 AffLvl1 = 0;
  UINT8                 AffLvl0 = 0;
  UINTN                 SocketIndex;

  for (Ptr = (ACPI_GIC_STRUCTURE *) (ApicTable + 1);
      (UINTN) Ptr < (UINTN) ApicTable + ApicTable->Header.Length;
      Ptr = (ACPI_GIC_STRUCTURE *) ((UINTN) Ptr + Ptr->Length)) {

    // Avoid dead loop due to corrupted MADT
    if (Ptr->Length == 0) {
      DEBUG ((DEBUG_ERROR, "[%a:%d] - Invalid MADT sub structure at 0x%x\n",
            __FUNCTION__, __LINE__, (UINTN) Ptr - (UINTN) ApicTable));
      break;
    }

    if (Ptr->Type != EFI_ACPI_5_1_GIC ||
        (Ptr->Flags & EFI_ACPI_5_1_GIC_ENABLED) == 0) {
      continue;
    }
    GetAffLvl (Ptr->MPIDR, &AffLvl3, &AffLvl2, &AffLvl1, &AffLvl0);
    // AffLvl3 is not used for Hi1620
    // And socket index is calculated by AffLvl2

    SocketIndex = AffLvl2 / MAX_SCL_PER_SOCKET;
    if (mSocketOffset[SocketIndex] == 0) {
      //Add socket for type0 table
      ResourceNo = PPTT_SOCKET_COMPONENT_NO;
      mSocketOffset[SocketIndex] = PpttTable->Length;
      Parent = 0;
      AddSocketTable (
        PpttTable,
        &PpttTableLengthRemain,
        Parent,
        ResourceNo
        );
    }

    if (mScclOffset[AffLvl2] == 0) {
      //Add SCCL for type0 table
      ResourceNo = 1;
      mScclOffset[AffLvl2] = PpttTable->Length ;
      Parent = mSocketOffset[SocketIndex];
      AddScclTable (
        PpttTable,
        &PpttTableLengthRemain,
        Parent,
        ResourceNo
        );
    }

    if (mClusterOffset[AffLvl2][AffLvl1] == 0) {
      // Add cluster for type0 table
      // No private resource for cluster on Hi1620
      ResourceNo = 0;
      mClusterOffset[AffLvl2][AffLvl1] = PpttTable->Length ;
      Parent = mScclOffset[AffLvl2];
      AddClusterTable (
        PpttTable,
        &PpttTableLengthRemain,
        Parent,
        ResourceNo
        );
    }

    //Add core for type0 table
    ResourceNo = 3;
    Parent = mClusterOffset[AffLvl2][AffLvl1];
    AddCoreTable (
      PpttTable,
      &PpttTableLengthRemain,
      Parent,
      ResourceNo,
      Ptr->AcpiProcessorUid
      );
  }
}


STATIC
VOID
PpttSetAcpiTable(
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINTN                                         AcpiTableHandle;
  EFI_STATUS                                    Status;
  UINT8                                         Checksum;
  EFI_ACPI_SDT_HEADER                           *Table;
  ACPI_MADT_TABLE_HEADER                        *ApicTable;
  EFI_ACPI_TABLE_VERSION                        TableVersion;
  EFI_ACPI_DESCRIPTION_HEADER                   *PpttTable;
  UINTN                                         TableKey;
  UINT32                                        Index0;
  UINT32                                        PpttTableLengthRemain = 0;

  gBS->CloseEvent (Event);

  InitCacheInfo ();

  PpttTable = (EFI_ACPI_DESCRIPTION_HEADER *)AllocateZeroPool (PPTT_TABLE_MAX_LEN);
  gBS->CopyMem (
         (VOID *)PpttTable,
         &mPpttHeader,
         sizeof (EFI_ACPI_DESCRIPTION_HEADER)
         );
  PpttTableLengthRemain = PPTT_TABLE_MAX_LEN - sizeof (EFI_ACPI_DESCRIPTION_HEADER);

  for (Index0 = 0; Index0 < EFI_ACPI_MAX_NUM_TABLES; Index0++) {
    Status = mAcpiSdtProtocol->GetAcpiTable (
                                 Index0,
                                 &Table,
                                 &TableVersion,
                                 &TableKey
                                 );
    if (EFI_ERROR (Status)) {
      break;
    }

    // Find APIC table
    if (Table->Signature == EFI_ACPI_6_1_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE) {
      break;
    }

  }

  if (!EFI_ERROR (Status) && (Index0 != EFI_ACPI_MAX_NUM_TABLES)) {
    ApicTable = (ACPI_MADT_TABLE_HEADER *)Table;

    GetApic (ApicTable, PpttTable, PpttTableLengthRemain);

    Checksum = CalculateCheckSum8 ((UINT8 *)(PpttTable), PpttTable->Length);
    PpttTable->Checksum = Checksum;

    AcpiTableHandle = 0;
    Status = mAcpiTableProtocol->InstallAcpiTable (
                                   mAcpiTableProtocol,
                                   PpttTable,
                                   PpttTable->Length,
                                   &AcpiTableHandle);
  }

  FreePool (PpttTable);
  return ;
}

EFI_STATUS
EFIAPI
PpttEntryPoint(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_EVENT               ReadyToBootEvent;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID **)&mAcpiTableProtocol);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  (VOID **)&mAcpiSdtProtocol);
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventReadyToBootEx (
             TPL_NOTIFY,
             PpttSetAcpiTable,
             NULL,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Acpi Pptt init done.\n"));

  return Status;
}
