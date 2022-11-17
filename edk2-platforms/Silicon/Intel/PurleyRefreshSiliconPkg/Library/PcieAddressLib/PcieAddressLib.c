/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <SysHost.h>

#include "PcieAddress.h"

#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION) && !defined(SIM_BUILD)
#include <Library/DebugLib.h>
#endif

#ifdef _MSC_VER
#pragma optimize ("",off)
#endif //_MSC_VER
//////////////////////////////////////////////////////////////////////////
//
// Common Silicon Address Library
// This Lib provide the way use platform Library instance
//
//////////////////////////////////////////////////////////////////////////

PCIE_MMCFG_TABLE_TYPE mMmcfgTable =\
  {
    {
      {'M', 'C', 'F', 'G'},   // Signature
      0x00000090,             // Length
      0x01,                   // Revision
      0x08,                   // The Maximum number of Segments
      0x00FF,                 // Valid Segment Bit Map, LSB Bit0 for Seg0, bit1 for seg1 ...
      {0x00,0x00,0x00,0x00}   // Reserved
    },
    {{
      0, //MMCFG_BASE_ADDRESS,     // Base Address Low
      0x00000000,             // Base Address High
      0x0000,                 // Segment 0
      0x00,                   // Start Bus
      0xFF,                   // End Bus
      {0x00,0x00,0x00,0x00}   // Reserved
    }}
};
//
// Segment 1 ~ 7
//
PCIE_MMCFG_BASE_ADDRESS_TYPE mMmcfgAddr[] = \
{
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0001,                 // Segment 1
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0002,                 // Segment 2
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0003,                 // Segment 3
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },

  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0004,                 // Segment 4
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0005,                 // Segment 5
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },

  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0006,                 // Segment 6
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0007,                 // Segment 7
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  }
};


/**
  This Lib is used for platfor to set platform specific Pcie MMCFG Table

  @param  MmcfgTable: A pointer of the MMCFG Table structure for PCIE_MMCFG_TABLE_TYPE type.
  @param  NumOfSeg: Sumber of Segments in the table.

  @retval <>NULL The function completed successfully.
  @retval NULL Returen Error
**/
UINTN
SetSocketMmcfgTable (
  IN UINT8                SocketLastBus[],
  IN UINT8                SocketFirstBus[],
  IN UINT8                segmentSocket[],
  IN UINT32               mmCfgBaseH[],
  IN UINT32               mmCfgBaseL[],
  IN UINT8                NumOfSocket
  )
{
#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION) && !defined(SIM_BUILD)
  UINT32                  MmcfgTableSize;
  PCIE_MMCFG_TABLE_TYPE   *HobMmcfgTable;
  UINT8                   i, *Dest, *Source;

  union {
    UINT64  D64;
    UINT32  D32[2];
  } Data;

  MmcfgTableSize = sizeof(PCIE_MMCFG_HEADER_TYPE) + (NumOfSocket * sizeof(PCIE_MMCFG_BASE_ADDRESS_TYPE));

  HobMmcfgTable = (PCIE_MMCFG_TABLE_TYPE *) PcdGetPtr (PcdPcieMmcfgTablePtr);
  ASSERT (MmcfgTableSize < PcdGetSize (PcdPcieMmcfgTablePtr));

  Data.D64 = PcdGet64 (PcdPciExpressBaseAddress);
  mMmcfgTable.MmcfgBase[0].BaseAddressL = Data.D32[0];
  mMmcfgTable.MmcfgBase[0].BaseAddressH = Data.D32[1];

  //1. copy global variable mMcfgTable to HobMmcfgTable
  //   note that mMmcfgTable only has PCIE_MMCFG_BASE_ADDRESS_TYPE for segment 0 (for socket 0)
  //   need to copy base addresses for other segments corresponding to sockets 1 through NumOfSocket-1
  Dest = (UINT8*)HobMmcfgTable;
  Source = (UINT8*)&mMmcfgTable;
  for(i=0; i<sizeof(PCIE_MMCFG_TABLE_TYPE); i++)
  {
    Dest[i] = Source[i];
  }

  //2. copy remaining segments 1 to NumOfSocket-1 from global array to HobMmcfgTable
  if(NumOfSocket > 1){
    Dest = (UINT8*)&HobMmcfgTable->MmcfgBase[1];
    Source = (UINT8*)&mMmcfgAddr[0];//array of base addresses starting with segment 1 (max segment = 7)
    for(i = 0; i< (MmcfgTableSize - sizeof(PCIE_MMCFG_TABLE_TYPE)); i++){
      Dest[i] = Source[i];
    }
  }

  HobMmcfgTable->Header.Length = MmcfgTableSize;
  for(i=0; i<NumOfSocket; i++)
  {
    HobMmcfgTable->MmcfgBase[i].StartBus = SocketFirstBus[i];
    HobMmcfgTable->MmcfgBase[i].EndBus = SocketLastBus[i];
    HobMmcfgTable->MmcfgBase[i].Segment = segmentSocket[i];
    HobMmcfgTable->MmcfgBase[i].BaseAddressH = mmCfgBaseH[i];
    HobMmcfgTable->MmcfgBase[i].BaseAddressL = mmCfgBaseL[i];
  }
#endif
  return 0;
};


