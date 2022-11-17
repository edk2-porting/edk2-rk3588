/** @file
  Protocol used to support ACPI VT-d DMA remapping reporting

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __DMA_REMAP_H__
#define __DMA_REMAP_H__

//
// Protocol for GUID.
//
typedef struct _DMA_REMAP_PROTOCOL DMA_REMAP_PROTOCOL;

#define  DRHD_SIGNATURE  (('D'<<24) + ('R'<<16) + ('H'<<8) + 'D')
#define  RMRR_SIGNATURE  (('R'<<24) + ('M'<<16) + ('R'<<8) + 'R')
#define  ATSR_SIGNATURE  (('A'<<24) + ('T'<<16) + ('S'<<8) + 'R')
#define  RHSA_SIGNATURE  (('A'<<24) + ('S'<<16) + ('H'<<8) + 'R')
#define  ANDD_SIGNATURE  (('A'<<24) + ('N'<<16) + ('D'<<8) + 'D')

typedef enum {
  DrhdType,
  RmrrType,
  AtsrType,
  RhsaType
} REMAP_TYPE;

typedef enum {
  PciEndpoint = 1,
  PciBridge   = 2
} PCI_DEV_TYPE;

typedef struct {
  UINT8         Device;
  UINT8         Function;
} PCI_NODE;

typedef struct {
  UINT8         DeviceType;
  UINT8         EnumerationID;
  UINT8         StartBusNumber;
  PCI_NODE      *PciNode;
} DEVICE_SCOPE;

typedef struct {
  UINT32        Signature;
  UINT8         Flags;
  UINT16        SegmentNumber;
  UINT64        RegisterBase;
  UINTN         DeviceScopeNumber;
  DEVICE_SCOPE  *DeviceScope;
} DMAR_DRHD;

typedef struct {
  UINT32        Signature;
  UINT16        SegmentNumber;
  UINT64        RsvdMemBase;
  UINT64        RsvdMemLimit;
  UINTN         DeviceScopeNumber;
  DEVICE_SCOPE  *DeviceScope;
} DMAR_RMRR;

typedef struct {
  UINT32        Signature;
  UINT8         Flags;
  UINT16        SegmentNumber;
  UINTN         DeviceScopeNumber;
  UINT32        ATSRPresentBit;
  DEVICE_SCOPE  *DeviceScope;
} DMAR_ATSR;

typedef struct {
  UINT32        Signature;
  UINT64        RegisterBase;
  UINT32        Domian;
  UINT16        RhsaCount;
} DMAR_RHSA;

typedef
EFI_STATUS
(EFIAPI *INSERT_DMA_REMAP) (
  IN  DMA_REMAP_PROTOCOL                                        *This,
  IN  REMAP_TYPE                                                RemapType,
  IN  VOID                                                      *RemapEntry
  );

typedef
EFI_STATUS
(EFIAPI *GET_DMAR_TABLE) (
  IN  DMA_REMAP_PROTOCOL                                        *This,
  IN  VOID                                                      **DmarTable
  );

typedef struct _DMA_REMAP_PROTOCOL {
  BOOLEAN                   VTdSupport;
  BOOLEAN                   DmaCtrlOptIn;
  BOOLEAN                   InterruptRemap;
  BOOLEAN                   X2ApicOptOut;
  BOOLEAN                   ATS;
  INSERT_DMA_REMAP          InsertDmaRemap;
  GET_DMAR_TABLE            GetDmarTable;
} DMA_REMAP_PROTOCOL;

extern EFI_GUID gDmaRemapProtocolGuid;

#endif
