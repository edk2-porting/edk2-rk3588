/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __USRA_ACCESS_TYPE_H__
#define __USRA_ACCESS_TYPE_H__

typedef enum {
  AddrTypePCIE    = 0,
  AddrTypePCIEBLK,
  AddrTypeCSR,
  AddrTypeMMIO,
  AddrTypeIO,
  AddrTypeMaximum
} USRA_ADDR_TYPE;

typedef enum {
  UsraWidth8  = 0,
  UsraWidth16,
  UsraWidth32,
  UsraWidth64,
  UsraWidthFifo8,
  UsraWidthFifo16,
  UsraWidthFifo32,
  UsraWidthFifo64,
  UsraWidthFill8,
  UsraWidthFill16,
  UsraWidthFill32,
  UsraWidthFill64,
  UsraWidthMaximum
} USRA_ACCESS_WIDTH;

typedef enum {
  CsrBoxInst = 0,
  CsrChId,
  CsrMcId,
  CsrSubTypeMax
} CSR_INST_TYPE;

#define USRA_ENABLE         1;
#define USRA_DISABLE        0;

#pragma pack (1)

typedef struct
  {
    UINT32              RawData32[2];       // RawData of two UINT32 type, place holder
    UINT32              AddrType:8;         // Address type: CSR, PCIE, MMIO, IO, SMBus ...
    UINT32              AccessWidth:4;      // The Access width for 8, 16,32,64 -bit access
    UINT32              FastBootEn:1;       // Fast Boot Flag, can be used to log register access trace for fast boot
    UINT32              S3Enable:1;         // S3 Enable bit, when enabled, it will save the write to script to support S3
    UINT32              HptrType:1;         // Host Pointer type, below or above 4GB
    UINT32              ConvertedType:1;    // The address type was from converted type, use this field for address migration support
    UINT32              RFU3:16;            // Reserved for User use or Future Use

    UINT32              HostPtr:32;         // The Host Pointer, to point to Attribute buffer etc.
} ADDR_ATTRIBUTE_TYPE;

typedef struct
  {
    UINT32              Offset:12;          // The PCIE Register Offset
    UINT32              Func:3;             // The PCIE Function
    UINT32              Dev:5;              // The PCIE Device
    UINT32              Bus:8;              // The PCIE Bus
    UINT32              RFU1:4;             // Reserved for User use or Future Use

    UINT32              Seg:16;             // The PCI Segment
    UINT32              Count:16;           // Access Count

} USRA_PCIE_ADDR_TYPE;

typedef struct
  {
    UINT32              Offset;             // This Offset  occupies 32 bits. It's platform code's responsibilty to define the meaning of specific
                                            // bits and use them accordingly.
    UINT32              InstId:8;           // The Box Instance, 0 based, Index/Port within the box, Set Index as 0 if the box has only one instances
    UINT32              SocketId:8;         // The socket Id
    UINT32              InstType:8;         // The Instance Type, it can be Box, Memory Channel etc.
    UINT32              RFU:8;              // Reserved for User use or Future Ues

} USRA_CSR_ADDR_TYPE;

typedef struct
  {
    UINT32              Offset:32;          // The MMIO Offset

    UINT32              OffsetH: 32;        // The MMIO Offset Higher 32-bit
} USRA_MMIO_ADDR_TYPE;

typedef struct
  {
    UINT32              Offset:16;          // The IO Offset
    UINT32              RFU1:16;            // Reserved for User use or Future Use

    UINT32              RFU2:32;            // Reserved for User use or Future Use

} USRA_IO_ADDR_TYPE;

#pragma pack()

typedef union {
    UINT32                  dwRawData[4];
    ADDR_ATTRIBUTE_TYPE     Attribute;          // The address attribute type.
    USRA_PCIE_ADDR_TYPE     Pcie;
    USRA_PCIE_ADDR_TYPE     PcieBlk;
    USRA_CSR_ADDR_TYPE      Csr;
    USRA_MMIO_ADDR_TYPE     Mmio;
    USRA_IO_ADDR_TYPE       Io;
} USRA_ADDRESS;