/**
  This Lib is used for platform to set platform specific Pcie MMCFG Table

  @param  MmcfgTable: A pointer of the MMCFG Table structure for PCIE_MMCFG_TABLE_TYPE type.
  @param  NumOfSeg: Sumber of Segments in the table.

  @retval <>NULL The function completed successfully.
  @retval NULL Returen Error
**/
UINTN
EFIAPI
SetPcieSegMmcfgTable (
  IN PCIE_MMCFG_TABLE_TYPE *MmcfgTable,
  IN UINT32                 NumOfSeg
  )
{
#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION) && !defined(SIM_BUILD)
  UINT32                  MmcfgTableSize;
  PCIE_MMCFG_TABLE_TYPE   *HobMmcfgTable;

  union {
    UINT64  D64;
    UINT32  D32[2];
  } Data;

  Data.D32[0] = Data.D32[1] = 0;
  MmcfgTableSize = sizeof(PCIE_MMCFG_HEADER_TYPE) + (NumOfSeg * sizeof(PCIE_MMCFG_BASE_ADDRESS_TYPE));

  HobMmcfgTable = (PCIE_MMCFG_TABLE_TYPE *) PcdGetPtr (PcdPcieMmcfgTablePtr);
  //ASSERT (MmcfgTableSize < PcdGetSize (PcdPcieMmcfgTablePtr));

  //InternalMemCopyMem(HobMmcfgTable, MmcfgTable, PcdGetSize (PcdPcieMmcfgTablePtr));
  MmcfgTable->Header.Length = MmcfgTableSize;
  if((MmcfgTable->MmcfgBase[0].BaseAddressL == 0) && (MmcfgTable->MmcfgBase[0].BaseAddressH == 0))
  {
    //
    // The first time default should be the PcdPciExpressBaseAddress
    //
    Data.D64 = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);
    HobMmcfgTable->MmcfgBase[0].BaseAddressL = Data.D32[0];
    HobMmcfgTable->MmcfgBase[0].BaseAddressH = Data.D32[1];
  };
#endif
  return 0;
};


/**
  This Lib return PCIE MMCFG Base Address

  @param  Address: A pointer of the address of the Common Address Structure for PCIE type.
  @param  Buffer: A pointer of buffer for the value read from platform.

  @retval <>NULL The function completed successfully.
  @retval NULL Returen Error
  **/

UINTN
EFIAPI
GetPcieSegMmcfgBaseAddress (
  IN USRA_ADDRESS    *Address
  )
{
  UINT32                        BaseAddressL=0;       // Processor-relative Base Address (Lower 32-bit) for the Enhanced Configuration Access Mechanism
  UINT32                        BaseAddressH=0;
  UINTN SegMmcfgBase;

#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION)
  PCIE_MMCFG_TABLE_TYPE         *MmcfgTable=NULL;
  union {
    UINTN   D64;
    UINT32  D32[2];
  } Data;
#endif
#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION)
  if(Address->Attribute.HostPtr == 0) {
    MmcfgTable = (PCIE_MMCFG_TABLE_TYPE *) PcdGetPtr (PcdPcieMmcfgTablePtr);
    if(MmcfgTable->Header.Length != 0)
    {
      BaseAddressH = MmcfgTable->MmcfgBase[Address->Pcie.Seg].BaseAddressH;
      BaseAddressL = MmcfgTable->MmcfgBase[Address->Pcie.Seg].BaseAddressL;
    } else {
    //
      // if it is not valid MMCFG pointer, initialize it to use the predefined default MMCFG Table
      //
      SetPcieSegMmcfgTable(&mMmcfgTable, PcdGet32 (PcdNumOfPcieSeg));
      BaseAddressH = mMmcfgTable.MmcfgBase[Address->Pcie.Seg].BaseAddressH;
      BaseAddressL = mMmcfgTable.MmcfgBase[Address->Pcie.Seg].BaseAddressL;

      if((BaseAddressL == 0) && (BaseAddressH == 0)){
        Data.D32[0] = Data.D32[1] = 0;
        Data.D64 = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);
        BaseAddressL = Data.D32[0];
        BaseAddressH = Data.D32[1];
      }
  }
  }
  else
#endif
  {
    BaseAddressH = 0;
    BaseAddressL = 0;
  }

  if((BaseAddressL == 0) && (BaseAddressH == 0))
  {

#if defined(MINIBIOS_BUILD) || defined(KTI_SW_SIMULATION)
    BaseAddressL = 0x80000000;
    BaseAddressH = 0;
#else
    //
    // The first time default should be the PcdPciExpressBaseAddress
    //
    Data.D32[0] = Data.D32[1] = 0;
    Data.D64 = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);
    BaseAddressL = Data.D32[0];
    BaseAddressH = Data.D32[1];
#endif
  }
  return SegMmcfgBase = BaseAddressL;

};

