/** @file
  Si Config Block

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SI_CONFIG_H_
#define _SI_CONFIG_H_

#define SI_CONFIG_REVISION  3

extern EFI_GUID gSiConfigGuid;


#pragma pack (push,1)

/**
  The Silicon Policy allows the platform code to publish a set of configuration
  information that the RC drivers will use to configure the silicon hardware.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added TraceHubMemBase
  <b>Revision 3</b>
  - Deprecated SkipPostBootSai
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Offset 0 - 27 Config Block Header
  //
  // Platform specific common policies that used by several silicon components.
  //
  UINT32 CsmFlag          :  1;  ///< Offset 44 BIT0: CSM status flag.
  /**
    @deprecated since revision 3
  **/
  UINT32 SkipPostBootSai  :  1;
  UINT32 RsvdBits         : 30;  ///< Reserved
  UINT32 *SsidTablePtr;          // Offset 48
  UINT16 NumberOfSsidTableEntry; // Offset 52
  UINT16 Reserved;               // Offset 54
  /**
    If Trace Hub is enabled and trace to memory is desired, Platform code or BootLoader needs to allocate trace hub memory
    as reserved, and save allocated memory base to TraceHubMemBase to ensure Trace Hub memory is configured properly.
    To get total trace hub memory size please refer to TraceHubCalculateTotalBufferSize ()

    Noted: If EDKII memory service is used to allocate memory, it will require double memory size to support size-aligned memory allocation,
    so Platform code or FSP Wrapper code should ensure enough memory available for size-aligned TraceHub memory allocation.
  **/
  UINT32 TraceHubMemBase;        // Offset 58
} SI_CONFIG;

#pragma pack (pop)

#define DEFAULT_SSVID    0x8086
#define DEFAULT_SSDID    0x7270
#define MAX_DEVICE_COUNT 70

///
/// Subsystem Vendor ID / Subsystem ID
///
typedef struct {
  UINT16         SubSystemVendorId;
  UINT16         SubSystemId;
} SVID_SID_VALUE;

//
// Below is to match PCI_SEGMENT_LIB_ADDRESS () which can directly send to PciSegmentRead/Write functions.
//
typedef struct {
  union {
    struct {
      UINT64  Register:12;
      UINT64  Function:3;
      UINT64  Device:5;
      UINT64  Bus:8;
      UINT64  Reserved1:4;
      UINT64  Segment:16;
      UINT64  Reserved2:16;
    } Bits;
    UINT64    SegBusDevFuncRegister;
  } Address;
  SVID_SID_VALUE SvidSidValue;
  UINT32 Reserved;
} SVID_SID_INIT_ENTRY;

#endif // _SI_CONFIG_H_
