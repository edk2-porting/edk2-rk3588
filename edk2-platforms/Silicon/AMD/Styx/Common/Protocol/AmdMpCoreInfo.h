/** @file

  Copyright (c) 2016, AMD Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _AMD_MP_CORE_INFO_H_
#define _AMD_MP_CORE_INFO_H_

extern EFI_GUID gAmdMpCoreInfoProtocolGuid;

typedef
ARM_CORE_INFO *
(EFIAPI *GET_ARM_CORE_INFO_TABLE) (
  OUT UINTN  *NumEntries
  );

typedef
EFI_STATUS
(EFIAPI *GET_PMU_SPI_FROM_MPID) (
  IN  UINT32  MpId,
  OUT UINT32  *PmuSpi
  );

typedef struct _AMD_MP_CORE_INFO_PROTOCOL {
  GET_ARM_CORE_INFO_TABLE  GetArmCoreInfoTable;
  GET_PMU_SPI_FROM_MPID    GetPmuSpiFromMpId;
} AMD_MP_CORE_INFO_PROTOCOL;

#endif // _AMD_MP_CORE_INFO_H_
