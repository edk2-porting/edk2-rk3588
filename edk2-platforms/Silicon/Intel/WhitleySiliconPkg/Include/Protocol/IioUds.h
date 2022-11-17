/** @file
  This protocol provides access to the Ioh Universal Data Structure
  This protocol is EFI compatible.

  @copyright
  Copyright 2005 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_IIO_UDS_PROTOCOL_H_
#define _EFI_IIO_UDS_PROTOCOL_H_

#include <IioUniversalData.h>

#define EFI_IIO_UDS_PROTOCOL_GUID  \
  { 0xa7ced760, 0xc71c, 0x4e1a, 0xac, 0xb1, 0x89, 0x60, 0x4d, 0x52, 0x16, 0xcb }

typedef struct _EFI_IIO_UDS_PROTOCOL EFI_IIO_UDS_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *IIH_ENABLE_VC) (
  IN EFI_IIO_UDS_PROTOCOL     *This,
  IN UINT32                    VcCtrlData
  );
/**

  Enables the requested VC in IIO

  @param This                    Pointer to the EFI_IOH_UDS_PROTOCOL instance.
  @param VcCtrlData              Data read from VC resourse control reg.

**/


typedef struct _EFI_IIO_UDS_PROTOCOL {
  IIO_UDS          *IioUdsPtr;
  IIH_ENABLE_VC    EnableVc;
} EFI_IIO_UDS_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiIioUdsProtocolGuid;

#endif
