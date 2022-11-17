/** @file
  UBA Iio Port Bifurcation Init Library Header File.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_IIO_PORT_BIFURCATION_INIT_LIB_H
#define _UBA_IIO_PORT_BIFURCATION_INIT_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <IioPlatformData.h>

#define IIO_PORT_BIFURCATION_INIT_SIGNATURE  SIGNATURE_32 ('P', 'B', 'I', 'F')
#define IIO_PORT_BIFURCATION_INIT_VERSION    01

// {853E5958-B3D6-4D98-A77C-100BB4ED940B}
#define   IIO_PORT_BIFURCATION_INIT_GUID \
{ 0x853e5958, 0xb3d6, 0x4d98, { 0xa7, 0x7c, 0x10, 0xb, 0xb4, 0xed, 0x94, 0xb } }

typedef
VOID
(*IIO_PORT_BIFURCATION_INIT_CALLBACK) (
  IN IIO_GLOBALS *IioGlobalData
);

typedef struct {
  UINT32                                 Signature;
  UINT32                                 Version;

  IIO_PORT_BIFURCATION_INIT_CALLBACK     CallUpdate;

} IIO_PORT_BIFURCATION_INIT_TABLE;

EFI_STATUS
IioPortBifurcationInit (
  IN IIO_GLOBALS *IioGlobalData
);

STATIC  EFI_GUID gIioPortBifurcationInitDataGuid = IIO_PORT_BIFURCATION_INIT_GUID;

#endif //_UBA_IIO_PORT_BIFURCATION_INIT_LIB_H

