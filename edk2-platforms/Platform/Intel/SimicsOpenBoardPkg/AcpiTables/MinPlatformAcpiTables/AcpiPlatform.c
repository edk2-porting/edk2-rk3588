/** @file
  ACPI Platform Driver

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "AcpiPlatform.h"

#define MAX_CPU_NUM (FixedPcdGet32(PcdMaxCpuThreadCount) * FixedPcdGet32(PcdMaxCpuCoreCount) * FixedPcdGet32(PcdMaxCpuSocketCount))

#pragma pack(1)

typedef struct {
  UINT32   AcpiProcessorId;
  UINT32   ApicId;
  UINT32   Flags;
  UINT32   SwProcApicId;
  UINT32   SocketNum;
} EFI_CPU_ID_ORDER_MAP;

//
// Private Driver Data
//
//
// Define Union of IO APIC & Local APIC structure;
//
typedef union {
  EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE AcpiLocalApic;
  EFI_ACPI_4_0_IO_APIC_STRUCTURE              AcpiIoApic;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE AcpiLocalx2Apic;
  struct {
    UINT8 Type;
    UINT8 Length;
  } AcpiApicCommon;
} ACPI_APIC_STRUCTURE_PTR;

#pragma pack()

extern EFI_ACPI_5_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  Facs;
extern EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE Fadt;
extern EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER  Hpet;
extern EFI_ACPI_WSMT_TABLE Wsmt;

VOID  *mLocalTable[] = {
  &Facs,
  &Fadt,
  &Hpet,
  &Wsmt,
};

EFI_ACPI_TABLE_PROTOCOL       *mAcpiTable;

UINT32                      mNumOfBitShift = 6;
BOOLEAN                     mForceX2ApicId;
BOOLEAN                     mX2ApicEnabled;

EFI_MP_SERVICES_PROTOCOL    *mMpService;
BOOLEAN                     mCpuOrderSorted;
EFI_CPU_ID_ORDER_MAP        mCpuApicIdOrderTable[MAX_CPU_NUM];
UINTN                       mNumberOfCPUs = 0;
UINTN                       mNumberOfEnabledCPUs = 0;
//
// following are possible APICID Map for SKX
//
static const UINT32 ApicIdMapA[] = {  //for SKUs have number of core > 16
  //it is 14 + 14 + 14 + 14 format
  0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,
  0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x00000010, 0x00000011,
  0x00000012, 0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018, 0x00000019,
  0x0000001A, 0x0000001B, 0x0000001C, 0x0000001D, 0x00000020, 0x00000021, 0x00000022, 0x00000023,
  0x00000024, 0x00000025, 0x00000026, 0x00000027, 0x00000028, 0x00000029, 0x0000002A, 0x0000002B,
  0x0000002C, 0x0000002D, 0x00000030, 0x00000031, 0x00000032, 0x00000033, 0x00000034, 0x00000035,
  0x00000036, 0x00000037, 0x00000038, 0x00000039, 0x0000003A, 0x0000003B, 0x0000003C, 0x0000003D
};

static const UINT32 ApicIdMapB[] = { //for SKUs have number of cores <= 16 use 32 ID space
  //
  //it is 16+16 format
  //
  0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,
  0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F,
  0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017,
  0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x0000001C, 0x0000001D, 0x0000001E, 0x0000001F,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};


static const UINT32 ApicIdMapC[] = { //for SKUs have number of cores <= 16 use 64 ID space
  //
  //it is 16+0+16+0 format
  //
  0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,
  0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F,
  0x00000020, 0x00000021, 0x00000022, 0x00000023, 0x00000024, 0x00000025, 0x00000026, 0x00000027,
  0x00000028, 0x00000029, 0x0000002A, 0x0000002B, 0x0000002C, 0x0000002D, 0x0000002E, 0x0000002F,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

static const UINT32 ApicIdMapD[] = { //for SKUs have number of cores <= 8 use 16 ID space
  //
  //it is 16 format
  //
  0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,
  0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C, 0x0000000D, 0x0000000E, 0x0000000F,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

const UINT32 *mApicIdMap = NULL;

/**
  This function detect the APICID map and update ApicID Map pointer

  @param None

  @retval VOID

**/
VOID DetectApicIdMap(VOID)
{
  UINTN                  CoreCount;

  CoreCount = 0;

  if(mApicIdMap != NULL) {
    return;   //aleady initialized
  }

  mApicIdMap = ApicIdMapA;  // default to > 16C SKUs

  CoreCount = mNumberOfEnabledCPUs / 2;
  DEBUG ((DEBUG_INFO, "CoreCount - %d\n", CoreCount));

  if(CoreCount <= 16) {

    if(mNumOfBitShift == 4) {
      mApicIdMap = ApicIdMapD;
    }

    if(mNumOfBitShift == 5) {
      mApicIdMap = ApicIdMapB;
    }

    if(mNumOfBitShift == 6) {
      mApicIdMap = ApicIdMapC;
    }

  }

  return;
}

/**
  This function return the CoreThreadId of ApicId from ACPI ApicId Map array

  @param ApicId

  @retval Index of ACPI ApicId Map array

**/
UINT32
GetIndexFromApicId (
  UINT32 ApicId
  )
{
  UINT32 CoreThreadId;
  UINT32 i;

  ASSERT (mApicIdMap != NULL);

  CoreThreadId = ApicId & ((1 << mNumOfBitShift) - 1);

  for(i = 0; i < (FixedPcdGet32(PcdMaxCpuCoreCount) * FixedPcdGet32(PcdMaxCpuThreadCount)); i++) {
    if(mApicIdMap[i] == CoreThreadId) {
      break;
    }
  }

  ASSERT (i <= (FixedPcdGet32(PcdMaxCpuCoreCount) * FixedPcdGet32(PcdMaxCpuThreadCount)));

  return i;
}

UINT32
ApicId2SwProcApicId (
  UINT32 ApicId
  )
{
  UINT32 Index;

  for (Index = 0; Index < MAX_CPU_NUM; Index++) {
    if ((mCpuApicIdOrderTable[Index].Flags == 1) && (mCpuApicIdOrderTable[Index].ApicId == ApicId)) {
      return Index;
    }
  }

  return (UINT32) -1;

}

VOID
DebugDisplayReOrderTable(
  VOID
  )
{
  UINT32 Index;

  DEBUG ((EFI_D_ERROR, "Index  AcpiProcId  ApicId  Flags  SwApicId  Skt\n"));
  for (Index=0; Index<MAX_CPU_NUM; Index++) {
    DEBUG ((EFI_D_ERROR, " %02d       0x%02X      0x%02X      %d      0x%02X     %d\n",
                           Index, mCpuApicIdOrderTable[Index].AcpiProcessorId,
                           mCpuApicIdOrderTable[Index].ApicId,
                           mCpuApicIdOrderTable[Index].Flags,
                           mCpuApicIdOrderTable[Index].SwProcApicId,
                           mCpuApicIdOrderTable[Index].SocketNum));
  }
}

