/** @file

  Copyright (c) 2020, Hisilicon Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef HISI_SAS_CONFIG_H_
#define HISI_SAS_CONFIG_H_

typedef struct{
  UINT32                             CtrlId;
  BOOLEAN                            Enable;
  UINT32                             Bar32;
  UINT64                             ResetBase;
  UINTN                              Segment;
  UINTN                              Bus;
  UINTN                              Device;
  UINTN                              Fun;
} SAS_CONTROLLER_DATA;

typedef
VOID *
(EFIAPI *HISI_SAS_GET_CONTROLLER_DATA) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *HISI_SAS_GET_ADDRESS) (
  IN     UINT8 Index,
  IN OUT UINT8 *SasAddrBuffer
  );

typedef struct {
  HISI_SAS_GET_ADDRESS GetAddr;
  HISI_SAS_GET_CONTROLLER_DATA GetControllerData;
} HISI_SAS_CONFIG_PROTOCOL;

extern EFI_GUID gHisiSasConfigProtocolGuid;

#endif
