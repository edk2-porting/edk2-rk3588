/** @file
  Unified Silicon Register Access Types

  @copyright
  Copyright 2011 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __USRA_ACCESS_TYPE_H__
#define __USRA_ACCESS_TYPE_H__

typedef enum {
  AddrTypePCIE = 0,
  AddrTypePCIEBLK,
  AddrTypeCSR,
  AddrTypePCIIO,
  AddrTypeCSRMEM,
  AddrTypeCSRCFG,
  AddrTypeMaximum
} USRA_ADDR_TYPE;

typedef enum {
  CsrBoxInst = 0,
  CsrMcId,
  CsrChId,
  CsrIoId,
  InstTypeMax
} CSR_INSTANCE_TYPE;

typedef enum {
  UsraWidth8 = 0,
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

#define USRA_ENABLE         1;
#define USRA_DISABLE        0;

#define PCI_CONFIGURATION_ADDRESS_PORT  0xCF8
#define PCI_CONFIGURATION_DATA_PORT     0xCFC

#pragma pack (1)

typedef struct {
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

typedef struct {
  UINT32              Offset:12;          // The PCIE Register Offset
  UINT32              Func:3;             // The PCIE Function
  UINT32              Dev:5;              // The PCIE Device
  UINT32              Bus:8;              // The PCIE Bus
  UINT32              RFU1:4;             // Reserved for User use or Future Use

  UINT32              Seg:16;             // The PCI Segment
  UINT32              Count:16;           // Access Count

} USRA_PCIE_ADDR_TYPE;

#define BUS_DEV_FUN_OFFSET_MASK    0x0FFFFFFF

typedef struct {
  UINT32              Offset;             // This Offset  occupies 32 bits. It's platform code's responsibilty to define the meaning of specific
  // bits and use them accordingly.
  UINT32              InstId:8;           // The Box Instance, 0 based, Index/Port within the box, Set Index as 0 if the box has only one instances
  UINT32              SocketId:8;         // The Socket Id
  UINT32              InstType:8;         // The Instance Type
  UINT32              RFU:8;              // Reserved for User use or Future Ues
} USRA_CSR_ADDR_TYPE;

typedef struct {
  UINT32              Offset:8;           // The PCIIO Register Offset
  UINT32              Func:3;             // The PCIIO Function
  UINT32              Dev:5;              // The PCIIO Device
  UINT32              Bus:8;              // The PCIIO Bus
  UINT32              RFU:7;              // Reserved for User use or Future Use
  UINT32              EnableBit:1;        // The PCIIO Register Enable Bit
} USRA_PCIIO_ADDR_TYPE;

typedef struct {
  UINT32              Offset:32;          // The register offset
  UINT32              SocketId:8;         // The socket ID
  UINT32              MemBarId:8;         // The ID of the BAR
  UINT32              High64Split:1;      // Move address up to access top of 64 bit register
  UINT32              Reserved:15;        // Reserved for User use or Future Use
} USRA_CSR_MEM_ADDR_TYPE;

typedef struct {
  UINT32              Offset:32;          // The register offset
  UINT32              SocketId:7;         // The socket ID
  UINT32              Bus:8;              // Bus
  UINT32              Device:8;           // Device
  UINT32              Function:8;         // Function
  UINT32              High64Split:1;      // Move address up to access top of 64 bit register
} USRA_CSR_CFG_ADDR_TYPE;

#pragma pack()

typedef union {
  UINT32                  dwRawData[4];
  ADDR_ATTRIBUTE_TYPE     Attribute;          // The address attribute type.
  USRA_PCIE_ADDR_TYPE     Pcie;
  USRA_PCIE_ADDR_TYPE     PcieBlk;
  USRA_CSR_ADDR_TYPE      Csr;
  USRA_PCIIO_ADDR_TYPE    PciIo;
  USRA_CSR_MEM_ADDR_TYPE  CsrMem;
  USRA_CSR_CFG_ADDR_TYPE  CsrCfg;
} USRA_ADDRESS;

//
// Assemble macro for USRA_PCIE_ADDR_TYPE
//
#define USRA_PCIE_SEG_ADDRESS(Address, WIDTH, SEG, BUS, DEV, FUNC, OFFSET)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypePCIE; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Seg = (UINT32)(SEG); \
    ((USRA_ADDRESS *)(&Address))->Pcie.Bus = (UINT32)(BUS)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Dev = (UINT32)(DEV)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Func = (UINT32)(FUNC) & 0x07; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Offset = (UINT32)(OFFSET) & 0x0FFF

  //
  // Assemble macro for USRA_BDFO_ADDR_TYPE
  //
#define USRA_PCIE_SEG_BDFO_ADDRESS(Address, WIDTH, SEG, BDFO)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypePCIE; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Seg = (UINT32)(SEG); \
    ((USRA_ADDRESS *)(&Address))->Pcie.Bus = (UINT32)(BDFO >> 20)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Dev = (UINT32)(BDFO >> 15)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Func = (UINT32)(BDFO >> 12)  & 0x07; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Offset = (UINT32)(BDFO) & 0x0FFF

  //
  // Assemble macro for USRA_PCIE_SEG_LIB_ADDR_TYPE
  //
#define USRA_PCIE_SEG_LIB_ADDRESS(Address, PCISEGADDR, WIDTH)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypePCIE; \
    ((USRA_ADDRESS *)(&Address))->Pcie.Seg = (UINT32)((PCISEGADDR >> 32) & 0x0000FFFF); \
    ((USRA_ADDRESS *)(&Address))->Attribute.RawData32[0] = (UINT32)(PCISEGADDR & BUS_DEV_FUN_OFFSET_MASK)

  //
  // Assemble macro for USRA_PCIE_BLK_ADDR_TYPE
  //
#define USRA_BLOCK_PCIE_ADDRESS(Address, WIDTH, COUNT, SEG, BUS, DEV, FUNC, OFFSET)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypePCIEBLK; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Count = (UINT32)COUNT; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Seg = (UINT32)SEG; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Bus = (UINT32)(BUS)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Dev = (UINT32)(DEV)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Func = (UINT32)(FUNC) & 0x07; \
    ((USRA_ADDRESS *)(&Address))->PcieBlk.Offset = (UINT32)(OFFSET) & 0x0FFF
  //
  // Assemble macro for USRA_PCIE_SEG_ADDR_TYPE
  //
#define USRA_PCIE_ADDRESS(Address, WIDTH, BUS, DEV, FUNC, OFFSET)  \
    USRA_PCIE_SEG_ADDRESS(Address, WIDTH, 0, BUS, DEV, FUNC, OFFSET)

  //
  // Assemble macro for USRA CSR common Address
  //
#define USRA_CSR_COMMON_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Csr.SocketId = SOCKETID; \
    ((USRA_ADDRESS *)(&Address))->Csr.InstId = INSTID; \
    ((USRA_ADDRESS *)(&Address))->Csr.Offset = CSROFFSET

  //
  // Assemble macro for address type CSR
  //
#define USRA_CSR_OFFSET_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET)  \
    USRA_CSR_COMMON_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET); \
    ((USRA_ADDRESS *)(&Address))->Csr.InstType = CsrBoxInst; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypeCSR

#define USRA_CSR_MCID_OFFSET_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET)  \
    USRA_CSR_COMMON_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET); \
    ((USRA_ADDRESS *)(&Address))->Csr.InstType = CsrMcId; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypeCSR

#define USRA_CSR_CHID_OFFSET_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET)  \
    USRA_CSR_COMMON_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET); \
    ((USRA_ADDRESS *)(&Address))->Csr.InstType = CsrChId; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypeCSR

#define USRA_CSR_IOID_OFFSET_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET)  \
    USRA_CSR_COMMON_ADDRESS(Address, SOCKETID, INSTID, CSROFFSET); \
    ((USRA_ADDRESS *)(&Address))->Csr.InstType = CsrIoId; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypeCSR

  //
  // Assemble macro for USRA_PCIIO_ADDR_TYPE
  //
#define  USRA_PCI_IO_ADDRESS(Address, WIDTH, BUS, DEV, FUNC, OFFSET)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypePCIIO; \
    ((USRA_ADDRESS *)(&Address))->PciIo.Bus = (UINT32)(BUS)  & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->PciIo.Dev = (UINT32)(DEV)  & 0x1F; \
    ((USRA_ADDRESS *)(&Address))->PciIo.Func = (UINT32)(FUNC) & 0x07; \
    ((USRA_ADDRESS *)(&Address))->PciIo.Offset = (UINT32)(OFFSET) & 0xFF; \
    ((USRA_ADDRESS *)(&Address))->PciIo.EnableBit = 1

  //
  // Assemble macro for USRA_BDFO_PCIIO_ADDR_TYPE
  //
#define USRA_PCI_IO_BDFO_ADDRESS(Address, WIDTH, BDFO)  \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypePCIIO; \
    ((USRA_ADDRESS *)(&Address))->Attribute.RawData32[0] = (UINT32)(BDFO) & 0xFFFFFF; \
    ((USRA_ADDRESS *)(&Address))->PciIo.EnableBit = 1

  //
  // Assemble macro for USRA_CSR_MMIO_ADDR_TYPE
  //
#define USRA_CSR_MEM_ADDRESS(Address, SOCKETID, MEMBARID, OFFSET, WIDTH) \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypeCSRMEM; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->CsrMem.SocketId = SOCKETID; \
    ((USRA_ADDRESS *)(&Address))->CsrMem.MemBarId = MEMBARID; \
    ((USRA_ADDRESS *)(&Address))->CsrMem.Offset = OFFSET; \
    ((USRA_ADDRESS *)(&Address))->CsrMem.High64Split = 0;

  //
  // Assemble macro for USRA_CSR_CFG_ADDR_TYPE
  //
#define USRA_CSR_CFG_ADDRESS(Address, SOCKETID, BUS, DEVICE, FUNCTION, OFFSET, WIDTH) \
    USRA_ZERO_ADDRESS(Address); \
    ((USRA_ADDRESS *)(&Address))->Attribute.AddrType = AddrTypeCSRCFG; \
    ((USRA_ADDRESS *)(&Address))->Attribute.AccessWidth = WIDTH; \
    ((USRA_ADDRESS *)(&Address))->CsrCfg.SocketId = SOCKETID; \
    ((USRA_ADDRESS *)(&Address))->CsrCfg.Bus = BUS; \
    ((USRA_ADDRESS *)(&Address))->CsrCfg.Device = DEVICE; \
    ((USRA_ADDRESS *)(&Address))->CsrCfg.Function = Function; \
    ((USRA_ADDRESS *)(&Address))->CsrCfg.Offset = OFFSET; \
    ((USRA_ADDRESS *)(&Address))->CsrCfg.High64Split = 0;

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