EFI_STATUS
AppendCpuMapTableEntry (
    IN VOID   *ApicPtr,
    IN UINT32 LocalApicCounter
  )
{
  EFI_STATUS    Status;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE *LocalApicPtr;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE *LocalX2ApicPtr;
  UINT8         Type;

  Status = EFI_SUCCESS;
  Type = ((ACPI_APIC_STRUCTURE_PTR *)ApicPtr)->AcpiApicCommon.Type;
  LocalApicPtr = (EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE *)(&((ACPI_APIC_STRUCTURE_PTR *)ApicPtr)->AcpiLocalApic);
  LocalX2ApicPtr = (EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE *)(&((ACPI_APIC_STRUCTURE_PTR *)ApicPtr)->AcpiLocalx2Apic);

  if(Type == EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC) {
    if(!mX2ApicEnabled) {
      LocalApicPtr->Flags           = (UINT8)mCpuApicIdOrderTable[LocalApicCounter].Flags;
      LocalApicPtr->ApicId          = (UINT8)mCpuApicIdOrderTable[LocalApicCounter].ApicId;
      LocalApicPtr->AcpiProcessorId = (UINT8)mCpuApicIdOrderTable[LocalApicCounter].AcpiProcessorId;
    } else {
      LocalApicPtr->Flags           = 0;
      LocalApicPtr->ApicId          = 0xFF;
      LocalApicPtr->AcpiProcessorId = (UINT8)0xFF;
      Status = EFI_UNSUPPORTED;
    }
  } else if(Type == EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC) {
    if(mX2ApicEnabled) {
      LocalX2ApicPtr->Flags            = (UINT8)mCpuApicIdOrderTable[LocalApicCounter].Flags;
      LocalX2ApicPtr->X2ApicId         = mCpuApicIdOrderTable[LocalApicCounter].ApicId;
      LocalX2ApicPtr->AcpiProcessorUid = mCpuApicIdOrderTable[LocalApicCounter].AcpiProcessorId;
    } else {
      LocalX2ApicPtr->Flags            = 0;
      LocalX2ApicPtr->X2ApicId         = (UINT32)-1;
      LocalX2ApicPtr->AcpiProcessorUid = (UINT32)-1;
      Status = EFI_UNSUPPORTED;
    }
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;

}

EFI_STATUS
SortCpuLocalApicInTable (
  VOID
  )
{
  EFI_STATUS                                Status;
  EFI_PROCESSOR_INFORMATION                 ProcessorInfoBuffer;
  UINT32                                    Index;
  UINT32                                    CurrProcessor;
  UINT32                                    BspApicId;
  UINT32                                    TempVal = 0;
  EFI_CPU_ID_ORDER_MAP                      *CpuIdMapPtr;
  UINT32                                    CoreThreadMask;

  Index      = 0;
  Status     = EFI_SUCCESS;

  CoreThreadMask = (UINT32) ((1 << mNumOfBitShift) - 1);

  if(!mCpuOrderSorted) {

    Index  = 0;

    for (CurrProcessor = 0; CurrProcessor < mNumberOfCPUs; CurrProcessor++) {
      Status = mMpService->GetProcessorInfo (
                                            mMpService,
                                            CurrProcessor,
                                            &ProcessorInfoBuffer
                                            );

      if ((ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0) {
        if(ProcessorInfoBuffer.ProcessorId & 1) { //is 2nd thread
          CpuIdMapPtr = (EFI_CPU_ID_ORDER_MAP *)&mCpuApicIdOrderTable[(Index - 1) + MAX_CPU_NUM / 2];
        } else { //is primary thread
          CpuIdMapPtr = (EFI_CPU_ID_ORDER_MAP *)&mCpuApicIdOrderTable[Index];
          Index++;
        }
        CpuIdMapPtr->ApicId  = (UINT32)ProcessorInfoBuffer.ProcessorId;
        CpuIdMapPtr->Flags   = ((ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0);
        CpuIdMapPtr->SocketNum = (UINT32)ProcessorInfoBuffer.Location.Package;
        CpuIdMapPtr->AcpiProcessorId = (CpuIdMapPtr->SocketNum * FixedPcdGet32(PcdMaxCpuCoreCount) * FixedPcdGet32(PcdMaxCpuThreadCount)) + GetIndexFromApicId(CpuIdMapPtr->ApicId); //CpuIdMapPtr->ApicId;
        CpuIdMapPtr->SwProcApicId = ((UINT32)(ProcessorInfoBuffer.Location.Package << mNumOfBitShift) + (((UINT32)ProcessorInfoBuffer.ProcessorId) & CoreThreadMask));
        if(mX2ApicEnabled) { //if X2Apic, re-order the socket # so it starts from base 0 and contiguous
          //may not necessory!!!!!
        }

        //update processorbitMask
        if (CpuIdMapPtr->Flags == 1) {

          if(mForceX2ApicId) {
            CpuIdMapPtr->SocketNum &= 0x7;
            CpuIdMapPtr->AcpiProcessorId &= 0xFF; //keep lower 8bit due to use Proc obj in dsdt
            CpuIdMapPtr->SwProcApicId &= 0xFF;
          }
        }
      } else {  //not enabled
        CpuIdMapPtr = (EFI_CPU_ID_ORDER_MAP *)&mCpuApicIdOrderTable[Index];
        CpuIdMapPtr->ApicId  = (UINT32)-1;
        CpuIdMapPtr->Flags   = 0;
        CpuIdMapPtr->AcpiProcessorId = (UINT32)-1;
        CpuIdMapPtr->SwProcApicId = (UINT32)-1;
        CpuIdMapPtr->SocketNum = (UINT32)-1;
      } //end if PROC ENABLE
    } //end for CurrentProcessor
    //
    //keep for debug purpose
	//
    DEBUG(( EFI_D_ERROR, "::ACPI::  APIC ID Order Table Init.   CoreThreadMask = %x,  mNumOfBitShift = %x\n", CoreThreadMask, mNumOfBitShift));
    DebugDisplayReOrderTable();
    //
    //make sure 1st entry is BSP
	//
    if(mX2ApicEnabled) {
      BspApicId = (UINT32)AsmReadMsr64(0x802);
    } else {
      BspApicId = (*(volatile UINT32 *)(UINTN)0xFEE00020) >> 24;
    }
    DEBUG ((EFI_D_INFO, "BspApicId - 0x%x\n", BspApicId));

    if(mCpuApicIdOrderTable[0].ApicId != BspApicId) {
      //
      //check to see if 1st entry is BSP, if not swap it
	  //
      Index = ApicId2SwProcApicId(BspApicId);

      if(MAX_CPU_NUM <= Index) {
        DEBUG ((EFI_D_ERROR, "Asserting the SortCpuLocalApicInTable Index Bufferflow\n"));
        ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
      }

      TempVal = mCpuApicIdOrderTable[Index].ApicId;
      mCpuApicIdOrderTable[Index].ApicId = mCpuApicIdOrderTable[0].ApicId;
      mCpuApicIdOrderTable[0].ApicId = TempVal;
      mCpuApicIdOrderTable[Index].Flags = mCpuApicIdOrderTable[0].Flags;
      mCpuApicIdOrderTable[0].Flags = 1;
      TempVal = mCpuApicIdOrderTable[Index].SwProcApicId;
      mCpuApicIdOrderTable[Index].SwProcApicId = mCpuApicIdOrderTable[0].SwProcApicId;
      mCpuApicIdOrderTable[0].SwProcApicId = TempVal;
      //
      //swap AcpiProcId
	  //
      TempVal = mCpuApicIdOrderTable[Index].AcpiProcessorId;
      mCpuApicIdOrderTable[Index].AcpiProcessorId = mCpuApicIdOrderTable[0].AcpiProcessorId;
      mCpuApicIdOrderTable[0].AcpiProcessorId = TempVal;

    }
    //
    //Make sure no holes between enabled threads
	//
    for(CurrProcessor = 0; CurrProcessor < MAX_CPU_NUM; CurrProcessor++) {

      if(mCpuApicIdOrderTable[CurrProcessor].Flags == 0) {
        //
        //make sure disabled entry has ProcId set to FFs
		//
        mCpuApicIdOrderTable[CurrProcessor].ApicId = (UINT32)-1;
        mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = (UINT32)-1;
        mCpuApicIdOrderTable[CurrProcessor].SwProcApicId = (UINT32)-1;

        for(Index = CurrProcessor+1; Index < MAX_CPU_NUM; Index++) {
          if(mCpuApicIdOrderTable[Index].Flags == 1) {
            //
            //move enabled entry up
			//
            mCpuApicIdOrderTable[CurrProcessor].Flags = 1;
            mCpuApicIdOrderTable[CurrProcessor].ApicId = mCpuApicIdOrderTable[Index].ApicId;
            mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = mCpuApicIdOrderTable[Index].AcpiProcessorId;
            mCpuApicIdOrderTable[CurrProcessor].SwProcApicId = mCpuApicIdOrderTable[Index].SwProcApicId;
            mCpuApicIdOrderTable[CurrProcessor].SocketNum = mCpuApicIdOrderTable[Index].SocketNum;
            //
            //disable moved entry
            //
            mCpuApicIdOrderTable[Index].Flags = 0;
            mCpuApicIdOrderTable[Index].ApicId = (UINT32)-1;
            mCpuApicIdOrderTable[Index].AcpiProcessorId = (UINT32)-1;
            mCpuApicIdOrderTable[Index].SwProcApicId = (UINT32)-1;
            break;
          }
        }
      }
    }
    //
    //keep for debug purpose
    //
    DEBUG ((EFI_D_ERROR, "APIC ID Order Table ReOrdered\n"));
    DebugDisplayReOrderTable();

    mCpuOrderSorted = TRUE;
  }

  return Status;
}


/** Structure of a sub-structure of the ACPI header.

  This structure contains the type and length fields, which are common to every
  sub-structure of the ACPI tables. A pointer to any structure can be cast as this.
**/
typedef struct {
  UINT8 Type;
  UINT8 Length;
} STRUCTURE_HEADER;

STRUCTURE_HEADER mMadtStructureTable[] = {
  {EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC,          sizeof (EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE)},
  {EFI_ACPI_4_0_IO_APIC,                       sizeof (EFI_ACPI_4_0_IO_APIC_STRUCTURE)},
  {EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE,     sizeof (EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE)},
  {EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE, sizeof (EFI_ACPI_4_0_NON_MASKABLE_INTERRUPT_SOURCE_STRUCTURE)},
  {EFI_ACPI_4_0_LOCAL_APIC_NMI,                sizeof (EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE)},
  {EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE,   sizeof (EFI_ACPI_4_0_LOCAL_APIC_ADDRESS_OVERRIDE_STRUCTURE)},
  {EFI_ACPI_4_0_IO_SAPIC,                      sizeof (EFI_ACPI_4_0_IO_SAPIC_STRUCTURE)},
  {EFI_ACPI_4_0_LOCAL_SAPIC,                   sizeof (EFI_ACPI_4_0_PROCESSOR_LOCAL_SAPIC_STRUCTURE)},
  {EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES,    sizeof (EFI_ACPI_4_0_PLATFORM_INTERRUPT_SOURCES_STRUCTURE)},
  {EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC,        sizeof (EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE)},
  {EFI_ACPI_4_0_LOCAL_X2APIC_NMI,              sizeof (EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE)}
};

/**
  Get the size of the ACPI table.

  This function calculates the size needed for the ACPI Table based on the number and
  size of the sub-structures that will compose it.

  @param[in]  TableSpecificHdrLength  Size of the table specific header, not the ACPI standard header size.
  @param[in]  Structures              Pointer to an array of sub-structure pointers.
  @param[in]  StructureCount          Number of structure pointers in the array.

  @return     Total size needed for the ACPI table.
**/
UINT32
GetTableSize (
  IN  UINTN                 TableSpecificHdrLength,
  IN  STRUCTURE_HEADER      **Structures,
  IN  UINTN                 StructureCount
  )
{
  UINT32  TableLength;
  UINT32  Index;

  //
  // Compute size of the ACPI table; header plus all structures needed.
  //
  TableLength = (UINT32) TableSpecificHdrLength;

  for (Index = 0; Index < StructureCount; Index++) {
    ASSERT (Structures[Index] != NULL);
    if (Structures[Index] == NULL) {
      return 0;
    }

    TableLength += Structures[Index]->Length;
  }

  return TableLength;
}

/**
  Allocate the ACPI Table.

  This function allocates space for the ACPI table based on the number and size of
  the sub-structures that will compose it.

  @param[in]  TableSpecificHdrLength  Size of the table specific header, not the ACPI standard header size.
  @param[in]  Structures  Pointer to an array of sub-structure pointers.
  @param[in]  StructureCount  Number of structure pointers in the array.
  @param[out] Table            Newly allocated ACPI Table pointer.

  @retval EFI_SUCCESS           Successfully allocated the Table.
  @retval EFI_OUT_OF_RESOURCES  Space for the Table could not be allocated.
**/
EFI_STATUS
AllocateTable (
  IN  UINTN                        TableSpecificHdrLength,
  IN  STRUCTURE_HEADER             **Structures,
  IN  UINTN                        StructureCount,
  OUT EFI_ACPI_DESCRIPTION_HEADER  **Table
  )
{
  EFI_STATUS  Status;
  UINT32      Size;
  EFI_ACPI_DESCRIPTION_HEADER *InternalTable;

  //
  // Get the size of the ACPI table and allocate memory.
  //
  Size = GetTableSize (TableSpecificHdrLength, Structures, StructureCount);
  InternalTable = (EFI_ACPI_DESCRIPTION_HEADER *) AllocatePool (Size);

  if (InternalTable == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((
      DEBUG_ERROR,
      "Failed to allocate %d bytes for ACPI Table\n",
      Size
      ));
  } else {
    Status = EFI_SUCCESS;
    DEBUG ((
      DEBUG_INFO,
      "Successfully allocated %d bytes for ACPI Table at 0x%p\n",
      Size,
      InternalTable
      ));
    *Table = InternalTable;
  }

  return Status;
}

/**
  Initialize the header.

  This function fills in the standard table header with correct values,
  except for the length and checksum fields, which are filled in later.

  @param[in,out]  Header        Pointer to the header structure.

  @retval EFI_SUCCESS           Successfully initialized the header.
  @retval EFI_INVALID_PARAMETER Pointer parameter was null.
**/
EFI_STATUS
InitializeHeader (
  IN OUT  EFI_ACPI_DESCRIPTION_HEADER *Header,
  IN      UINT32                      Signature,
  IN      UINT8                       Revision,
  IN      UINT32                      OemRevision
  )
{
  UINT64 AcpiTableOemId;

  if (Header == NULL) {
    DEBUG ((DEBUG_ERROR, "Header pointer is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  Header->Signature  = Signature;
  Header->Length     = 0; // filled in by Build function
  Header->Revision   = Revision;
  Header->Checksum   = 0; // filled in by InstallAcpiTable

  CopyMem (
    (VOID *) &Header->OemId,
    PcdGetPtr (PcdAcpiDefaultOemId),
    sizeof (Header->OemId)
    );

  AcpiTableOemId = PcdGet64 (PcdAcpiDefaultOemTableId);
  CopyMem (
    (VOID *) &Header->OemTableId,
    (VOID *) &AcpiTableOemId,
    sizeof (Header->OemTableId)
    );

  Header->OemRevision     = OemRevision;
  Header->CreatorId       = 0;
  Header->CreatorRevision = 0;

  return EFI_SUCCESS;
}

/**
  Initialize the MADT header.

  This function fills in the MADT's standard table header with correct values,
  except for the length and checksum fields, which are filled in later.

  @param[in,out]  MadtHeader    Pointer to the MADT header structure.

  @retval EFI_SUCCESS           Successfully initialized the MADT header.
  @retval EFI_INVALID_PARAMETER Pointer parameter was null.
**/
EFI_STATUS
InitializeMadtHeader (
  IN OUT EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *MadtHeader
  )
{
  EFI_STATUS Status;

  if (MadtHeader == NULL) {
    DEBUG ((DEBUG_ERROR, "MADT header pointer is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = InitializeHeader (
    &MadtHeader->Header,
    EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE,
    EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION,
    0
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MadtHeader->LocalApicAddress       = PcdGet32(PcdLocalApicAddress);
  MadtHeader->Flags                  = EFI_ACPI_4_0_PCAT_COMPAT;

  return EFI_SUCCESS;
}

/**
  Copy an ACPI sub-structure; MADT and SRAT supported

  This function validates the structure type and size of a sub-structure
  and returns a newly allocated copy of it.

  @param[in]  Header            Pointer to the header of the table.
  @param[in]  Structure         Pointer to the structure to copy.
  @param[in]  NewStructure      Newly allocated copy of the structure.

  @retval EFI_SUCCESS           Successfully copied the structure.
  @retval EFI_INVALID_PARAMETER Pointer parameter was null.
  @retval EFI_INVALID_PARAMETER Structure type was unknown.
  @retval EFI_INVALID_PARAMETER Structure length was wrong for its type.
  @retval EFI_UNSUPPORTED       Header passed in is not supported.
**/
EFI_STATUS
CopyStructure (
  IN  EFI_ACPI_DESCRIPTION_HEADER *Header,
  IN  STRUCTURE_HEADER *Structure,
  OUT STRUCTURE_HEADER **NewStructure
  )
{
  STRUCTURE_HEADER      *NewStructureInternal;
  STRUCTURE_HEADER      *StructureTable;
  UINTN                 TableNumEntries;
  BOOLEAN               EntryFound;
  UINT8                 Index;

  //
  // Initialize the number of table entries and the table based on the table header passed in.
  //
  if (Header->Signature == EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE) {
    TableNumEntries = sizeof (mMadtStructureTable) / sizeof (STRUCTURE_HEADER);
    StructureTable = mMadtStructureTable;
  } else {
    return EFI_UNSUPPORTED;
  }

  //
  // Check the incoming structure against the table of supported structures.
  //
  EntryFound = FALSE;
  for (Index = 0; Index < TableNumEntries; Index++) {
    if (Structure->Type == StructureTable[Index].Type) {
      if (Structure->Length == StructureTable[Index].Length) {
        EntryFound = TRUE;
      } else {
        DEBUG ((
          DEBUG_ERROR,
          "Invalid length for structure type %d: expected %d, actually %d\n",
          Structure->Type,
          StructureTable[Index].Length,
          Structure->Length
          ));
        return EFI_INVALID_PARAMETER;
      }
    }
  }

  //
  // If no entry in the table matches the structure type and length passed in
  // then return invalid parameter.
  //
  if (!EntryFound) {
    DEBUG ((
      DEBUG_ERROR,
      "Unknown structure type: %d\n",
      Structure->Type
      ));
    return EFI_INVALID_PARAMETER;
  }

  NewStructureInternal = (STRUCTURE_HEADER *) AllocatePool (Structure->Length);
  if (NewStructureInternal == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "Failed to allocate %d bytes for type %d structure\n",
      Structure->Length,
      Structure->Type
      ));
    return EFI_OUT_OF_RESOURCES;
  } else {
    DEBUG ((
      DEBUG_INFO,
      "Successfully allocated %d bytes for type %d structure at 0x%p\n",
      Structure->Length,
      Structure->Type,
      NewStructureInternal
      ));
  }

  CopyMem (
    (VOID *) NewStructureInternal,
    (VOID *) Structure,
    Structure->Length
    );

  *NewStructure = NewStructureInternal;
  return EFI_SUCCESS;
}

/**
  Build ACPI Table. MADT tables supported.

  This function builds the ACPI table from the header plus the list of sub-structures
  passed in. The table returned by this function is ready to be installed using
  the ACPI table protocol's InstallAcpiTable function, which copies it into
  ACPI memory. After that, the caller should free the memory returned by this
  function.

  @param[in]  AcpiHeader             Pointer to the header structure.
  @param[in]  TableSpecificHdrLength Size of the table specific header, not the ACPI standard header size.
  @param[in]  Structures             Pointer to an array of sub-structure pointers.
  @param[in]  StructureCount         Number of structure pointers in the array.
  @param[out] NewTable               Newly allocated and initialized pointer to the ACPI Table.

  @retval EFI_SUCCESS           Successfully built the ACPI table.
  @retval EFI_INVALID_PARAMETER Pointer parameter was null.
  @retval EFI_INVALID_PARAMETER Header parameter had the wrong signature.
  @retval EFI_OUT_OF_RESOURCES  Space for the ACPI Table could not be allocated.
**/
EFI_STATUS
BuildAcpiTable (
  IN  EFI_ACPI_DESCRIPTION_HEADER  *AcpiHeader,
  IN  UINTN                        TableSpecificHdrLength,
  IN  STRUCTURE_HEADER             **Structures,
  IN  UINTN                        StructureCount,
  OUT UINT8                        **NewTable
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_DESCRIPTION_HEADER *InternalTable;
  UINTN                       Index;
  UINT8                       *CurrPtr;
  UINT8                       *EndOfTablePtr;

  if (AcpiHeader == NULL) {
    DEBUG ((DEBUG_ERROR, "AcpiHeader pointer is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (AcpiHeader->Signature != EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((
      DEBUG_ERROR,
      "MADT header signature is expected, actually 0x%08x\n",
      AcpiHeader->Signature
      ));
    return EFI_INVALID_PARAMETER;
  }

  if (Structures == NULL) {
    DEBUG ((DEBUG_ERROR, "Structure array pointer is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < StructureCount; Index++) {
    if (Structures[Index] == NULL) {
      DEBUG ((DEBUG_ERROR, "Structure pointer %d is NULL\n", Index));
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Allocate the memory needed for the table.
  //
  Status = AllocateTable (
    TableSpecificHdrLength,
    Structures,
    StructureCount,
    &InternalTable
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Copy Header and patch in structure length, checksum is programmed later
  // after all structures are populated.
  //
  CopyMem (
    (VOID *) InternalTable,
    (VOID *) AcpiHeader,
    TableSpecificHdrLength
    );

  InternalTable->Length = GetTableSize (TableSpecificHdrLength, Structures, StructureCount);

  //
  // Copy all the sub structures to the table.
  //
  CurrPtr = ((UINT8 *) InternalTable) + TableSpecificHdrLength;
  EndOfTablePtr = ((UINT8 *) InternalTable) + InternalTable->Length;

  for (Index = 0; Index < StructureCount; Index++) {
    ASSERT (Structures[Index] != NULL);
    if (Structures[Index] == NULL) {
      break;
    }

    CopyMem (
      (VOID *) CurrPtr,
      (VOID *) Structures[Index],
      Structures[Index]->Length
      );

    CurrPtr += Structures[Index]->Length;
    ASSERT (CurrPtr <= EndOfTablePtr);
    if (CurrPtr > EndOfTablePtr) {
      break;
    }
  }

  //
  // Update the return pointer.
  //
  *NewTable = (UINT8 *) InternalTable;
  return EFI_SUCCESS;
}

/**
  Build from scratch and install the MADT.

  @retval EFI_SUCCESS           The MADT was installed successfully.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate required structures.
**/
EFI_STATUS
InstallMadtFromScratch (
  VOID
  )
{
  EFI_STATUS                                          Status;
  UINTN                                               Index;
  EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *NewMadtTable;
  UINTN                                               TableHandle;
  EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER MadtTableHeader;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE         ProcLocalApicStruct;
  EFI_ACPI_4_0_IO_APIC_STRUCTURE                      IoApicStruct;
  EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE    IntSrcOverrideStruct;
  EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE               LocalApciNmiStruct;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE       ProcLocalX2ApicStruct;
  EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE             LocalX2ApicNmiStruct;
  STRUCTURE_HEADER                                    **MadtStructs;
  UINTN                                               MaxMadtStructCount;
  UINTN                                               MadtStructsIndex;
  UINT32                                              CurrentIoApicAddress = (UINT32)(PcdGet32(PcdPcIoApicAddressBase));
  UINT32                                              PcIoApicEnable;
  UINT32                                              PcIoApicMask;
  UINTN                                               PcIoApicIndex;

  DetectApicIdMap();

  // Call for Local APIC ID Reorder
  SortCpuLocalApicInTable ();

  NewMadtTable = NULL;

  MaxMadtStructCount = (UINT32) (
    MAX_CPU_NUM +    // processor local APIC structures
    MAX_CPU_NUM +    // processor local x2APIC structures
    1 + PcdGet8(PcdPcIoApicCount) +   // I/O APIC structures
    2 +              // interrupt source override structures
    1 +              // local APIC NMI structures
    1                // local x2APIC NMI structures
    );               // other structures are not used

  MadtStructs = (STRUCTURE_HEADER **) AllocateZeroPool (MaxMadtStructCount * sizeof (STRUCTURE_HEADER *));
  if (MadtStructs == NULL) {
    DEBUG ((DEBUG_ERROR, "Could not allocate MADT structure pointer array\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize the next index into the structure pointer array. It is
  // incremented every time a structure of any type is copied to the array.
  //
  MadtStructsIndex = 0;

  //
  // Initialize MADT Header Structure
  //
  Status = InitializeMadtHeader (&MadtTableHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "InitializeMadtHeader failed: %r\n", Status));
    goto Done;
  }

  DEBUG ((EFI_D_INFO, "Number of CPUs detected = %d \n", mNumberOfCPUs));

  //
  // Build Processor Local APIC Structures and Processor Local X2APIC Structures
  //
  ProcLocalApicStruct.Type = EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC;
  ProcLocalApicStruct.Length = sizeof (EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE);

  ProcLocalX2ApicStruct.Type = EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC;
  ProcLocalX2ApicStruct.Length = sizeof (EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE);
  ProcLocalX2ApicStruct.Reserved[0] = 0;
  ProcLocalX2ApicStruct.Reserved[1] = 0;

  for (Index = 0; Index < MAX_CPU_NUM; Index++) {
    //
    // If x2APIC mode is not enabled, and if it is possible to express the
    // APIC ID as a UINT8, use a processor local APIC structure. Otherwise,
    // use a processor local x2APIC structure.
    //
    if (!mX2ApicEnabled && mCpuApicIdOrderTable[Index].ApicId < MAX_UINT8) {
      ProcLocalApicStruct.Flags           = (UINT8) mCpuApicIdOrderTable[Index].Flags;
      ProcLocalApicStruct.ApicId          = (UINT8) mCpuApicIdOrderTable[Index].ApicId;
      ProcLocalApicStruct.AcpiProcessorId = (UINT8) mCpuApicIdOrderTable[Index].AcpiProcessorId;

      ASSERT (MadtStructsIndex < MaxMadtStructCount);
      Status = CopyStructure (
        &MadtTableHeader.Header,
        (STRUCTURE_HEADER *) &ProcLocalApicStruct,
        &MadtStructs[MadtStructsIndex++]
        );
    } else if (mCpuApicIdOrderTable[Index].ApicId != 0xFFFFFFFF) {
      ProcLocalX2ApicStruct.Flags            = (UINT8) mCpuApicIdOrderTable[Index].Flags;
      ProcLocalX2ApicStruct.X2ApicId         = mCpuApicIdOrderTable[Index].ApicId;
      ProcLocalX2ApicStruct.AcpiProcessorUid = mCpuApicIdOrderTable[Index].AcpiProcessorId;

      ASSERT (MadtStructsIndex < MaxMadtStructCount);
      Status = CopyStructure (
        &MadtTableHeader.Header,
        (STRUCTURE_HEADER *) &ProcLocalX2ApicStruct,
        &MadtStructs[MadtStructsIndex++]
        );
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "CopyMadtStructure (local APIC/x2APIC) failed: %r\n", Status));
      goto Done;
    }
  }

  //
  // Build I/O APIC Structures
  //
  IoApicStruct.Type = EFI_ACPI_4_0_IO_APIC;
  IoApicStruct.Length = sizeof (EFI_ACPI_4_0_IO_APIC_STRUCTURE);
  IoApicStruct.Reserved = 0;

  PcIoApicEnable = PcdGet32(PcdPcIoApicEnable);

  if (FixedPcdGet32(PcdMaxCpuSocketCount) <= 4) {
    IoApicStruct.IoApicId                  = PcdGet8(PcdIoApicId);
    IoApicStruct.IoApicAddress             = PcdGet32(PcdIoApicAddress);
    IoApicStruct.GlobalSystemInterruptBase = 0;
    ASSERT (MadtStructsIndex < MaxMadtStructCount);
    Status = CopyStructure (
      &MadtTableHeader.Header,
      (STRUCTURE_HEADER *) &IoApicStruct,
      &MadtStructs[MadtStructsIndex++]
      );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "CopyMadtStructure (I/O APIC) failed: %r\n", Status));
      goto Done;
    }
  }

  for (PcIoApicIndex = 0; PcIoApicIndex < PcdGet8(PcdPcIoApicCount); PcIoApicIndex++) {
      PcIoApicMask = (1 << PcIoApicIndex);
      if ((PcIoApicEnable & PcIoApicMask) == 0) {
        continue;
      }

      IoApicStruct.IoApicId                  = (UINT8)(PcdGet8(PcdPcIoApicIdBase) + PcIoApicIndex);
      IoApicStruct.IoApicAddress             = CurrentIoApicAddress;
      CurrentIoApicAddress                   = (CurrentIoApicAddress & 0xFFFF8000) + 0x8000;
      IoApicStruct.GlobalSystemInterruptBase = (UINT32)(24 + (PcIoApicIndex * 8));
      ASSERT (MadtStructsIndex < MaxMadtStructCount);
      Status = CopyStructure (
        &MadtTableHeader.Header,
        (STRUCTURE_HEADER *) &IoApicStruct,
        &MadtStructs[MadtStructsIndex++]
        );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "CopyMadtStructure (I/O APIC) failed: %r\n", Status));
        goto Done;
      }
  }

  //
  // Build Interrupt Source Override Structures
  //
  IntSrcOverrideStruct.Type = EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE;
  IntSrcOverrideStruct.Length = sizeof (EFI_ACPI_4_0_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE);

  //
  // IRQ0=>IRQ2 Interrupt Source Override Structure
  //
  IntSrcOverrideStruct.Bus = 0x0;                   // Bus - ISA
  IntSrcOverrideStruct.Source = 0x0;                // Source - IRQ0
  IntSrcOverrideStruct.GlobalSystemInterrupt = 0x2; // Global System Interrupt - IRQ2
  IntSrcOverrideStruct.Flags = 0x0;                 // Flags - Conforms to specifications of the bus

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &IntSrcOverrideStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "CopyMadtStructure (IRQ2 source override) failed: %r\n", Status));
    goto Done;
  }

  //
  // IRQ9 (SCI Active High) Interrupt Source Override Structure
  //
  IntSrcOverrideStruct.Bus = 0x0;                   // Bus - ISA
  IntSrcOverrideStruct.Source = 0x9;                // Source - IRQ9
  IntSrcOverrideStruct.GlobalSystemInterrupt = 0x9; // Global System Interrupt - IRQ9
  IntSrcOverrideStruct.Flags = 0xD;                 // Flags - Level-tiggered, Active High

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &IntSrcOverrideStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "CopyMadtStructure (IRQ9 source override) failed: %r\n", Status));
    goto Done;
  }

  //
  // Build Local APIC NMI Structures
  //
  LocalApciNmiStruct.Type   = EFI_ACPI_4_0_LOCAL_APIC_NMI;
  LocalApciNmiStruct.Length = sizeof (EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE);
  LocalApciNmiStruct.AcpiProcessorId = 0xFF;      // Applies to all processors
  LocalApciNmiStruct.Flags           = 0x000D;    // Flags - Level-tiggered, Active High
  LocalApciNmiStruct.LocalApicLint   = 0x1;

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &LocalApciNmiStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "CopyMadtStructure (APIC NMI) failed: %r\n", Status));
    goto Done;
  }

  //
  // Build Local x2APIC NMI Structure
  //
  LocalX2ApicNmiStruct.Type   = EFI_ACPI_4_0_LOCAL_X2APIC_NMI;
  LocalX2ApicNmiStruct.Length = sizeof (EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE);
  LocalX2ApicNmiStruct.Flags  = 0x000D;                // Flags - Level-tiggered, Active High
  LocalX2ApicNmiStruct.AcpiProcessorUid = 0xFFFFFFFF;  // Applies to all processors
  LocalX2ApicNmiStruct.LocalX2ApicLint  = 0x01;
  LocalX2ApicNmiStruct.Reserved[0] = 0x00;
  LocalX2ApicNmiStruct.Reserved[1] = 0x00;
  LocalX2ApicNmiStruct.Reserved[2] = 0x00;

  ASSERT (MadtStructsIndex < MaxMadtStructCount);
  Status = CopyStructure (
    &MadtTableHeader.Header,
    (STRUCTURE_HEADER *) &LocalX2ApicNmiStruct,
    &MadtStructs[MadtStructsIndex++]
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "CopyMadtStructure (x2APIC NMI) failed: %r\n", Status));
    goto Done;
  }

  //
  // Build Madt Structure from the Madt Header and collection of pointers in MadtStructs[]
  //
  Status = BuildAcpiTable (
    (EFI_ACPI_DESCRIPTION_HEADER *) &MadtTableHeader,
    sizeof (EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER),
    MadtStructs,
    MadtStructsIndex,
    (UINT8 **)&NewMadtTable
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "BuildAcpiTable failed: %r\n", Status));
    goto Done;
  }

  //
  // Publish Madt Structure to ACPI
  //
  Status = mAcpiTable->InstallAcpiTable (
    mAcpiTable,
    NewMadtTable,
    NewMadtTable->Header.Length,
    &TableHandle
    );

