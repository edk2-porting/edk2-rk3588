/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// Statements that include other files
//

//
// Statements that include other header files
//
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PcdLib.h>

#include <Guid/MemoryMapData.h>
#include <Guid/GlobalVariable.h>

#include <IndustryStandard/HighPrecisionEventTimerTable.h>

#include <Platform.h>
#include <Acpi/GlobalNvsAreaDef.h>
#include <Protocol/IioUds.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/SerialIo.h>
#include <Protocol/DevicePath.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/AcpiAml.h>

#include <Guid/SocketMpLinkVariable.h>
#include <Guid/SocketIioVariable.h>
#include <Guid/SocketPowermanagementVariable.h>
#include <Guid/SocketCommonRcVariable.h>

#include "Register/PchRegsUsb.h"

#include <PiDxe.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>

#include <IndustryStandard/Acpi.h>

#include <Protocol/MpService.h>

#include <Protocol/AcpiSystemDescriptionTable.h>

extern BOOLEAN                     mCpuOrderSorted;

typedef struct {
    char                   *Pathname;      /* Full pathname (from root) to the object */
    unsigned short         ParentOpcode;   /* AML opcode for the parent object */
    unsigned long          NamesegOffset;  /* Offset of last nameseg in the parent namepath */
    unsigned char          Opcode;         /* AML opcode for the data */
    unsigned long          Offset;         /* Offset for the data */
    unsigned long long     Value;          /* Original value of the data (as applicable) */
} AML_OFFSET_TABLE_ENTRY;

extern AML_OFFSET_TABLE_ENTRY      *mAmlOffsetTablePointer;
extern AML_OFFSET_TABLE_ENTRY      DSDT_PLATWFP__OffsetTable[];

#define AML_NAME_OP           0x08
#define AML_NAME_PREFIX_SIZE  0x06
#define AML_NAME_DWORD_SIZE   0x0C

#define MEM_ADDR_SHFT_VAL         26    // For 64 MB granularity

#pragma pack(1)

typedef struct {
    UINT8                           DescriptorType;
    UINT16                          ResourceLength;
    UINT8                           ResourceType;
    UINT8                           Flags;
    UINT8                           SpecificFlags;
    UINT64                          Granularity;
    UINT64                          Minimum;
    UINT64                          Maximum;
    UINT64                          TranslationOffset;
    UINT64                          AddressLength;
} AML_RESOURCE_ADDRESS64;


typedef struct {
    UINT8                           DescriptorType;
    UINT16                          ResourceLength;
    UINT8                           ResourceType;
    UINT8                           Flags;
    UINT8                           SpecificFlags;
    UINT32                          Granularity;
    UINT32                          Minimum;
    UINT32                          Maximum;
    UINT32                          TranslationOffset;
    UINT32                          AddressLength;
} AML_RESOURCE_ADDRESS32;


typedef struct {
    UINT8                           DescriptorType;
    UINT16                          ResourceLength;
    UINT8                           ResourceType;
    UINT8                           Flags;
    UINT8                           SpecificFlags;
    UINT16                          Granularity;
    UINT16                          Minimum;
    UINT16                          Maximum;
    UINT16                          TranslationOffset;
    UINT16                          AddressLength;
} AML_RESOURCE_ADDRESS16;

#pragma pack()

#define PCIE_PORT_4_DEV   0x00
#define PCIE_PORT_5_DEV   0x00

#define PORTS_PER_SOCKET   0x0F
#define PCIE_PORT_ALL_FUNC 0x00

typedef struct _PCIE_PORT_INFO {
    UINT8       Device;
    UINT8       Stack;
} PCIE_PORT_INFO;

#pragma optimize("",off)

extern BIOS_ACPI_PARAM             *mAcpiParameter;

extern struct SystemMemoryMapHob   *mSystemMemoryMap;
extern EFI_IIO_UDS_PROTOCOL        *mIioUds;


extern SOCKET_MP_LINK_CONFIGURATION  mSocketMpLinkConfiguration;
extern SOCKET_IIO_CONFIGURATION     mSocketIioConfiguration;
extern SOCKET_POWERMANAGEMENT_CONFIGURATION mSocketPowermanagementConfiguration;

extern UINT32                         mNumOfBitShift;

AML_OFFSET_TABLE_ENTRY            *mAmlOffsetTablePointer = DSDT_PLATWFP__OffsetTable;

