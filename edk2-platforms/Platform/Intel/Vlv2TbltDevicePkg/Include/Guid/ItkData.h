/*++

  Copyright (c) 2004  - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  ItkData.h

Abstract:

--*/

#ifndef _ITKDATA_GUID_H_
#define _ITKDATA_GUID_H_

//
// This GUID is for the ITK related data found in a Variable  {3812723D-7E48-4e29-BC27-F5A39AC94EF1}
//
#define ITK_DATA_VAR_GUID \
  { 0x3812723d, 0x7e48, 0x4e29, 0xbc, 0x27, 0xf5, 0xa3, 0x9a, 0xc9, 0x4e, 0xf1 }

extern EFI_GUID gItkDataVarGuid;

#define ITK_DATA_VAR_NAME L"ItkDataVar"

extern CHAR16 gItkDataVarName[];

#define ITK_BIOS_MOD_VAR_NAME L"ItkBiosModVar"

extern CHAR16 gItkBiosModVarName[];

#pragma pack(1)
typedef struct {
  UINT32    Type;
  UINT32    RecordLength;
} EFI_ITK_DATA_HEADER;

typedef struct {
  EFI_ITK_DATA_HEADER   ItkHeader;
  UINT32                HecetaAddress;
} EFI_ITK_HECETA_ADDRESS;

typedef struct {
  UINT16    VarEqName;
  UINT16    VarEqValue;
} EFI_ITK_VAR_EQ_RECORD;

typedef struct {
  EFI_ITK_DATA_HEADER   ItkHeader;
  EFI_ITK_VAR_EQ_RECORD VarEqRecord[0x10000];
} EFI_ITK_VAR_EQ;
#pragma pack()

#define EFI_ITK_HECETA_ADDRESS_TYPE    1
#define EFI_ITK_MOBILE_BIOS_TYPE       2
#define EFI_ITK_VAR_EQ_TYPE            3

#endif