Done:
  //
  // Free memory
  //
  for (MadtStructsIndex = 0; MadtStructsIndex < MaxMadtStructCount; MadtStructsIndex++) {
    if (MadtStructs[MadtStructsIndex] != NULL) {
      FreePool (MadtStructs[MadtStructsIndex]);
    }
  }

  FreePool (MadtStructs);

  if (NewMadtTable != NULL) {
    FreePool (NewMadtTable);
  }

  return Status;
}

EFI_STATUS
InstallMcfgFromScratch (
  VOID
  )
{
  EFI_STATUS                                                                            Status;
  EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER                        *McfgTable;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE *Segment;
  UINTN                                                                                 Index;
  UINTN                                                                                 SegmentCount;
  PCI_SEGMENT_INFO                                                                      *PciSegmentInfo;
  UINTN                                                                                 TableHandle;

  PciSegmentInfo = GetPciSegmentInfo (&SegmentCount);

  McfgTable = AllocateZeroPool (
                sizeof(EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER) +
                sizeof(EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE) * SegmentCount
                );
  if (McfgTable == NULL) {
    DEBUG ((DEBUG_ERROR, "Could not allocate MCFG structure\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = InitializeHeader (
    &McfgTable->Header,
    EFI_ACPI_3_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE,
    EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_SPACE_ACCESS_TABLE_REVISION,
    0
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Set MCFG table "Length" field based on the number of PCIe segments enumerated so far
  //
  McfgTable->Header.Length = (UINT32)(sizeof (EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER) + 
                                      sizeof (EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE) * SegmentCount);

  Segment = (VOID *)(McfgTable + 1);

  for (Index = 0; Index < SegmentCount; Index++) {
    Segment[Index].PciSegmentGroupNumber  = PciSegmentInfo[Index].SegmentNumber;
    Segment[Index].BaseAddress    = PciSegmentInfo[Index].BaseAddress;
    Segment[Index].StartBusNumber = PciSegmentInfo[Index].StartBusNumber;
    Segment[Index].EndBusNumber   = PciSegmentInfo[Index].EndBusNumber;
  }

  //
  // Publish Madt Structure to ACPI
  //
  Status = mAcpiTable->InstallAcpiTable (
    mAcpiTable,
    McfgTable,
    McfgTable->Header.Length,
    &TableHandle
    );

  return Status;
}

/**
  This function will update any runtime platform specific information.
  This currently includes:
    Setting OEM table values, ID, table ID, creator ID and creator revision.
    Enabling the proper processor entries in the APIC tables
  It also indicates with which ACPI table version the table belongs.

  @param[in] Table        The table to update
  @param[in] Version      Where to install this table

  @retval EFI_SUCCESS     Updated tables commplete.
**/
EFI_STATUS
PlatformUpdateTables (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  EFI_ACPI_DESCRIPTION_HEADER               *TableHeader;
  UINT8                                     *TempOemId;
  UINT64                                    TempOemTableId;
  EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE *FadtHeader;
  EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER *HpetTable;
  UINT32                                           HpetBaseAddress;
  EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_BLOCK_ID     HpetBlockId;
  UINT32                                           HpetCapabilitiesData;
  HPET_GENERAL_CAPABILITIES_ID_REGISTER            HpetCapabilities;

  TableHeader             = NULL;

  //
  // By default, a table belongs in all ACPI table versions published.
  // Some tables will override this because they have different versions of the table.
  //
  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

  //
  // Update the OEM and creator information for every table except FACS.
  //
  if (Table->Signature != EFI_ACPI_1_0_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE) {
    TempOemId = (UINT8 *)PcdGetPtr(PcdAcpiDefaultOemId);
    CopyMem (&TableHeader->OemId, TempOemId, 6);

    //
    // Skip OEM table ID and creator information for DSDT, SSDT and PSDT tables, since these are
    // created by an ASL compiler and the creator information is useful.
    //
    if (Table->Signature != EFI_ACPI_1_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE &&
        Table->Signature != EFI_ACPI_1_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE &&
        Table->Signature != EFI_ACPI_1_0_PERSISTENT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE
        ) {
      TempOemTableId = PcdGet64(PcdAcpiDefaultOemTableId);
      CopyMem (&TableHeader->OemTableId, &TempOemTableId, 8);

      //
      // Update the creator ID
      //
      TableHeader->CreatorId = PcdGet32(PcdAcpiDefaultCreatorId);

      //
      // Update the creator revision
      //
      TableHeader->CreatorRevision = PcdGet32(PcdAcpiDefaultCreatorRevision);
    }
  }


  //
  // By default, a table belongs in all ACPI table versions published.
  // Some tables will override this because they have different versions of the table.
  //
  *Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  //
  // Update the various table types with the necessary updates
  //
  switch (Table->Signature) {

  case EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:
    ASSERT(FALSE);
    break;

  case EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
    FadtHeader    = (EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE *) Table;

    FadtHeader->PreferredPmProfile = PcdGet8 (PcdFadtPreferredPmProfile);
    FadtHeader->IaPcBootArch       = PcdGet16 (PcdFadtIaPcBootArch);
    FadtHeader->Flags              = PcdGet32 (PcdFadtFlags);

    FadtHeader->AcpiEnable  = PcdGet8 (PcdAcpiEnableSwSmi);
    FadtHeader->AcpiDisable = PcdGet8 (PcdAcpiDisableSwSmi);

    FadtHeader->Pm1aEvtBlk = PcdGet16 (PcdAcpiPm1AEventBlockAddress);
    FadtHeader->Pm1bEvtBlk = PcdGet16 (PcdAcpiPm1BEventBlockAddress);
    FadtHeader->Pm1aCntBlk = PcdGet16 (PcdAcpiPm1AControlBlockAddress);
    FadtHeader->Pm1bCntBlk = PcdGet16 (PcdAcpiPm1BControlBlockAddress);
    FadtHeader->Pm2CntBlk  = PcdGet16 (PcdAcpiPm2ControlBlockAddress);
    FadtHeader->PmTmrBlk   = PcdGet16 (PcdAcpiPmTimerBlockAddress);
    FadtHeader->Gpe0Blk    = PcdGet16 (PcdAcpiGpe0BlockAddress);
    FadtHeader->Gpe1Blk    = PcdGet16 (PcdAcpiGpe1BlockAddress);

    FadtHeader->XPm1aEvtBlk.Address = PcdGet16 (PcdAcpiPm1AEventBlockAddress);
    FadtHeader->XPm1bEvtBlk.Address = PcdGet16 (PcdAcpiPm1BEventBlockAddress);
    if (FadtHeader->XPm1bEvtBlk.Address == 0) {
      FadtHeader->XPm1bEvtBlk.AccessSize = 0;
    }
    FadtHeader->XPm1aCntBlk.Address = PcdGet16 (PcdAcpiPm1AControlBlockAddress);
    FadtHeader->XPm1bCntBlk.Address = PcdGet16 (PcdAcpiPm1BControlBlockAddress);
    if (FadtHeader->XPm1bCntBlk.Address == 0) {
      FadtHeader->XPm1bCntBlk.AccessSize = 0;
    }
    FadtHeader->XPm2CntBlk.Address  = PcdGet16 (PcdAcpiPm2ControlBlockAddress);
    //if (FadtHeader->XPm2CntBlk.Address == 0) {
      FadtHeader->XPm2CntBlk.AccessSize = 0;
    //}
    FadtHeader->XPmTmrBlk.Address   = PcdGet16 (PcdAcpiPmTimerBlockAddress);
    FadtHeader->XGpe0Blk.Address    = PcdGet16 (PcdAcpiGpe0BlockAddress);
    FadtHeader->XGpe1Blk.Address    = PcdGet16 (PcdAcpiGpe1BlockAddress);
    if (FadtHeader->XGpe1Blk.Address == 0) {
      FadtHeader->XGpe1Blk.AccessSize = 0;
    }

    DEBUG(( EFI_D_ERROR, "ACPI FADT table @ address 0x%x\n", Table ));
    DEBUG(( EFI_D_ERROR, "  IaPcBootArch 0x%x\n", FadtHeader->IaPcBootArch ));
    DEBUG(( EFI_D_ERROR, "  Flags 0x%x\n", FadtHeader->Flags ));
    break;

  case EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE:
    HpetTable = (EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER *)Table;
    HpetBaseAddress = PcdGet32 (PcdHpetBaseAddress);
    HpetTable->BaseAddressLower32Bit.Address = HpetBaseAddress;
    HpetTable->BaseAddressLower32Bit.RegisterBitWidth = 0;
    HpetCapabilitiesData     = MmioRead32 (HpetBaseAddress + HPET_GENERAL_CAPABILITIES_ID_OFFSET);
    HpetCapabilities.Uint64  = HpetCapabilitiesData;
    HpetCapabilitiesData     = MmioRead32 (HpetBaseAddress + HPET_GENERAL_CAPABILITIES_ID_OFFSET + 4);
    HpetCapabilities.Uint64 |= LShiftU64 (HpetCapabilitiesData, 32);
    HpetBlockId.Bits.Revision       = HpetCapabilities.Bits.Revision;
    HpetBlockId.Bits.NumberOfTimers = HpetCapabilities.Bits.NumberOfTimers;
    HpetBlockId.Bits.CounterSize    = HpetCapabilities.Bits.CounterSize;
    HpetBlockId.Bits.Reserved       = 0;
    HpetBlockId.Bits.LegacyRoute    = HpetCapabilities.Bits.LegacyRoute;
    HpetBlockId.Bits.VendorId       = HpetCapabilities.Bits.VendorId;
    HpetTable->EventTimerBlockId    = HpetBlockId.Uint32;
    HpetTable->MainCounterMinimumClockTickInPeriodicMode = (UINT16)HpetCapabilities.Bits.CounterClockPeriod;
    DEBUG(( EFI_D_ERROR, "ACPI HPET table @ address 0x%x\n", Table ));
    DEBUG(( EFI_D_ERROR, "  HPET base 0x%x\n", PcdGet32 (PcdHpetBaseAddress) ));
    break;

  case EFI_ACPI_3_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
    ASSERT(FALSE);
    break;

  default:
    break;
  }
  return EFI_SUCCESS;
}

/**
  This function calculates RCR based on PCI Device ID and Vendor ID from the devices
  available on the platform.
  It also includes other instances of BIOS change to calculate CRC and provides as
  HWSignature filed in FADT table.
**/
VOID
IsHardwareChange (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT32                        CRC;
  UINT32                        *HWChange;
  UINTN                         HWChangeSize;
  UINT32                        PciId;
  UINTN                         Handle;
  EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *FacsPtr;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE    *pFADT;

  HandleCount  = 0;
  HandleBuffer = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return; // PciIO protocol not installed yet!
  }

  //
  // Allocate memory for HWChange and add additional entrie for
  // pFADT->XDsdt
  //
  HWChangeSize = HandleCount + 1;
  HWChange = AllocateZeroPool( sizeof(UINT32) * HWChangeSize );
  ASSERT( HWChange != NULL );

  if (HWChange == NULL) return;

  //
  // add HWChange inputs: PCI devices
  //
  for (Index = 0; HandleCount > 0; HandleCount--) {
    PciId = 0;
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
    if (!EFI_ERROR (Status)) {
      Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, 0, 1, &PciId);
      if (EFI_ERROR (Status)) {
        continue;
      }
      HWChange[Index++] = PciId;
    }
  }

  //
  // Locate FACP Table
  //
  Handle = 0;
  Status = LocateAcpiTableBySignature (
              EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
              (EFI_ACPI_DESCRIPTION_HEADER **) &pFADT,
              &Handle
              );
  if (EFI_ERROR (Status) || (pFADT == NULL)) {
    return;  //Table not found or out of memory resource for pFADT table
  }

  //
  // add HWChange inputs: others
  //
  HWChange[Index++] = (UINT32)pFADT->XDsdt;

  //
  // Calculate CRC value with HWChange data.
  //
  Status = gBS->CalculateCrc32(HWChange, HWChangeSize, &CRC);
  DEBUG((DEBUG_INFO, "CRC = %x and Status = %r\n", CRC, Status));

  //
  // Set HardwareSignature value based on CRC value.
  //
  FacsPtr = (EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *)(UINTN)pFADT->FirmwareCtrl;
  FacsPtr->HardwareSignature = CRC;
  FreePool( HWChange );
}

VOID
UpdateLocalTable (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  EFI_ACPI_TABLE_VERSION        Version;
  UINTN                         TableHandle;
  UINTN                         Index;

  for (Index = 0; Index < sizeof(mLocalTable)/sizeof(mLocalTable[0]); Index++) {
    CurrentTable = mLocalTable[Index];

    PlatformUpdateTables (CurrentTable, &Version);

    TableHandle = 0;

    if (Version != EFI_ACPI_TABLE_VERSION_NONE) {
      Status = mAcpiTable->InstallAcpiTable (
                              mAcpiTable,
                              CurrentTable,
                              CurrentTable->Length,
                              &TableHandle
                              );
      ASSERT_EFI_ERROR (Status);
    }
  }
}


VOID
EFIAPI
AcpiEndOfDxeEvent (
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
  )
{

  if (Event != NULL) {
    gBS->CloseEvent(Event);
  }


  //
  // Calculate Hardware Signature value based on current platform configurations
  //
  IsHardwareChange();
}

/**
  ACPI Platform driver installation function.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval EFI_ABORTED        The driver encountered an error and could not complete installation of
                             the ACPI tables.

**/
EFI_STATUS
EFIAPI
InstallAcpiPlatform (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_EVENT                     EndOfDxeEvent;


  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mMpService);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&mAcpiTable);
  ASSERT_EFI_ERROR (Status);

  //
  // Create an End of DXE event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiEndOfDxeEvent,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Determine the number of processors
  //
  mMpService->GetNumberOfProcessors (
              mMpService,
              &mNumberOfCPUs,
              &mNumberOfEnabledCPUs
              );
  ASSERT (mNumberOfCPUs <= MAX_CPU_NUM && mNumberOfEnabledCPUs >= 1);
  DEBUG ((DEBUG_INFO, "mNumberOfCPUs - %d\n", mNumberOfCPUs));
  DEBUG ((DEBUG_INFO, "mNumberOfEnabledCPUs - %d\n", mNumberOfEnabledCPUs));

  DEBUG ((DEBUG_INFO, "mX2ApicEnabled - 0x%x\n", mX2ApicEnabled));
  DEBUG ((DEBUG_INFO, "mForceX2ApicId - 0x%x\n", mForceX2ApicId));

  // support up to 64 threads/socket
  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;
  DEBUG ((DEBUG_INFO, "mNumOfBitShift - 0x%x\n", mNumOfBitShift));

  UpdateLocalTable ();

  InstallMadtFromScratch ();
  InstallMcfgFromScratch ();

  return EFI_SUCCESS;
}
