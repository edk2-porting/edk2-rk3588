/** @file

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _DXE_TEST_POINT_DXE_H_
#define _DXE_TEST_POINT_DXE_H_

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/TestPointLib.h>

#include <Protocol/AdapterInformation.h>

#define TEST_POINT_AIP_PRIVATE_SIGNATURE  SIGNATURE_32('T', 'S', 'P', 'T')

typedef struct {
  UINT32                            Signature;
  EFI_ADAPTER_INFORMATION_PROTOCOL  Aip;
  VOID                              *TestPoint;
  UINTN                             TestPointSize;
  UINTN                             TestPointMaxSize;
} TEST_POINT_AIP_PRIVATE_DATA;

#define TEST_POINT_AIP_PRIVATE_DATA_FROM_THIS(a) \
  CR (a, \
      TEST_POINT_AIP_PRIVATE_DATA, \
      Aip, \
      TEST_POINT_AIP_PRIVATE_SIGNATURE \
      )

extern EFI_ADAPTER_INFORMATION_PROTOCOL mDxeAdapterInformationProtocol;

/**
  Return if input TestPoint data is valid.

  @param TestPointData  TestPoint data
  @param TestPointSize  TestPoint size

  @retval TRUE  TestPoint data is valid.
  @retval FALSE TestPoint data is not valid.
**/
BOOLEAN
InternalTestPointIsValidTable (
  IN VOID                     *TestPointData,
  IN UINTN                    TestPointSize
  );

#endif