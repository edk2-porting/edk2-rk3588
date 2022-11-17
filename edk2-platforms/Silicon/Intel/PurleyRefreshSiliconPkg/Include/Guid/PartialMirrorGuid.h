/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PARTIAL_MIRROR_GUID_H_
#define _PARTIAL_MIRROR_GUID_H_

#define ADDRESS_BASED_MIRROR_VARIABLE_GUID { 0x7b9be2e0, 0xe28a, 0x4197, 0xad, 0x3e, 0x32, 0xf0, 0x62, 0xf9, 0x46, 0x2c }

#define ADDRESS_RANGE_MIRROR_VARIABLE_CURRENT L"MirrorCurrent"
#define ADDRESS_RANGE_MIRROR_VARIABLE_REQUEST L"MirrorRequest"
#define ADDRESS_BASED_MIRROR_VARIABLE_SIZE sizeof(ADDRESS_RANGE_MIRROR_VARIABLE_DATA)
#define ADDRESS_BASED_MIRROR_VARIABLE_ATTRIBUTE (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS)
#define ADDRESS_RANGE_MIRROR_VARIABLE_VERSION 1
#define MIRROR_STATUS_SUCCESS 0
#define MIRROR_STATUS_MIRROR_INCAPABLE 1
#define MIRROR_STATUS_VERSION_MISMATCH 2
#define MIRROR_STATUS_INVALID_REQUEST  3
#define MIRROR_STATUS_UNSUPPORTED_CONFIG 4
#define MIRROR_STATUS_OEM_SPECIFIC_CONFIGURATION 5

extern EFI_GUID gAddressBasedMirrorGuid;

#pragma pack(1)

typedef struct {
//
// MirroredAmountAbove4GB is the amount of available memory above 4GB that needs to be mirrored
// measured in basis point (hundredths of percent e.g. 12% = 1275).
// In a multi-socket system, platform is required to distribute the mirrored memory ranges such that the
// amount mirrored is approximately  proportional to the amount of memory on each NUMA node. E.g. on
// a two node machine with 64GB on node 0 and 32GB on node 1, a request for 12GB of mirrored memory
// should be allocated with 8GB of mirror on node 0 and 4GB on node 1.
//
// For example, if the total memory in the system is 48GB and 12GB of memory above the 4GB addresses needs to be mirrored then the amount would be:
//   Total Memory  = 48 GB
//   Total Memory above 4GB = 44 GB
//   Percentage  = 8/44 * 100 = 18.18% = 1818 basis points
// Consider a 2S system with 32 GB of memory attached to socket 0 and 16GB on socket 1,
// then socket 0 should mirror 8 GB of memory and socket 1 mirror 4GB to maintain the requested 18%.
// This ensures that OS has an adequate amount of mirrored memory on each NUMA domain.
//
  UINT8         MirrorVersion;
  BOOLEAN       MirrorMemoryBelow4GB;
  UINT16        MirroredAmountAbove4GB;
  UINT8         MirrorStatus;
} ADDRESS_RANGE_MIRROR_VARIABLE_DATA;

typedef struct {
  ADDRESS_RANGE_MIRROR_VARIABLE_DATA MirrorCurrentType;
  ADDRESS_RANGE_MIRROR_VARIABLE_DATA MirrorRequestType;
} RASMEMORYINFO;
#pragma pack()


#endif