//
// Assemble macro for USRA_PCIE_ADDR_TYPE
//
#define USRA_PCIE_SEG_ADDRESS(Address, WIDTH, SEG, BUS, DEV, FUNC, OFFSET)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth  = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType     = AddrTypePCIE; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Seg    = (UINT32)(SEG); \
    ((USRA_ADDRESS *)(&Address))->Pcie.Bus    = (UINT32)(BUS)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Dev    = (UINT32)(DEV)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Func   = (UINT32)(FUNC) & 0x07; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Offset = (UINT32)(OFFSET) & 0x0FFF

//
// Assemble macro for USRA_BDFO_ADDR_TYPE
//
#define USRA_PCIE_SEG_BDFO_ADDRESS(Address, WIDTH, SEG, BDFO)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth  = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType     = AddrTypePCIE; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Seg    = (UINT32)(SEG); \
    ((USRA_ADDRESS *)(&Address))->Pcie.Bus    = (UINT32)(BDFO >> 20)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Dev    = (UINT32)(BDFO >> 15)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Func   = (UINT32)(BDFO >> 12)  & 0x07; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Offset = (UINT32)(BDFO) & 0x0FFF

//
// Assemble macro for USRA_PCIE_BLK_ADDR_TYPE
//
#define USRA_BLOCK_PCIE_ADDRESS(Address, WIDTH, COUNT, SEG, BUS, DEV, FUNC, OFFSET)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth  = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType     = AddrTypePCIEBLK; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Count  = (UINT32)COUNT; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Seg    = (UINT32)SEG; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Bus    = (UINT32)(BUS)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Dev    = (UINT32)(DEV)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Func   = (UINT32)(FUNC) & 0x07; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Offset = (UINT32)(OFFSET) & 0x0FFF
//
// Assemble macro for USRA_PCIE_SEG_ADDR_TYPE
//
#define USRA_PCIE_ADDRESS(Address, WIDTH, BUS, DEV, FUNC, OFFSET)  \
    USRA_PCIE_SEG_ADDRESS(Address, WIDTH, 0, BUS, DEV, FUNC, OFFSET)

//
// Assemble macro for USRA_CSR_ADDR_TYPE
//
#define USRA_CSR_OFFSET_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET, INSTTYPE)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType     = AddrTypeCSR; \
    ((USRA_ADDRESS *)(&Address))->Csr.InstType = INSTTYPE; \
    ((USRA_ADDRESS *)(&Address))->Csr.SocketId = SOCKETID; \
    ((USRA_ADDRESS *)(&Address))->Csr.InstId   = INSTID; \
    ((USRA_ADDRESS *)(&Address))->Csr.Offset   = CSROFFSET

//
// Assemble macro for ZERO_USRA ADDRESS
//
#define USRA_ZERO_ADDRESS(Address)  \
    ((UINT32 *)&Address)[3] = (UINT32)0; \
    ((UINT32 *)&Address)[2] = (UINT32)0; \
    ((UINT32 *)&Address)[1] = (UINT32)0; \
    ((UINT32 *)&Address)[0] = (UINT32)0

//
// Assemble macro for ZERO_ADDR_TYPE
//
#define USRA_ZERO_ADDRESS_TYPE(Address, AddressType)  \
    ((UINT32 *)&Address)[3] = (UINT32)0; \
    ((UINT32 *)&Address)[2] = (UINT32)((AddressType) & 0x0FF); \
    ((UINT32 *)&Address)[1] = (UINT32)0; \
    ((UINT32 *)&Address)[0] = (UINT32)0

#define USRA_ADDRESS_COPY(DestAddrPtr, SourceAddrPtr)  \
    ((UINT32 *)DestAddrPtr)[3] = ((UINT32 *)SourceAddrPtr)[3]; \
    ((UINT32 *)DestAddrPtr)[2] = ((UINT32 *)SourceAddrPtr)[2]; \
    ((UINT32 *)DestAddrPtr)[1] = ((UINT32 *)SourceAddrPtr)[1]; \
    ((UINT32 *)DestAddrPtr)[0] = ((UINT32 *)SourceAddrPtr)[0];

#endif