/**

    Update the DSDT table

    @param *TableHeader   - The table to be set

    @retval EFI_SUCCESS - DSDT updated
    @retval EFI_INVALID_PARAMETER - DSDT not updated

**/
EFI_STATUS
PatchDsdtTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  )
{
  PCIE_PORT_INFO PCIEPortDefaults[] = {
    // DMI/PCIE 0
    { PCIE_PORT_0_DEV, IIO_CSTACK },
    //IOU0
    { PCIE_PORT_1A_DEV, IIO_PSTACK0 },
    { PCIE_PORT_1B_DEV, IIO_PSTACK0 },
    { PCIE_PORT_1C_DEV, IIO_PSTACK0 },
    { PCIE_PORT_1D_DEV, IIO_PSTACK0 },
    //IOU1
    { PCIE_PORT_2A_DEV, IIO_PSTACK1 },
    { PCIE_PORT_2B_DEV, IIO_PSTACK1 },
    { PCIE_PORT_2C_DEV, IIO_PSTACK1 },
    { PCIE_PORT_2D_DEV, IIO_PSTACK1 },
    //IOU2
    { PCIE_PORT_3A_DEV, IIO_PSTACK2 },
    { PCIE_PORT_3B_DEV, IIO_PSTACK2 },
    { PCIE_PORT_3C_DEV, IIO_PSTACK2 },
    { PCIE_PORT_3D_DEV, IIO_PSTACK2 },
    //MCP0 and MCP1
    { PCIE_PORT_4_DEV, IIO_PSTACK3 },
    { PCIE_PORT_5_DEV, IIO_PSTACK4 }
  };
  EFI_STATUS Status;
  UINT8   *DsdtPointer;
  UINT32  *Signature;
  UINT32  Fixes, NodeIndex;
  UINT8   Counter;
  UINT16  i;  // DSDT_PLATEXRP_OffsetTable LUT entries extends beyond 256!
  UINT64  MemoryBaseLimit = 0;
  UINT64  PciHGPEAddr = 0;
  UINT64  BusDevFunc = 0;
  UINT64  PcieHpBus = 0;
  UINT64  PcieHpDev = 0;
  UINT64  PcieHpFunc= 0;
  UINT8   PortCount = 0;
  UINT8   StackNumBus = 0;
  UINT8   StackNumIo = 0;
  UINT8   StackNumMem32 = 0;
  UINT8   StackNumMem64 = 0;
  UINT8   StackNumVgaIo0 = 1; // Start looking for Stack 1
  UINT8   StackNumVgaIo1 = 1; // Start looking for Stack 1
  UINT8   StackNumVgaMmioL = 0;
  UINT8   Stack = 0;
  UINT8   CurrSkt = 0, CurrStack = 0;
  UINT64  IioBusIndex = 0;
  UINT8   BusBase = 0, BusLimit = 0;
  UINT16  IoBase  = 0, IoLimit  = 0;
  UINT32  MemBase32 = 0, MemLimit32 = 0;
  UINT64  MemBase64 = 0, MemLimit64 = 0;
  AML_RESOURCE_ADDRESS16 *AmlResourceAddress16Pointer;
  AML_RESOURCE_ADDRESS32 *AmlResourceAddress32Pointer;
  AML_RESOURCE_ADDRESS64 *AmlResourceAddress64Pointer;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;

  Status = EFI_SUCCESS;
  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *)Table;

  if (mAmlOffsetTablePointer == NULL) return EFI_INVALID_PARAMETER;

  mAcpiParameter->MemoryBoardBitMask = 0;

  for(Counter = 0; Counter < mSystemMemoryMap->numberEntries; Counter++) {
    NodeIndex = mSystemMemoryMap->Element[Counter].NodeId;
    if((mAcpiParameter->MemoryBoardBitMask) & (1 << NodeIndex)){
      MemoryBaseLimit = mAcpiParameter->MemoryBoardRange[NodeIndex] + LShiftU64(mSystemMemoryMap->Element[Counter].ElementSize, MEM_ADDR_SHFT_VAL);
      mAcpiParameter->MemoryBoardRange[NodeIndex] = MemoryBaseLimit;
    } else {
      mAcpiParameter->MemoryBoardBitMask |= 1 << NodeIndex;
      MemoryBaseLimit = LShiftU64(mSystemMemoryMap->Element[Counter].BaseAddress, 30);
      mAcpiParameter->MemoryBoardBase[NodeIndex] = MemoryBaseLimit;
      MemoryBaseLimit = LShiftU64((mSystemMemoryMap->Element[Counter].BaseAddress + mSystemMemoryMap->Element[Counter].ElementSize), MEM_ADDR_SHFT_VAL);
      mAcpiParameter->MemoryBoardRange[NodeIndex] = MemoryBaseLimit;
    }
  }

  //
  // Mark all spare memory controllers as 1 in MemSpareMask bitmap.
  //
  mAcpiParameter->MemSpareMask = ~mAcpiParameter->MemoryBoardBitMask;

  mAcpiParameter->IioPresentBitMask = 0;
  mAcpiParameter->SocketBitMask = 0;

  for (Counter = 0; Counter < MAX_SOCKET; Counter++) {
    if (!mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Counter].Valid) continue;
    mAcpiParameter->SocketBitMask |= 1 << Counter;
    mAcpiParameter->IioPresentBitMask |= LShiftU64(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Counter].stackPresentBitmap, (Counter * 8));
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      mAcpiParameter->BusBase[Counter * MAX_IIO_STACK + Stack] = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Counter].StackRes[Stack].BusBase;
    }
  }

  PciHGPEAddr           = mIioUds->IioUdsPtr->PlatformData.PciExpressBase + 0x188;
  BusDevFunc            = 0x00;
  PcieHpBus             = 0;
  PcieHpDev             = 0;
  PcieHpFunc            = 0;

  Fixes = 0;
  //
  // Loop through the AML looking for values that we must fix up.
  //
  for (i = 0; mAmlOffsetTablePointer[i].Pathname != 0; i++) {
    //
    // Point to offset in DSDT for current item in AmlOffsetTable.
    //
    DsdtPointer = (UINT8 *) (TableHeader) + mAmlOffsetTablePointer[i].Offset;

    if (mAmlOffsetTablePointer[i].Opcode == AML_DWORD_PREFIX) {
      //
      // If Opcode is 0x0C, then operator is Name() or OperationRegion().
      // (TableHeader + AmlOffsetTable.Offset) is at offset for value to change.
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(DsdtPointer[-1] == mAmlOffsetTablePointer[i].Opcode);
      //
      // AmlOffsetTable.Value has FIX tag, so check that to decide what to modify.
      //
      Signature = (UINT32 *) (&mAmlOffsetTablePointer[i].Value);
      switch (*Signature) {
        //
        // PSYS - "FIX0" OperationRegion() in Acpi\AcpiTables\Dsdt\CommonPlatform.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '0')):
          DEBUG ((DEBUG_INFO, "FIX0 - 0x%x\n", mAcpiParameter));
          * (UINT32 *) DsdtPointer = (UINT32) (UINTN) mAcpiParameter;
          Fixes++;
          break;
        //
        // "FIX8" OperationRegion() in Acpi\AcpiTables\Dsdt\PcieHp.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '8')):
          Stack = PCIEPortDefaults[PortCount % PORTS_PER_SOCKET].Stack;
          PcieHpBus = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioBusIndex].StackRes[Stack].BusBase;
          PcieHpDev = PCIEPortDefaults[PortCount % PORTS_PER_SOCKET].Device;
          PcieHpFunc = PCIE_PORT_ALL_FUNC;

          //DEBUG((DEBUG_ERROR,"IioBus = %x, hpDev = %x, HpFunc= %x\n",IioBusIndex, PcieHpDev,PcieHpFunc));
          PciHGPEAddr &= ~(0xFFFF000);    // clear bus device func numbers
          BusDevFunc = (PcieHpBus << 8) | (PcieHpDev << 3) | PcieHpFunc;
          * (UINT32 *) DsdtPointer = (UINT32) (UINTN) (PciHGPEAddr + (BusDevFunc << 12));
          //DEBUG((DEBUG_ERROR,", BusDevFunc= %x, PortCount = %x\n",BusDevFunc, PortCount));

          PortCount++;
          Fixes++;
          break;

        default:
          break;
      }
    } else if (mAmlOffsetTablePointer[i].Opcode == AML_INDEX_OP) {
      //
      // If Opcode is 0x88, then operator is WORDBusNumber() or WORDIO().
      // (TableHeader + AmlOffsetTable.Offset) must be cast to AML_RESOURCE_ADDRESS16 to change values.
      //
      AmlResourceAddress16Pointer = (AML_RESOURCE_ADDRESS16 *) (DsdtPointer);
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(AmlResourceAddress16Pointer->DescriptorType == mAmlOffsetTablePointer[i].Opcode);

      //
      // Last 4 chars of AmlOffsetTable.Pathname has FIX tag.
      //
      Signature = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 4);
      switch (*Signature) {
        //
        // "FIX1" BUS resource for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '1')):
          CurrSkt = StackNumBus / MAX_IIO_STACK;
          CurrStack = StackNumBus % MAX_IIO_STACK;
          BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].BusBase;
          BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].BusLimit;
          AmlResourceAddress16Pointer->Granularity = 0;
          if (BusLimit > BusBase) {
            AmlResourceAddress16Pointer->Minimum = (UINT16) BusBase;
            AmlResourceAddress16Pointer->Maximum = (UINT16) BusLimit;
            AmlResourceAddress16Pointer->AddressLength = (UINT16) (BusLimit - BusBase + 1);
          }
          //DEBUG((DEBUG_ERROR,", FIX1 BusBase = 0x%x, BusLimit = 0x%x\n",BusBase, BusLimit));
          StackNumBus++;
          Fixes++;
          break;

        //
        // "FIX2" IO resource for for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '2')):
          AmlResourceAddress16Pointer->Granularity = 0;
          CurrSkt = StackNumIo / MAX_IIO_STACK;
          CurrStack = StackNumIo % MAX_IIO_STACK;
          IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].PciResourceIoBase;
          IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].PciResourceIoLimit;
          if (IoLimit > IoBase) {
            AmlResourceAddress16Pointer->Minimum = (UINT16) IoBase;
            AmlResourceAddress16Pointer->Maximum = (UINT16) IoLimit;
            AmlResourceAddress16Pointer->AddressLength = (UINT16) (IoLimit - IoBase + 1);
          }
          //DEBUG((DEBUG_ERROR,", FIX2 IoBase = 0x%x, IoLimit = 0x%x\n",IoBase, IoLimit));
          StackNumIo++;
          Fixes++;
          break;

        //
        // "FIX6" IO resource for PCXX in Acpi\AcpiTables\Dsdt\PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '6')):
          AmlResourceAddress16Pointer->Granularity = 0;
          CurrSkt = StackNumVgaIo0 / MAX_IIO_STACK;
          CurrStack = StackNumVgaIo0 % MAX_IIO_STACK;
          if ((mSocketMpLinkConfiguration.LegacyVgaSoc == CurrSkt) &&
              (mSocketMpLinkConfiguration.LegacyVgaStack == CurrStack)){
              AmlResourceAddress16Pointer->Minimum = (UINT16) 0x03b0;
              AmlResourceAddress16Pointer->Maximum = (UINT16) 0x03bb;
              AmlResourceAddress16Pointer->AddressLength = (UINT16) 0x000C;
          }
          StackNumVgaIo0++;
          Fixes++;
         break;

        //
        // "FIX7" IO resource for PCXX in Acpi\AcpiTables\Dsdt\PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '7')):
          AmlResourceAddress16Pointer->Granularity = 0;
          CurrSkt = StackNumVgaIo1 / MAX_IIO_STACK;
          CurrStack = StackNumVgaIo1 % MAX_IIO_STACK;
          if ((mSocketMpLinkConfiguration.LegacyVgaSoc == CurrSkt) &&
              (mSocketMpLinkConfiguration.LegacyVgaStack == CurrStack)) {
        	  AmlResourceAddress16Pointer->Minimum = (UINT16) 0x03c0;
              AmlResourceAddress16Pointer->Maximum = (UINT16) 0x03df;
              AmlResourceAddress16Pointer->AddressLength = (UINT16) 0x0020;
          }
          StackNumVgaIo1++;
          Fixes++;
          break;

        default:
          break;
      }
    } else if (mAmlOffsetTablePointer[i].Opcode == AML_SIZE_OF_OP) {
      //
      // If Opcode is 0x87, then operator is DWORDMemory().
      // (TableHeader + AmlOffsetTable.Offset) must be cast to AML_RESOURCE_ADDRESS32 to change values.
      //
      AmlResourceAddress32Pointer = (AML_RESOURCE_ADDRESS32 *) (DsdtPointer);
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(AmlResourceAddress32Pointer->DescriptorType == mAmlOffsetTablePointer[i].Opcode);
      //
      // Last 4 chars of AmlOffsetTable.Pathname has FIX tag.
      //
      Signature = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 4);
      switch (*Signature) {
        //
        // "FIX3" PCI32 resource for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '3')):
          AmlResourceAddress32Pointer->Granularity = 0;
          CurrSkt = StackNumMem32 / MAX_IIO_STACK;
          CurrStack = StackNumMem32 % MAX_IIO_STACK;
          MemBase32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].PciResourceMem32Base;
          MemLimit32 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].PciResourceMem32Limit;
          if (MemLimit32 > MemBase32) {
            AmlResourceAddress32Pointer->Minimum = (UINT32) MemBase32;
            AmlResourceAddress32Pointer->Maximum = (UINT32) MemLimit32;
            AmlResourceAddress32Pointer->AddressLength = (UINT32) (MemLimit32 - MemBase32 + 1);
          }
          //DEBUG((DEBUG_ERROR,", FIX3 MemBase32 = 0x%08x, MemLimit32 = 0x%08x\n",MemBase32, MemLimit32));
          StackNumMem32++;
          Fixes++;
          break;

        //
        // "FIX5" IO resource for PCXX in Acpi\AcpiTables\Dsdt\PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '5')):
          AmlResourceAddress32Pointer->Granularity = 0;
          CurrSkt = StackNumVgaMmioL / MAX_IIO_STACK;
          CurrStack = StackNumVgaMmioL % MAX_IIO_STACK;
          if ((mSocketMpLinkConfiguration.LegacyVgaSoc == CurrSkt) &&
              (mSocketMpLinkConfiguration.LegacyVgaStack == CurrStack)) {
              AmlResourceAddress32Pointer->Minimum = 0x000a0000;
              AmlResourceAddress32Pointer->Maximum = 0x000bffff;
              AmlResourceAddress32Pointer->AddressLength = 0x00020000;
          }
          StackNumVgaMmioL++;
          Fixes++;
          break;

        default:
          break;
      }
    } else if (mAmlOffsetTablePointer[i].Opcode == AML_CREATE_DWORD_FIELD_OP) {
      //
      // If Opcode is 0x8A, then operator is QWORDMemory().
      // (TableHeader + AmlOffsetTable.Offset) must be cast to AML_RESOURCE_ADDRESS64 to change values.
      //
      AmlResourceAddress64Pointer = (AML_RESOURCE_ADDRESS64 *) (DsdtPointer);
      //
      // The assert below confirms that AML structure matches the offsets table.
      // If not then patching the AML would just corrupt it and result in OS failure.
      // If you encounter this assert something went wrong in *.offset.h files
      // generation. Remove the files and rebuild.
      //
      ASSERT(AmlResourceAddress64Pointer->DescriptorType == mAmlOffsetTablePointer[i].Opcode);
      //
      // Last 4 chars of AmlOffsetTable.Pathname has FIX tag.
      //
      Signature = (UINT32 *) (mAmlOffsetTablePointer[i].Pathname + AsciiStrLen(mAmlOffsetTablePointer[i].Pathname) - 4);
      switch (*Signature) {
        //
        // "FIX4" PCI64 resource for PCXX in Acpi\AcpiTables\Dsdt\SysBus.asi and PCXX.asi
        //
        case (SIGNATURE_32 ('F', 'I', 'X', '4')):
          DEBUG((DEBUG_ERROR,"Pci64BitResourceAllocation = 0x%x\n",mSocketIioConfiguration.Pci64BitResourceAllocation));
          if (mSocketIioConfiguration.Pci64BitResourceAllocation) {
            AmlResourceAddress64Pointer->Granularity = 0;
            CurrSkt = StackNumMem64 / MAX_IIO_STACK;
            CurrStack = StackNumMem64 % MAX_IIO_STACK;
            MemBase64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].PciResourceMem64Base;
            MemLimit64 = mIioUds->IioUdsPtr->PlatformData.IIO_resource[CurrSkt].StackRes[CurrStack].PciResourceMem64Limit;
            if (MemLimit64 > MemBase64) {
              AmlResourceAddress64Pointer->Minimum = (UINT64) MemBase64;
              AmlResourceAddress64Pointer->Maximum = (UINT64) MemLimit64;
              AmlResourceAddress64Pointer->AddressLength = (UINT64) (MemLimit64 - MemBase64 + 1);
            }
            DEBUG((DEBUG_ERROR,", FIX4 MemBase64 = 0x%x, MemLimit64 = 0x%x\n",MemBase64, MemLimit64));
            StackNumMem64++;
            Fixes++;
          }
          break;
        default:
          break;
      }
    }
  }

  //return Status;
  return EFI_SUCCESS;

}
